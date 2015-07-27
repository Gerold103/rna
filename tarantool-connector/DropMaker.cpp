#include "DropMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ D R O P   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~~~~~~~~~

DropMaker::DropMaker(DropStatement *_statement) : statement(_statement) { Logger::LogObject(DEBUG, *statement); }

bool DropMaker::MakeDrop()
{
	last_error.clear();
	std::string table_name = statement->GetName();
	LogFL(DEBUG) << "DropMaker::MakeDrop(): space name = " << table_name << "\n";

	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));

	while (1) {
		request->AddDropSpace(table_name);
		if (request->Used() > msg_size) {
			if (msg_size > MSG_MAX_SIZE) {
				last_error = "max create message size was reached ¯\\_(ツ)_/¯";
				LogFL(DEBUG) << "DropMaker::MakeDrop(): max create message size was reached ¯\\_(ツ)_/¯\n";
				return false;
			}
			msg_size *= 2;
			request.reset(new TP(DataStructure(msg_size)));
		} else {
			break;
		}
	}
	LogFL(DEBUG) << "DropMaker::MakeDrop(): drop space added\n";

	TPResponse resp(SendRequest(request));
	if (resp.GetState() == -1) {
		last_error = "failed to parse response";
		LogFL(DEBUG) << "DropMaker::MakeDrop(): failed to parse response\n";
		return false;
	}
	if (resp.GetCode() != 0) {
		std::stringstream tmp;
		tmp << "DropMaker::MakeDrop():  server respond: " << resp.GetCode() << ", " << resp.GetError();
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return false;
	} else {
		LogFL(DEBUG) << "DropMaker::MakeDrop(): succes receive\n";
	}

	MValue obj = MValue::FromMSGPack(resp.GetData());
	LogFL(DEBUG) << "DropMaker::MakeDrop(): mvalue = " << obj << "\n";

	return true;
}