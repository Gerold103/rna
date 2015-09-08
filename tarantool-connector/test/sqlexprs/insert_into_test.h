void test_insert_into(Session &ses, TarantoolInfo &tinfo)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ M a k i n g   q u e r y ~~~~~~~~~~~~~~~~~~~~~~~~

	std::string sql_query;

	std::vector<std::string> sql_queries;
	std::vector<MValue> results;

	MValue res;

	//~~~~~~~~~~~~~~~~~~~~~~~~ I N S E R T   I N T O   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	Logger::Log(TEST) << "~~~~~~~~ I n s e r t   i n t o   t e s t s ~~~~~~~~\n";

	sql_queries.clear();
	results.clear();

	sql_queries.push_back("INSERT INTO users VALUES (1, 'Ivan', 'Ivanov', 1000);");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO users (id, name, surname, salary) VALUES (2, 'Vasya', 'Vasyov', 2000);");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO users (name, surname) VALUES ('Petr', 'Petrov');");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO users VALUES (3, 'Nikolay', 'Nikolayev', 1000, 'trash', 12345, 6789.123)");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO users VALUES (4, 'Georgiy', 'Georgiev')");
	results.push_back(MValue(true));

	//in users
	/*
		(1, 'Ivan', 'Ivanov', 1000)
		(2, 'Vasya', 'Vasyov', 2000)
		(0, 'Petr', 'Petrov', 0)
		(3, 'Nikolay', 'Nikolayev', 1000)
		(4, 'Georgiy', 'Georgiev', 0)
	*/

	sql_queries.push_back("INSERT INTO books VALUES (0, 1, 'C++', 'Straustrup', 2004)");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO books (id, user_id, name, author, year) VALUES (1, 3, 'Book1', 'Author1', 1995)");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO books (id, user_id, name, author) VALUES (2, 3, 'Book2', 'Author1')");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO books VALUES (3, 0, 'Book3', 'Author2', 2013, 12345, 6789.123)");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO books VALUES (4, 0, 'Book4', 'Author3')");
	results.push_back(MValue(true));

	//in books
	/*
		(0, 1, 'C++', 'Straustrup', 2004)
		(1, 3, 'Book1', 'Author1', 1995)
		(2, 3, 'Book2', 'Author1', 0)
		(3, 0, 'Book3', 'Author2', 2013)
		(4, 0, 'Book4', 'Author3', 0)
	*/



	sql_queries.push_back("INSERT INTO users VALUES (1, 'Ivan', 'Ivanov', 1000);");
	results.push_back(MValue(false));

	sql_queries.push_back("INSERT INTO users (name, surname) VALUES ('Ksenia', 'Ivanova');");
	results.push_back(MValue(false));

	sql_queries.push_back("INSERT INTO users (id, name) VALUES (5, 39)");
	results.push_back(MValue(false));

	sql_queries.push_back("INSERT INTO users (id, surname) VALUES (6)");
	results.push_back(MValue(false));

	sql_queries.push_back("INSERT INTO users (id, surname) VALUES (6, 7, 8)");
	results.push_back(MValue(false));

	for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
		try {
			res = MakeQuery(ses, tinfo, sql_queries[i]);
			Logger::Log(TEST) << "test n = " << i << " accepted: " << (MValue(!!res) == results[i]) << "\n";
		}
		catch(const std::string &mes) { Logger::Log(TEST) << "error on test " << i << ", sql_query = '" << sql_queries[i] << "\n"; }
		catch(...) { Logger::Log(TEST) << "unknown error on test " << i << "\n"; }
	}
}