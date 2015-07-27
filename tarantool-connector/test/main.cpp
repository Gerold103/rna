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
		std::cout << statement_list->parser_msg << std::endl;
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
	std::vector<MValue> results;

	MValue res;

	//~~~~~~~~~~~~~~~~~~~~~~~~ C R E A T E   T A B L E   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	std::cout << "~~~~~~~~ C r e a t e   t a b l e   t e s t s ~~~~~~~~" << std::endl;

	sql_queries.push_back("CREATE TABLE users (id INT, name TEXT, surname TEXT, salary INT, PRIMARY KEY (id, salary), UNIQUE(id), INDEX(name, surname), INDEX(name, salary));");
	results.push_back(MValue(true));

	sql_queries.push_back("CREATE TABLE users (id INT, name TEXT, foo INT, bar DOUBLE);");
	results.push_back(MValue(false));

	sql_queries.push_back("CREATE TABLE books (id INT, user_id INT, name TEXT, author TEXT, year INT, PRIMARY KEY (id, user_id), INDEX(name), INDEX(author));");
	results.push_back(MValue(true));

	for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
		res = MakeQuery(sql_queries[i]);
		std::cout << "test n = " << i << " accepted: " << (res == results[i]) << std::endl;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~ I N S E R T   I N T O   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	std::cout << "~~~~~~~~ I n s e r t   i n t o   t e s t s ~~~~~~~~" << std::endl;

	sql_queries.clear();
	results.clear();

	sql_queries.push_back("INSERT INTO users VALUES (1, 'Ivan', 'Ivanov', 1000);");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO users (id, name, surname, salary) VALUES (2, 'Vasya', 'Vasyov', 2000);");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO users (name, surname) VALUES ('Petr', 'Petrov');");
	results.push_back(MValue(true));

	sql_queries.push_back("INSERT INTO users VALUES (3, 'Nikolay', 'Nikolayev', 1000, 'trash', 12345, 6789.123)");
	results.push_back(MValue(true));



	sql_queries.push_back("INSERT INTO users VALUES (1, 'Ivan', 'Ivanov', 1000);");
	results.push_back(MValue(false));

	sql_queries.push_back("INSERT INTO users (name, surname) VALUES ('Ksenia', 'Ivanova');");
	results.push_back(MValue(false));

	sql_queries.push_back("INSERT INTO users (id, name) VALUES (5, 39)");
	results.push_back(MValue(false));

	sql_queries.push_back("INSERT INTO users (id, surname) VALUES (6)");
	results.push_back(MValue(false));

	sql_queries.push_back("INSERT INTO users (id, surname) VALUES (6, 7, 8)");
	results.push_back(MValue(false));

	try {
		for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
			res = MakeQuery(sql_queries[i]);
			std::cout << "test n = " << i << " accepted: " << (MValue(!!res) == results[i]) << std::endl;
		}
	} catch (std::string &m) { std::cout << "Error while testing: " << m << std::endl; }
	catch (...) { std::cout << "error" << std::endl; }

	//~~~~~~~~~~~~~~~~~~~~~~~~ D R O P   T A B L E   t e s t s ~~~~~~~~~~~~~~~~~~~~~~~~

	std::cout << "~~~~~~~~ D r o p   t a b l e   t e s t s ~~~~~~~~" << std::endl;

	sql_queries.clear();
	results.clear();

	sql_queries.push_back("DROP TABLE users");
	results.push_back(MValue(true));

	sql_queries.push_back("DROP TABLE books");
	results.push_back(MValue(true));

	sql_queries.push_back("DROP TABLE foo_bar");
	results.push_back(MValue(false));

	for (size_t i = 0, size1 = sql_queries.size(), size2 = results.size(); (i < size1) && (i < size2); ++i) {
		res = MakeQuery(sql_queries[i]);
		//std::cout << "query maked. " << std::flush;
		//std::cout << "test n = " << i << " accepted: " << (res == results[i]) << std::endl;
	}

	std::cout << "sql_tarantool> " << std::flush;

	while(std::getline(std::cin, sql_query)) {
		MakeQuery(sql_query);
		std::cout << "sql_tarantool> " << std::flush;
	}

	return 0;
}