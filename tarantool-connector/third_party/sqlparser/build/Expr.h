#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <stdlib.h>
#include <memory>
#include <string>
#include <ostream>
#include <iostream>
#include "astnode.h"

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
struct Expr : public ASTNode {
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
		SUBSTRING,

		DEFAULT,
	} OperatorType;

	static std::string OperatorTypeToString(OperatorType tp);



	Expr(ExprType type);

	~Expr();
	
	ExprType type;

	Expr* expr;
	Expr* expr2;
	Expr* expr3;

	int args_number;

	bool SetName(const char *s);
	bool SetTable(const char *s);
	bool SetDatabase(const char *s);
	bool SetAlias(const char *s);

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

	int GetInt() const;
	float GetFloat() const;
	const std::string &GetString() const;
	int GetArgumentsNumber() const;


	/**
	 * Convenience accessor methods
	 */

	ExprType GetType() const;
	OperatorType GetOperator() const;
	bool isType(ExprType e_type);
	bool isLiteral();
	bool hasAlias();
	bool hasTable();
	bool hasDatabase();
	const std::string &getName() const;
	const std::string &GetRealName() const;
	std::string ToString() const;
	char GetOperatorChar() const;
	bool isSimpleOp();
	bool isSimpleOp(char op);


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
	static Expr *makeFunctionRef(Expr::OperatorType func_type, Expr *expr_1);
	static Expr *makeFunctionRef(Expr::OperatorType func_type, Expr *expr_1, Expr *expr_2);
	static Expr *makeFunctionRef(Expr::OperatorType func_type, Expr *expr_1, Expr *expr_2, Expr *expr_3);

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
