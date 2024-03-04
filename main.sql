CREATE DATABASE library; 

use library; 

CREATE TABLE users (
 username varchar(30),
 password varchar(30) 
); 

INSERT INTO users (username, password) VALUES ('admin', 'admin');

CREATE TABLE book (
 BookNo int, 
Book_Name varchar(30), 
Author varchar(30), 
Issued_By varchar(30) 
);