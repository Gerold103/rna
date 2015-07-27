#include "SQLStatement.h"

namespace hsql {

//~~~~~~~~~~~~~~~~~~~~~~~~ S Q L S t a t e m e n t   L i s t ~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

SQLStatementList::SQLStatementList() : isValid(true) { }

SQLStatementList::SQLStatementList(SQLStatement* stmt) : isValid(true) {
	addStatement(stmt);	
};

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

void SQLStatementList::addStatement(SQLStatement* stmt) { statements.push_back(stmt); }

bool SQLStatementList::SetParserMsg(const char *s) {
	if (s == NULL) return false;
	else parser_msg = s;
	return true;
}

//~~~~~~~~ G e t   m e t h o d s ~~~~~~~~

SQLStatement* SQLStatementList::getStatement(int id) { return statements[id]; }

size_t SQLStatementList::numStatements() const { return statements.size(); }

//~~~~~~~~ O t h e r ~~~~~~~~

SQLStatementList::~SQLStatementList() {
	for (size_t i = 0, size = statements.size(); i < size; ++i) {
		delete statements[i];
	}
}
}