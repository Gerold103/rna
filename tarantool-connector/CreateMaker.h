#ifndef CREATE_MAKER_H_INCLUDED
#define CREATE_MAKER_H_INCLUDED

#include "SQLMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ C R E A T E   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

class CreateMaker : public SQLMaker {
private:
	CreateStatement *statement;

public:
	CreateMaker(Session &ses_, TarantoolInfo &tinfo_, CreateStatement *_statement);

	MValue MakeCreate();
};

#endif