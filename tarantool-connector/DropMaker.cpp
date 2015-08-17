#include "DropMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ D R O P   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~~~~~~~~~

DropMaker::DropMaker(Session &ses_, TarantoolInfo &tinfo_, DropStatement *_statement) : SQLMaker(ses_, tinfo_), statement(_statement) { }

MValue DropMaker::MakeDrop()
{
	last_error.clear();
	std::string table_name = statement->GetName();
	
	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));
	request->AddDropSpace(table_name);

	TPResponse resp(ses->SendRequest(request));
	if (resp.GetState() == -1) {
		last_error = "failed to parse response";
		LogFL(DEBUG) << "DropMaker::MakeDrop(): failed to parse response\n";
		return MValue(false);
	}
	if (resp.GetCode() != 0) {
		std::stringstream tmp;
		tmp << "DropMaker::MakeDrop():  server respond: " << resp.GetCode() << ", " << resp.GetError();
		last_error = tmp.str();
		LogFL(DEBUG) << last_error << "\n";
		return MValue(false);
	}

	return MValue::FromMSGPack(resp.GetData());
}