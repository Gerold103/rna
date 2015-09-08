CREATE TABLE users (id INT, name TEXT, surname TEXT, salary INT, PRIMARY KEY (id, salary), UNIQUE(id), INDEX(name, surname), INDEX(name, salary));

CREATE TABLE books (id INT, user_id INT, name TEXT, author TEXT, year INT, PRIMARY KEY (id), INDEX(name), INDEX(author));

CREATE TABLE tmp_table (id INT, str1 TEXT, str2 TEXT, num1 INT, num2 INT, foo INT, bar TEXT, PRIMARY KEY (id), INDEX(str1, str2), INDEX(num1, num2), INDEX(foo));

INSERT INTO users VALUES (1, 'Ivan', 'Ivanov', 1000);

INSERT INTO users (id, name, surname, salary) VALUES (2, 'Vasya', 'Vasyov', 2000);

INSERT INTO users (name, surname) VALUES ('Petr', 'Petrov');

INSERT INTO users VALUES (3, 'Nikolay', 'Nikolayev', 1000, 'trash', 12345, 6789.123)

INSERT INTO users VALUES (4, 'Georgiy', 'Georgiev')

//in users
/*
	(1, 'Ivan', 'Ivanov', 1000)
	(2, 'Vasya', 'Vasyov', 2000)
	(0, 'Petr', 'Petrov', 0)
	(3, 'Nikolay', 'Nikolayev', 1000)
	(4, 'Georgiy', 'Georgiev', 0)
*/

INSERT INTO books VALUES (0, 1, 'C++', 'Straustrup', 2004)

INSERT INTO books (id, user_id, name, author, year) VALUES (1, 3, 'Book1', 'Author1', 1995)

INSERT INTO books (id, user_id, name, author) VALUES (2, 3, 'Book2', 'Author1')

INSERT INTO books VALUES (3, 0, 'Book3', 'Author2', 2013, 12345, 6789.123)

INSERT INTO books VALUES (4, 0, 'Book4', 'Author3')

//in books
/*
	(0, 1, 'C++', 'Straustrup', 2004)
	(1, 3, 'Book1', 'Author1', 1995)
	(2, 3, 'Book2', 'Author1', 0)
	(3, 0, 'Book3', 'Author2', 2013)
	(4, 0, 'Book4', 'Author3', 0)
*/

SELECT * FROM users WHERE (salary >= 1000)

SELECT id, name as nm FROM users WHERE (salary < 2000)

SELECT b.id as BookID, (users.name + ' ' + u.surname) as UserName, books.name as BookName, year FrOm users as u join books b ON (b.user_id = u.id) WHERE year >= 1995

DROP TABLE users

DROP TABLE books

DROP TABLE tmp_table