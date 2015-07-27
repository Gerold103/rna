#ifndef __CREATE_STATEMENT_H__
#define __CREATE_STATEMENT_H__

#include "SQLStatement.h"
#include "Table.h"

namespace hsql {

/**
 * @struct ColumnDefinition
 * @brief Represents definition of a table column
 */
struct ColumnDefinition {

	//~~~~ T y p e s ~~~~

	enum DataType {
		TEXT,
		INT,
		DOUBLE,
		PRIMARY,
		UNIQUE,
		INDEX
	};

	//~~~~ A t t r i b u t e s ~~~~

	std::string name;
	DataType type;
	std::vector<std::string> column_names;

	//~~~~ C o n s t r u c t o r s ~~~~

	ColumnDefinition(const char* name_, DataType type_);

	ColumnDefinition(const std::vector<std::string> &col_names, DataType tp);

	//~~~~ G e t   m e t h o d s ~~~~

	const std::string &GetName() const;

	DataType GetDataType() const;

	const std::vector<std::string> &GetColNames() const;

	bool IsIndexDefinition() const;

	//~~~~ S e t   m e t h o d s ~~~~

	bool SetName(const char *s);

	//~~~~ O t h e r ~~~~

	virtual ~ColumnDefinition();
};

std::string ToString(ColumnDefinition::DataType dt);


/**
 * @struct CreateStatement
 * @brief Represents "CREATE TABLE students (name TEXT, student_number INTEGER, city TEXT, grade DOUBLE)"
 */
struct CreateStatement : SQLStatement {

	//~~~~ T y p e s ~~~~

	enum CreateType {
		kTable,
		kTableFromTbl,
	};

	//~~~~ A t t r i b u t e s ~~~~

	CreateType type;
	bool if_not_exists;

	std::vector<ColumnDefinition*>* columns;

	std::string file_path;
	TableName *table_name;

	//~~~~ C o n s t r u c t o r s ~~~~

	CreateStatement(CreateType type);

	//~~~~ G e t   m e t h o d s ~~~~

	CreateType GetType() const;
	bool IsNotExists() const;
	const std::vector<ColumnDefinition*> &GetColumns() const;
	const std::string &GetFilePath() const;
	const TableName &GetTableName() const;

	//~~~~ S e t   m e t h o d s ~~~~

	bool SetFilePath(const char *s);

	//~~~~ O t h e r ~~~~

	virtual ~CreateStatement();
};

std::string ToString(CreateStatement::CreateType ct);

std::ostream &operator<<(std::ostream &stream, const ColumnDefinition &ob);
std::ostream &operator<<(std::ostream &stream, const std::vector<ColumnDefinition *> &ob);
std::ostream &operator<<(std::ostream &stream, const CreateStatement &ob);

} // namespace hsql
#endif