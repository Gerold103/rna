#include "utils.h"
#include <algorithm>

std::string DebugStr(const std::string &file, int line, const std::string &msg)
{
	return "File: " + file + ", line: " + std::to_string(line) + ": " + msg;
}

namespace Convert {
std::string ToString(const int &val)
{
	return std::to_string(val);
}

std::string ToString(const int64_t &val)
{
	return std::to_string(val);
}

std::string ToString(const uint32_t &val)
{
	return std::to_string(val);
}

std::string ToString(const uint64_t &val)
{
	return std::to_string(val);
}

std::string ToString(const double &val)
{
	return std::to_string(val);
}

std::string ToString(const float &val)
{
	return std::to_string(val);
}

std::string ToString(const DataStructure &val)
{
	return "DataStructure: size = " + std::to_string(val.Size());
}

std::string ToString(const std::string &val)
{
	return val;
}

std::string ToString(const tp_type &val)
{
	std::string res("TP_");
	switch (val) {
		case TP_NIL:
			res += "NIL"; break;
		case TP_UINT:
			res += "UINT"; break;
		case TP_INT:
			res += "INT"; break;
		case TP_STR:
			res += "STR"; break;
		case TP_BIN:
			res += "BIN"; break;
		case TP_ARRAY:
			res += "ARRAY"; break;
		case TP_MAP:
			res += "MAP"; break;
		case TP_BOOL:
			res += "BOOL"; break;
		case TP_FLOAT:
			res += "FLOAT"; break;
		case TP_DOUBLE:
			res += "DOUBLE"; break;
		case TP_EXT:
			res += "EXT"; break;
		case TP_DEFAULT:
			res += "DEFAULT"; break;
	}
	return res;
}

tp_iterator_type ToIteratorType(char op_char)
{
	switch(op_char) {
		case '=': return TP_ITERATOR_EQ;
		case '>': return TP_ITERATOR_GT;
		case '<': return TP_ITERATOR_LT;
		default: return TP_ITERATOR_ALL;
	}
}

tp_iterator_type ToIteratorType(Expr::OperatorType op_tp)
{
	switch(op_tp) {
		case Expr::LESS_EQ: return TP_ITERATOR_LE;
		case Expr::GREATER_EQ: return TP_ITERATOR_GE;
		default: return TP_ITERATOR_ALL;
	}
}

}