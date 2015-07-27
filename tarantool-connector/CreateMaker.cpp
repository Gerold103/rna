#include "CreateMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ C R E A T E   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

CreateMaker::CreateMaker(CreateStatement *_statement) : SQLMaker(), statement(_statement) {
	Logger::LogObject(DEBUG, *statement);
}

MValue CreateMaker::MakeCreate() {
	last_error.clear();
	std::string table_name = statement->GetTableName();
	LogFL(DEBUG) << "CreateMaker::MakeCreate(): space name = " << table_name << "\n";
	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));

	//~~~~~~~~ C r e a t e   s p a c e ~~~~~~~~

	while (1) {
		request->AddCreateSpace(table_name);
		if (request->Used() > msg_size) {
			if (msg_size > MSG_MAX_SIZE) {
				last_error = "max create message size was reached ¯\\_(ツ)_/¯";
				LogFL(DEBUG) << "CreateMaker::MakeCreate(): max create message size was reached ¯\\_(ツ)_/¯\n";
				return MValue(false);
			}
			msg_size *= 2;
			request.reset(new TP(DataStructure(msg_size)));
		} else {
			break;
		}
	}
	LogFL(DEBUG) << "CreateMaker::MakeCreate(): create space added\n";

	TPResponse resp(SendRequest(request));
	if (resp.GetState() == -1) {
		last_error = "failed to parse response";
		LogFL(DEBUG) << "CreateMaker::MakeCreate(): failed to parse response\n";
		return MValue(false);
	}
	if (resp.GetCode() != 0) {
		std::stringstream tmp;
		tmp << "CreateMaker::MakeCreate(): server respond: " << resp.GetCode() << ", " << resp.GetError();
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	} else {
		LogFL(DEBUG) << "CreateMaker::MakeCreate(): succes receive\n";
	}

	MValue obj = MValue::FromMSGPack(resp.GetData());
	LogFL(DEBUG) << "CreateMaker::MakeCreate(): mvalue = " << obj << "\n";
	if (obj.GetType() != TP_ARRAY) {
		std::stringstream tmp;
		tmp << "CreateMaker::MakeCreate(): result isn't array = " << obj;
		last_error = tmp.str();
		LogFL(DEBUG) << "CreateMaker::MakeCreate(): result isn't array = " << obj << "\n";
		return MValue(false);
	}
	if (obj.GetArray().empty()) {
		last_error = "CreateMaker::MakeCreate(): returned array is empty";
		LogFL(DEBUG) << "CreateMaker::MakeCreate(): returned array is empty\n";
		return MValue(false);
	}
	if (obj.GetArray()[0].GetType() != TP_STR) {
		last_error = "CreateMaker::MakeCreate(): type of result is not string";
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}
	if (obj.GetArray()[0].GetStr() != "created") {
		std::stringstream tmp;
		tmp << "CreateMaker::MakeCreate(): space wasn't created, result = " << obj;
		last_error = tmp.str();
		LogFL(DEBUG) << "CreateMaker::MakeCreate(): space wasn't created, result = " << obj << "\n";
		return MValue(false);
	}


	//~~~~~~~~ C r e a t e   f o r m a t ~~~~~~~~

	std::vector<std::pair<std::string, std::string> > columns;
	const std::vector<ColumnDefinition*> &tmp = statement->GetColumns();
	for (int i = 0, size = static_cast<int>(tmp.size()); i < size; ++i) {
		if (tmp[i]->IsIndexDefinition()) continue;
		std::pair<std::string, std::string> new_col;
		new_col.first = tmp[i]->GetName();
		switch(tmp[i]->GetDataType()) {
			case ColumnDefinition::TEXT:
				new_col.second = "STR";
				break;
			case ColumnDefinition::INT: case ColumnDefinition::DOUBLE:
				new_col.second = "NUM";
				break;
			default:
				LogFL(DEBUG) << "CreateMaker::MakeCreate(): error in column datatype\n";
				return MValue(false);
		}
		columns.push_back(new_col);
	}

	msg_size = MSG_START_SIZE;
	request.reset(new TP(DataStructure(msg_size)));

	while (1) {
		request->AddSpaceFormat(table_name, columns);
		if (request->Used() > msg_size) {
			if (msg_size > MSG_MAX_SIZE) {
				last_error = "max create message size was reached ¯\\_(ツ)_/¯";
				LogFL(DEBUG) << "CreateMaker::MakeCreate(): max create message size was reached ¯\\_(ツ)_/¯\n";
				return MValue(false);
			}
			msg_size *= 2;
			request.reset(new TP(DataStructure(msg_size)));
		} else {
			break;
		}
	}
	LogFL(DEBUG) << "CreateMaker::MakeCreate(): create space format added\n";

	resp = SendRequest(request);
	if (resp.GetState() == -1) {
		last_error = "failed to parse response";
		LogFL(DEBUG) << "CreateMaker::MakeCreate(): failed to parse response\n";
		return MValue(false);
	}
	if (resp.GetCode() != 0) {
		std::stringstream tmp;
		tmp << "CreateMaker::MakeCreate(): server respond: " << resp.GetCode() << ", " << resp.GetError();
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	} else {
		LogFL(DEBUG) << "CreateMaker::MakeCreate(): succes receive\n";
	}

	obj = MValue::FromMSGPack(resp.GetData());
	LogFL(DEBUG) << "CreateMaker::MakeCreate(): mvalue = " << obj << "\n";

	//~~~~~~~~ C r e a t e   i n d i c e s ~~~~~~~~

	for (int i = 0, size = static_cast<int>(tmp.size()); i < size; ++i) {
		if (!(tmp[i]->IsIndexDefinition())) continue;

		msg_size = MSG_START_SIZE;
		request.reset(new TP(DataStructure(msg_size)));

		while (1) {
			MValueMap options;
			const std::vector<std::string> &col_names = tmp[i]->GetColNames();
			MValueVector parts;
			std::string index_name("sql_");

			for (int j = 0, sizej = static_cast<int>(col_names.size()); j < sizej; ++j) {
				index_name += col_names[j] + "__";
				for (int k = 0, sizek = static_cast<int>(columns.size()); k < sizek; ++k) {
					if (columns[k].first == col_names[j]) {
						parts.push_back(MValue(k + 1));
						parts.push_back(MValue(columns[k].second));
						break;
					}
					if (k == sizek - 1) {
						LogFL(DEBUG) << "CreateMaker::MakeCreate(): column with name " << col_names[j] << " wasn't found\n";
						return MValue(false);
					}
				}
			}

			switch(tmp[i]->GetDataType()) {
				case ColumnDefinition::PRIMARY: {
					index_name = "primary";
					LogFL(DEBUG) << "CreateMaker::MakeCreate(): primary index added\n";
					break;
				}
				case ColumnDefinition::UNIQUE: {
					options.insert(std::make_pair(MValue("unique"), MValue(true)));
					index_name += "unique";
					LogFL(DEBUG) << "CreateMaker::MakeCreate(): unique index added\n";
					break;
				}
				case ColumnDefinition::INDEX: {
					index_name += "index";
					LogFL(DEBUG) << "CreateMaker::MakeCreate(): index added\n";
					break;
				}
				default: {
					LogFL(DEBUG) << "CreateMaker::MakeCreate(): unknown index type\n";
					return MValue(false);
				}
			}

			options.insert(std::make_pair(MValue("parts"), MValue(parts)));
			request->AddCreateIndex(table_name, index_name, options);

			if (request->Used() > msg_size) {
				if (msg_size > MSG_MAX_SIZE) {
					last_error = "max create message size was reached ¯\\_(ツ)_/¯";
					LogFL(DEBUG) << "CreateMaker::MakeCreate(): max create message size was reached ¯\\_(ツ)_/¯\n";
					return MValue(false);
				}
				msg_size *= 2;
				request.reset(new TP(DataStructure(msg_size)));
			} else {
				break;
			}
		}

		resp = SendRequest(request);
		if (resp.GetState() == -1) {
			last_error = "failed to parse response";
			LogFL(DEBUG) << "CreateMaker::MakeCreate(): failed to parse response\n";
			return MValue(false);
		}
		if (resp.GetCode() != 0) {
			std::stringstream tmp;
			tmp << "CreateMaker::MakeCreate(): server respond: " << resp.GetCode() << ", " << resp.GetError();
			last_error = tmp.str();
			LogFL(DEBUG) << last_error << "\n";
			return MValue(false);
		} else {
			LogFL(DEBUG) << "CreateMaker::MakeCreate(): succes receive\n";
		}

		obj = MValue::FromMSGPack(resp.GetData());
		LogFL(DEBUG) << "CreateMaker::MakeCreate(): mvalue = " << obj << "\n";
	}

	return MValue(true);
}