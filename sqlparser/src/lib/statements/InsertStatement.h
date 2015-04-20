#ifndef __INSERT_STATEMENT_H__
#define __INSERT_STATEMENT_H__

#include "SQLStatement.h"
#include "SelectStatement.h"

namespace hsql {


/**
 * @struct InsertStatement
 * @brief Represents "INSERT INTO students VALUES ('Max', 1112233, 'Musterhausen', 2.3)"
 */
struct InsertStatement : SQLStatement {
	enum InsertType {
		kInsertValues,
		kInsertSelect
	};

	InsertStatement(InsertType type) :
		SQLStatement(kStmtInsert),
		type(type),
		table_name(NULL),
		columns(NULL),
		values(NULL),
		select(NULL) {}
	
	virtual ~InsertStatement() {
		delete columns;
		if (values != NULL) {
			for (size_t i = 0, size = values->size(); i < size; ++i) {
				delete values->at(i);
			}
		}
		delete values;
		delete select;
	}

	InsertType type;
	TableName *table_name;
	std::vector<std::string>* columns;
	std::vector<Expr*>* values;
	SelectStatement* select;
};




} // namsepace hsql
#endif