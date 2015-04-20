#ifndef __SQLPARSER_H_
#define __SQLPARSER_H_

#include "sqllib.h"
#include "bison_parser.h"

namespace hsql {
std::ostream &operator<<(std::ostream &stream, const std::vector<std::string> &ob);
std::vector<std::string> read_key_words();
/*!
 * \mainpage SQLParser (C++)
 */
std::string ReplaceSQLTokens(const char *text);
/*!
 * @brief Main class for parsing SQL strings
 */
class SQLParser {
public:
	static SQLStatementList* parseSQLString(const char* sql);
	static SQLStatementList* parseSQLString(const std::string& sql);

private:
	SQLParser();
};

	
} // namespace hsql


#endif