#include "SelectMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ S E L E C T   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

SelectMaker::SelectMaker(SelectStatement *_statement) : SQLMaker(), statement(_statement) {
	Logger::LogObject(DEBUG, *statement);

	if (statement->select_list != NULL) {
		for (int i = 0, size = static_cast<int>(statement->select_list->size()); i < size; ++i) {
			Expr *tmp = statement->select_list->at(i);
			if (tmp->alias.length() > 0) {
				aliases.insert(std::make_pair(tmp->alias, i));
				std::cout << "alias: " << tmp->alias << ", i = " << i << std::endl;
			}
			switch(tmp->GetType()) {
				case ExprType::kExprColumnRef: {
					aliases.insert(std::make_pair(tmp->GetRealName(), i));
					std::cout << "alias: " << tmp->GetRealName() << ", i = " << i << std::endl;
				}
				default: continue;
			}
		}
	}
}

#define _op_result(op) CalculateFromTupleAndExpr(expr->expr, tuple, original_tuple, schm, space_id, aliases) op CalculateFromTupleAndExpr(expr->expr2, tuple, original_tuple, schm, space_id, aliases)

MValue CalculateFromTupleAndExpr(Expr *expr, const TupleObj &tuple, const TupleObj &original_tuple, const TarantoolSchema &schm, int space_id, const std::map<std::string, int> &aliases = std::map<std::string, int>())
{
	if (expr == NULL) return MValue();
	switch(expr->GetType()) {
		case ExprType::kExprLiteralFloat: return MValue(expr->GetFloat());
		case ExprType::kExprLiteralString: return MValue(expr->GetRealName());
		case ExprType::kExprLiteralInt: return MValue(expr->GetInt());
		case ExprType::kExprStar: return MValue();
		case ExprType::kExprPlaceholder: return MValue();
		case ExprType::kExprColumnRef: {
			int num = schm.GetColNumberByName(space_id, expr->GetRealName());
			if (num == -1) {
				auto tmp = aliases.find(expr->GetRealName());
				if (tmp == aliases.end()) {
					LogFL(DEBUG) << "CalculateFromTupleAndExpr(): column with name \"" << expr->GetRealName() << "\" was not found\n";
					return MValue();
				}
				std::cout << "num = -1: " << tmp->first << ", " << tmp->second << std::endl;
				std::cout << tuple[tmp->second] << std::endl;
				return tuple[tmp->second];
			}
			std::cout << "num = " << num << ", orig.tpl: " << original_tuple << std::endl;
			return original_tuple[num];
		}
		case ExprType::kExprOperator: {
			switch(expr->GetOperator()) {
				case Expr::AND: 		return _op_result(&&);
				case Expr::OR: 			return _op_result(||);
				case Expr::NOT_EQUALS: 	return _op_result(!=);
				case Expr::GREATER_EQ: 	return _op_result(>=);
				case Expr::LESS_EQ: 	return _op_result(<=);
				case Expr::SIMPLE_OP: {
					switch(expr->GetOperatorChar()) {
						case '=': return _op_result(==);
						case '+': return _op_result(+);
						case '-': return _op_result(-);
						case '*': return _op_result(*);
						case '/': return _op_result(/);
						case '%': return _op_result(%);
						case '>': return _op_result(>);
						case '<': return _op_result(<);
						default: {
							LogFL(DEBUG) << "CalculateFromTupleAndExpr(): operator char is undefined and equal = '" << expr->GetOperatorChar() << "'\n";
							return MValue();
						}
					}
				}
				default: {
					LogFL(DEBUG) << "CalculateFromTupleAndExpr(): expr type is undefided and equal = " << Expr::OperatorTypeToString(expr->GetOperator()) << ".\n";
					Logger::LogObject(DEBUG, *expr);
					return MValue();
				}
			}
		}
		case ExprType::kExprFunctionRef: {
			switch(expr->GetOperator()) {
				case Expr::SUBSTRING: {
					switch(expr->GetArgumentsNumber()) {
						case 2: {
							Expr *first = expr->expr;
							Expr *second = expr->expr2;
							MValue left, right;
							switch(first->GetType()) {
								case ExprType::kExprLiteralString: left = MValue(first->GetRealName()); break;
								case ExprType::kExprColumnRef: left = tuple[schm.GetColNumberByName(space_id, first->GetRealName())]; break;
								default: {
									LogFL(DEBUG) << "CalculateFromTupleAndExpr(): first argument of function SUBSTRING has incorrect type\n";
									return MValue();
								}
							}
							switch(second->GetType()) {
								case ExprType::kExprLiteralInt: right = MValue(second->GetInt()); break;
								case ExprType::kExprColumnRef: right = tuple[schm.GetColNumberByName(space_id, second->GetRealName())]; break;
								default: {
									LogFL(DEBUG) << "CalculateFromTupleAndExpr(): second argument of function SUBSTRING has incorrect type\n";
									return MValue();
								}
							}
							if (left.GetType() != TP_STR) {
								LogFL(DEBUG) << "CalculateFromTupleAndExpr(): first argument of function SUBSTRING must have string type, but type is " << ToString(left.GetType()) << "\n";
								return MValue();
							}
							switch(right.GetType()) {
								case TP_INT: {
									if (right.GetInt() >= left.GetStr().length()) return MValue(std::string());
									return MValue(left.GetStr().substr(right.GetInt() - 1));
								}
								case TP_UINT: {
									if (right.GetUint() >= left.GetStr().length()) return MValue(std::string());
									return MValue(left.GetStr().substr(static_cast<int>(right.GetUint()) - 1));
								}
								default: {
									LogFL(DEBUG) << "CalculateFromTupleAndExpr(): type fo second argument of SUBSTRING must be int or uint, but type is " << ToString(right.GetType()) << "\n";
									return MValue();
								}
							}
						}
						default: {
							LogFL(DEBUG) << "CalculateFromTupleAndExpr(): this number of arguments while is not implemented\n";
							return MValue();
						}
					}
				}
				default: {
					LogFL(DEBUG) << "CalculateFromTupleAndExpr(): another functions while are not implemented\n";
					return MValue();
				}
			}
		}
		default: {
			LogFL(DEBUG) << "CalculateFromTupleAndExpr(): undefined type of Expr\n";
			return MValue();
		}
	}
}

#undef _op_result

SpaceObject SelectMaker::MakeOneTable()
{
	last_error.clear();
	SpaceObject res;
	size_t limit = MSG_START_RECS_COUNT;
	size_t offset = 0, index = 0;

	TableRef *from_table = statement->from_table;

	std::string name = from_table->getName();
	int space_id = schm.GetSpaceIDByString(name);
	LogFL(DEBUG) << "SelectMaker::MakeOneTable(): table name = " << name << ", id = " << space_id << "\n";

	//~~~~~~~~ P a r s i n g   f i r s t   a n s w e r ~~~~~~~~

	SpaceObject next_tuples = NextSpacePart(space_id, index, offset, limit);
	if (next_tuples.Size() == 0) {
		LogFL(DEBUG) << "SelectMaker::MakeOneTable(): response is empty\n";
		return res;
	}

	//~~~~~~~~ P a r s i n g   n e x t   t u p l e s ~~~~~~~~

	Logger::LogObject(DEBUG, next_tuples);

	while (next_tuples.Size()) {
		SpaceObject processed_tuples;

		if (statement->select_list != NULL) {
			if (statement->select_list->at(0)->GetType() == ExprType::kExprStar) {
				processed_tuples = next_tuples;
			} else {
				for (int i = 0, size = static_cast<int>(next_tuples.Size()); i < size; ++i) {
					TupleObj tmp;
					for (int j = 0, size2 = static_cast<int>(statement->select_list->size()); j < size2; ++j) {
						MValue tmp2 = CalculateFromTupleAndExpr(statement->select_list->at(j), next_tuples[i], next_tuples[i], schm, space_id, aliases);
						if (tmp2.GetType() != TP_DEFAULT) {
							tmp.PushBack(tmp2);
						} else {
							LogFL(DEBUG) << "SelectMaker::MakeOneTable(): incorrect MValue, i = " << i << ", j = " << j << "\n";
						}
					}
					if (tmp.Size()) processed_tuples.PushBack(tmp);
				}
			}
		}

		std::cout << "processed_tuples: " << processed_tuples << std::endl << "next_tuples: " << next_tuples << std::endl;

		if (statement->where_clause != NULL) {
			for (int i = static_cast<int>(processed_tuples.Size()) - 1; i >= 0; --i) {
				try {
					MValue tmp = CalculateFromTupleAndExpr(statement->where_clause, processed_tuples[i], next_tuples[i], schm, space_id, aliases);
					if (tmp.GetType() == TP_BOOL) {
						if (!tmp.GetBool()) processed_tuples.Remove(i);
						else Logger::LogObject(DEBUG, processed_tuples[i]);
					} else {
						LogFL(DEBUG) << "SelectMaker::MakeOneTable(): incorrect MValue at iteration i = " << i << "\n";
					}
				} catch(const std::string &m) { LogFL(DEBUG) << m << "\n"; }
			}
		}

		LogFL(DEBUG) << "after cycle\n";
		res.PushBack(processed_tuples);

		offset += MSG_START_RECS_COUNT;
		next_tuples = NextSpacePart(space_id, index, offset, limit);
		LogFL(DEBUG) << "after new tuples\n";
	}
	LogFL(DEBUG) << "after while\n";

	if (statement->select_list != NULL) {
		for (int i = 0, size = static_cast<int>(statement->select_list->size()); i < size; ++i) {
			res.PushName(statement->select_list->at(i)->ToString());
		}
	}

	return res;
}

SpaceObject SelectMaker::MakeJoinTables()
{
	return SpaceObject();
}

SpaceObject SelectMaker::NextSpacePart(size_t space_id, size_t index, size_t offset, size_t limit)
{
	last_error.clear();
	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));
	while (1) {
		request->AddSelect(space_id, index, offset, TP_ITERATOR_ALL, limit);
		request->ReserveKeyParts();
		if (request->Used() > msg_size) {
			if (msg_size > MSG_MAX_SIZE) {
				last_error = "max select message size was reached ¯\\_(ツ)_/¯";
				LogFL(DEBUG) << "SelectMaker::NextSpacePart(): max select message size was reached ¯\\_(ツ)_/¯\n";
				return SpaceObject();
			}
			msg_size *= 2;
			request.reset(new TP(DataStructure(msg_size)));
		} else {
			break;
		}
	}
	LogFL(DEBUG) << "SelectMaker::NextSpacePart(): select added\n";

	TPResponse resp(SendRequest(request));
	if (resp.GetState() == -1) {
		last_error = "failed to parse response";
		LogFL(DEBUG) << "SelectMaker::NextSpacePart(): failed to parse response\n";
		return SpaceObject();
	}
	if (resp.GetCode() != 0) {
		last_error = "respone code != 0";
		LogFL(DEBUG) << "SelectMaker::NextSpacePart(): server respond: " << resp.GetCode() << ", " << resp.GetError() << "\n";
	} else {
		LogFL(DEBUG) << "SelectMaker::NextSpacePart(): succes receive\n";
	}
	return SpaceObject::FromMSGPack(resp.GetData());
}

SpaceObject SelectMaker::MakeSelect() {
	last_error.clear();
	Logger::LogObject(DEBUG, *select);

	if (statement->from_table == NULL) {
		last_error = "SelectMaker::MakeSelect(): from_table is null";
		LogFL(DEBUG) << "SelectMaker::MakeSelect(): from_table is null\n";
		return SpaceObject();
	}

	TableRef *from_table = statement->from_table;

	switch(from_table->GetType()) {
		case TableRefType::kTableName: {
			return MakeOneTable();
		}
		case TableRefType::kTableJoin: {
			return MakeJoinTables();
		}
		default:
			last_error = "SelectMaker::MakeSelect(): default from_table type";
			return SpaceObject();
	}
	last_error = "SelectMaker::MakeSelect(): default from_table type 2";
	return SpaceObject();
}