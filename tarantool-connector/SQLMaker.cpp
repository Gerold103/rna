#include "SQLMaker.h"

#include <algorithm>
#include <sstream>

//~~~~~~~~~~~~~~~~~~~~~~~~ S Q L M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

SQLMaker::SQLMaker() : ses() {
	last_error.clear();
	if (ses.Connect() == -1) {
		last_error = "connection failed";
		LogFL(DEBUG) << "SQLMaker::SQLMaker(): connection failed\n";
		is_connected = false;
		return;
	}
	is_connected = true;
	DataStructure greet(128);
	ses.Receive(greet);
	schm = TarantoolSchema(ses);
}

bool SQLMaker::MakeEval(const std::string &function)
{
	last_error.clear();
	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));
	while (1) {
		request->AddEval(function);
		if (request->Used() > msg_size) {
			if (msg_size > MSG_MAX_SIZE) {
				last_error = "max select message size was reached ¯\\_(ツ)_/¯";
				LogFL(DEBUG) << "SQLMaker::MakeEval(): max select message size was reached ¯\\_(ツ)_/¯\n";
				return false;
			}
			msg_size *= 2;
			request.reset(new TP(DataStructure(msg_size)));
		} else {
			break;
		}
	}
	LogFL(DEBUG) << "SQLMaker::MakeEval(): select added\n";
	int ss = ses.Send(*request);
	if (ss < 0) {
		last_error = "error while sending request was occured";
		LogFL(DEBUG) << "SQLMaker::MakeEval(): error while sending request was occured, return = " << ss << "\n";
		return false;
	}
	ss = ses.Sync();
	if (ss < 0) {
		last_error = "error while syncing sesson was occured";
		LogFL(DEBUG) << "SQLMaker::MakeEval(): error while syncing sesson was occured, return = " << ss << "\n";
		return false;
	}
	LogFL(DEBUG) << "SQLMaker::MakeEval(): sync result = " << ss << "\n";
	return true;
}

bool SQLMaker::IsConnected() const { return is_connected; }

std::string SQLMaker::Error() const
{
	return last_error;
}

TPResponse SQLMaker::SendRequest(TP_p &request)
{
	last_error.clear();
	int ss = ses.Send(*request);
	if (ss < 0) {
		last_error = "error while sending request was occured";
		LogFL(DEBUG) << "SQLMaker::SendRequest(): error while sending request was occured, return = " << ss << "\n";
		return TPResponse();
	}
	ss = ses.Sync();
	if (ss < 0) {
		last_error = "error while syncing sesson was occured";
		LogFL(DEBUG) << "SQLMaker::SendRequest(): error while syncing sesson was occured, return = " << ss << "\n";
		return TPResponse();
	}
	LogFL(DEBUG) << "SQLMaker::SendRequest(): sync result = " << ss << "\n";

	DataStructure response_buffer(MSG_HEADER_SIZE);
	int len = ses.Receive(response_buffer);
	LogFL(DEBUG) << "after receive\n";
	if (len < MSG_HEADER_SIZE) {
		last_error = "length of receive less than header size";
		LogFL(DEBUG) << "SQLMaker::SendRequest(): length of receive less than header size = " << len << "\n";
		return TPResponse();
	}
	if (mp_typeof(*response_buffer.Data()) != MP_UINT) {
		last_error = "bad reply length type";
		LogFL(DEBUG) << "SQLMaker::SendRequest(): bad reply length type = " << mp_typeof(*response_buffer.Data()) << "\n";
		return TPResponse();
	}
	int body = tp_get_uint(response_buffer.Data());
	LogFL(DEBUG) << "SQLMaker::SendRequest(): body length = " << body << "\n";
	response_buffer.Resize(MSG_HEADER_SIZE + body);
	len = ses.Receive(response_buffer, MSG_HEADER_SIZE);
	LogFL(DEBUG) << "SQLMaker::SendRequest(): received = " << len << "\n";
	if (len < body) {
		last_error = "receiving body failed";
		LogFL(DEBUG) << "SQLMaker::SendRequest(): receiving body failed, len = " << len << "\n";
		return TPResponse();
	}
	return TPResponse(response_buffer);
}

SQLMaker::~SQLMaker() { }