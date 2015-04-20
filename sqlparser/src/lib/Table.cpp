#include "Table.h"

namespace hsql {

TableRef::~TableRef() {
	delete select;
	if (list == NULL) {
		for (size_t i = 0, size = list->size(); i < size; ++i) {
			delete list->at(i);
		}
	}
	delete list;
	delete name;
}

std::string TableRefTypeToString(TableRefType tp)
{
	switch(tp)
	{
		case TableRefType::kTableName:
			return "kTableName";
		case TableRefType::kTableSelect:
			return "kTableSelect";
		case TableRefType::kTableJoin:
			return "kTableJoin";
		case TableRefType::kTableCrossProduct:
			return "kTableCrossProduct";
		default:
			return "#undef#";
	}
}

std::string JoinTypeToString(JoinType tp)
{
	switch(tp)
	{
		case JoinType::kJoinInner:
			return "kJoinInner";
		case JoinType::kJoinOuter:
			return "kJoinOuter";
		case JoinType::kJoinLeft:
			return "kJoinLeft";
		case JoinType::kJoinRight:
			return "kJoinRight";
		default:
			return "#undef#";
	}
}

std::ostream &operator<<(std::ostream &stream, const TableName &ob)
{
	stream << "TableName: " << std::endl;
	stream << "database: " << ob.database << ", name: " << ob.name << std::endl;
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const JoinDefinition &ob)
{
	stream << "JoinDefinition: " << std::endl;
	stream << "type: " << JoinTypeToString(ob.type) << std::endl;
	if (ob.left != NULL) {
		//stream << "left not null" << std::endl;
		stream << "left: " << *(ob.left) << std::endl;
	}
	else stream << "left is null" << std::endl;
	if (ob.right != NULL) {
		//stream << "right not null" << std::endl;
		stream << "right: " << *(ob.right) << std::endl;
	}
	else stream << "right is null" << std::endl;
	if (ob.condition != NULL) {
		stream << "condition: " << *(ob.condition) << std::endl;
	}
	else stream << "condition is null" << std::endl;
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const TableRef &ref)
{
	stream << "TableRef: " << std::endl;
	stream << "type: " << TableRefTypeToString(ref.type) << std::endl;
	if (!ref.schema.empty()) stream << "schema: " << ref.schema << std::endl;
	if (!ref.name->empty()) stream << "name: " << *(ref.name) << std::endl;
	if (!ref.alias.empty()) stream << "alias: " << ref.alias << std::endl;
	if (ref.select != NULL) stream << "select not null" << std::endl;
	else stream << "select is null" << std::endl;
	if (ref.list != NULL) stream << "list: size = " << ref.list->size() << std::endl;

	if (ref.prev_join != NULL) stream << "prev_join: " << *(ref.prev_join) << std::endl;
	else stream << "prev join is null" << std::endl;

	if (ref.join != NULL) {
		if (1) {
			stream << "middle join: " << *(ref.join) << std::endl;
		} else {
			stream << "join not null, but next join not null" << std::endl;
		}
	}
	else stream << "join is null" << std::endl;

	if (ref.next_join != NULL) {
		//stream << *(ref.next_join) << std::endl;
		stream << "next join not null" << std::endl;
	}
	else stream << "next join is null" << std::endl;
	return stream;
}

}