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

//~~~~~~~~~~~~~~~~~~~~~~~~ O R D E R   D E S C R I P T I O N ~~~~~~~~~~~~~~~~~~~~~~~~

struct OrderDescription {
	OrderDescription(OrderType type, Expr* expr);
		
	virtual ~OrderDescription();

	OrderType type;
	Expr* expr;	
};

std::ostream &operator<<(std::ostream &stream, const OrderDescription &ob);

//~~~~~~~~~~~~~~~~~~~~~~~~ L I M I T   D E S C R I P T I O N ~~~~~~~~~~~~~~~~~~~~~~~~

const int64_t kNoLimit = -1;
const int64_t kNoOffset = -1;
struct LimitDescription {
	LimitDescription(int64_t limit, int64_t offset);

	int64_t limit;
	int64_t offset;	
};

std::ostream &operator<<(std::ostream &stream, const LimitDescription &ob);

/**
 * @struct GroupByDescription
 */
struct GroupByDescription {
	GroupByDescription();

	~GroupByDescription();

	std::vector<Expr*>* columns;
	Expr* having;
};

//~~~~~~~~~~~~~~~~~~~~~~~~ S E L E C T   S T A T E M E N T ~~~~~~~~~~~~~~~~~~~~~~~~

struct SelectStatement : SQLStatement {
	SelectStatement();

	virtual ~SelectStatement();

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