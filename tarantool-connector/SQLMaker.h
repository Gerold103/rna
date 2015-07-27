#ifndef SQL_MAKER_H_INCLUDED
#define SQL_MAKER_H_INCLUDED

#include <string>

#include "tarantool_schema_.h"
#include "tp_wrap.h"
#include "mvalue.h"
#include "tarantool_objs.h"
#include "logger.h"

#include "SQLParser.h"
extern "C" {
	#include "session.h"
	#include "tp.h"
}

#define MSG_START_SIZE 1024
#define MSG_MAX_SIZE 1048576
#define MSG_START_RECS_COUNT 3

using namespace hsql;

//~~~~~~~~~~~~~~~~~~~~~~~~ S Q L M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

class SQLMaker {
protected:
	Session ses;
	TarantoolSchema schm;
	bool is_connected;

	std::string last_error;
public:
	SQLMaker();

	bool MakeEval(const std::string &function);

	bool IsConnected() const;
	std::string Error() const;

	TPResponse SendRequest(TP_p &request);

	virtual ~SQLMaker();
};


#endif