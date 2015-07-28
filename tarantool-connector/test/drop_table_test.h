void test_drop_table()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ I n i t   T a r a n t o o l   S c h e m a ~~~~~~~~~~~~~~~~~~~~~~~~

	Session ses;
	if (ses.Connect() == -1) {
		LogFL(DEBUG) << "test_drop_table(): connection failed, error code: " << ses.GetErrorCode() << ";\n";
		return;
	} else {
		LogFL(DEBUG) << "test_drop_table(): conected;\n";
	}

	DataStructure greet(128);
	LogFL(DEBUG) << "test_drop_table(): greet size: " << ses.Receive(greet) << "\n";
	TarantoolSchema schm(ses);

	Logger::LogObject(DEBUG, schm);

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

	for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
		try {
			res = MakeQuery(sql_queries[i]);
			Logger::Log(TEST) << "test n = " << i << " accepted: " << (MValue(!!res) == results[i]) << "\n";
		}
		catch(const std::string &mes) { Logger::Log(TEST) << "error on test " << i << ", sql_query = '" << sql_queries[i] << "\n"; }
		catch(...) { Logger::Log(TEST) << "unknown error on test " << i << "\n"; }
	}
}