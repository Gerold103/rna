#ifndef __TABLEREF_H__
#define __TABLEREF_H__

#include "Expr.h"
#include <stdio.h>
#include <vector>
#include <ostream>
#include <string>
#include <iostream>

namespace hsql {

struct SelectStatement;
struct JoinDefinition;
struct TableRef;


/**
 * @enum TableRefType
 * Types table references
 */
typedef enum {
	kTableName,
	kTableSelect,
	kTableJoin,
	kTableCrossProduct
} TableRefType;

std::string TableRefTypeToString(TableRefType tp);

struct TableName {
	std::string name;
	std::string database;
	bool SetName(const char *s) {
		if (s == NULL) return false;
		else name = s;
		return true;
	}
	bool SetDatabase(const char *s) {
		if (s == NULL) return false;
		else database = s;
		return true;
	}
	bool empty() const { return name.empty() && (database.empty()); }
	operator std::string() const
	{
		if (database.empty()) return name;
		else return database + "." + name;
	}
	TableName() {}
	TableName(std::string n) : name(n) { }
	TableName(std::string n, std::string d) : name(n), database(d) { }
	TableName(const char *n) { SetName(n); }
	TableName(const char *n, const char *d) { SetName(n); SetDatabase(d); }
};


/**
 * @struct TableRef
 * @brief Holds reference to tables. Can be either table names or a select statement.
 */
struct TableRef {
	TableRef(TableRefType type) :
		type(type),
		name(NULL),
		select(NULL),
		list(NULL),
		prev_join(NULL),
		join(NULL),
		next_join(NULL)
		{}
		
	virtual ~TableRef();

	TableRefType type;

	bool SetSchema(const char *s) {
		if (s == NULL) return false;
		else schema = s;
		return true;
	}
	bool SetAlias(const char *s) {
		if (s == NULL) return false;
		else alias = s;
		return true;
	}

	std::string schema;
	TableName* name;
	std::string alias;

	SelectStatement* select;
	std::vector<TableRef*>* list;
	TableRef* prev_join;
	JoinDefinition* join;
	TableRef* next_join;


	/**
	 * Convenience accessor methods
	 */
	inline bool hasSchema() { return !schema.empty(); }

	inline std::string getName() {
		if (!alias.empty()) return alias;
		else return std::string(*name);
	}
};


/**
 * @enum JoinType
 * Types of joins
 */ 
typedef enum {
	kJoinInner,
	kJoinOuter,
	kJoinLeft,
	kJoinRight,
} JoinType;

std::string JoinTypeToString(JoinType tp);

/**
 * @struct JoinDefinition
 * @brief Definition of a join table
 */
struct JoinDefinition {
	JoinDefinition() :
		left(NULL),
		right(NULL),
		condition(NULL),
		type(kJoinInner) {}

	virtual ~JoinDefinition() {
		delete left;
		delete right;
		delete condition;
	}

	TableRef* left;
	TableRef* right;

	Expr* condition;

	JoinType type;
};

std::ostream &operator<<(std::ostream &stream, const TableName &ob);

std::ostream &operator<<(std::ostream &stream, const JoinDefinition &ob);

std::ostream &operator<<(std::ostream &stream, const TableRef &ref);

} // namespace hsql
#endif
