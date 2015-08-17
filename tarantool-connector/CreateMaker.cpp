#include "CreateMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ C R E A T E   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

CreateMaker::CreateMaker(Session &ses_, TarantoolInfo &tinfo_, CreateStatement *_statement) : SQLMaker(ses_, tinfo_), statement(_statement) { }

MValue CreateMaker::MakeCreate() {
	last_error.clear();
	std::string table_name = statement->GetTableName();
	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));

	//~~~~~~~~ C r e a t e   s p a c e ~~~~~~~~

	request->AddCreateSpace(table_name);

	TPResponse resp(ses->SendRequest(request));
	if (resp.GetState() == -1) {
		last_error = "CreateMaker::MakeCreate(): failed to parse response";
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}
	if (resp.GetCode() != 0) {
		std::stringstream tmp;
		tmp << "CreateMaker::MakeCreate(): server respond: " << resp.GetCode() << ", " << resp.GetError();
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}

	MValue obj = MValue::FromMSGPack(resp.GetData());
	if (obj.GetType() != TP_ARRAY) {
		std::stringstream tmp;
		tmp << "CreateMaker::MakeCreate(): result isn't array = " << obj;
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}
	if (obj.GetArray().empty()) {
		last_error = "CreateMaker::MakeCreate(): returned array is empty";
		LogFL(DEBUG) << last_error << "\n";
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
		LogFL(DEBUG) << last_error << "\n";
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
				last_error = "CreateMaker::MakeCreate(): error in column datatype";
				LogFL(DEBUG) << last_error << "\n";
				return MValue(false);
		}
		columns.push_back(new_col);
	}

	msg_size = MSG_START_SIZE;
	request.reset(new TP(DataStructure(msg_size)));

	request->AddSpaceFormat(table_name, columns);

	resp = ses->SendRequest(request);
	if (resp.GetState() == -1) {
		last_error = "CreateMaker::MakeCreate(): failed to parse response";
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}
	if (resp.GetCode() != 0) {
		std::stringstream tmp;
		tmp << "CreateMaker::MakeCreate(): server respond: " << resp.GetCode() << ", " << resp.GetError();
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}

	//~~~~~~~~ C r e a t e   i n d i c e s ~~~~~~~~

	for (int i = 0, size = static_cast<int>(tmp.size()); i < size; ++i) {
		if (!(tmp[i]->IsIndexDefinition())) continue;

		msg_size = MSG_START_SIZE;
		request.reset(new TP(DataStructure(msg_size)));

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
					last_error = "CreateMaker::MakeCreate(): column with name " + col_names[j] + " wasn't found";
					LogFL(DEBUG) << last_error << "\n";
					return MValue(false);
				}
			}
		}

		switch(tmp[i]->GetDataType()) {
			case ColumnDefinition::PRIMARY: {
				index_name = "primary";
				break;
			}
			case ColumnDefinition::UNIQUE: {
				options.insert(std::make_pair(MValue("unique"), MValue(true)));
				index_name += "unique";
				break;
			}
			case ColumnDefinition::INDEX: {
				options.insert(std::make_pair(MValue("unique"), MValue(false)));
				index_name += "index";
				break;
			}
			default: {
				last_error = "CreateMaker::MakeCreate(): unknown index type";
				LogFL(DEBUG) << last_error << "\n";
				return MValue(false);
			}
		}

		options.insert(std::make_pair(MValue("parts"), MValue(parts)));
		request->AddCreateIndex(table_name, index_name, options);

		resp = ses->SendRequest(request);
		if (resp.GetState() == -1) {
			last_error = "CreateMaker::MakeCreate(): failed to parse response";
			LogFL(DEBUG) << last_error << "\n";
			return MValue(false);
		}
		if (resp.GetCode() != 0) {
			std::stringstream tmp;
			tmp << "CreateMaker::MakeCreate(): server respond: " << resp.GetCode() << ", " << resp.GetError();
			last_error = tmp.str();
			LogFL(DEBUG) << last_error << "\n";
			return MValue(false);
		}
	}

	tinfo->Update();

	return MValue(true);
}