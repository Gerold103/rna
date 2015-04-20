#ifndef __UPDATE_STATEMENT_H__
#define __UPDATE_STATEMENT_H__

#include "SQLStatement.h"

namespace hsql {


/**
 * @struct UpdateClause
 * @brief Represents "column = value" expressions
 */
struct UpdateClause {
	bool SetColumn(const char *s) {
		if (s == NULL) return false;
		else column = s;
		return true;
	}
	std::string column;
	~UpdateClause() {}//{ delete value; }
	Expr* value;
};


/**
 * @struct UpdateStatement
 * @brief Represents "UPDATE"
 */
struct UpdateStatement : SQLStatement {
	UpdateStatement() :
		SQLStatement(kStmtUpdate),
		table(NULL),
		updates(NULL),
		where(NULL) {}
	
	virtual ~UpdateStatement() {
		delete table;
		if (updates != NULL) {
			for (size_t i = 0, size = updates->size(); i < size; ++i) {
				delete updates->at(i);
			}
		}
		delete updates;
		delete where;
	}

	// TODO: switch to char* instead of TableRef
	TableRef* table;
	std::vector<UpdateClause*>* updates;
	Expr* where;
};



} // namsepace hsql
#endif