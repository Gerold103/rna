/*
 * sql_tests.cpp
 */

#include "tests/test.h"
#include "tests/helper.h"
#include "SQLParser.h"
#include "sqlhelper.h"

using namespace hsql;


TEST(DeleteStatementTest) {
	SQLStatementList* stmt_list = SQLParser::parseSQLString("DELETE FROM students WHERE grade > 2.0;");
	ASSERT(stmt_list->isValid);
	ASSERT_EQ(stmt_list->numStatements(), 1);
	ASSERT(stmt_list->getStatement(0)->type() == kStmtDelete);

	DeleteStatement* stmt = (DeleteStatement*) stmt_list->getStatement(0);
	ASSERT_STREQ(std::string(*(stmt->table_name)), "students");
	ASSERT_NOTNULL(stmt->expr);
	ASSERT(stmt->expr->isType(kExprOperator));
	ASSERT_STREQ(stmt->expr->expr->name, "grade");
	ASSERT_EQ(stmt->expr->expr2->fval, 2.0);
}

TEST(CreateStatementTest) {
	SQLStatementList* stmt_list = SQLParser::parseSQLString("CREATE TABLE students (name TEXT, student_number INT, city INTEGER, grade DOUBLE)");
	ASSERT(stmt_list->isValid);
	ASSERT_EQ(stmt_list->numStatements(), 1);
	ASSERT_EQ(stmt_list->getStatement(0)->type(), kStmtCreate);

	CreateStatement* stmt = (CreateStatement*) stmt_list->getStatement(0);
	ASSERT_EQ(stmt->type, CreateStatement::kTable);
	ASSERT_STREQ(std::string(*(stmt->table_name)), "students");
	ASSERT_NOTNULL(stmt->columns);
	ASSERT_EQ(stmt->columns->size(), 4);
	ASSERT_STREQ(stmt->columns->at(0)->name, "name");
	ASSERT_STREQ(stmt->columns->at(1)->name, "student_number");
	ASSERT_STREQ(stmt->columns->at(2)->name, "city");
	ASSERT_STREQ(stmt->columns->at(3)->name, "grade");
	ASSERT_EQ(stmt->columns->at(0)->type, ColumnDefinition::TEXT);
	ASSERT_EQ(stmt->columns->at(1)->type, ColumnDefinition::INT);
	ASSERT_EQ(stmt->columns->at(2)->type, ColumnDefinition::INT);
	ASSERT_EQ(stmt->columns->at(3)->type, ColumnDefinition::DOUBLE);
}


TEST(UpdateStatementTest) {
	SQLStatementList* stmt_list = SQLParser::parseSQLString("UPDATE students SET grade = 5.0, name = 'test' WHERE name = 'Max Mustermann';");
	ASSERT(stmt_list->isValid);
	ASSERT_EQ(stmt_list->numStatements(), 1);
	ASSERT_EQ(stmt_list->getStatement(0)->type(), kStmtUpdate);

	UpdateStatement* stmt = (UpdateStatement*) stmt_list->getStatement(0);
	ASSERT_NOTNULL(stmt->table);
	ASSERT_STREQ(std::string(*(stmt->table->name)), "students");
	
	ASSERT_NOTNULL(stmt->updates);
	ASSERT_EQ(stmt->updates->size(), 2);
	ASSERT_STREQ(stmt->updates->at(0)->column, "grade");
	ASSERT_STREQ(stmt->updates->at(1)->column, "name");
	ASSERT(stmt->updates->at(0)->value->isType(kExprLiteralFloat));
	ASSERT(stmt->updates->at(1)->value->isType(kExprLiteralString));
	ASSERT_EQ(stmt->updates->at(0)->value->fval, 5.0);
	ASSERT_STREQ(stmt->updates->at(1)->value->name, "test");

	ASSERT_NOTNULL(stmt->where);
	ASSERT(stmt->where->isType(kExprOperator));
	ASSERT(stmt->where->isSimpleOp('='));
	ASSERT_STREQ(stmt->where->expr->name, "name");
	ASSERT_STREQ(stmt->where->expr2->name, "Max Mustermann");

}


TEST(InsertStatementTest) {
	TEST_PARSE_SINGLE_SQL("INSERT INTO students VALUES ('Max Mustermann', 12345, 'Musterhausen', 2.0)", kStmtInsert, InsertStatement, stmt);

	ASSERT_EQ(stmt->values->size(), 4);
	// TODO
}


TEST(DropTableStatementTest) {
	TEST_PARSE_SINGLE_SQL("DROP TABLE students", kStmtDrop, DropStatement, stmt);

	ASSERT_EQ(stmt->type, DropStatement::kTable);
	ASSERT_NOTEMPTY(std::string(*(stmt->name)));
	ASSERT_STREQ(std::string(*(stmt->name)), "students");
}


TEST(PrepareStatementTest) {
	std::string query = "PREPARE test {"
		"INSERT INTO test VALUES(?);"
		"SELECT ?, test FROM test WHERE c1 = ?;"
		"};"
		"PREPARE stmt: SELECT * FROM data WHERE c1 = ?;"
		"DEALLOCATE PREPARE stmt;";

	TEST_PARSE_SQL_QUERY(query, stmt_list, 3);

	TEST_CAST_STMT(stmt_list, 0, kStmtPrepare, PrepareStatement, prep1);
	TEST_CAST_STMT(stmt_list, 1, kStmtPrepare, PrepareStatement, prep2);
	TEST_CAST_STMT(stmt_list, 2, kStmtDrop, DropStatement, drop);

	// Prepare Statement #1
	ASSERT_STREQ(prep1->name, "test");
	ASSERT_EQ(prep1->placeholders.size(), 3);
	ASSERT_EQ(prep1->query->numStatements(), 2);
	
	TEST_CAST_STMT(prep1->query, 0, kStmtInsert, InsertStatement, insert);
	TEST_CAST_STMT(prep1->query, 1, kStmtSelect, SelectStatement, select);

	ASSERT(insert->values->at(0)->isType(kExprPlaceholder));
	ASSERT(select->select_list->at(0)->isType(kExprPlaceholder));
	ASSERT(select->where_clause->expr2->isType(kExprPlaceholder));

	// Check IDs of placeholders
	ASSERT_EQ(insert->values->at(0)->ival, 0);
	ASSERT_EQ(insert->values->at(0), prep1->placeholders[0]);

	ASSERT_EQ(select->select_list->at(0)->ival, 1);
	ASSERT_EQ(select->select_list->at(0), prep1->placeholders[1]);

	ASSERT_EQ(select->where_clause->expr2->ival, 2);
	ASSERT_EQ(select->where_clause->expr2, prep1->placeholders[2]);

	// Prepare Statement #2
	ASSERT_STREQ(prep2->name, "stmt");
	ASSERT_EQ(prep2->placeholders.size(), 1);

	// Deallocate Statement
	ASSERT_EQ(drop->type, DropStatement::kPreparedStatement);
	ASSERT_STREQ(std::string(*(drop->name)), "stmt");
}


TEST(ExecuteStatementTest) {
	TEST_PARSE_SINGLE_SQL("EXECUTE test(1, 2);", kStmtExecute, ExecuteStatement, stmt);

	ASSERT_STREQ(stmt->name, "test");
	ASSERT_EQ(stmt->parameters->size(), 2);
}