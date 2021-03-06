void test_select_from(Session &ses, TarantoolInfo &tinfo)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ M a k i n g   q u e r y ~~~~~~~~~~~~~~~~~~~~~~~~

	std::string sql_query;

	std::vector<std::string> sql_queries;
	std::vector<SpaceObject> results;

	SpaceObject res;

	//~~~~~~~~~~~~~~~~~~~~~~~~ S E L E C T   F R O M   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	Logger::Log(TEST) << "~~~~~~~~ S e l e c t   f r o m   t e s t s ~~~~~~~~\n";

	sql_queries.clear();
	results.clear();

	sql_queries.push_back("SELECT * FROM users WHERE (salary >= 1000)");
	{
		SpaceObject tuples;
		std::vector<TColumn> names;
		names.push_back(TColumn("id", "users"));
		names.push_back(TColumn("name", "users"));
		names.push_back(TColumn("surname", "users"));
		names.push_back(TColumn("salary", "users"));
		tuples.SetNames(names);
		TupleObj tmp;

		tmp.PushBack(TColumn("id", "users", "", MValue(1)));
		tmp.PushBack(TColumn("name", "users", "", MValue("Ivan")));
		tmp.PushBack(TColumn("surname", "users", "", MValue("Ivanov")));
		tmp.PushBack(TColumn("salary", "users", "", MValue(1000)));
		tuples.PushBack(tmp);
		tmp.Clear();

		tmp.PushBack(TColumn("id", "users", "", MValue(2)));
		tmp.PushBack(TColumn("name", "users", "", MValue("Vasya")));
		tmp.PushBack(TColumn("surname", "users", "", MValue("Vasyov")));
		tmp.PushBack(TColumn("salary", "users", "", MValue(2000)));
		tuples.PushBack(tmp);
		tmp.Clear();

		tmp.PushBack(TColumn("id", "users", "", MValue(3)));
		tmp.PushBack(TColumn("name", "users", "", MValue("Nikolay")));
		tmp.PushBack(TColumn("surname", "users", "", MValue("Nikolayev")));
		tmp.PushBack(TColumn("salary", "users", "", MValue(1000)));
		tuples.PushBack(tmp);
		tmp.Clear();

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
		SpaceObject tuples;
		std::vector<TColumn> names;
		names.push_back(TColumn("id", "users"));
		names.push_back(TColumn("name", "users", "nm"));
		tuples.SetNames(names);
		TupleObj tmp;

		tmp.PushBack(TColumn("id", "users", "", MValue(1)));
		tmp.PushBack(TColumn("name", "users", "nm", MValue("Ivan")));
		tuples.PushBack(tmp);
		tmp.Clear();

		tmp.PushBack(TColumn("id", "users", "", MValue(0)));
		tmp.PushBack(TColumn("name", "users", "nm", MValue("Petr")));
		tuples.PushBack(tmp);
		tmp.Clear();

		tmp.PushBack(TColumn("id", "users", "", MValue(3)));
		tmp.PushBack(TColumn("name", "users", "nm", MValue("Nikolay")));
		tuples.PushBack(tmp);
		tmp.Clear();

		tmp.PushBack(TColumn("id", "users", "", MValue(4)));
		tmp.PushBack(TColumn("name", "users", "nm", MValue("Georgiy")));
		tuples.PushBack(tmp);
		tmp.Clear();

		results.push_back(tuples);
	}

	sql_queries.push_back("SELECT b.id as BookID, (users.name + ' ' + u.surname) as UserName, books.name as BookName, year FrOm users as u join books b ON (b.user_id = u.id) WHERE year >= 1995");
	{
		SpaceObject tuples;
		std::vector<TColumn> names;
		names.push_back(TColumn("id", "books", "BookID"));
		names.push_back(TColumn("((users.name + ' ') + u.surname)", "", "UserName"));
		names.push_back(TColumn("name", "books", "BookName"));
		names.push_back(TColumn("year", "books"));
		tuples.SetNames(names);
		TupleObj tmp;

		tmp.PushBack(TColumn("id", "books", "BookID", MValue(0)));
		tmp.PushBack(TColumn("((users.name + ' ') + u.surname)", "", "UserName", MValue("Ivan Ivanov")));
		tmp.PushBack(TColumn("name", "books", "BookName", MValue("C++")));
		tmp.PushBack(TColumn("year", "books", "", MValue(2004)));
		tuples.PushBack(tmp);
		tmp.Clear();

		tmp.PushBack(TColumn("id", "books", "BookID", MValue(3)));
		tmp.PushBack(TColumn("((users.name + ' ') + u.surname)", "", "UserName", MValue("Petr Petrov")));
		tmp.PushBack(TColumn("name", "books", "BookName", MValue("Book3")));
		tmp.PushBack(TColumn("year", "books", "", MValue(2013)));
		tuples.PushBack(tmp);
		tmp.Clear();

		tmp.PushBack(TColumn("id", "books", "BookID", MValue(1)));
		tmp.PushBack(TColumn("((users.name + ' ') + u.surname)", "", "UserName", MValue("Nikolay Nikolayev")));
		tmp.PushBack(TColumn("name", "books", "BookName", MValue("Book1")));
		tmp.PushBack(TColumn("year", "books", "", MValue(1995)));
		tuples.PushBack(tmp);
		tmp.Clear();

		results.push_back(tuples);
	}

	for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
		try {
			res = MakeSelect(ses, tinfo, sql_queries[i]);
			bool t = (SpaceObject(res) == SpaceObject(results[i]));
			Logger::Log(TEST) << "test n = " << i << " accepted: " << t << "\n";
			if (!t) {
				Logger::Log(TEST) << "result: " << res << ";\nmust be: " << results[i] << "\n";
			}
		}
		catch(const std::string &mes) { Logger::Log(TEST) << "error on test " << i << ", sql_query = '" << sql_queries[i] << ", res = " << res << "\n must be = " << results[i] << "\n"; }
		catch(...) { Logger::Log(TEST) << "unknown error on test " << i << "\n"; }
	}
}