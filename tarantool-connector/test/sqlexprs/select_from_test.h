void test_select_from(Session &ses, TarantoolInfo &tinfo)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ M a k i n g   q u e r y ~~~~~~~~~~~~~~~~~~~~~~~~

	std::string sql_query;

	std::vector<std::string> sql_queries;
	std::vector<MValue> results;

	MValue res;

	//~~~~~~~~~~~~~~~~~~~~~~~~ S E L E C T   F R O M   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	Logger::Log(TEST) << "~~~~~~~~ S e l e c t   f r o m   t e s t s ~~~~~~~~\n";

	sql_queries.clear();
	results.clear();

	sql_queries.push_back("SELECT * FROM users WHERE (salary >= 1000)");
	{
		MValueVector tuples;
		MValueVector tmp;
		tmp.push_back(MValue(1));
		tmp.push_back(MValue("Ivan"));
		tmp.push_back(MValue("Ivanov"));
		tmp.push_back(MValue(1000));
		tuples.push_back(MValue(tmp));
		tmp.clear();

		tmp.push_back(MValue(2));
		tmp.push_back(MValue("Vasya"));
		tmp.push_back(MValue("Vasyov"));
		tmp.push_back(MValue(2000));
		tuples.push_back(MValue(tmp));
		tmp.clear();

		tmp.push_back(MValue(3));
		tmp.push_back(MValue("Nikolay"));
		tmp.push_back(MValue("Nikolayev"));
		tmp.push_back(MValue(1000));
		tuples.push_back(MValue(tmp));
		tmp.clear();

		results.push_back(tuples);
	}

	//in users
	/*
		(1, 'Ivan', 'Ivanov', 1000)
		(2, 'Vasya', 'Vasyov', 2000)
		(0, 'Petr', 'Petrov', 0)
		(3, 'Nikolay', 'Nikolayev', 1000)
		(4, 'Georgiy', 'Georgiev', 0)
	*/

	sql_queries.push_back("SELECT id, name as nm FROM users WHERE (salary < 2000)");
	{
		MValueVector tuples;
		MValueVector tmp;
		tmp.push_back(MValue(1));
		tmp.push_back(MValue("Ivan"));
		tuples.push_back(MValue(tmp));
		tmp.clear();

		tmp.push_back(MValue(0));
		tmp.push_back(MValue("Petr"));
		tuples.push_back(MValue(tmp));
		tmp.clear();

		tmp.push_back(MValue(3));
		tmp.push_back(MValue("Nikolay"));
		tuples.push_back(MValue(tmp));
		tmp.clear();

		tmp.push_back(MValue(4));
		tmp.push_back(MValue("Georgiy"));
		tuples.push_back(MValue(tmp));
		tmp.clear();

		results.push_back(tuples);
	}

	for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
		try {
			res = MakeQuery(ses, tinfo, sql_queries[i]);
			Logger::Log(TEST) << "test n = " << i << " accepted: " << (SpaceObject(res) == SpaceObject(results[i])) << "\n";
		}
		catch(const std::string &mes) { Logger::Log(TEST) << "error on test " << i << ", sql_query = '" << sql_queries[i] << "\n"; }
		catch(...) { Logger::Log(TEST) << "unknown error on test " << i << "\n"; }
	}
}