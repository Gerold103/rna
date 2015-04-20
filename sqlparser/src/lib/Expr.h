#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <stdlib.h>
#include <memory>
#include <string>
#include <ostream>
#include <iostream>

namespace hsql {

char* substr(const char* source, int from, int to);

typedef enum {
	kExprLiteralFloat,
	kExprLiteralString,
	kExprLiteralInt,
	kExprStar,
	kExprPlaceholder,
	kExprColumnRef,
	kExprFunctionRef,
	kExprOperator
} ExprType;

std::string ExprTypeToString(ExprType tp);

typedef struct Expr Expr;

/** 
 * @class Expr
 * @brief Represents SQL expressions (i.e. literals, operators, column_refs)
 */
struct Expr {
	/**
	 * Operator types. These are important for expressions of type kExprOperator
	 * Trivial types are those that can be described by a single character e.g:
	 * + - * / < > = %
	 * Non-trivial are:
	 * <> <= >= LIKE ISNULL NOT
	 */
	typedef enum {
		SIMPLE_OP,
		// Binary
		NOT_EQUALS,
		LESS_EQ,
		GREATER_EQ,
		LIKE,
		NOT_LIKE,
		AND,
		OR,
		// Unary
		NOT,
		UMINUS,
		ISNULL,
		ISNOTNULL,
		//Functons
		IF,
		SUBSTRING_INDEX,
	} OperatorType;

	static std::string OperatorTypeToString(OperatorType tp);



	Expr(ExprType type) :
		type(type),
		expr(NULL),
		expr2(NULL),
		expr3(NULL),
		name(""),
		table(""),
		database(""),
		alias("") {};

	~Expr();
	
	ExprType type;

	Expr* expr;
	Expr* expr2;
	Expr* expr3;

	bool SetName(const char *s) {
		if (s == NULL) return false;
		else name = s;
		return true;
	}
	bool SetTable(const char *s) {
		if (s == NULL) return false;
		else table = s;
		return true;
	}
	bool SetDatabase(const char *s) {
		if (s == NULL) return false;
		else database = s;
		return true;
	}
	bool SetAlias(const char *s) {
		if (s == NULL) return false;
		else alias = s;
		return true;
	}

	std::string name;
	std::string table;
	std::string database;
	std::string alias;
	float fval;
	int64_t ival;
	int64_t ival2;

	OperatorType op_type;
	char op_char;
	bool distinct;


	/**
	 * Convenience accessor methods
	 */
	inline bool isType(ExprType e_type) { return e_type == type; }
	inline bool isLiteral() { return isType(kExprLiteralInt) || isType(kExprLiteralFloat) || isType(kExprLiteralString) || isType(kExprPlaceholder); }
	inline bool hasAlias() { return !alias.empty(); }
	inline bool hasTable() { return !table.empty(); }
	inline bool hasDatabase() { return !database.empty(); }
	inline const std::string &getName() {
		if (!alias.empty()) return alias;
		else return name;
	}
	inline bool isSimpleOp() { return op_type == SIMPLE_OP; }
	inline bool isSimpleOp(char op) { return isSimpleOp() && op_char == op; }


	/**
	 * Static expression constructors
	 */
	static Expr* makeOpUnary(OperatorType op, Expr* expr);
	static Expr* makeOpBinary(Expr* expr1, char op, Expr* expr2);
	static Expr* makeOpBinary(Expr* expr1, OperatorType op, Expr* expr2);
	static Expr* makeOpTernary(Expr* expr1, Expr* expr2, Expr* expr3, OperatorType tp);

	static Expr* makeLiteral(int64_t val);
	static Expr* makeLiteral(double val);
	static Expr* makeLiteral(std::string val);

	static Expr* makeColumnRef(const char *name);
	static Expr* makeColumnRef(const char *table, const char *name);
	static Expr* makeColumnRef(const char *database, const char *table, const char *name);
	static Expr* makeFunctionRef(const char *func_name, Expr* expr, bool distinct);

	static Expr* makePlaceholder(int id);
};

#define ALLOC_EXPR(var, type) 		\
	Expr* var;						\
	do {							\
		Expr zero = {type};			\
		var = (Expr*)malloc(sizeof *var);	\
		*var = zero;				\
	} while(0);
#undef ALLOC_EXPR

std::ostream &operator<<(std::ostream &stream, const Expr &ob);

} // namespace hsql

#endif
