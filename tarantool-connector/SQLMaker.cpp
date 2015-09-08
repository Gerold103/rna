#include "SQLMaker.h"

#include <algorithm>
#include <sstream>

//~~~~~~~~~~~~~~~~~~~~~~~~ S Q L M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

SQLMaker::SQLMaker(Session &ses_, TarantoolInfo &tinfo_) : ses(&ses_), tinfo(&tinfo_) { }

bool SQLMaker::MakeEval(const std::string &function)
{
	last_error.clear();
	size_t msg_size = MSG_START_SIZE;
	TP_p request(new TP(DataStructure(msg_size)));
	request->AddEval(function);
	
	int ss = ses->Send(*request);
	if (ss < 0) {
		last_error = "SQLMaker::MakeEval(): error while sending request was occured, return = " + std::to_string(ss);
		LogFL(DEBUG) << last_error << "\n";
		return false;
	}
	ss = ses->Sync();
	if (ss < 0) {
		last_error = "SQLMaker::MakeEval(): error while syncing sesson was occured, return = " + std::to_string(ss);
		LogFL(DEBUG) << last_error << "\n";
		return false;
	}
	return true;
}

std::string SQLMaker::Error() const
{
	return last_error;
}

SQLMaker::~SQLMaker() { }

//~~~~~~~~~~~~~~~~~~~~~~~~ S Q L   C O N D I T I O N ~~~~~~~~~~~~~~~~~~~~~~~~

SQLCondition::SQLCondition() { }

std::ostream &operator<<(std::ostream &stream, const SQLCondition &ob)
{
	stream << ob.column << ": " << ob.column.value;
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const std::vector<SQLCondition> &ob)
{
	for (int i = 0, size = static_cast<int>(ob.size()); i < size; ++i) {
		stream << ob[i];
		if (i < size - 1) stream << "; ";
	}
	return stream;
}