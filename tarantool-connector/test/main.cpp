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

using namespace hsql;


int main()
{
	Logger::SetStreamForMessages(TEST, std::cout);

	test_create_table();
	test_insert_into();
	test_select_from();
	test_drop_table();

	std::string sql_query;

	std::cout << "sql_tarantool> " << std::flush;

	while(std::getline(std::cin, sql_query)) {
		MakeQuery(sql_query);
		std::cout << "sql_tarantool> " << std::flush;
	}

	return 0;
}