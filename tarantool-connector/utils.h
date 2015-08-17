#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include "datastructure.h"
#include "logger.h"
#include "Expr.h"
#include <cstddef>
#include <cstdint>
extern "C" {
	#include "session.h"
	#include "tp.h"
}

std::string DebugStr(const std::string &file, int line, const std::string &msg);
#define MakeDbgStr(MSG) DebugStr(std::string(__FILE__), __LINE__, MSG)

using namespace hsql;

namespace Convert {

std::string ToString(const int &val);

std::string ToString(const int64_t &val);

std::string ToString(const uint32_t &val);

std::string ToString(const uint64_t &val);

std::string ToString(const double &val);

std::string ToString(const float &val);

std::string ToString(const DataStructure &val);

std::string ToString(const std::string &val);

std::string ToString(const tp_type &val);

tp_iterator_type ToIteratorType(char op_char);

tp_iterator_type ToIteratorType(Expr::OperatorType op_tp);

}

#endif