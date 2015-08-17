#include "InsertMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ I N S E R T   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

InsertMaker::InsertMaker(Session &ses_, TarantoolInfo &tinfo_, InsertStatement *_statement) : SQLMaker(ses_, tinfo_), statement(_statement) { }

MValue InsertMaker::MakeInsert()
{
	last_error.clear();
	std::string table_name = statement->GetName();
	int space_id = tinfo->SpaceBy(table_name)->ID();
	if (space_id == -1) {
		last_error = "InsertMaker::MakeInsert(): space with name '" + table_name + "' was not found in schema";
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}

	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));
	auto space_format = tinfo->SpaceFormat(space_id);
	auto values = statement->GetValues();
	if (values == NULL) {
		last_error = "InsertMaker::MakeInsert(): attempt to insert empty tuple";
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}
	size_t values_size = values->size();

	request->AddInsert(space_id);
	request->ReserveTupleFields(space_format.size());

	if (statement->GetColumns() == NULL) {
		size_t size = space_format.size();
		for (size_t i = 0; (i < size) && (i < values_size); ++i) {
			switch(values->at(i)->GetType()) {
				case kExprLiteralFloat: {
					request->AddFloat(values->at(i)->GetFloat());
					break;
				}
				case kExprLiteralString: {
					request->AddString(values->at(i)->GetString());
					break;
				}
				case kExprLiteralInt: {
					request->AddInt(values->at(i)->GetInt());
					break;
				}
				default: {
					last_error = "InsertMaker::MakeInsert(): expr with type = " + ExprTypeToString(values->at(i)->GetType()) + " can't be added to insert";
					LogFL(DEBUG) << last_error << "\n";
					return MValue(false);
				}
			}
		}
		for (size_t i = values_size; i < size; ++i) {
			switch(space_format[i].type) {
				case FT_STR: request->AddString(""); break;
				case FT_NUM: request->AddInt(0); break;
				default: {
					last_error = "InsertMaker::MakeInsert(): type of value = " + Convert::ToString(space_format[i].type) + " can't be added to request\n";
					LogFL(DEBUG) << last_error << "\n";
					return MValue(false);
				}
			}
		}
	} else {
		if (statement->GetColumns()->size() != statement->GetValues()->size()) {
			last_error = "InsertMaker::MakeInsert(): count of columns and values are not equal";
			LogFL(DEBUG) << last_error << "\n";
			return MValue(false);
		}
		for (size_t i = 0, size = space_format.size(); i < size; ++i) {
			std::string &col_name = space_format[i].name;
			auto val = statement->GetValueByColumn(col_name);
			if (val != NULL) {
				switch(val->GetType()) {
					case kExprLiteralFloat: {
						request->AddFloat(val->GetFloat());
						break;
					}
					case kExprLiteralString: {
						request->AddString(val->GetString());
						break;
					}
					case kExprLiteralInt: {
						request->AddInt(val->GetInt());
						break;
					}
					default: {
						last_error = "InsertMaker::MakeInsert(): type of value = " + ExprTypeToString(val->GetType()) + " can't be added to request";
						LogFL(DEBUG) << last_error << "\n";
						return MValue(false);
					}
				}
				continue;
			}
			switch(space_format[i].type) {
				case FT_NUM: request->AddInt(0); break;
				case FT_STR: request->AddString(""); break;
				default: {
					last_error = "InsertMaker::MakeInsert(): type of value = " + Convert::ToString(space_format[i].type) + " can't be added to request";
					LogFL(DEBUG) << last_error << "\n";
					return MValue(false);
				}
			}
		}
	}

	TPResponse resp(ses->SendRequest(request));
	if (resp.GetState() == -1) {
		last_error = "InsertMaker::MakeInsert(): failed to parse response";
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}
	if (resp.GetCode() != 0) {
		std::stringstream tmp;
		tmp << "InsertMaker::MakeInsert(): server respond: " << resp.GetCode() << ", " << resp.GetError();
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}
	return MValue::FromMSGPack(resp.GetData());
}