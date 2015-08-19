#include "SelectMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ S E L E C T   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

void SelectMaker::init_aliases(const TableRef *from_table)
{
	if (from_table == NULL) return;
	if (from_table->alias.length() > 0) {
		table_aliases.insert(std::make_pair(from_table->name->GetName(), from_table->alias));
		LogFL(DEBUG) << "SelectMaker::init_aliases(): name = " << from_table->name->GetName() << ", alias = " << from_table->alias << "\n";
	}
	if (from_table->list != NULL) {
		for (size_t i = 0, size = from_table->list->size(); i < size; ++i) {
			this->init_aliases(from_table->list->at(i));
		}
	}
	this->init_aliases(from_table->prev_join);
	if (from_table->join != NULL) {
		this->init_aliases(from_table->join->left);
		this->init_aliases(from_table->join->right);
	}
	this->init_aliases(from_table->next_join);
}

SelectMaker::SelectMaker(Session &ses_, TarantoolInfo &tinfo_, SelectStatement *_statement) : SQLMaker(ses_, tinfo_), statement(_statement) {
	Logger::LogObject(DEBUG, *statement);

	if (statement->select_list != NULL) {
		for (int i = 0, size = static_cast<int>(statement->select_list->size()); i < size; ++i) {
			Expr *tmp = statement->select_list->at(i);
			if (tmp->alias.length() > 0) {
				aliases.insert(std::make_pair(tmp->alias, i));
			}
			switch(tmp->GetType()) {
				case ExprType::kExprColumnRef: {
					aliases.insert(std::make_pair(tmp->GetRealName(), i));
					break;
				}
				default: continue;
			}
		}
	}
	this->init_aliases(statement->from_table);
}

#define _op_result(op) CalculateFromTupleAndExpr(expr->expr, tuple, original_tuple, tinfo, space_id, aliases) op CalculateFromTupleAndExpr(expr->expr2, tuple, original_tuple, tinfo, space_id, aliases)

MValue CalculateFromTupleAndExpr(Expr *expr, const TupleObj &tuple, const TupleObj &original_tuple, const TarantoolInfo *tinfo, int space_id, const std::map<std::string, int> &aliases = std::map<std::string, int>())
{
	if (expr == NULL) return MValue();
	switch(expr->GetType()) {
		case ExprType::kExprLiteralFloat: return MValue(expr->GetFloat());
		case ExprType::kExprLiteralString: return MValue(expr->GetRealName());
		case ExprType::kExprLiteralInt: return MValue(expr->GetInt());
		case ExprType::kExprStar: return MValue();
		case ExprType::kExprPlaceholder: return MValue();
		case ExprType::kExprColumnRef: {
			int num = tinfo->SpaceBy(space_id)->ColumnNumber(expr->GetRealName());
			if (num == -1) {
				auto tmp = aliases.find(expr->GetRealName());
				if (tmp == aliases.end()) {
					LogFL(DEBUG) << "CalculateFromTupleAndExpr(): column with name \"" << expr->GetRealName() << "\" was not found\n";
					return MValue();
				}
				return tuple[tmp->second];
			}
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
								case ExprType::kExprColumnRef: left = tuple[tinfo->SpaceBy(space_id)->ColumnNumber(first->GetRealName())]; break;
								default: {
									LogFL(DEBUG) << "CalculateFromTupleAndExpr(): first argument of function SUBSTRING has incorrect type\n";
									return MValue();
								}
							}
							switch(second->GetType()) {
								case ExprType::kExprLiteralInt: right = MValue(second->GetInt()); break;
								case ExprType::kExprColumnRef: right = tuple[tinfo->SpaceBy(space_id)->ColumnNumber(second->GetRealName())]; break;
								default: {
									LogFL(DEBUG) << "CalculateFromTupleAndExpr(): second argument of function SUBSTRING has incorrect type\n";
									return MValue();
								}
							}
							if (left.GetType() != TP_STR) {
								LogFL(DEBUG) << "CalculateFromTupleAndExpr(): first argument of function SUBSTRING must have string type, but type is " << Convert::ToString(left.GetType()) << "\n";
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
									LogFL(DEBUG) << "CalculateFromTupleAndExpr(): type fo second argument of SUBSTRING must be int or uint, but type is " << Convert::ToString(right.GetType()) << "\n";
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

//while only EQ
std::vector<SQLCondition> SelectMaker::GetIndexConditions(const Expr *where) const
{
	std::vector<SQLCondition> res;
	if (where == NULL) return res;

	switch(where->type) {
		case ExprType::kExprOperator: {
			switch(where->op_type) {
				case Expr::AND: {
					auto left = this->GetIndexConditions(where->expr);
					auto right = this->GetIndexConditions(where->expr2);
					res.insert(res.end(), left.begin(), left.end());
					res.insert(res.end(), right.begin(), right.end());
					return res;
				}
				case Expr::SIMPLE_OP: {
					switch(where->op_char) {
						case '=': /*case '>': case '<':*/ {
							SQLCondition tmp(where);
							tmp.type = Convert::ToIteratorType(where->op_char);
							res.push_back(tmp);
							return res;
						}
						default: return res;
					}
				}
				/*case Expr::LESS_EQ: case Expr::GREATER_EQ: {
					SQLCondition tmp(where);
					tmp.type = ToIteratorType(where->op_type);
					res.push_back(tmp);
					return res;
				}*/
				default: throw std::string("SelectMaker::GetIndexConditions(): this op type is not supported now");
			}
			break;
		}
		default: throw std::string("SelectMaker::GetIndexConditions(): this where type is not supported now");
	}
}

SpaceObject SelectMaker::MakeOneTableInTarantool()
{
	last_error.clear();
	SpaceObject res;
	size_t limit = MSG_START_RECS_COUNT;
	size_t offset = 0, index = 0;

	TableRef *from_table = statement->from_table;
	std::string name = from_table->getName();
	int space_id = tinfo->SpaceBy(name)->ID();
	LogFL(DEBUG) << "SelectMaker::MakeOneTableInTarantool(): table name = " << name << ", id = " << space_id << "\n";

	//~~~~~~~~ M a k i n g   r e q u e s t ~~~~~~~~

	//check on EQ

	std::vector<SQLCondition> cols_and_vals;
	try {
		cols_and_vals = this->GetIndexConditions(statement->where_clause);
	} catch(const std::string &tmp) {
		LogFL(DEBUG) << "SelectMaker::MakeOneTableInTarantool(): error while getting SQLConditions: " << tmp << " and this query will be executed via full scan\n";
		return this->MakeOneTable();
	}

	//~~~~~~~~ S e l e c t   b e s t   i n d e x ~~~~~~~~

	std::vector<std::string> select_columns;
	for (size_t i = 0, size = cols_and_vals.size(); i < size; ++i) select_columns.push_back(cols_and_vals[i].column);

	auto index_format = tinfo->SpaceBy(space_id)->Indices();

	if (index_format.size() < 2) {
		LogFL(DEBUG) << "SelectMaker::MakeOneTableInTarantool(): indices count = " << index_format.size() << " and Select will be executed via full scan\n";
		return this->MakeOneTable();
	}

	int best_index_id = 0;
	int max_first_cols = 0;
	std::string best_index_name;
	for (size_t i = 0, size = index_format.size(); i < size; ++i) {
		//TODO: remove indices doubles
		const std::vector<SpacePart> &index_parts = index_format[i]->Parts();

		for (size_t j = 0, size_j = index_parts.size(); j < size_j; ++j) {
			if (std::find(select_columns.begin(), select_columns.end(), index_parts[j].name) != select_columns.end()) {
				if ((j + 1) > max_first_cols) {
					max_first_cols = j + 1;
					best_index_name = index_format[i]->Name();
				}
			} else break;
		}
	}
	best_index_id = tinfo->SpaceBy(space_id)->IndexBy(best_index_name)->ID();
	LogFL(DEBUG) << "SelectMaker::MakeOneTableInTarantool(): best_index_name = " << best_index_name << ", best_index_id = " << best_index_id << "\n";
	MValueVector keys;
	for (int i = 0; i < max_first_cols; ++i) {
		keys.push_back(cols_and_vals[i].value);
	}
	LogFL(DEBUG) << "SelectMaker::MakeOneTableInTarantool(): keys = " << keys << "\n";

	//~~~~~~~~ P a r s i n g   f i r s t   a n s w e r ~~~~~~~~

	return this->MakeOneTable();
}

SpaceObject SelectMaker::MakeOneTable()
{
	last_error.clear();
	SpaceObject res;
	size_t limit = MSG_START_RECS_COUNT;
	size_t offset = 0, index = 0;

	TableRef *from_table = statement->from_table;

	std::string name = from_table->getName();
	int space_id = tinfo->SpaceBy(name)->ID();
	LogFL(DEBUG) << "SelectMaker::MakeOneTable(): table name = " << name << ", id = " << space_id << "\n";

	//~~~~~~~~ P a r s i n g   f i r s t   a n s w e r ~~~~~~~~

	SpaceObject next_tuples = NextSpacePart(space_id, index, offset, limit);
	if (next_tuples.Size() == 0) {
		LogFL(DEBUG) << "SelectMaker::MakeOneTable(): response is empty\n";
		return res;
	}

	//~~~~~~~~ P a r s i n g   n e x t   t u p l e s ~~~~~~~~

	while (next_tuples.Size()) {
		SpaceObject processed_tuples;

		if (statement->select_list != NULL) {
			if (statement->select_list->at(0)->GetType() == ExprType::kExprStar) {
				processed_tuples = next_tuples;
			} else {
				for (int i = 0, size = static_cast<int>(next_tuples.Size()); i < size; ++i) {
					TupleObj tmp;
					for (int j = 0, size2 = static_cast<int>(statement->select_list->size()); j < size2; ++j) {
						MValue tmp2 = CalculateFromTupleAndExpr(statement->select_list->at(j), next_tuples[i], next_tuples[i], tinfo, space_id, aliases);
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

		if (statement->where_clause != NULL) {
			for (int i = static_cast<int>(processed_tuples.Size()) - 1; i >= 0; --i) {
				try {
					MValue tmp = CalculateFromTupleAndExpr(statement->where_clause, processed_tuples[i], next_tuples[i], tinfo, space_id, aliases);
					if (tmp.GetType() == TP_BOOL) {
						if (!tmp.GetBool()) processed_tuples.Remove(i);
					} else {
						LogFL(DEBUG) << "SelectMaker::MakeOneTable(): incorrect MValue at iteration i = " << i << "\n";
					}
				} catch(const std::string &m) { LogFL(DEBUG) << m << "\n"; }
			}
		}

		res.PushBack(processed_tuples);
		offset += MSG_START_RECS_COUNT;
		next_tuples = NextSpacePart(space_id, index, offset, limit);
	}

	if (statement->select_list != NULL) {
		for (int i = 0, size = static_cast<int>(statement->select_list->size()); i < size; ++i) {
			res.PushName(statement->select_list->at(i)->ToString());
		}
	}

	return res;
}

SpaceObject SelectMaker::MakeJoinTables()
{
	if (statement->from_table->prev_join == NULL) {
		//it is first join in statement
		JoinDefinition *cur_join = statement->from_table->join;
		if (cur_join->left->type != kTableName) {
			LogFL(DEBUG) << "SelectMaker::MakeJoinTables(): error\n";
			return SpaceObject();
		}
		if (cur_join->right->type != kTableName) {
			LogFL(DEBUG) << "SelectMaker::MakeJoinTables(): error\n";
			return SpaceObject();
		}
		std::string left_space = cur_join->left->name->GetName();
		std::string right_space = cur_join->right->name->GetName();
		if (tinfo->SpaceBy(left_space) == nullptr) {
			last_error = "SelectMaker::MakeJoinTables(): space with name = " + left_space + " was not found";
			LogFL(DEBUG) << last_error << "\n";
			return SpaceObject();
		}
		if (tinfo->SpaceBy(right_space) == nullptr) {
			last_error = "SelectMaker::MakeJoinTables(): space with name = " + right_space + " was not found";
			LogFL(DEBUG) << last_error << "\n";
			return SpaceObject();
		}
		int left_id = tinfo->SpaceBy(left_space)->ID();
		int right_id = tinfo->SpaceBy(right_space)->ID();
		LogFL(DEBUG) << "SelectMaker::MakeJoinTables(): left = " << left_space << ": " << left_id << ", right = " << right_space << ": " << right_id << "\n";
	
		int left_index = 0, left_offset = 0, left_limit = MSG_START_RECS_COUNT;
		tp_iterator_type left_iterator = TP_ITERATOR_ALL;
		std::vector<MValue> left_keys;

		int right_index = 0, right_offset = 0, right_limit = MSG_START_RECS_COUNT;
		tp_iterator_type right_iterator = TP_ITERATOR_ALL;
		std::vector<MValue> right_keys;

		SpaceObject left_part = NextSpacePart(left_id, left_index, left_offset, left_limit, left_iterator, left_keys);
		SpaceObject right_part = NextSpacePart(right_id, right_index, right_offset, right_limit, right_iterator, right_keys);
		if (left_part.Size() == 0) return right_part;
		if (right_part.Size() == 0) return left_part;

		std::vector<std::string> left_names = tinfo->SpaceBy(left_id)->ColumnNames();
		std::vector<std::string> right_names = tinfo->SpaceBy(right_id)->ColumnNames();

		SpaceObject res;
		std::vector<std::string> all_names;
		all_names.insert(all_names.end(), left_names.begin(), left_names.end());
		all_names.insert(all_names.end(), right_names.begin(), right_names.end());
		res.SetNames(all_names);

		do {
			left_part.SetNames(left_names);
			do {
				right_part.SetNames(right_names);
				LogFL(DEBUG) << "left.size = " << left_part.Size() << ", right.size = " << right_part.Size() << "\n";
				SpaceObject product = SpaceObject::CartesianProduct(left_part, right_part);
				LogFL(DEBUG) << "product.size = " << product.Size() << "\n";
				res.PushBack(product);
				right_offset += MSG_START_RECS_COUNT;
				right_part = NextSpacePart(right_id, right_index, right_offset, right_limit, right_iterator, right_keys);
			} while (right_part.Size());

			left_offset += MSG_START_RECS_COUNT;
			left_part = NextSpacePart(left_id, left_index, left_offset, left_limit, left_iterator, left_keys);
			right_offset = 0;
			right_part = NextSpacePart(right_id, right_index, 0, right_limit, right_iterator, right_keys);
		} while (left_part.Size());

		return res;
	}
	
	return SpaceObject();
}

SpaceObject SelectMaker::NextSpacePart(size_t space_id, size_t index, size_t offset, size_t limit, tp_iterator_type iterator, const std::vector<MValue> &keys)
{
	last_error.clear();
	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));
	request->AddSelect(space_id, index, offset, iterator, limit);
	request->ReserveKeyParts(keys.size());
	for (size_t i = 0, size = keys.size(); i < size; ++i) {
		request->AddMValue(keys[i]);
	}

	TPResponse resp(ses->SendRequest(request));
	if (resp.GetState() == -1) {
		last_error = "failed to parse response";
		LogFL(DEBUG) << "SelectMaker::NextSpacePart(): failed to parse response\n";
		return SpaceObject();
	}
	if (resp.GetCode() != 0) {
		last_error = "respone code != 0";
		LogFL(DEBUG) << "SelectMaker::NextSpacePart(): server respond: " << resp.GetCode() << ", " << resp.GetError() << "\n";
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
			return this->MakeOneTableInTarantool();
		}
		case TableRefType::kTableJoin: {
			return this->MakeJoinTables();
		}
		default:
			last_error = "SelectMaker::MakeSelect(): default from_table type";
			return SpaceObject();
	}
	last_error = "SelectMaker::MakeSelect(): default from_table type 2";
	return SpaceObject();
}

SpaceObject SelectMaker::MakeSelectInTarantool()
{
	last_error.clear();
	Logger::LogObject(DEBUG, *select);

	if (statement->from_table == NULL) {
		last_error = "SelectMaker::MakeSelectInTarantool(): from_table is null";
		LogFL(DEBUG) << last_error << "\n";
		return SpaceObject();
	}

	TableRef *from_table = statement->from_table;

	// switch (from_table->GetType()) {
	// 	case TableRefType::kTableName: {

	// 	}
	// }

	return SpaceObject();
}