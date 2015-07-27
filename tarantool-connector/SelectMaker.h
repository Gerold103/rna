#ifndef SELECT_MAKER_H_INCLUDED
#define SELECT_MAKER_H_INCLUDED

#include "SQLMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ S E L E C T   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

class SelectMaker : public SQLMaker {
private:
	SelectStatement *statement;

	std::map<std::string, int> aliases; //alias and its number in tuple

	SpaceObject MakeOneTable();
	SpaceObject MakeJoinTables();
	SpaceObject NextSpacePart(size_t space_id, size_t index, size_t offset, size_t limit);

public:
	SelectMaker(SelectStatement *_statement);

	SpaceObject MakeSelect();
};

#endif