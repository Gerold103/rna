#ifndef __INSERT_STATEMENT_H__
#define __INSERT_STATEMENT_H__

#include "SQLStatement.h"
#include "SelectStatement.h"

#include <ostream>

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

	InsertType type;
	TableName *table_name;
	std::vector<std::string>* columns;
	std::vector<Expr*>* values;
	SelectStatement* select;

	InsertStatement(InsertType type);

	InsertType GetType() const;
	const std::string GetName() const;
	const std::string GetDatabase() const;
	const std::vector<std::string> *GetColumns() const;
	const std::vector<Expr*> *GetValues() const;
	const SelectStatement *GetSelectStatement() const;

	const Expr *GetValueByColumn(const std::string &col_name);
	
	virtual ~InsertStatement();
};

std::string InsertTypeToString(InsertStatement::InsertType tp);

std::ostream &operator<<(std::ostream &stream, const InsertStatement &ob);


} // namsepace hsql
#endif