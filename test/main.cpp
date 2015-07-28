#include <iostream>
#include <memory>
#include <cstring>
#include <vector>

#include "tarantool_schema_.h"
#include "tp_wrap.h"
#include "mvalue.h"
#include "tarantool_objs.h"
#include "logger.h"

#include "SQLParser.h"
#include "SQLMaker.h"
#include "SelectMaker.h"
#include "CreateMaker.h"
#include "DropMaker.h"
#include "InsertMaker.h"
#include <cstddef>
#include <cstdint>
extern "C" {
	#include <session.h>
	#include <tp.h>
}

using namespace hsql;


void hexDump(char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

MValue MakeQuery(const std::string &sql_query)
{
	if (sql_query.substr(0, 5) == "FUNC:") {
		SQLMaker maker;
		std::string tmp(sql_query.substr(5, sql_query.length() - 5));
		return MValue(maker.MakeEval(tmp));
	}

	std::shared_ptr<SQLStatementList> statement_list(SQLParser::parseSQLString(sql_query));
	if (statement_list->isValid == false) {
		LogFL(DEBUG) << statement_list->parser_msg << "\n";
		return MValue(false);
	}

	for (size_t i = 0, size = statement_list->numStatements(); i < size; ++i) {
		SQLStatement *statement = statement_list->getStatement(i);

		switch(statement->type()) {
			case StatementType::kStmtError:
				LogFL(DEBUG) << "main(): statement type is kStmtError\n";
				return MValue(false);
			case StatementType::kStmtSelect: {
				LogFL(DEBUG) << "main(): statement type is kStmtSelect\n";
				SelectMaker maker(dynamic_cast<SelectStatement *>(statement));
				MValue tmp = maker.MakeSelect();
				LogFL(DEBUG) << "main(): MakeSelect result = " << tmp << "\n";
				return tmp;
			}
			case StatementType::kStmtCreate: {
				LogFL(DEBUG) << "main(): statement type is kStmtCreate\n";
				CreateMaker maker(dynamic_cast<CreateStatement *>(statement));
				MValue tmp = maker.MakeCreate();
				LogFL(DEBUG) << "main(): MakeCreate result = " << tmp << "\n";
				return tmp;
			}
			case StatementType::kStmtDrop: {
				LogFL(DEBUG) << "main(): statement type is kStmtDrop\n";
				DropMaker maker(dynamic_cast<DropStatement *>(statement));
				MValue tmp = maker.MakeDrop();
				LogFL(DEBUG) << "main(): MakeDrop result = " << tmp << "\n";
				return tmp;
			}
			case StatementType::kStmtInsert: {
				LogFL(DEBUG) << "main(): statement type is kStmtInsert\n";
				InsertMaker maker(dynamic_cast<InsertStatement *>(statement));
				MValue tmp = maker.MakeInsert();
				LogFL(DEBUG) << "main(): MakeInsert result = " << tmp << "\n";
				return tmp;
			}
			default:
				LogFL(DEBUG) << "main(): statement type unknown\n";
				return MValue(false);
		}
	}
}

int main()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ I n i t   T a r a n t o o l   S c h e m a ~~~~~~~~~~~~~~~~~~~~~~~~

	//Logger::SetStreamForMessages(DEBUG, std::cout);
	Session ses;
	if (ses.Connect() == -1) {
		LogFL(DEBUG) << "main(): connection failed, error code: " << ses.GetErrorCode() << ";\n";
		return 0;
	} else {
		LogFL(DEBUG) << "main(): conected;\n";
	}

	DataStructure greet(128);
	LogFL(DEBUG) << "main(): greet size: " << ses.Receive(greet) << "\n";
	TarantoolSchema schm(ses);

	Logger::LogObject(DEBUG, schm);

	//~~~~~~~~~~~~~~~~~~~~~~~~ M a k i n g   q u e r y ~~~~~~~~~~~~~~~~~~~~~~~~

	std::string sql_query;

	std::vector<std::string> sql_queries;

	MValue res;

	//~~~~~~~~~~~~~~~~~~~~~~~~ C R E A T E   T A B L E   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	//must return
	sql_queries.push_back("CREATE TABLE users (id INT, name TEXT, surname TEXT, salary INT, PRIMARY KEY (id, salary), UNIQUE(id), INDEX(name, surname), INDEX(name, salary));");

	//must return
	sql_queries.push_back("CREATE TABLE users (id INT, name TEXT, foo INT, bar DOUBLE);");

	//must return
	sql_queries.push_back("CREATE TABLE books (id INT, user_id INT, name TEXT, author TEXT, year INT, PRIMARY KEY (id, user_id), INDEX(name), INDEX(author));")

	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~start~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

	res = MakeQuery(sql_queries[0]);

	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~end~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

	res = MakeQuery(sql_queries[1]);

	res = MakeQuery(sql_queries[2]);

	std::cout << "sql_tarantool> " << std::flush;

	while(std::getline(std::cin, sql_query)) {
		MakeQuery(sql_query);
		std::cout << "sql_tarantool> " << std::flush;
	}

	return 0;
}