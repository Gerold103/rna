#include "DropStatement.h"

namespace hsql {

DropStatement::DropStatement(DropStatement::EntityType type) : SQLStatement(kStmtDrop), type(type), name(NULL) { }

const std::string &DropStatement::GetName() const { return name->GetName(); }

const std::string &DropStatement::GetDatabase() const { return name->GetDatabase(); }

DropStatement::EntityType DropStatement::GetType() const { return type; }

std::string EntityTypeToString(DropStatement::EntityType tp)
{
	switch(tp) {
		case DropStatement::kTable: return "kTable";
		case DropStatement::kSchema: return "kSchema";
		case DropStatement::kIndex: return "kIndex";
		case DropStatement::kView: return "kView";
		case DropStatement::kPreparedStatement: return "kPreparedStatement";
		default: return "default";
	}
}

std::ostream &operator<<(std::ostream &stream, const DropStatement &ob)
{
	stream << "DropStatement: type = " << EntityTypeToString(ob.GetType()) << ", name = " << ob.GetName() << ", database = " << ob.GetDatabase();

	return stream;
}

}