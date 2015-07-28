#include "InsertMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ I N S E R T   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

InsertMaker::InsertMaker(InsertStatement *_statement) : SQLMaker(), statement(_statement) {
	Logger::LogObject(DEBUG, *statement);
}

MValue InsertMaker::MakeInsert()
{
	last_error.clear();
	std::string table_name = statement->GetName();
	int space_id = schm.GetSpaceIDByString(table_name);
	if (space_id == -1) {
		last_error = "InsertMaker::MakeInsert(): space with name '" + table_name + "' was not found in schema";
		return MValue(false);
	}

	LogFL(DEBUG) << "InsertMaker::MakeInsert(): space name = " << table_name << ", id =  " << space_id << "\n";

	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));
	auto space_format = schm.GetSpaceFormat(space_id);
	auto values = statement->GetValues();
	if (values == NULL) {
		last_error = "InsertMaker::MakeInsert(): attempt to insert empty tuple";
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}
	size_t values_size = values->size();

	while (1) {
		LogFL(DEBUG) << "InsertMaker::MakeInsert(): next iter\n";
		request->AddInsert(space_id);
		request->ReserveTupleFields(space_format.size());

		if (statement->GetColumns() == NULL) {
			LogFL(DEBUG) << "InsertMaker::MakeInsert(): cols = null\n";
			size_t size = space_format.size();
			for (size_t i = 0; (i < size) && (i < values_size); ++i) {
				LogFL(DEBUG) << "InsertMaker::MakeInsert(): i = " << i << "\n";
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
				LogFL(DEBUG) << "InsertMaker::MakeInsert(): i = " << i << "\n";
				switch(space_format[i].second) {
					case FT_STR: request->AddString(""); break;
					case FT_NUM: request->AddInt(0); break;
					default: {
						last_error = "InsertMaker::MakeInsert(): type of value = " + FieldTypeToString(space_format[i].second) + " can't be added to request\n";
						LogFL(DEBUG) << last_error << "\n";
						return MValue(false);
					}
				}
			}
		} else {
			LogFL(DEBUG) << "InsertMaker::MakeInsert(): cols != null\n";
			if (statement->GetColumns()->size() != statement->GetValues()->size()) {
				last_error = "InsertMaker::MakeInsert(): count of columns and values are not equal";
				LogFL(DEBUG) << last_error << "\n";
				return MValue(false);
			}
			for (size_t i = 0, size = space_format.size(); i < size; ++i) {
				LogFL(DEBUG) << "InsertMaker::MakeInsert(): i = " << i << "\n";
				std::string &col_name = space_format[i].first;
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
				switch(space_format[i].second) {
					case FT_NUM: request->AddInt(0); break;
					case FT_STR: request->AddString(""); break;
					default: {
						last_error = "InsertMaker::MakeInsert(): type of value = " + FieldTypeToString(space_format[i].second) + " can't be added to request";
						LogFL(DEBUG) << last_error << "\n";
						return MValue(false);
					}
				}
			}
		}

		if (request->Used() > msg_size) {
			if (msg_size > MSG_MAX_SIZE) {
				last_error = "max create message size was reached ¯\\_(ツ)_/¯";
				LogFL(DEBUG) << "InsertMaker::MakeInsert(): max create message size was reached ¯\\_(ツ)_/¯\n";
				return MValue(false);
			}
			msg_size *= 2;
			request.reset(new TP(DataStructure(msg_size)));
		} else {
			break;
		}
	}

	LogFL(DEBUG) << "InsertMaker::MakeInsert(): insert added\n";

	TPResponse resp(SendRequest(request));
	if (resp.GetState() == -1) {
		last_error = "failed to parse response";
		LogFL(DEBUG) << "InsertMaker::MakeInsert(): failed to parse response\n";
		return MValue(false);
	}
	if (resp.GetCode() != 0) {
		std::stringstream tmp;
		tmp << "InsertMaker::MakeInsert(): server respond: " << resp.GetCode() << ", " << resp.GetError();
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	} else {
		LogFL(DEBUG) << "InsertMaker::MakeInsert(): succes receive\n";
	}

	MValue obj = MValue::FromMSGPack(resp.GetData());
	LogFL(DEBUG) << "InsertMaker::MakeInsert(): mvalue = " << obj << "\n";
	return obj;
}