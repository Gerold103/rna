#include "test_utils.h"

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
				MValue tmp = maker.MakeSelect().ToMValue();
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