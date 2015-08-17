#ifndef INSERT_MAKER_H_INCLUDED
#define INSERT_MAKER_H_INCLUDED

#include "SQLMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ I N S E R T   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

class InsertMaker : public SQLMaker {
private:
	InsertStatement *statement;

public:
	InsertMaker(Session &ses_, TarantoolInfo &tinfo_, InsertStatement *_statement);

	MValue MakeInsert();
};

#endif