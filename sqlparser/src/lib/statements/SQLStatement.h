/*
 * SQLStatement.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "Expr.h"
#include <vector>
#include <iostream>

namespace hsql {


typedef enum {
	kStmtError,
	kStmtSelect,
	kStmtImport,
	kStmtInsert,
	kStmtUpdate,
	kStmtDelete,
	kStmtCreate,
	kStmtDrop,
	kStmtPrepare,
	kStmtExecute,
	kStmtExport,
	kStmtRename,
	kStmtAlter
} StatementType;


/**
 * @struct SQLStatement
 * @brief Base class for every SQLStatement
 */
struct SQLStatement {
	SQLStatement(StatementType type) :
		_type(type) {};

	virtual ~SQLStatement() {}

	virtual StatementType type() { return _type; }

private:
	StatementType _type;
};


/**
 * @struct SQLStatementList
 * @brief Represents the result of the SQLParser. If parsing was successful it is a list of SQLStatement.
 */
struct SQLStatementList {
public:
	SQLStatementList() :
		isValid(true) {}

	SQLStatementList(SQLStatement* stmt) :
		isValid(true) {
		addStatement(stmt);	
	};
		
	virtual ~SQLStatementList() {
		std::cout << "~SQLStatementList()" << std::endl;
		for (size_t i = 0, size = statements.size(); i < size; ++i) {
			delete statements[i];
		}
	}

	void addStatement(SQLStatement* stmt) { statements.push_back(stmt); }
	SQLStatement* getStatement(int id) { return statements[id]; }
	size_t numStatements() { return statements.size(); }

	bool SetParserMsg(const char *s) {
		if (s == NULL) return false;
		else parser_msg = s;
		return true;
	}
	
	std::vector<SQLStatement*> statements;
	bool isValid;
	std::string parser_msg;
	int error_line;
	int error_col;
};



} // namespace hsql

#endif // __STATEMENT_H__
