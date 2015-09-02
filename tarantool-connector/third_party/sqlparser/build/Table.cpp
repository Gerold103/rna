#include "Table.h"

namespace hsql {

//~~~~~~~~~~~~~~~~~~~~~~~~ T A B L E   N A M E ~~~~~~~~~~~~~~~~~~~~~~~~

bool TableName::SetName(const char *s) {
	if (s == NULL) return false;
	else name = s;
	return true;
}

bool TableName::SetDatabase(const char *s) {
	if (s == NULL) return false;
	else database = s;
	return true;
}

bool TableName::empty() const { return name.empty() && (database.empty()); }

TableName::operator std::string() const
{
	if (database.empty()) return name;
	else return database + "." + name;
}

TableName::TableName() { }

TableName::TableName(std::string n) : name(n) { }

TableName::TableName(std::string n, std::string d) : name(n), database(d) { }

TableName::TableName(const char *n) { SetName(n); }

TableName::TableName(const char *n, const char *d) { SetName(n); SetDatabase(d); }

const std::string &TableName::GetName() const { return name; }

const std::string &TableName::GetDatabase() const { return database; }

std::ostream &operator<<(std::ostream &stream, const TableName &ob)
{
	stream << "TableName: " << std::endl;
	stream << "database: " << ob.database << ", name: " << ob.name << std::endl;
	return stream;
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T A B L E   R E F ~~~~~~~~~~~~~~~~~~~~~~~~

TableRef::TableRef(TableRefType type)
	: ASTNode(ASTNode::CATALOG_OBJECT), type(type), name(NULL), select(NULL), list(NULL), prev_join(NULL), join(NULL), next_join(NULL) { }

bool TableRef::SetSchema(const char *s) {
	if (s == NULL) return false;
	else schema = s;
	return true;
}
bool TableRef::SetAlias(const char *s) {
	if (s == NULL) return false;
	else alias = s;
	return true;
}

bool TableRef::hasSchema() { return !schema.empty(); }

std::string TableRef::getName() {
	if (!alias.empty()) return alias;
	else return std::string(*name);
}

TableRefType TableRef::GetType() const
{
	return type;
}

TableRef::~TableRef() {
	delete select;
	if (list != NULL) {
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

//~~~~~~~~~~~~~~~~~~~~~~~~ J O I N   D E F I N I T I O N ~~~~~~~~~~~~~~~~~~~~~~~~

JoinDefinition::JoinDefinition() : left(NULL), right(NULL), condition(NULL), type(kJoinInner) { }

JoinDefinition::~JoinDefinition() {
	delete left;
	delete right;
	delete condition;
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
	if (ref.name == NULL) stream << "name is null" << std::endl;
	else if (!ref.name->empty()) stream << "name: " << *(ref.name) << std::endl;
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