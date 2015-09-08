#include <iostream>
#include <cstring>
#include "sqlconnector.h"

int main(int argc, char **argv)
{
	std::cout << "hello" << std::endl;
	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i) {
		args.push_back(std::string(argv[i]));
	}
	if ((args.size() != 1) && (args.size() != 0)) {
		std::cout << "Arguments count must be 1: port, or 0, then port is 33013" << std::endl;
		return 0;
	}
	int port = 33013;
	if (args.size()) {
		port = std::stoi(args[0]);
	}
	Session ses(port);

	if (ses.Connect() == -1) {
		std::cout << "Connection failed, error code: " << ses.GetErrorCode() << ";" << std::endl;
		return 0;
	} else {
		std::cout << "Conected;" << std::endl;
	}

	DataStructure greet(128);
	ses.Receive(greet);

	TarantoolInfo tinfo(ses);


	std::string sql_query;

	std::cout << "sql_tarantool> " << std::flush;

	while(std::getline(std::cin, sql_query)) {
		std::shared_ptr<SQLStatementList> statement_list(SQLParser::parseSQLString(sql_query));
		if (statement_list->isValid == false) {
			std::cout << "Parse error: " << statement_list->parser_msg << std::endl;
			std::cout << "sql_tarantool> " << std::flush;
			continue;
		}
		for (size_t i = 0, size = statement_list->numStatements(); i < size; ++i) {
			SQLStatement *statement = statement_list->getStatement(i);

			switch(statement->type()) {
				case StatementType::kStmtError:
					std::cout << "Statement type is kStmtError" << std::endl;
					break;
				case StatementType::kStmtSelect: {
					std::cout << "Statement type is kStmtSelect" << std::endl;
					SelectMaker maker(ses, tinfo, dynamic_cast<SelectStatement *>(statement));
					SpaceObject tmp = maker.MakeSelect();
					std::cout << "Result, size = " << tmp.Size() << ": \n" << tmp << std::endl;
					break;
				}
				case StatementType::kStmtCreate: {
					std::cout << "Statement type is kStmtCreate" << std::endl;
					CreateMaker maker(ses, tinfo, dynamic_cast<CreateStatement *>(statement));
					MValue tmp = maker.MakeCreate();
					std::cout << "Result: " << tmp << std::endl;
					break;
				}
				case StatementType::kStmtDrop: {
					std::cout << "Statement type is kStmtDrop" << std::endl;
					DropMaker maker(ses, tinfo, dynamic_cast<DropStatement *>(statement));
					MValue tmp = maker.MakeDrop();
					std::cout << "Result: " << tmp << std::endl;
					break;
				}
				case StatementType::kStmtInsert: {
					std::cout << "Statement type is kStmtInsert" << std::endl;
					InsertMaker maker(ses, tinfo, dynamic_cast<InsertStatement *>(statement));
					MValue tmp = maker.MakeInsert();
					std::cout << "Result: " << tmp << std::endl;
					break;
				}
				default:
					std::cout << "Statement type unknown" << std::endl;
					break;
			}
		}
		std::cout << "sql_tarantool> " << std::flush;
	}
	return 0;
}