#ifndef SQL_MAKER_H_INCLUDED
#define SQL_MAKER_H_INCLUDED

#include <string>

#include "tarantool_schema_.h"
#include "tp_wrap.h"
#include "mvalue.h"
#include "tarantool_objs.h"
#include "logger.h"

#include "SQLParser.h"
#include "Table.h"
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
	Session *ses;
	TarantoolInfo *tinfo;
	std::string last_error;

public:
	SQLMaker(Session &ses_, TarantoolInfo &tinfo_);

	bool MakeEval(const std::string &function);

	std::string Error() const;

	virtual ~SQLMaker();
};


//~~~~~~~~~~~~~~~~~~~~~~~~ S Q L   C O N D I T I O N ~~~~~~~~~~~~~~~~~~~~~~~~

struct SQLCondition {
public:
	tp_iterator_type type;
	TColumn column;

	SQLCondition();
};

std::ostream &operator<<(std::ostream &stream, const SQLCondition &ob);
std::ostream &operator<<(std::ostream &stream, const std::vector<SQLCondition> &ob);

#endif