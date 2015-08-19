void test_create_table(Session &ses, TarantoolInfo &tinfo)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ M a k i n g   q u e r y ~~~~~~~~~~~~~~~~~~~~~~~~

	std::string sql_query;

	std::vector<std::string> sql_queries;
	std::vector<MValue> results;

	MValue res;

	//~~~~~~~~~~~~~~~~~~~~~~~~ C R E A T E   T A B L E   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	Logger::Log(TEST) << "~~~~~~~~ C r e a t e   t a b l e   t e s t s ~~~~~~~~\n";

	sql_queries.push_back("CREATE TABLE users (id INT, name TEXT, surname TEXT, salary INT, PRIMARY KEY (id, salary), UNIQUE(id), INDEX(name, surname), INDEX(name, salary));");
	results.push_back(MValue(true));

	sql_queries.push_back("CREATE TABLE users (id INT, name TEXT, foo INT, bar DOUBLE);");
	results.push_back(MValue(false));

	sql_queries.push_back("CREATE TABLE books (id INT, user_id INT, name TEXT, author TEXT, year INT, PRIMARY KEY (id), INDEX(name), INDEX(author));");
	results.push_back(MValue(true));

	sql_queries.push_back("CREATE TABLE tmp_table (id INT, str1 TEXT, str2 TEXT, num1 INT, num2 INT, foo INT, bar TEXT, PRIMARY KEY (id), INDEX(str1, str2), INDEX(num1, num2), INDEX(foo));");
	results.push_back(MValue(true));

	for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
		try {
			res = MakeQuery(ses, tinfo, sql_queries[i]);
			Logger::Log(TEST) << "test n = " << i << " accepted: " << (res == results[i]) << "\n";
		}
		catch(const std::string &mes) { Logger::Log(TEST) << "error on test " << i << ", sql_query = '" << sql_queries[i] << "\n"; }
		catch(...) { Logger::Log(TEST) << "unknown error on test " << i << "\n";}
	}
}