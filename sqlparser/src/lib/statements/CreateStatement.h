#ifndef __CREATE_STATEMENT_H__
#define __CREATE_STATEMENT_H__

#include "SQLStatement.h"

namespace hsql {

/**
 * @struct ColumnDefinition
 * @brief Represents definition of a table column
 */
struct ColumnDefinition {
	enum DataType {
		TEXT,
		INT,
		DOUBLE
	};

	ColumnDefinition(const char* name_, DataType type_) :
		name(),
		type(type_)
	{
		SetName(name_);
	}

	virtual ~ColumnDefinition() { }

	bool SetName(const char *s) {
		if (s == NULL) return false;
		else name = s;
		return true;
	}

	std::string name;
	DataType type;
};


/**
 * @struct CreateStatement
 * @brief Represents "CREATE TABLE students (name TEXT, student_number INTEGER, city TEXT, grade DOUBLE)"
 */
struct CreateStatement : SQLStatement {
	enum CreateType {
		kTable,
		kTableFromTbl, // Hyrise file format
	};

	CreateStatement(CreateType type) :
		SQLStatement(kStmtCreate),
		type(type),
		if_not_exists(false),
		columns(NULL),
		file_path(""),
		table_name(NULL) {};

	virtual ~CreateStatement() {
		if (columns != NULL) {
			for (size_t i = 0, size = columns->size(); i < size; ++i) {
				delete columns->at(i);
			}
		}
		delete columns;
	}

	CreateType type;
	bool if_not_exists;

	std::vector<ColumnDefinition*>* columns;

	bool SetFilePath(const char *s) {
		if (s == NULL) return false;
		else file_path = s;
		return true;
	}

	std::string file_path;
	TableName *table_name;
};



} // namespace hsql
#endif