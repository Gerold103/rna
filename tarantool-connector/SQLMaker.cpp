#include "SQLMaker.h"

#include <algorithm>
#include <sstream>

//~~~~~~~~~~~~~~~~~~~~~~~~ S Q L M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

SQLMaker::SQLMaker(Session &ses_, TarantoolInfo &tinfo_) : ses(&ses_), tinfo(&tinfo_) { }

bool SQLMaker::MakeEval(const std::string &function)
{
	last_error.clear();
	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));
	request->AddEval(function);
	
	int ss = ses->Send(*request);
	if (ss < 0) {
		last_error = "SQLMaker::MakeEval(): error while sending request was occured, return = " + std::to_string(ss);
		LogFL(DEBUG) << last_error << "\n";
		return false;
	}
	ss = ses->Sync();
	if (ss < 0) {
		last_error = "SQLMaker::MakeEval(): error while syncing sesson was occured, return = " + std::to_string(ss);
		LogFL(DEBUG) << last_error << "\n";
		return false;
	}
	return true;
}

std::string SQLMaker::Error() const
{
	return last_error;
}

SQLMaker::~SQLMaker() { }

//~~~~~~~~~~~~~~~~~~~~~~~~ S Q L   C O N D I T I O N ~~~~~~~~~~~~~~~~~~~~~~~~

SQLCondition::SQLCondition(const std::string column_, const MValue &value_, tp_iterator_type type_) : type(type_), column(column_), value(value_) { }

SQLCondition::SQLCondition() { }

SQLCondition::SQLCondition(const Expr *condition)
{
	MValue value_;
	std::string column_;
	switch(condition->expr->type) {
		case ExprType::kExprColumnRef: {
			column_ = condition->expr->GetString();
			break;
		}
		default: return;
	}
	switch(condition->expr2->type) {
		case ExprType::kExprLiteralFloat: {
			value_.SetValue(condition->expr2->GetFloat());
			break;
		}
		case ExprType::kExprLiteralString: {
			value_.SetValue(condition->expr2->GetString());
			break;
		}
		case ExprType::kExprLiteralInt: {
			value_.SetValue(condition->expr2->GetInt());
			break;
		}
		default: return;
	}
	value = value_;
	column = column_;
}