-- Remove conflicting tables
DROP TABLE IF EXISTS car CASCADE;
DROP TABLE IF EXISTS dealer CASCADE;
DROP TABLE IF EXISTS customer CASCADE;
DROP TABLE IF EXISTS car_dealer CASCADE;
-- End of removing
-- Create DEALER table
CREATE TABLE DEALER (
    personalNumber BIGINT PRIMARY KEY,
    age INT,
    name VARCHAR(255) NOT NULL,
    nickname VARCHAR(255)
);

-- Create CUSTOMER table
CREATE TABLE CUSTOMER (
    id_customer BIGINT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255),
    bought_car BIGINT,
    FOREIGN KEY (bought_car) REFERENCES CAR(id_car)
);

-- Create CAR table
CREATE TABLE CAR (
    id_car BIGINT PRIMARY KEY AUTO_INCREMENT,
    production_year INT NOT NULL,
    colour VARCHAR(255) NOT NULL,
    brand VARCHAR(255) NOT NULL
);

-- Create many-to-many relationship table (car_dealer)
CREATE TABLE car_dealer (
    personal_number BIGINT,
    id_car BIGINT,
    FOREIGN KEY (personal_number) REFERENCES DEALER(personalNumber),
    FOREIGN KEY (id_car) REFERENCES CAR(id_car)
);

-- Add data to DEALER table
INSERT INTO DEALER (personalNumber, age, name, nickname) VALUES
    (1, 30, 'John Doe', 'JD'),
    (2, 35, 'Alice Smith', 'AS'),
    (3, 28, 'Bob Johnson', 'BJ'),
    (4, 40, 'Eve Turner', NULL),
    (5, 45, 'Charlie Brown', 'CB');

-- Add data to CAR table
INSERT INTO CAR (production_year, colour, brand) VALUES
    (2019, 'Red', 'Toyota'),
    (2020, 'Blue', 'Honda'),
    (2018, 'Black', 'Ford'),
    (2021, 'Silver', 'Chevrolet'),
    (2017, 'White', 'BMW');

-- Add relationships between CAR and DEALER (many-to-many)
INSERT INTO car_dealer (personal_number, id_car) VALUES
    (1, 1),
    (2, 2),
    (3, 3),
    (4, 4),
    (5, 5);

-- Create DEALER table
CREATE TABLE DEALER (
    personalNumber BIGINT PRIMARY KEY,
    age INT,
    name VARCHAR(255) NOT NULL,
    nickname VARCHAR(255)
);

-- Create CUSTOMER table
CREATE TABLE CUSTOMER (
    id_customer BIGINT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255),
    bought_car BIGINT,
    FOREIGN KEY (bought_car) REFERENCES CAR(id_car)
);

-- Create CAR table
CREATE TABLE CAR (
    id_car BIGINT PRIMARY KEY AUTO_INCREMENT,
    production_year INT NOT NULL,
    colour VARCHAR(255) NOT NULL,
    brand VARCHAR(255) NOT NULL
);

-- Create many-to-many relationship table (car_dealer)
CREATE TABLE car_dealer (
    personal_number BIGINT,
    id_car BIGINT,
    FOREIGN KEY (personal_number) REFERENCES DEALER(personalNumber),
    FOREIGN KEY (id_car) REFERENCES CAR(id_car)
);

-- Add data to DEALER table
INSERT INTO DEALER (personalNumber, age, name, nickname) VALUES
    (1, 30, 'John Doe', 'JD'),
    (2, 35, 'Alice Smith', 'AS'),
    (3, 28, 'Bob Johnson', 'BJ'),
    (4, 40, 'Eve Turner', NULL),
    (5, 45, 'Charlie Brown', 'CB');

-- Add data to CAR table
INSERT INTO CAR (production_year, colour, brand) VALUES
    (2019, 'Red', 'Toyota'),
    (2020, 'Blue', 'Honda'),
    (2018, 'Black', 'Ford'),
    (2021, 'Silver', 'Chevrolet'),
    (2017, 'White', 'BMW');


INSERT INTO car_dealer (personal_number, id_car) VALUES
    (1, 1),
    (2, 2),
    (3, 3),
    (4, 4),
    (5, 5);

INSERT INTO CUSTOMER (name, bought_car) VALUES
    ('Customer 1', 1),
    ('Customer 2', 2),
    ('Customer 3', 3),
    ('Customer 4', 4),
    ('Customer 5', 5);
