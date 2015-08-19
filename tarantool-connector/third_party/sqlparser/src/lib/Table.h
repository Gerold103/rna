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
	bool SetName(const char *s);
	bool SetDatabase(const char *s);
	bool empty() const;
	operator std::string() const;
	TableName();
	TableName(std::string n);
	TableName(std::string n, std::string d);
	TableName(const char *n);
	TableName(const char *n, const char *d);

	const std::string &GetName() const;
	const std::string &GetDatabase() const;
};


/**
 * @struct TableRef
 * @brief Holds reference to tables. Can be either table names or a select statement.
 */
struct TableRef : public ASTNode {
	TableRef(TableRefType type);
		
	virtual ~TableRef();

	TableRefType type;

	bool SetSchema(const char *s);
	bool SetAlias(const char *s);

	std::string schema;
	TableName* name;
	std::string alias;

	SelectStatement* select;
	std::vector<TableRef*>* list;
	TableRef* prev_join;
	JoinDefinition* join;
	TableRef* next_join;

	bool hasSchema();

	std::string getName();

	TableRefType GetType() const;
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
	JoinDefinition();

	virtual ~JoinDefinition();

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
