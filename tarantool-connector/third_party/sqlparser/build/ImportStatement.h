#ifndef __IMPORT_STATEMENT_H__
#define __IMPORT_STATEMENT_H__

#include "SQLStatement.h"

namespace hsql {




/**
 * @struct ImportStatement
 * @brief Represents "IMPORT"
 */
struct ImportStatement : SQLStatement {
	enum ImportType {
		kImportCSV,
		kImportTbl,
	};


	ImportStatement(ImportType type) :
		SQLStatement(kStmtImport),
		type(type),
		file_path(""),
		table_name(NULL) {};

	bool SetFilePath(const char *s) {
		if (s == NULL) return false;
		else file_path = s;
		return true;
	}

	~ImportStatement() { delete table_name; }

	ImportType type;
	std::string file_path;
	TableName *table_name;
};



} // namespace hsql


#endif