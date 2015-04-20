#include <iostream>
#include <ostream>
#include "SQLParser.h"

using namespace hsql;

int main()
{
	// auto kw = read_key_words();
	// std::cout << kw << std::endl;
	// for (size_t i = 0, size = kw.size(); i < size; ++i) {
	// 	std::cout << "Next query: " << i << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT " + kw[i] + " FROM orders;");
	// 	std::cout << list->numStatements() << std::endl;
	// 	if (list->numStatements() < 1) {
	// 		std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
	// 	}
	// }
	{
		std::cout << "Next query: 1" << std::endl;
		auto list = hsql::SQLParser::parseSQLString("SELECT \"table\" FROM orders;");
		std::cout << list->numStatements() << std::endl;
		for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
			std::cout << "next statement: " << list->statements[i]->type() << std::endl;
		}
			if (list->numStatements() < 1) {
			std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
		}
		std::cout << std::endl;
	}
	{
		std::cout << "Next query: 2" << std::endl;
		auto list = hsql::SQLParser::parseSQLString("CREATE TABLE tbltbl FROM TBL FILE 'students.tbl';");
		std::cout << list->numStatements() << std::endl;
		for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
			std::cout << "next statement: " << list->statements[i]->type() << std::endl;
		}
			if (list->numStatements() < 1) {
			std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
		}
		std::cout << std::endl;
	}
	// {
	// 	std::cout << "Next query: 2" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT a FROM foo WHERE a > 12 OR b > 3 AND NOT c LIMIT 10");
	// 	std::cout << list->numStatements() << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 3" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT col1 AS myname, col2, 'test' FROM \"table\", foo AS t WHERE age > 12 AND zipcode = 12345 GROUP BY col1;");
	// 	std::cout << list->numStatements() << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 4" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT * from \"table\" JOIN table2 ON a = b WHERE (b OR NOT a) AND a = 12.5");
	// 	std::cout << list->numStatements() << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 5" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT * FROM \"table\" LIMIT 10 OFFSET 10; SELECT * FROM second;");
	// 	std::cout << list->numStatements() << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 6" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT * FROM t1 UNION SELECT * FROM t2 ORDER BY col1;");
	// 	std::cout << list->numStatements() << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 7" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT t1.a, t1.b, t2.c FROM \"table\" AS t1 JOIN (SELECT * FROM foo JOIN bar ON foo.id = bar.id) t2 ON t1.a = t2.b WHERE (t1.b OR NOT t1.a) AND t2.c = 12.5");
	// 	std::cout << list->numStatements() << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 7.5" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT t1.a, t1.b, t2.c, (imap.ConnectionMode = 'ssl') as f_1 FROM \"table\" WHERE (t1.b OR NOT t1.a) AND t2.c = 12.5");
	// 	std::cout << list->numStatements() << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 8" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("select UserID as f_24 , UserEmail as f_23 , Password as f_22 , Email as f_21 , Folder as f_20 , Time as f_19 , KeepTime as f_18 , LastTime as f_17 , LastOK as f_16 , LastMsg as f_15 , Flags as f_14 , ContactFetchRetries as f_13 , OldThreshold as f_12 , EncPassword as f_11 , (AutoConfigure = 'no') as f_10 , User as f_9 , rpop.imap.ID as f_8 , IF(rpop.imap.flags & 262144, race.Host, rice.Host) as f_7 , IF(rpop.imap.flags & 262144, race.Port, rice.Port) as f_6 , (IF(rpop.imap.flags & 262144, race.ConnectionMode, rice.ConnectionMode) = 'ssl') as f_5 , IF(rpop.imap.flags & 262144, race.UserNameMode, rice.UserNameMode) as f_4 , rpop.imap.Host as f_3 , rpop.imap.Port as f_2 , (rpop.imap.ConnectionMode = 'ssl') as f_1  from rpop.imap  LEFT JOIN rpop.imap_configured_email rice ON (rpop.imap.flags & 262144 = 0 and rice.EmailDomain = SUBSTRING_INDEX(Email, '@', -1)) LEFT JOIN rpop.activesync_configured_email race ON (rpop.imap.flags & 262144 != 0 and race.EmailDomain = SUBSTRING_INDEX(Email, '@', -1)) where ((rpop.imap.flags & 262144  = 0 and (rice.Enable = 'yes' or rice.Enable is NULL)) or (rpop.imap.flags & 262144 != 0 and (race.Enable = 'yes' or race.Enable is NULL))) and UserID = 1006051515");
	// 	std::cout << list->numStatements() << std::endl;
	// 	std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 9" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("select abc from rpop LEFT JOIN race ON (rpop != 0) where ((rice.Enable is NULL) or (rpop.imap.flags & 262144 != 0 and (race.Enable = 'yes' or race.Enable is NULL))) and UserID = 1006051515");
	// 	std::cout << list->numStatements() << std::endl;
	// 	std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 10" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("select flags, Host1, Host2 from table1 LEFT JOIN table2 ON (table1.id = table2.id and msg1 = msg2) LEFT JOIN table3 ON (msg3 = msg4)");
	// 	std::cout << list->numStatements() << std::endl;
	// 	//std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 11" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("select flags, Host1, Host2 from table1 WHERE id != 5");
	// 	std::cout << list->numStatements() << std::endl;
	// 	//std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 11.5" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("select \"time\", tbll as tm from rpop.imap;");
	// 	std::cout << list->numStatements() << std::endl;
	// 	//std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	// {
	// 	std::cout << "Next query: 12" << std::endl;
	// 	auto list = hsql::SQLParser::parseSQLString("SELECT rpop.imap.indent as id FROM rpop.imap;");
	// 	std::cout << list->numStatements() << std::endl;
	// 	//std::cout << "err line: " << list->error_line << ", col: " << list->error_col << ", msg: " << list->parser_msg << std::endl;
	// 	for (size_t i = 0, size = list->numStatements(); i < size; ++i) {
	// 		if (list->statements[i] == NULL) {
	// 			std::cout << "statement " << i << " is NULL" << std::endl;
	// 			continue;
	// 		}
	// 		std::cout << "next statement: " << list->statements[i]->type() << std::endl;
	// 		switch(list->statements[i]->type())
	// 		{
	// 			case StatementType::kStmtSelect: {
	// 				std::cout << "select statement: " << std::endl;
	// 				SelectStatement *statement = dynamic_cast<SelectStatement *>(list->statements[i]);
	// 				std::cout << *statement << std::endl;
	// 				break;
	// 			}
	// 		}
	// 	}
	// 	std::cout << std::endl;
	// }
	return 0;
}