
#include "Expr.h"
#include <stdio.h>
#include <string.h>

namespace hsql {

char* substr(const char* source, int from, int to) {
	if (source == NULL) {
		char *res = new char[1];
		res[0] = '\0';
		return res;
	}
	int len = to-from;
	char* copy = new char[len+1];
	strncpy(copy, source+from, len);
	copy[len] = '\0';
	return copy;
}

Expr::Expr(ExprType type)
	: ASTNode(ASTNode::EXPRESSION), type(type), expr(NULL), expr2(NULL), expr3(NULL), name(""),
	table(""), database(""), alias(""), fval(0), ival(0), ival2(0), op_type(Expr::DEFAULT), op_char('#'), args_number(0) { };

bool Expr::SetName(const char *s) {
	if (s == NULL) return false;
	else name = s;
	return true;
}
bool Expr::SetTable(const char *s) {
	if (s == NULL) return false;
	else table = s;
	return true;
}
bool Expr::SetDatabase(const char *s) {
	if (s == NULL) return false;
	else database = s;
	return true;
}
bool Expr::SetAlias(const char *s) {
	if (s == NULL) return false;
	else alias = s;
	return true;
}

int Expr::GetInt() const {
	return ival;
}

float Expr::GetFloat() const
{
	return fval;
}

const std::string &Expr::GetString() const
{
	return name;
}

int Expr::GetArgumentsNumber() const {
	return args_number;
}

ExprType Expr::GetType() const
{
	return type;
}

Expr::OperatorType Expr::GetOperator() const
{
	return op_type;
}

bool Expr::isType(ExprType e_type) { return e_type == type; }

bool Expr::isLiteral() { return isType(kExprLiteralInt) || isType(kExprLiteralFloat) || isType(kExprLiteralString) || isType(kExprPlaceholder); }

bool Expr::hasAlias() { return !alias.empty(); }

bool Expr::hasTable() { return !table.empty(); }

bool Expr::hasDatabase() { return !database.empty(); }

const std::string &Expr::getName() const {
	if (!alias.empty()) return alias;
	else return name;
}

const std::string &Expr::GetRealName() const
{
	return name;
}

std::string Expr::ToString() const
{
	//if (alias.length() > 0) return alias;
	switch(GetType()) {
		case ExprType::kExprLiteralFloat: {
			return std::to_string(GetFloat());
		}
		case ExprType::kExprLiteralString: {
			return "'" + GetRealName() + "'";
		}
		case ExprType::kExprLiteralInt: {
			return std::to_string(GetInt());
		}
		case ExprType::kExprStar: {
			return "*";
		}
		case ExprType::kExprPlaceholder: {
			return "kExprPlaceholder";
		}
		case ExprType::kExprColumnRef: {
			if (alias.length()) return alias;
			if (table.length()) {
				return table + "." + name;
			}
			return getName();
		}
		case ExprType::kExprFunctionRef: {
			std::string res;
			res = OperatorTypeToString(GetOperator()) + "(";
			if (GetArgumentsNumber() > 3) return "#error#";
			if (GetArgumentsNumber() >= 1) {
				res += expr->ToString();
				if (GetArgumentsNumber() >= 2) {
					res += ", " + expr2->ToString();
					if (GetArgumentsNumber() == 3) {
						res += ", " + expr3->ToString();
					}
				}
			}
			res += ")";
			return res;
		}
		case ExprType::kExprOperator: {
			std::string res;
			if (GetOperator() != Expr::SIMPLE_OP) {
				return "(" + expr->ToString() + " " + OperatorTypeToString(GetOperator()) + " " + expr2->ToString() + ")";
			}
			return "(" + expr->ToString() + " " + GetOperatorChar() + " " + expr2->ToString() + ")";
		}
	}
}

char Expr::GetOperatorChar() const
{
	return op_char;
}

bool Expr::isSimpleOp() { return op_type == SIMPLE_OP; }

bool Expr::isSimpleOp(char op) { return isSimpleOp() && op_char == op; }

std::string ExprTypeToString(ExprType tp)
{
	switch(tp)
	{
		case ExprType::kExprLiteralFloat:
			return "kExprLiteralFloat";
		case ExprType::kExprLiteralString:
			return "kExprLiteralString";
		case ExprType::kExprLiteralInt:
			return "kExprLiteralInt";
		case ExprType::kExprStar:
			return "kExprStar";
		case ExprType::kExprPlaceholder:
			return "kExprPlaceholder";
		case ExprType::kExprColumnRef:
			return "kExprColumnRef";
		case ExprType::kExprFunctionRef:
			return "kExprFunctionRef";
		case ExprType::kExprOperator:
			return "kExprOperator";
		default:
			return "#undef#";
	}
}


std::string Expr::OperatorTypeToString(OperatorType tp)
{
	switch(tp)
	{
		case Expr::OperatorType::SIMPLE_OP:
			return "SIMPLE_OP";
		case Expr::OperatorType::IF:
			return "IF";
		case Expr::OperatorType::NOT_EQUALS:
			return "NOT_EQUALS";
		case Expr::OperatorType::LESS_EQ:
			return "LESS_EQ";
		case Expr::OperatorType::GREATER_EQ:
			return "GREATER_EQ";
		case Expr::OperatorType::LIKE:
			return "LIKE";
		case Expr::OperatorType::NOT_LIKE:
			return "NOT_LIKE";
		case Expr::OperatorType::AND:
			return "AND";
		case Expr::OperatorType::OR:
			return "OR";
		case Expr::OperatorType::NOT:
			return "NOT";
		case Expr::OperatorType::UMINUS:
			return "UMINUS";
		case Expr::OperatorType::ISNULL:
			return "ISNULL";
		case Expr::OperatorType::SUBSTRING:
			return "SUBSTRING";
		case Expr::OperatorType::DEFAULT:
			return "DEFAULT";
		default:
			return "#undef#";
	}
}

Expr* Expr::makeOpUnary(OperatorType op, Expr* expr) {
	Expr* e = new Expr(kExprOperator);
	e->op_type = op;
	e->expr = expr;
	e->expr2 = NULL;
	return e;
}



Expr* Expr::makeOpBinary(Expr* expr1, OperatorType op, Expr* expr2) {
	Expr* e = new Expr(kExprOperator);
	e->op_type = op;
	e->op_char = 0;
	e->expr = expr1;
	e->expr2 = expr2;
	return e;
}

Expr* Expr::makeOpBinary(Expr* expr1, char op, Expr* expr2) {
	Expr* e = new Expr(kExprOperator);
	e->op_type = SIMPLE_OP;
	e->op_char = op;
	e->expr = expr1;
	e->expr2 = expr2;
	return e;
}

Expr* Expr::makeOpTernary(Expr* expr1, Expr* expr2, Expr* expr3, OperatorType tp) {
	Expr* e = new Expr(kExprOperator);
	e->op_type = tp;
	e->op_char = 0;
	e->expr = expr1;
	e->expr2 = expr2;
	e->expr3 = expr3;
	return e;
}

Expr* Expr::makeLiteral(int64_t val) {
	Expr* e = new Expr(kExprLiteralInt);
	e->ival = val;
	return e;
}

Expr* Expr::makeLiteral(double value) {
	Expr* e = new Expr(kExprLiteralFloat);
	e->fval = value;
	return e;
}

Expr* Expr::makeLiteral(std::string string) {
	Expr* e = new Expr(kExprLiteralString);
	e->name = string;
	return e;
}


Expr* Expr::makeColumnRef(const char *name) {
	Expr* e = new Expr(kExprColumnRef);
	e->SetName(name);
	return e;
}

Expr* Expr::makeColumnRef(const char *table, const char *name) {
	Expr* e = new Expr(kExprColumnRef);
	e->SetName(name);
	e->SetTable(table);
	return e;
}

Expr* Expr::makeColumnRef(const char *database, const char *table, const char *name)
{
	Expr *e = new Expr(kExprColumnRef);
	e->SetDatabase(database);
	e->SetName(name);
	e->SetTable(table);
	return e;
}

Expr* Expr::makeFunctionRef(const char * func_name, Expr* expr, bool distinct) {
	Expr* e = new Expr(kExprFunctionRef);
	e->SetName(func_name);
	e->expr = expr;
	e->distinct = distinct;
	return e;
}

Expr *Expr::makeFunctionRef(Expr::OperatorType func_type, Expr *expr_1)
{
	Expr *e = new Expr(kExprFunctionRef);
	e->op_type = func_type;
	e->expr = expr_1;
	e->args_number = 1;
	return e;
}
Expr *Expr::makeFunctionRef(Expr::OperatorType func_type, Expr *expr_1, Expr *expr_2)
{
	Expr *e = new Expr(kExprFunctionRef);
	e->op_type = func_type;
	e->expr = expr_1;
	e->expr2 = expr_2;
	e->args_number = 2;
	return e;
}
Expr *Expr::makeFunctionRef(Expr::OperatorType func_type, Expr *expr_1, Expr *expr_2, Expr *expr_3)
{
	Expr *e = new Expr(kExprFunctionRef);
	e->op_type = func_type;
	e->expr = expr_1;
	e->expr2 = expr_2;
	e->expr3 = expr_3;
	e->args_number = 3;
	return e;
}

Expr* Expr::makePlaceholder(int id) {
	Expr* e = new Expr(kExprPlaceholder);
	e->ival = id;
	return e;
}

Expr::~Expr() {
	delete expr;
	delete expr2;
	delete expr3;
}

std::ostream &operator<<(std::ostream &stream, const Expr &ob)
{
	stream << "Expr: " << std::endl;
	stream << "expr_type: " << ExprTypeToString(ob.type) << std::endl;
	stream << "op_type: " << Expr::OperatorTypeToString(ob.op_type) << std::endl;
	stream << "op_char: " << ob.op_char << "; distinct: " << ob.distinct << std::endl;
	if (ob.expr != NULL) stream << "expr: " << *(ob.expr) << std::endl;
	if (ob.expr2 != NULL) stream << "expr2: " << *(ob.expr2) << std::endl;
	if (ob.expr3 != NULL) stream << "expr3: " << *(ob.expr3) << std::endl;
	if (!ob.name.empty()) stream << "name: " << ob.name << std::endl;
	if (!ob.table.empty()) stream << "table: " << ob.table << std::endl;
	if (!ob.database.empty()) stream << "database: " << ob.database << std::endl;
	if (!ob.alias.empty()) stream << "alias: " << ob.alias << std::endl;
	stream << "fval: " << ob.fval << "; ival: " << ob.ival << "; ival2: " << ob.ival2 << std::endl;
	return stream;
} 

} // namespace hsql