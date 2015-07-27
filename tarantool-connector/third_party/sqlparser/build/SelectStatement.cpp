#include "SelectStatement.h"

namespace hsql {

std::string OrderTypeToString(OrderType tp)
{
	switch(tp)
	{
		case OrderType::kOrderAsc:
			return "kOrderAsc";
		case OrderType::kOrderDesc:
			return "kOrderDesc";
		default:
			return "#undef#";
	}
}

std::ostream &operator<<(std::ostream &stream, const OrderDescription &ob)
{
	stream << "OrderDescription: " << std::endl;
	stream << "type: " << OrderTypeToString(ob.type) << std::endl;
	//expr
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const LimitDescription &ob)
{
	stream << "LimitDescription: " << std::endl;
	stream << "limit: " << ob.limit << std::endl;
	stream << "offset: " << ob.offset << std::endl;
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const SelectStatement &ob)
{
	stream << "SelectStatement: " << std::endl;
	if (ob.from_table != NULL) {
		//stream << "from table not null" << std::endl;
		stream << *(ob.from_table) << std::endl;
	}
	else stream << "from_table is null" << std::endl;

	stream << "is distinct: " << ob.select_distinct << std::endl;

	if (ob.select_list != NULL) {
		std::vector<Expr*>& sel_list = *(ob.select_list);
		for (int i = 0, size = ob.select_list->size(); i < size; ++i) {
			stream << "expr " << i << ": " << *(sel_list[i]) << std::endl;
		}
	}
	else stream << "select_list is null" << std::endl;

	if (ob.where_clause != NULL) {
		stream << *(ob.where_clause) << std::endl;
	}
	else stream << "where_clause is null" << std::endl;

	if (ob.group_by != NULL) stream << "group_by not null" << std::endl;
	else stream << "group_by is null" << std::endl;

	if (ob.union_select != NULL) stream << "union_select not null" << std::endl;
	else stream << "union_select is null" << std::endl;

	if (ob.order != NULL) stream << *(ob.order) << std::endl;
	else stream << "order is null" << std::endl;

	if (ob.limit != NULL) stream << *(ob.limit) << std::endl;
	else stream << "limit is null" << std::endl;
	
	return stream;
}

}