#ifndef __EXECUTE_STATEMENT_H__
#define __EXECUTE_STATEMENT_H__

#include "SQLStatement.h"

namespace hsql {


/**
 * @struct ExecuteStatement
 * @brief Represents "EXECUTE ins_prep(100, "test", 2.3);"
 */
struct ExecuteStatement : SQLStatement {
	ExecuteStatement() :
		SQLStatement(kStmtExecute),
		name(""),
		parameters(NULL) {}
	
	virtual ~ExecuteStatement() {
		if (parameters != NULL) {
			for (size_t i = 0, size = parameters->size(); i < size; ++i) {
				delete parameters->at(i);
			}
		}
		delete parameters;
	}

	bool SetName(const char *s) {
		if (s == NULL) return false;
		else name = s;
		return true;
	}

	std::string name;
	std::vector<Expr*>* parameters;
};




} // namsepace hsql
#endif