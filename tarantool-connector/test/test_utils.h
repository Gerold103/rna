#ifndef TEST_UTILS_H_INCLUDED
#define TEST_UTILS_H_INCLUDED

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
#include <cstddef>
#include <cstdint>
extern "C" {
	#include <session.h>
	#include <tp.h>
}

using namespace hsql;


void hexDump(char *desc, void *addr, int len);

MValue MakeQuery(const std::string &sql_query);

#endif