#ifndef DROP_MAKER_H_INCLUDED
#define DROP_MAKER_H_INCLUDED

#include "SQLMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ D R O P   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

class DropMaker : public SQLMaker {
private:
	DropStatement *statement;

public:
	DropMaker(Session &ses_, TarantoolInfo &tinfo_, DropStatement *_statement);

	MValue MakeDrop();
};

#endif