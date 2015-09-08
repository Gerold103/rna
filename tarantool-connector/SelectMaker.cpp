#include "SelectMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ S E L E C T   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

void SelectMaker::init_aliases(const TableRef *from_table, std::vector<const TableRef *> &getted)
{
	if (from_table == NULL) return;
	if (std::find(getted.begin(), getted.end(), from_table) != getted.end()) return;
	getted.push_back(from_table);
	if (from_table->alias.length() > 0) {
		table_aliases.insert(std::make_pair(from_table->alias, from_table->name->GetName()));
		LogFL(DEBUG) << "SelectMaker::init_aliases(): name = " << from_table->name->GetName() << ", alias = " << from_table->alias << "\n";
	}
	if (from_table->list != NULL) {
		for (size_t i = 0, size = from_table->list->size(); i < size; ++i) {
			this->init_aliases(from_table->list->at(i), getted);
		}
	}
	this->init_aliases(from_table->prev_join, getted);
	if (from_table->join != NULL) {
		this->init_aliases(from_table->join->left, getted);
		this->init_aliases(from_table->join->right, getted);
	}
	this->init_aliases(from_table->next_join, getted);
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
	std::vector<const TableRef *> getted;
	this->init_aliases(statement->from_table, getted);
}

//while only EQ

TColumn GetColumnFromExpr(const TupleObj &reduced, const TupleObj &original, const Expr *expr, const TarantoolInfo *tinfo, const std::map<std::string, std::string> &aliases)
{
	TColumn res;
	switch (expr->type) {
		case ExprType::kExprLiteralFloat: {
			res.value.SetValue(expr->GetFloat());
			return res;
		}
		case ExprType::kExprLiteralString: {
			res.value.SetValue(expr->GetString());
			return res;
		}
		case ExprType::kExprLiteralInt: {
			res.value.SetValue(expr->GetInt());
			return res;
		}
		default: break;
	}
	if (expr->type != kExprColumnRef) return TColumn();
	if (expr->alias.length()) res.alias = expr->alias;
	if (expr->name.length() == 0) return TColumn();
	if (expr->table.length()) {
		std::string _table;
		if (tinfo->SpaceBy(expr->table) == nullptr) {
			//mybe it is alias
			auto it = aliases.find(expr->table);
			if ((it == aliases.end()) || (tinfo->SpaceBy(it->second) == nullptr)) {
				return TColumn();
			}
			_table = it->second;
		} else _table = expr->table;
		res.space = _table;
		if (tinfo->SpaceBy(_table)->ColumnBy(expr->name) == nullptr) {
			return TColumn();
		}
		res.name = expr->name;
	} else {
		TColumn tmp = reduced.GetValue(expr->name);
		if (tmp.Empty()) {
			tmp = original.GetValue(expr->name);
			if (tmp.Empty()) {
				LogFL(DEBUG) << "CalculateMValue(): column was not found in both tuples original and reduced, name = " << (expr->name) << "\n";
				return TColumn();
			}
		}
		res.name = tmp.name;
		res.space = tmp.space;
	}
	TColumn tmp = reduced.GetValue(res);
	if (tmp.Empty()) {
		tmp = original.GetValue(res);
		if (tmp.Empty()) {
			return TColumn();
		}
	}
	res.value = tmp.value;
	return res;
}

std::vector<SQLCondition> SelectMaker::GetIndexConditions(const TupleObj &reduced, const TupleObj &original, const Expr *where, const TarantoolInfo *tinfo, const std::map<std::string, std::string> &aliases) const
{
	std::vector<SQLCondition> res;
	if (where == NULL) return res;
	switch(where->type) {
		case kExprOperator: {
			switch(where->op_type) {
				case Expr::SIMPLE_OP: {
					switch (where->op_char) {
						case '=': {
							SQLCondition tmp;
							tmp.type = Convert::ToIteratorType(where->op_char);

							TColumn left = GetColumnFromExpr(reduced, original, where->expr, tinfo, aliases);
							TColumn right = GetColumnFromExpr(reduced, original, where->expr2, tinfo, aliases);
							if (left.Empty() || right.Empty()) return res;

							if ((left.value.GetType() != TP_DEFAULT) == (right.value.GetType() != TP_DEFAULT)) return res;
							if (left.value.GetType() != TP_DEFAULT) std::swap(left, right);
							tmp.column = left;
							tmp.column.value = right.value;
							res.push_back(tmp);
							return res;
						}
						default: return res;
					}
					break;
				}
				case Expr::AND: {
					auto left = this->GetIndexConditions(reduced, original, where->expr, tinfo, aliases);
					auto right = this->GetIndexConditions(reduced, original, where->expr2, tinfo, aliases);
					res.insert(res.end(), left.begin(), left.end());
					res.insert(res.end(), right.begin(), right.end());
					return res;
				}
				default: return res;
			}
		}
		default: return res;
	}
}

JoinVector SelectMaker::MakeJoinVector(const TableRef *table) const
{
	JoinVector res;
	if (table->prev_join != NULL) {
		res = this->MakeJoinVector(table->prev_join);
	}
	res.push_back(table->join);
	return res;
}

#undef _op_result
#define _op_result(_op_) MValue left = CalculateMValue(expr->expr, reduced, original, tinfo, strict, aliases); \
		MValue right = CalculateMValue(expr->expr2, reduced, original, tinfo, strict, aliases); \
		if (left.GetType() == TP_DEFAULT) { \
			if (strict) { \
				LogFL(DEBUG) << "CalculateMValue(): error while making op in left operand\n"; \
				return MValue(); \
			} \
			return MValue(true); \
		} \
		if (right.GetType() == TP_DEFAULT) { \
			if (strict) { \
				LogFL(DEBUG) << "CalculateMValue(): error while making op in right operand\n"; \
				return MValue(); \
			} \
			return MValue(true); \
		} \
		return (left _op_ right); \

#define _op_result_arith(_op_) MValue left = CalculateMValue(expr->expr, reduced, original, tinfo, strict, aliases); \
		MValue right = CalculateMValue(expr->expr2, reduced, original, tinfo, strict, aliases); \
		if (left.GetType() == TP_DEFAULT) { \
			LogFL(DEBUG) << "CalculateMValue(): error while making op in left operand\n"; \
			return MValue(); \
		} \
		if (right.GetType() == TP_DEFAULT) { \
			LogFL(DEBUG) << "CalculateMValue(): error while making op in right operand\n"; \
			return MValue(); \
		} \
		return (left _op_ right); \

MValue CalculateMValue(const Expr *expr, const TupleObj &reduced, const TupleObj &original, const TarantoolInfo *tinfo, bool strict, const std::map<std::string, std::string> &aliases)
{
	if (expr == NULL) return MValue();
	switch(expr->GetType()) {
		case ExprType::kExprLiteralFloat: return MValue(expr->GetFloat());
		case ExprType::kExprLiteralString: return MValue(expr->GetRealName());
		case ExprType::kExprLiteralInt: return MValue(expr->GetInt());
		case ExprType::kExprStar: return MValue();
		case ExprType::kExprPlaceholder: return MValue();
		case ExprType::kExprColumnRef: {
			TColumn col;
			if (expr->alias.length()) col.alias = expr->alias;
			if (expr->name.length() == 0) {
				LogFL(DEBUG) << "CalculateMValue(): column must have name\n";
				return MValue();
			}

			if (expr->table.length()) {
				std::string _table;
				if (tinfo->SpaceBy(expr->table) == nullptr) {
					//mybe it is alias
					auto it = aliases.find(expr->table);
					if ((it == aliases.end()) || (tinfo->SpaceBy(it->second) == nullptr)) {
						LogFL(DEBUG) << "CalculateMValue(): table " << expr->table << " was not found\n";
						return MValue();
					}
					_table = it->second;
				} else _table = expr->table;
				col.space = _table;
				if (tinfo->SpaceBy(_table)->ColumnBy(expr->name) == nullptr) {
					LogFL(DEBUG) << "CalculateMValue(): column with name = " << (expr->name) << " was not found in space = " << col.space << "\n";
					return MValue();
				}
				col.name = expr->name;
			} else {
				TColumn tmp = reduced.GetValue(expr->name);
				if (tmp.Empty()) {
					tmp = original.GetValue(expr->name);
					if (tmp.Empty()) {
						LogFL(DEBUG) << "CalculateMValue(): column was not found in both tuples original and reduced, name = " << (expr->name) << "\n";
						return MValue();
					}
				}
				col.name = tmp.name;
				col.space = tmp.space;
			}
			TColumn tmp = reduced.GetValue(col);
			if (tmp.Empty()) {
				tmp = original.GetValue(col);
				if (tmp.Empty()) {
					LogFL(DEBUG) << "CalculateMValue(): column " << col << " was not found in tuples\n";
					return MValue();
				}
			}
			return tmp.value;
		}
		case ExprType::kExprOperator: {
			switch(expr->GetOperator()) {
				case Expr::AND: {
					MValue left = CalculateMValue(expr->expr, reduced, original, tinfo, strict, aliases);
					MValue right = CalculateMValue(expr->expr2, reduced, original, tinfo, strict, aliases);
					if (left.GetType() == TP_DEFAULT) {
						if (strict) {
							LogFL(DEBUG) << "CalculateMValue(): error while making AND in left operand\n";
							return MValue();
						}
						left = MValue(true);
					}
					if (right.GetType() == TP_DEFAULT) {
						if (strict) {
							LogFL(DEBUG) << "CalculateMValue(): error while making AND in right operand\n";
							return MValue();
						}
						right = MValue(true);
					}

					return (left && right);
				}
				case Expr::OR: {
					MValue left = CalculateMValue(expr->expr, reduced, original, tinfo, strict, aliases);
					MValue right = CalculateMValue(expr->expr2, reduced, original, tinfo, strict, aliases);
					if (left.GetType() == TP_DEFAULT) {
						if (strict) {
							LogFL(DEBUG) << "CalculateMValue(): error while making AND in left operand\n";
							return MValue();
						}
						left = MValue(true);
					}
					if (right.GetType() == TP_DEFAULT) {
						if (strict) {
							LogFL(DEBUG) << "CalculateMValue(): error while making AND in right operand\n";
							return MValue();
						}
						right = MValue(true);
					}

					return (left || right);
				}
				case Expr::NOT_EQUALS: { _op_result(!=); }
				case Expr::GREATER_EQ: { _op_result(>=); }
				case Expr::LESS_EQ: { _op_result(<=); }
				case Expr::SIMPLE_OP: {
					switch(expr->GetOperatorChar()) {
						case '=': { _op_result(==); }
						case '+': { _op_result_arith(+); }
						case '-': { _op_result_arith(-); }
						case '*': { _op_result_arith(*); }
						case '/': { _op_result_arith(/); }
						case '%': { _op_result_arith(%); }
						case '>': { _op_result(>); }
						case '<': { _op_result(<); }
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
			break;
		}
		case ExprType::kExprFunctionRef: {
			switch(expr->GetOperator()) {
				case Expr::SUBSTRING: {
					switch(expr->GetArgumentsNumber()) {
						case 2: {
							MValue first, second;
							first = CalculateMValue(expr->expr, reduced, original, tinfo, strict, aliases);
							second = CalculateMValue(expr->expr2, reduced, original, tinfo, strict, aliases);
							if (first.GetType() != TP_STR) {
								LogFL(DEBUG) << "CalculateMValue(): first argument of SUBSTRING must be string\n";
								return MValue();
							}
							switch(second.GetType()) {
								case TP_INT: {
									if (second.GetInt() >= first.GetStr().length()) return MValue(std::string());
									return MValue(first.GetStr().substr(second.GetInt() - 1));
								}
								case TP_UINT: {
									if (second.GetUint() >= first.GetStr().length()) return MValue(std::string());
									return MValue(first.GetStr().substr(static_cast<int>(second.GetUint()) - 1));
								}
								default: {
									LogFL(DEBUG) << "CalculateMValue(): second argument of SUBSTRING must be integer\n";
									return MValue();
								}
							}
							break;
						}
						default: {
							LogFL(DEBUG) << "CalculateMValue(): this number of arguments while is not implemented\n";
							return MValue();
						}
					}
				}
				default: {
					LogFL(DEBUG) << "CalculateMValue(): another functions while are not implemented\n";
					return MValue();
				}
			}
		}
		default: {
			LogFL(DEBUG) << "CalculateMValue(): in default\n";
			return MValue();
		}
	}
}

#undef _op_result
#undef _op_result_arith

TColumn GetReducedName(const std::vector<TColumn> &orig_names, const Expr *select_list_elem, const TarantoolInfo *tinfo, const std::vector<std::string> &spaces, const std::map<std::string, std::string> &aliases)
{
	TColumn res;
	const Expr *elem = select_list_elem;
	if (elem->alias.length()) res.alias = elem->alias;
	switch(elem->type) {
		case kExprColumnRef: {
			const TarantoolSpaceInfo *space = nullptr;
			if (elem->table.length()) {
				space = tinfo->SpaceBy(elem->table);
				if (space == nullptr) {
					auto it = aliases.find(elem->table);
					if ((it == aliases.end()) || (tinfo->SpaceBy(it->second) == nullptr)) {
						LogFL(DEBUG) << "GetReducedName(): was not found table with name = " << elem->table << "\n";
						return TColumn();
					}
					if (std::find(spaces.begin(), spaces.end(), it->second) == spaces.end()) {
						LogFL(DEBUG) << "GetReducedName(): space " << it->second << " found in all spaces, but not found in current tuple\n";
						return TColumn();
					}
					space = tinfo->SpaceBy(it->second);
					res.space = space->Name();
				} else {
					res.space = elem->table;
					if (space->ColumnBy(elem->name) == nullptr) {
						LogFL(DEBUG) << "GetReducedName(): was not found column " << elem->name << " in space " << res.space << "\n";
						return TColumn();
					}
				}
				res.name = elem->name;
			} else {
				bool error_flag = false;
				for (size_t j = 0, size_j = spaces.size(); j < size_j; ++j) {
					space = tinfo->SpaceBy(spaces[j]);
					if (space == nullptr) continue;
					if (space->ColumnBy(elem->name) == nullptr) continue;
					if (res.space.length()) {
						error_flag = true;
						space = nullptr;
						break;
					}
					res.space = spaces[j];
					res.name = elem->name;
				}
				if (error_flag) {
					LogFL(DEBUG) << "GetReducedName(): column with name " << elem->name << " was found in several tuples\n";
					return TColumn();
				}
				if (res.name.length() == 0) {
					LogFL(DEBUG) << "GetReducedName(): column with name " << elem->name << " was not found in spaces\n";
					return TColumn();
				}
			}
			return res;
		}
		case kExprFunctionRef: {
			LogFL(DEBUG) << "GetReducedName(): functions aren't supported!\n";
			return TColumn();
		}
		case kExprLiteralFloat: case kExprLiteralString: case kExprLiteralInt: {
			res.name = elem->ToString();
			return res;
		}
		case kExprOperator: {
			res.name = elem->ToString();
			switch(elem->GetOperator()) {
				case Expr::AND: case Expr::OR: case Expr::NOT_EQUALS: case Expr::GREATER_EQ: case Expr::LESS_EQ: case Expr::SIMPLE_OP: {
					TColumn left = GetReducedName(orig_names, elem->expr, tinfo, spaces, aliases);
					TColumn right = GetReducedName(orig_names, elem->expr2, tinfo, spaces, aliases);
					if (left.Empty() || right.Empty()) {
						LogFL(DEBUG) << "GetReducedName(): error while getting names\n";
						return TColumn();
					}
					return res;
				}
				default: {
					LogFL(DEBUG) << "GetReducedName(): in default operator\n";
					return TColumn();
				}
			}
		}
		case kExprStar: {
			return TColumn();
		}
		default: {
			LogFL(DEBUG) << "GetReducedName(): in default\n";
			return TColumn();
		}
	}
}

SpaceObject ReduceTuples(const SpaceObject &space, const std::vector<Expr *> &select_list, const TarantoolInfo *tinfo,
						const std::vector<std::string> &spaces, const std::map<std::string, std::string> &aliases, bool strict = true)
{
	LogFL(DEBUG) << "ReduceTuples()\n";
	for (size_t i = 0, size = spaces.size(); i < size; ++i) LogFL(DEBUG) << spaces[i] << "\n";
	if (space.Size() == 0) return space;
	std::vector<TColumn> names;
	std::vector<int> numbers_in_list;
	std::vector<TColumn> values;

	if ((select_list.size() == 1) && (select_list[0]->type == kExprStar)) return space;

	//~~~~ n a m e s ~~~~

	for (size_t i = 0, size = select_list.size(); i < size; ++i) {
		const Expr *cur = select_list[i];
		TColumn col = GetReducedName(space.GetNames(), cur, tinfo, spaces, aliases);
		if (col.Empty() && strict) {
			LogFL(DEBUG) << "ReduceTuples(): was not found name number " << i << "\n";
			return SpaceObject();
		}
		if (!col.Empty()) {
			numbers_in_list.push_back(i);
			names.push_back(col);
		}
	}

	//~~~~ v a l u e s ~~~~

	SpaceObject res;
	res.SetNames(names);
	for (size_t i = 0, size = space.Size(); i < size; ++i) {
		values.clear();
		for (size_t j = 0, sizej = numbers_in_list.size(); j < sizej; ++j) {
			MValue val = CalculateMValue(select_list[numbers_in_list[j]], space[i], space[i], tinfo, strict, aliases);
			if (val.GetType() == TP_DEFAULT) {
				LogFL(DEBUG) << "ReduceTuples(): error while getting values, i = " << i << ", j = " << j << "\n";
				return SpaceObject();
			}
			TColumn col(names[j]);
			col.value = val;
			values.push_back(col);
		}
		res.PushBack(TupleObj(values));
	}

	return res;
}

SpaceObject SelectMaker::MakeOneTableSimple()
{
	last_error.clear();
	SpaceObject res;
	size_t limit = MSG_START_RECS_COUNT;
	size_t offset = 0, index = 0;

	TableRef *from_table = statement->from_table;

	std::string name = from_table->name->GetName();
	std::vector<std::string> spaces;
	spaces.push_back(name);
	int space_id = tinfo->SpaceBy(name)->ID();
	LogFL(DEBUG) << "SelectMaker::MakeOneTable(): table name = " << name << ", id = " << space_id << "\n";

	//~~~~~~~~ P a r s i n g   f i r s t   a n s w e r ~~~~~~~~

	std::vector<TColumn> reduced_names;
	if ((statement->select_list->size()) && (statement->select_list->at(0)->type != kExprStar))
		for (size_t i = 0, size = statement->select_list->size(); i < size; ++i) {
			reduced_names.push_back(GetReducedName(tinfo->SpaceBy(name)->ColumnNames(), statement->select_list->at(i), tinfo, spaces, table_aliases));
		}
	else reduced_names = tinfo->SpaceBy(name)->ColumnNames();
	std::vector<SQLCondition> cols_and_vals = GetIndexConditions(TupleObj(reduced_names), TupleObj(tinfo->SpaceBy(name)->ColumnNames()), statement->where_clause, tinfo, table_aliases);
	int best_index = 0;
	MValueVector keys;
	// try {
	// 	std::vector<std::string> select_columns;
	// 	for (size_t i = 0, size = cols_and_vals.size(); i < size; ++i) {
	// 		select_columns.push_back(cols_and_vals[i].column.name);
	// 	}
	// 	auto index_format = tinfo->SpaceBy(space_id)->Indices();
	// 	if (index_format.size() < 2) {
	// 		throw std::string("indices count < 2");
	// 	}
	// 	int max_first_cols = 0;
	// 	std::vector<int> col_nums;
	// 	std::string best_index_name;
	// 	for (size_t i = 0, size = index_format.size(); i < size; ++i) {
	// 		//TODO: remove indices doubles
	// 		const std::vector<SpacePart> &index_parts = index_format[i]->Parts();
	// 		for (size_t j = 0, size_j = index_parts.size(); j < size_j; ++j) {
	// 			for (size_t k = 0, sizek = select_columns.size(); k < sizek; ++k) {
	// 				if (select_columns[k] == index_parts[j].name) {
	// 					if ((j + 1) > max_first_cols) {
	// 						if (best_index_name != index_format[i]->Name()) col_nums.clear();
	// 						max_first_cols = j + 1;
	// 						best_index_name = index_format[i]->Name();
	// 						col_nums.push_back(k);
	// 					}
	// 					break;
	// 				}
	// 			}
	// 			if (std::find(select_columns.begin(), select_columns.end(), index_parts[j].name) != select_columns.end()) {
	// 				if ((j + 1) > max_first_cols) {
	// 					max_first_cols = j + 1;
	// 					best_index_name = index_format[i]->Name();
	// 				}
	// 			} else break;
	// 		}
	// 	}
	// 	if (best_index_name.length()) {
	// 		best_index = tinfo->SpaceBy(space_id)->IndexBy(best_index_name)->ID();
	// 	}
	// 	LogFL(DEBUG) << "SelectMaker::MakeOneTableInTarantool(): best_index_name = " << best_index_name << ", best_index_id = " << best_index << "\n";
	// 	for (size_t i = 0, size = col_nums.size(); i < size; ++i) {
	// 		keys.push_back(cols_and_vals[i].column.value);
	// 	}
	// 	LogFL(DEBUG) << "SelectMaker::MakeOneTableInTarantool(): keys = " << keys << "\n";
	// } catch(const std::string &tmp) {
	// 	LogFL(DEBUG) << "SelectMaker::MakeOneTableInTarantool(): error while getting SQLConditions: " << tmp << " and this query will be executed via full scan\n";
	// }

	SpaceObject next_tuples;
	if (keys.size()) {
		next_tuples = NextSpacePart(space_id, best_index, offset, limit, TP_ITERATOR_EQ, keys);
	} else {
		next_tuples = NextSpacePart(space_id, best_index, offset, limit);
	}
	next_tuples.SetNames(tinfo->SpaceBy(name)->ColumnNames());

	if (next_tuples.Size() == 0) {
		LogFL(DEBUG) << "SelectMaker::MakeOneTable(): response is empty\n";
		return res;
	}

	//~~~~~~~~ P a r s i n g   n e x t   t u p l e s ~~~~~~~~

	while (next_tuples.Size()) {
		SpaceObject processed_tuples;

		if (statement->select_list != NULL) {
			if (statement->select_list->at(0)->GetType() == ExprType::kExprStar)
				processed_tuples = next_tuples;
			else {
				processed_tuples = ReduceTuples(next_tuples, *(statement->select_list), tinfo, spaces, table_aliases);
				if (processed_tuples.Size() == 0) {
					LogFL(DEBUG) << "SelectMaker::MakeOneTableSimple(): error while reducing tuples\n";
					return SpaceObject();
				}
			}
		}


		if (statement->where_clause != NULL) {
			for (int i = static_cast<int>(processed_tuples.Size()) - 1; i >= 0; --i) {
				try {
					MValue tmp = CalculateMValue(statement->where_clause, processed_tuples[i], next_tuples[i], tinfo, true, table_aliases);
					if (tmp.GetType() == TP_BOOL) {
						if (!tmp.GetBool()) processed_tuples.Remove(i);
					} else {
						LogFL(DEBUG) << "SelectMaker::MakeOneTableSimple(): incorrect MValue in WHERE, i = " << i << "\n";
						return SpaceObject();
					}
				} catch(const std::string &m) { LogFL(DEBUG) << "SelectMaker::MakeOneTableSimple(): error, message = " << m << "\n"; return SpaceObject(); }
			}
		}

		if (res.Size() == 0) res = processed_tuples;
		else res.PushBack(processed_tuples);
		offset += MSG_START_RECS_COUNT;
		if (keys.size()) {
			next_tuples = NextSpacePart(space_id, best_index, offset, limit, TP_ITERATOR_EQ, keys);
		} else {
			next_tuples = NextSpacePart(space_id, best_index, offset, limit);
		}
		next_tuples.SetNames(tinfo->SpaceBy(name)->ColumnNames());
	}

	return res;
}

void CheckConditions(const Expr *condition, SpaceObject &reduced, SpaceObject &original, const TarantoolInfo *tinfo, bool strict, const std::map<std::string, std::string> &aliases)
{
	if (condition != NULL) {
		for (int i = static_cast<int>(reduced.Size()) - 1; i >= 0; --i) {
			try {
				MValue tmp = CalculateMValue(condition, reduced[i], original[i], tinfo, strict, aliases);
				if (tmp.GetType() == TP_BOOL) {
					if (!tmp.GetBool()) {
						reduced.Remove(i);
						original.Remove(i);
					}
				} else {
					LogFL(DEBUG) << "CheckConditions(): incorrect MValue in ON, i = " << i << "\n";
					return;
				}
			} catch(const std::string &m) { LogFL(DEBUG) << "CheckConditions(): error, message = " << m << "\n"; return; }
		}
	}
}

struct SpaceProperties {
	int index;
	int limit;
	int offset;
	int space_id;
	bool is_over;
	std::string space;
	tp_iterator_type iterator;
	MValueVector keys;
	SpaceObject cur_part;
};

std::ostream &operator<<(std::ostream &stream, const SpaceProperties &ob)
{
	stream << "index: " << ob.index << ", limit: " << ob.limit << ", offset: " << ob.offset << ", space_id: " << ob.space_id << ", space: " << ob.space << "\ncur_part: " << ob.cur_part;
	return stream;
}

SpaceObject SelectMaker::MakeJoinTables()
{
	JoinVector joins = this->MakeJoinVector(statement->from_table);
	SpaceObject res;
	SpaceObject original;
	std::vector<SpaceProperties> space_props;
	{	
		SpaceProperties tmp;
		tmp.index = 0;
		tmp.limit = MSG_START_RECS_COUNT;
		tmp.offset = 0;
		tmp.space = joins[0]->left->name->GetName();
		if (tinfo->SpaceBy(tmp.space) == nullptr) {
			LogFL(DEBUG) << "SelectMaker::MakeJoinTables(): space " << tmp.space << " was not found\n";
			return SpaceObject();
		}
		tmp.space_id = tinfo->SpaceBy(tmp.space)->ID();
		tmp.iterator = TP_ITERATOR_ALL;
		tmp.is_over = false;
		space_props.push_back(tmp);
		for (size_t i = 0, size = joins.size(); i < size; ++i) {
			tmp.index = 0;
			tmp.limit = MSG_START_RECS_COUNT;
			tmp.offset = 0;
			tmp.space = joins[i]->right->name->GetName();
			if (tinfo->SpaceBy(tmp.space) == nullptr) {
				LogFL(DEBUG) << "SelectMaker::MakeJoinTables(): space " << tmp.space << " was not found\n";
				return SpaceObject();
			}
			tmp.space_id = tinfo->SpaceBy(tmp.space)->ID();
			tmp.iterator = TP_ITERATOR_ALL;
			tmp.is_over = false;
			space_props.push_back(tmp);
		}
	}
	for (size_t i = 0, size = space_props.size(); i < size; ++i) {
		space_props[i].cur_part = NextSpacePart(space_props[i].space_id, space_props[i].index, space_props[i].offset, space_props[i].limit, space_props[i].iterator, space_props[i].keys);
		if (space_props[i].cur_part.Size() == 0) space_props[i].is_over = true;
		else {
			space_props[i].cur_part.SetNames(tinfo->SpaceBy(space_props[i].space_id)->ColumnNames());
			space_props[i].offset += MSG_START_RECS_COUNT;
		}
	}


	std::vector<std::vector<std::string> > first_spaces;
	for (size_t i = 0, size = space_props.size(); i < size; ++i) {
		std::vector<std::string> tmp;
		for (size_t j = 0; j <= i; ++j) tmp.push_back(space_props[j].space);
		first_spaces.push_back(tmp);
	}

	int last_space = space_props.size() - 1;
	int current_space = space_props.size() - 1;
	std::vector<SpaceObject> temporary_originals;
	std::vector<SpaceObject> temporary_reduceds;
	{
		temporary_originals.push_back(space_props[0].cur_part);
		SpaceObject tmp = ReduceTuples(temporary_originals[0], *(statement->select_list), tinfo, first_spaces[0], table_aliases, false);
		CheckConditions(joins[0]->condition, tmp, temporary_originals[0], tinfo, true, table_aliases);
		temporary_reduceds.push_back(tmp);
	}
	for (size_t i = 1, size = space_props.size(); i < size; ++i) {
		SpaceObject tmp_orig = SpaceObject::CartesianProduct(temporary_originals[i - 1], space_props[i].cur_part);
		SpaceObject tmp_red = ReduceTuples(tmp_orig, *(statement->select_list), tinfo, first_spaces[i], table_aliases, false);
		CheckConditions(joins[i - 1]->condition, tmp_red, tmp_orig, tinfo, true, table_aliases);
		temporary_reduceds.push_back(tmp_red);
		temporary_originals.push_back(tmp_orig);
	}
	SpaceObject res_reduced = temporary_reduceds.back();
	SpaceObject res_original = temporary_originals.back();

	for (size_t i = 0, size = temporary_reduceds.size(); i < size; ++i) {
		LogFL(DEBUG) << temporary_originals[i] << "\n" << temporary_reduceds[i] << "\n";
	}

	while (last_space >= 0) {
		SpaceProperties &cur_prop = space_props[current_space];
		cur_prop.cur_part = NextSpacePart(cur_prop.space_id, cur_prop.index, cur_prop.offset, cur_prop.limit, cur_prop.iterator, cur_prop.keys);
		if (cur_prop.cur_part.Size() == 0) {
			cur_prop.offset = 0;
			if (current_space == last_space) {
				last_space--;
			}
			current_space--;
			continue;
		}
		cur_prop.cur_part.SetNames(tinfo->SpaceBy(cur_prop.space_id)->ColumnNames());
		cur_prop.offset += MSG_START_RECS_COUNT;
		if (current_space > 0) {
			temporary_originals[current_space] = SpaceObject::CartesianProduct(temporary_originals[current_space - 1], cur_prop.cur_part);
		} else {
			temporary_originals[current_space] = cur_prop.cur_part;
		}
		temporary_reduceds[current_space] = ReduceTuples(temporary_originals[current_space], *(statement->select_list), tinfo, first_spaces[current_space], table_aliases, false);
		CheckConditions(joins[std::max(current_space - 1, 0)]->condition, temporary_reduceds[current_space], temporary_originals[current_space], tinfo, true, table_aliases);
		if (current_space == space_props.size() - 1) {
			res_reduced.PushBack(temporary_reduceds[current_space]);
			res_original.PushBack(temporary_originals[current_space]);
		} else {
			++current_space;
			continue;
		}
	}

	CheckConditions(statement->where_clause, res_reduced, res_original, tinfo, true, table_aliases);
	return res_reduced;
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
			return this->MakeOneTableSimple();
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