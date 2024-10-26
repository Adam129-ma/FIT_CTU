from socket import *
import threading
from collections import namedtuple
import re

SERVER_MOVE = b"102 MOVE\a\b"
SERVER_TURN_LEFT = b"103 TURN LEFT\a\b"
SERVER_TURN_RIGHT = b"104 TURN RIGHT\a\b"
SERVER_PICK_UP = b"105 GET MESSAGE\a\b"
SERVER_LOGOUT = b"106 LOGOUT\a\b"
SERVER_KEY_REQUEST = b"107 KEY REQUEST\a\b"
SERVER_OK = b"200 OK\a\b"
SERVER_LOGIN_FAILED = b"300 LOGIN FAILED\a\b"
SERVER_SYNTAX_ERROR = b"301 SYNTAX ERROR\a\b"
SERVER_LOGIC_ERROR = b"302 LOGIC ERROR\a\b"
SERVER_KEY_OUT_OF_RANGE_ERROR = b"303 KEY OUT OF RANGE\a\b"
KEY_TABLE = {
    0: (23019, 32037),
    1: (32037, 29295),
    2: (18789, 13603),
    3: (16443, 29533),
    4: (18189, 21952)
}

TIMEOUT = 1
TIMEOUT_RECHARGING = 5

CLIENT_MSG_LENGTHS = {"USERNAME": 20, "CONFIRM": 7, "OK": 12, "RECHARGING": 12, "FULL POWER": 12, "MESSAGE": 100,
                      "KEY_ID": 5, "COORDINATES": 7}

Direction = namedtuple("direction", "x y")


class CloseConnection(Exception):
    pass


class InvalidMessageError(Exception):
    pass


class MessageFound(Exception):
    pass


class Robot:
    def __init__(self):
        self.buffer = ""
        self.hash = None
        self.x = None
        self.y = None
        self.direction = None

    def move(self):
        self.x += self.direction.x
        self.y += self.direction.y

    def turn_left(self):
        self.direction = Direction(-self.direction.y, self.direction.x)

    def turn_right(self):
        self.direction = Direction(self.direction.y, -self.direction.x)


def extract_message(connection, robot, msg_type):
    msg_len = CLIENT_MSG_LENGTHS[msg_type]
    separator = robot.buffer.find("\a\b")
    if separator != -1:
        message = robot.buffer[:separator]
        robot.buffer = robot.buffer[separator + 2:]

        if len(message) > msg_len:
            connection.sendall(SERVER_SYNTAX_ERROR)
            raise CloseConnection("Invalid format of estimated message in the buffer")
        return message

    while True:
        data = connection.recv(msg_len).decode("ascii")
        if data == "":
            raise CloseConnection("Empty data received")

        robot.buffer += data
        separator = robot.buffer.find("\a\b")

        if separator == -1:
            if len(robot.buffer) < msg_len:
                continue
            else:
                connection.sendall(SERVER_SYNTAX_ERROR)
                raise CloseConnection("Invalid format of received message")

        message = robot.buffer[:separator]
        robot.buffer = robot.buffer[separator + 2:]
        if msg_type == "RECHARGING" and message == "RECHARGING":
            handle_recharging(connection, robot)
            continue

        return message


def check_confirmation_message(connection, robot, message, key_id):
    if message == "RECHARGING":
        handle_recharging(connection, robot)
        message = extract_message(connection, robot, "CONFIRM")

    pattern = re.compile("^\d{1,5}$")

    if pattern.match(message) is None:
        connection.sendall(SERVER_SYNTAX_ERROR)
        raise CloseConnection("Invalid CONFIRMATION message")

    return (int(message) - get_client_key(key_id)) % 65536


def handle_recharging(connection, robot):
    connection.settimeout(TIMEOUT_RECHARGING)
    message = extract_message(connection, robot, "FULL POWER")
    if message != "FULL POWER":
        connection.sendall(SERVER_LOGIC_ERROR)
        raise CloseConnection("Received something else than FULL POWER")
    connection.settimeout(TIMEOUT)


def send_move(connection, robot):
    connection.sendall(SERVER_MOVE)
    message = extract_message(connection, robot, "OK")
    x, y = check_ok_message(connection, robot, message)

    if (x, y) == (robot.x, robot.y):
        connection.sendall(SERVER_MOVE)
        message = extract_message(connection, robot, "OK")
        x, y = check_ok_message(connection, robot, message)

    return x, y


def handle_connection(connection):
    robot = Robot()

    try:
        authorize(connection, robot)
        move_to_goal_init_pos(connection, robot)
        send_pick_up(connection, robot)

        raise CloseConnection("No final message found")

    except CloseConnection as ex:
        print(*ex.args)

    except timeout:
        print("Connection timeout")

    except MessageFound as mf:
        connection.sendall(SERVER_LOGOUT)
        print("Message \"", *mf.args, "\" found!", sep="")

    connection.close()
    print("Connection closed")


def authorize(connection, robot):
    message = extract_message(connection, robot, "USERNAME")
    robot.hash = (sum(bytearray(message, "ascii")) * 1000) % 65536
    message = SERVER_KEY_REQUEST
    connection.sendall(message)
    message = extract_message(connection, robot, "KEY_ID")

    if message[0].isdigit():
        key_id = int(message[0])
        if key_id < 0 or key_id > 4:
            connection.sendall(SERVER_KEY_OUT_OF_RANGE_ERROR)
            raise CloseConnection("Key ID out of range")
    else:
        connection.sendall(SERVER_SYNTAX_ERROR)
        raise CloseConnection("Key ID is not a number")

    hash_to_send = bytearray(str((robot.hash + get_server_key(key_id)) % 65536), "ascii") + b"\a\b"
    connection.sendall(hash_to_send)

    message = extract_message(connection, robot, "RECHARGING")
    login_hash = check_confirmation_message(connection, robot, message, key_id)

    if login_hash != robot.hash:
        connection.sendall(SERVER_LOGIN_FAILED)
        raise CloseConnection("Login failed")

    connection.sendall(SERVER_OK)


def move_to_goal_init_pos(connection, robot):
    connection.sendall(SERVER_MOVE)
    message = extract_message(connection, robot, "OK")
    robot.x, robot.y = check_ok_message(connection, robot, message)

    turn_and_move(True, connection, robot, (0, 0))


class Server:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.l_socket = socket(AF_INET, SOCK_STREAM)
        self.threads = {}

    def start(self):
        try:
            self.l_socket.bind((self.host, self.port))
        except OSError:
            self.port = 12345
            self.l_socket.bind((self.host, self.port))
        self.l_socket.listen()

        while True:
            print("Waiting for connection on port", self.port, "...")
            c_socket, address = self.l_socket.accept()
            print("Connected to", address)
            c_socket.settimeout(TIMEOUT)

            self.threads[c_socket] = threading.Thread(target=handle_connection, args=(c_socket,))
            self.threads[c_socket].start()


if __name__ == "__main__":
    server = Server("127.0.0.1", 12345)
    server.start()
