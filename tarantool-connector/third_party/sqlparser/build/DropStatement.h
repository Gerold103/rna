#ifndef __DROP_STATEMENT_H__
#define __DROP_STATEMENT_H__

#include "SQLStatement.h"
#include "Table.h"
#include <ostream>

namespace hsql {


/**
 * @struct DropStatement
 * @brief Represents "DROP TABLE"
 */
struct DropStatement : SQLStatement {
	enum EntityType {
		kTable,
		kSchema,
		kIndex,
		kView,
		kPreparedStatement
	};


	DropStatement(EntityType type);

	EntityType type;
	TableName* name;

	const std::string &GetName() const;
	const std::string &GetDatabase() const;

	EntityType GetType() const;
};

std::string EntityTypeToString(DropStatement::EntityType tp); 

std::ostream &operator<<(std::ostream &stream, const DropStatement &ob);

} // namespace hsql
#endif