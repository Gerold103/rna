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

	//MakeQuery(ses, tinfo, "SELECT (t1.bar + bar2) as tbar, tester.id, t2.id, name4, t1.dig1, t3.id, name6 FROM tester AS t1 JOIN tester2 AS t2 ON (tbar >= 400) JOIN tester3 as t3;");
	//MakeQuery(ses, tinfo, "SELECT * FROM tester JOIN tester2 WHERE tester.bar = tester2.bar2");
	//MakeSelect(ses, tinfo, "SELECT id as i_al, tester.name1, t1.name2, (t1.name1 + t1.name2) as nmnm FROM tester as t1 WHERE dig1 = 61 AND name1 = 'str61' AND name2 = 'str62'");
	//MakeQuery(ses, tinfo, "CREATE TABLE tester3 (id INT, name5 TEXT, name6 TEXT, dig5 INT, dig6 INT, dig1 INT, dig2 INT, PRIMARY KEY (id, name5), UNIQUE(id), INDEX(name5, name6), INDEX(dig5, dig6), INDEX(dig1, dig2));");

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

	TarantoolInfo tinfo(ses);

	test(ses, tinfo);

	test_create_table(ses, tinfo);
	test_insert_into(ses, tinfo);
	test_select_from(ses, tinfo);
	test_drop_table(ses, tinfo);
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
