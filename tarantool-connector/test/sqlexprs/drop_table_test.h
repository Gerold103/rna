void test_drop_table(Session &ses, TarantoolInfo &tinfo)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ M a k i n g   q u e r y ~~~~~~~~~~~~~~~~~~~~~~~~

	std::string sql_query;

	std::vector<std::string> sql_queries;
	std::vector<MValue> results;

	MValue res;

	//~~~~~~~~~~~~~~~~~~~~~~~~ D R O P   T A B L E   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	Logger::Log(TEST) << "~~~~~~~~ D r o p   t a b l e   t e s t s ~~~~~~~~\n";

	sql_queries.clear();
	results.clear();

	sql_queries.push_back("DROP TABLE users");
	results.push_back(MValue(true));

	sql_queries.push_back("DROP TABLE books");
	results.push_back(MValue(true));

	sql_queries.push_back("DROP TABLE foo_bar");
	results.push_back(MValue(false));

	sql_queries.push_back("DROP TABLE tmp_table");
	results.push_back(MValue(true));

	for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
		try {
			res = MakeQuery(ses, tinfo, sql_queries[i]);
			Logger::Log(TEST) << "test n = " << i << " accepted: " << (MValue(!!res) == results[i]) << "\n";
		}
		catch(const std::string &mes) { Logger::Log(TEST) << "error on test " << i << ", sql_query = '" << sql_queries[i] << "\n"; }
		catch(...) { Logger::Log(TEST) << "unknown error on test " << i << "\n"; }
	}
}