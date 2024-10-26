from socket import *
import re
import time

SERVER_ADDRESS = ("127.0.0.1", 12345)
USERNAME = "user123"
KEY_ID = 0

CLIENT_MSG_LENGTHS = {
    "USERNAME": 20,
    "CONFIRM": 7,
    "OK": 12,
    "RECHARGING": 12,
    "FULL POWER": 12,
    "MESSAGE": 100,
    "KEY_ID": 5,
    "COORDINATES": 7
}

def get_client_key(key_id):
    KEY_TABLE = {
        0: (23019, 32037),
        1: (32037, 29295),
        2: (18789, 13603),
        3: (16443, 29533),
        4: (18189, 21952)
    }
    if key_id in KEY_TABLE:
        _, client_key = KEY_TABLE[key_id]
        return client_key
    else:
        raise ValueError(f"Invalid key_id: {key_id}")

def connect_to_server():
    client_socket = socket(AF_INET, SOCK_STREAM)
    client_socket.connect(SERVER_ADDRESS)
    return client_socket

def send_username(client_socket, username):
    username = username.ljust(CLIENT_MSG_LENGTHS["USERNAME"])
    client_socket.sendall(username.encode("ascii") + b"\a\b")

def send_key_id(client_socket, key_id):
    key_id_message = str(key_id).ljust(CLIENT_MSG_LENGTHS["KEY_ID"])
    client_socket.sendall(key_id_message.encode("ascii") + b"\a\b")

def send_recharging(client_socket):
    client_socket.sendall(b"RECHARGING\a\b")

def main():
    client_socket = connect_to_server()
    send_username(client_socket, USERNAME)
    send_key_id(client_socket, KEY_ID)

    # Wait for server response
    while True:
        try:
            data = client_socket.recv(1024).decode("ascii")
            print(f"Received from server: {data}")
            if data.startswith("200 OK"):
                send_recharging(client_socket)
                break
        except timeout:
            print("Connection timeout")
            break

    client_socket.close()

if __name__ == "__main__":
    main()
