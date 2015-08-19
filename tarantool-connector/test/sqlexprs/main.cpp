#include <iostream>
#include <memory>
#include <cstring>
#include <vector>

#include "tarantool_schema_.h"
#include "tp_wrap.h"
#include "mvalue.h"
#include "tarantool_objs.h"
#include "logger.h"

#include "SQLParser.h"
#include "SQLMaker.h"
#include "SelectMaker.h"
#include "CreateMaker.h"
#include "DropMaker.h"
#include "InsertMaker.h"

#include "test_utils.h"
#include "create_table_test.h"
#include "insert_into_test.h"
#include "select_from_test.h"
#include "drop_table_test.h"

#include <cstddef>
#include <cstdint>
extern "C" {
	#include <session.h>
	#include <tp.h>
}
#include <cstdlib>

using namespace hsql;

void test(Session &ses, TarantoolInfo &tinfo)
{
	//Logger::LogObject(DEBUG, schm);

	//~~~~~~~~~~~~~~~~~~~~~~~~ M a k i n g   q u e r y ~~~~~~~~~~~~~~~~~~~~~~~~

	std::string sql_query;

	std::vector<std::string> sql_queries;
	std::vector<MValue> results;

	MValue res;

	MakeQuery(ses, tinfo, "SELECT name3, t1.dig1, t2.bar, t1.bar FROM tester AS t1 JOIN tester2 AS t2 ON (t1.bar = t2.bar);");

	//MakeQuery("CREATE TABLE users (id INT, name TEXT, surname TEXT, salary INT, PRIMARY KEY (id, salary), UNIQUE(id), INDEX(name, surname), INDEX(name, salary));", port, address);

	//schm = TarantoolSchema(ses);

	//Logger::LogObject(DEBUG, schm);

	//MakeQuery("DROP TABLE users", port, address);

}

int main()
{
	Logger::SetStreamForMessages(TEST, std::cout);

	//int port = std::stoi(std::string(std::getenv("LISTEN")));
	int port = 33013;

	//~~~~~~~~~~~~~~~~~~~~~~~~ I n i t   T a r a n t o o l   S c h e m a ~~~~~~~~~~~~~~~~~~~~~~~~

	Session ses(port);
	if (ses.Connect() == -1) {
		LogFL(DEBUG) << "test(): connection failed, error code: " << ses.GetErrorCode() << ";\n";
		return 0;
	} else {
		LogFL(DEBUG) << "test(): conected;\n";
	}

	DataStructure greet(128);
	ses.Receive(greet);

	TarantoolInfo info(ses);

	test(ses, info);

	// test_create_table(ses, tinfo);
	// test_insert_into(ses, tinfo);
	// test_select_from(ses, tinfo);
	// test_drop_table(ses, tinfo);
/*
	std::string sql_query;

	std::cout << "sql_tarantool> " << std::flush;

	while(std::getline(std::cin, sql_query)) {
		MakeQuery(sql_query, port, "localhost");
		std::cout << "sql_tarantool> " << std::flush;
	}
*/
	return 0;
}
