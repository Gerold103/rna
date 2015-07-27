#include "CreateStatement.h"

namespace hsql {

//~~~~~~~~~~~~~~~~~~~~~~~~ C o l u m n   D e f i n i t i o n ~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

ColumnDefinition::ColumnDefinition(const char* name_, DataType type_) : name(), type(type_)
{
	SetName(name_);
}

ColumnDefinition::ColumnDefinition(const std::vector<std::string> &col_names, ColumnDefinition::DataType tp) : type(tp), column_names(col_names) { }

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

bool ColumnDefinition::SetName(const char *s) {
	if (s == NULL) return false;
	else name = s;
	return true;
}

//~~~~~~~~ G e t   m e t h o d s ~~~~~~~~

const std::string &ColumnDefinition::GetName() const
{
	return name;
}

ColumnDefinition::DataType ColumnDefinition::GetDataType() const
{
	return type;
}

const std::vector<std::string> &ColumnDefinition::GetColNames() const
{
	return column_names;
}

bool ColumnDefinition::IsIndexDefinition() const
{
	return (GetDataType() == ColumnDefinition::PRIMARY) || (GetDataType() == ColumnDefinition::UNIQUE) || (GetDataType() == ColumnDefinition::INDEX);
}

//~~~~~~~~ O t h e r ~~~~~~~~

ColumnDefinition::~ColumnDefinition() { }

std::string ToString(ColumnDefinition::DataType dt)
{
	switch(dt) {
		case ColumnDefinition::DataType::TEXT: return "TEXT";
		case ColumnDefinition::DataType::INT: return "INT";
		case ColumnDefinition::DataType::DOUBLE: return "DOUBLE";
		case ColumnDefinition::DataType::PRIMARY: return "PRIMARY";
		case ColumnDefinition::DataType::UNIQUE: return "UNIQUE";
		case ColumnDefinition::DataType::INDEX: return "INDEX";
		default: return "DEFAULT";
	}	
}

//~~~~~~~~ O p e r a t o r s ~~~~~~~~

std::ostream &operator<<(std::ostream &stream, const ColumnDefinition &ob)
{
	stream << "ColumnDefinition: data_type = " << ToString(ob.GetDataType()) << ", name = ";
	if (ob.GetName().length() > 0) stream << "'" << ob.GetName() << "'";
	else stream << "none";
	stream << std::endl;
	stream << "columns_list: ";
	if (ob.GetColNames().size()) {
		for (int i = 0, size = static_cast<int>(ob.GetColNames().size()); i < size; ++i) {
			stream << ob.GetColNames()[i];
			if (i < size - 1) stream << ", ";
		}
	} else {
		stream << "none";
	}
	stream << std::endl;
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const std::vector<ColumnDefinition *> &ob)
{
	for (int i = 0, size = static_cast<int>(ob.size()); i < size; ++i) {
		stream << "[" << i << "]: " << *(ob[i]) << std::endl;
	}
	return stream;
}

//~~~~~~~~~~~~~~~~~~~~~~~~ C r e a t e   S t a t e m e n t ~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

CreateStatement::CreateStatement(CreateType type) : SQLStatement(kStmtCreate), type(type), if_not_exists(false), columns(NULL),
file_path(""), table_name(NULL) { };

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

bool CreateStatement::SetFilePath(const char *s) {
	if (s == NULL) return false;
	else file_path = s;
	return true;
}

//~~~~~~~~ G e t   m e t h o d s ~~~~~~~~

CreateStatement::CreateType CreateStatement::GetType() const
{
	return type;
}

bool CreateStatement::IsNotExists() const
{
	return if_not_exists;
}

const std::vector<ColumnDefinition*> &CreateStatement::GetColumns() const
{
	return *columns;
}

const std::string &CreateStatement::GetFilePath() const
{
	return file_path;
}

const TableName &CreateStatement::GetTableName() const
{
	return *table_name;
}

//~~~~~~~~ O t h e r ~~~~~~~~

CreateStatement::~CreateStatement() {
	if (columns != NULL) {
		for (size_t i = 0, size = columns->size(); i < size; ++i) {
			delete columns->at(i);
		}
	}
	delete columns;
}

std::string ToString(CreateStatement::CreateType ct)
{
	switch(ct) {
		case CreateStatement::CreateType::kTable: return "kTable";
		case CreateStatement::CreateType::kTableFromTbl: return "kTableFromTbl";
		default: return "Defualt";
	}
}

//~~~~~~~~ O p e r a t o r s ~~~~~~~~

std::ostream &operator<<(std::ostream &stream, const CreateStatement &ob)
{
	stream << "CreateStatement: type = " << ToString(ob.GetType()) << ", if_not_exists = " << ob.IsNotExists() << ", columns: " << ob.GetColumns() << std::endl;
	stream << "file_path = " << ob.GetFilePath() << ", table_name: " << std::string(ob.GetTableName()) << std::endl;
	return stream;
}

}