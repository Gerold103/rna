/*
 * SQLStatement.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "Expr.h"
#include "astnode.h"
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
struct SQLStatement : public ASTNode {
	SQLStatement(StatementType type) :
		ASTNode(ASTNode::STATEMENT), _type(type) {};

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
	std::vector<SQLStatement*> statements;
	bool isValid;
	std::string parser_msg;
	int error_line;
	int error_col;

	//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

	SQLStatementList();

	SQLStatementList(SQLStatement* stmt);

	//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

	void addStatement(SQLStatement* stmt);

	bool SetParserMsg(const char *s);

	//~~~~~~~~ G e t   m e t h o d s ~~~~~~~~

	SQLStatement* getStatement(int id);
	size_t numStatements() const;

	//~~~~~~~~ O t h e r ~~~~~~~~

	virtual ~SQLStatementList();
};



} // namespace hsql

#endif // __STATEMENT_H__
