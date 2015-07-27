#ifndef __SELECT_STATEMENT_H__
#define __SELECT_STATEMENT_H__

#include "SQLStatement.h"
#include "Expr.h"
#include "Table.h"

#include <ostream>
#include <vector>
#include <string>
#include <iostream>

namespace hsql {



/**
 * @struct OrderDescription
 * @brief Description of the order by clause within a select statement
 */
typedef enum {
	kOrderAsc,
	kOrderDesc
} OrderType;

std::string OrderTypeToString(OrderType tp);

struct OrderDescription {
	OrderDescription(OrderType type, Expr* expr) :
		type(type),
		expr(expr) {}
		
	virtual ~OrderDescription() {
		delete expr;
	}

	OrderType type;
	Expr* expr;	
};

std::ostream &operator<<(std::ostream &stream, const OrderDescription &ob);

/**
 * @struct LimitDescription
 * @brief Description of the limit clause within a select statement
 */
const int64_t kNoLimit = -1;
const int64_t kNoOffset = -1;
struct LimitDescription {
	LimitDescription(int64_t limit, int64_t offset) :
		limit(limit),
		offset(offset) {}

	int64_t limit;
	int64_t offset;	
};

std::ostream &operator<<(std::ostream &stream, const LimitDescription &ob);

/**
 * @struct GroupByDescription
 */
struct GroupByDescription {
	GroupByDescription() : 
		columns(NULL),
		having(NULL) {}

	~GroupByDescription() {
		if (columns != NULL) {
			for(size_t i = 0, size = columns->size(); i < size; ++i) {
				delete columns->at(i);
			}
		}
		delete columns;
		delete having;
	}

	std::vector<Expr*>* columns;
	Expr* having;
};

/**
 * @struct SelectStatement
 * @brief Representation of a full select statement.
 * 
 * TODO: add union_order and union_limit
 */
struct SelectStatement : SQLStatement {
	SelectStatement() : 
		SQLStatement(kStmtSelect),
		from_table(NULL),
		select_list(NULL),
		where_clause(NULL),
		group_by(NULL),
		union_select(NULL),
		order(NULL),
		limit(NULL) {};

	virtual ~SelectStatement() {
		delete from_table;
		if (select_list != NULL) {
			for (size_t i = 0, size = select_list->size(); i < size; ++i) {
				delete select_list->at(i);
			}
		}
		delete select_list;
		delete where_clause;
		delete group_by;
		delete order;
		delete limit;
	}

	TableRef* from_table;
	bool select_distinct;
	std::vector<Expr*>* select_list;
	Expr* where_clause;	
	GroupByDescription* group_by;

	SelectStatement* union_select;
	OrderDescription* order;
	LimitDescription* limit;

	
};

std::ostream &operator<<(std::ostream &stream, const SelectStatement &ob);


} // namespace hsql

#endif