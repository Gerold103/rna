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
	#include "session.h"
	#include "tp.h"
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

int add_array_elem_test(TarantoolSchema &schm, const char **data)
{
	const char *tuple = *data;
	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "add_space_test(): typeof raw tuple not equal to MP_ARRAY, but equal = " << mp_typeof(*tuple) << "\n";
		throw MakeDbgStr("add_space_test(): typeof raw tuple not equal to MP_ARRAY, but equal = " + std::to_string(mp_typeof(*tuple)));
	}

	uint32_t tuple_len = mp_decode_array(&tuple);	
	MValueVector decoded_array;
	for (size_t i = 0; i < tuple_len; ++i) {
		decoded_array.push_back(MValue::FromMSGPack(&tuple));
	}
	*data = tuple;
	return 0;
}

int TestFunc(TarantoolSchema &schm, const DataStructure &data)
{
	const char *tuple = data.Data();
	if (mp_check(&tuple, tuple + data.Size()))
		return -1;
	tuple = data.Data();
	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "TestFunc(): tuple is not MP_ARRAY, but is equal to " << mp_typeof(*tuple) << "\n";
		return -1;
	}
	LogFL(DEBUG) << "TestFunc(): tuple is MP_ARRAY\n";
	uint32_t array_size = mp_decode_array(&tuple);
	LogFL(DEBUG) << "TestFunc(): array_size = " << array_size << "\n";
	while (array_size-- > 0) {
		if (add_array_elem_test(schm, &tuple)) {
			LogFL(DEBUG) << "TestFunc(): adding array_elem failed on array_size = " << array_size << "\n";
			return -1;
		}
	}
	LogFL(DEBUG) << "TestFunc(): all is OK\n";
	return 0;
}

void MakeQuery(const std::string &sql_query)
{
	if (sql_query.substr(0, 5) == "FUNC:") {
		SQLMaker maker;
		std::string tmp(sql_query.substr(5, sql_query.length() - 5));
		std::cout << "'" << tmp << "'" << std::endl;
		if (!maker.MakeEval(tmp)) {
			std::cout << "error: " << maker.Error() << std::endl;
		} else {
			std::cout << "success" << std::endl;
		}
		return;
	}

	std::shared_ptr<SQLStatementList> statement_list(SQLParser::parseSQLString(sql_query));
	if (statement_list->isValid == false) {
		LogFL(DEBUG) << statement_list->parser_msg << "\n";
		return;
	}

	for (size_t i = 0, size = statement_list->numStatements(); i < size; ++i) {
		SQLStatement *statement = statement_list->getStatement(i);

		switch(statement->type()) {
			case StatementType::kStmtError:
				LogFL(DEBUG) << "main(): statement type is kStmtError\n";
				break;
			case StatementType::kStmtSelect: {
				LogFL(DEBUG) << "main(): statement type is kStmtSelect\n";
				SelectMaker maker(dynamic_cast<SelectStatement *>(statement));
				SpaceObject res = maker.MakeSelect();
				std::cout << "-----------------RESULT----------------" << std::endl;
				std::cout << res << std::endl;
				break;
			}
			case StatementType::kStmtCreate: {
				LogFL(DEBUG) << "main(): statement type is kStmtCreate\n";
				CreateMaker maker(dynamic_cast<CreateStatement *>(statement));
				LogFL(DEBUG) << "main(): MakeCreate result = " << maker.MakeCreate() << "\n";
				break;
			}
			case StatementType::kStmtDrop: {
				LogFL(DEBUG) << "main(): statement type is kStmtDrop\n";
				DropMaker maker(dynamic_cast<DropStatement *>(statement));
				LogFL(DEBUG) << "main(): MakeDrop result = " << maker.MakeDrop() << "\n";
				break;
			}
			case StatementType::kStmtInsert: {
				LogFL(DEBUG) << "main(): statement type is kStmtInsert\n";
				InsertMaker maker(dynamic_cast<InsertStatement *>(statement));
				LogFL(DEBUG) << "main(): MakeInsert result = " << maker.MakeInsert() << "\n";
				break;
			}
			default:
				LogFL(DEBUG) << "main(): statement type unknown\n";
				break;
		}
	}
}

int main()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ I n i t   T a r a n t o o l   S c h e m a ~~~~~~~~~~~~~~~~~~~~~~~~

	Logger::SetStreamForMessages(DEBUG, std::cout);
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

	std::string sql_query;// = "CREATE TABLE users (id INT, name TEXT, surname TEXT, salary INT, PRIMARY KEY (id, salary), UNIQUE(id), INDEX(name, surname), INDEX(name, salary))";
	//MakeQuery(sql_query);

	sql_query = "CREATE TABLE users (id INT, name TEXT, surname TEXT, salary INT, PRIMARY KEY (id, salary), UNIQUE(id), INDEX(name, surname), INDEX(name, salary));";
	std::cout << "sql_tarantool> " << sql_query << std::endl;

	MakeQuery(sql_query);
	std::cout << "sql_tarantool> " << std::flush;

	while(std::getline(std::cin, sql_query)) {
		MakeQuery(sql_query);
		std::cout << "sql_tarantool> " << std::flush;
	}

	return 0;
}