#ifndef SELECT_MAKER_H_INCLUDED
#define SELECT_MAKER_H_INCLUDED

#include "SQLMaker.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ S E L E C T   M A K E R ~~~~~~~~~~~~~~~~~~~~~~~~

class SelectMaker : public SQLMaker {
private:
	SelectStatement *statement;

	std::map<std::string, int> aliases; //alias and its number in tuple

	SpaceObject MakeOneTableInTarantool();
	std::vector<SQLCondition> GetIndexConditions(const Expr *where) const;


	SpaceObject MakeOneTable();
	SpaceObject MakeJoinTables();
	SpaceObject NextSpacePart(size_t space_id, size_t index, size_t offset, size_t limit, tp_iterator_type iterator = TP_ITERATOR_ALL, const std::vector<MValue> &keys = std::vector<MValue>());

public:
	SelectMaker(Session &ses_, TarantoolInfo &tinfo_, SelectStatement *_statement);

	SpaceObject MakeSelect();

	SpaceObject MakeSelectInTarantool();
};

#endif