#include "InsertStatement.h"

namespace hsql {

InsertStatement::InsertStatement(InsertType type) : SQLStatement(kStmtInsert), type(type), table_name(NULL), columns(NULL), values(NULL), select(NULL) { }

InsertStatement::~InsertStatement() {
	delete columns;
	if (values != NULL) {
		for (size_t i = 0, size = values->size(); i < size; ++i) {
			delete values->at(i);
		}
	}
	delete values;
	delete select;
}

InsertStatement::InsertType InsertStatement::GetType() const { return type; }

const std::string InsertStatement::GetName() const { return table_name->GetName(); }

const std::string InsertStatement::GetDatabase() const { return table_name->GetDatabase(); }

const std::vector<std::string> *InsertStatement::GetColumns() const { return columns; }

const std::vector<Expr*> *InsertStatement::GetValues() const { return values; }

const SelectStatement *InsertStatement::GetSelectStatement() const { return select; }

const Expr *InsertStatement::GetValueByColumn(const std::string &col_name)
{
	auto cols = this->GetColumns();
	if (cols == NULL) return NULL;
	for (size_t i = 0, size = cols->size(); i < size; ++i) {
		if (cols->at(i) == col_name) {
			return this->GetValues()->at(i);
		}
	}
	return NULL;
}


std::string InsertTypeToString(InsertStatement::InsertType tp)
{
	switch(tp) {
		case InsertStatement::kInsertValues: return "kInsertValues";
		case InsertStatement::kInsertSelect: return "kInsertSelect";
		default: return "default";
	}
}

std::ostream &operator<<(std::ostream &stream, const InsertStatement &ob)
{
	stream << "InsertStatement: type = " << InsertTypeToString(ob.GetType()) << ", name = " << ob.GetName() << ", database = " << ob.GetDatabase() << ",\n";
	stream << "columns: ";
	auto columns = ob.GetColumns();
	if (columns == NULL) stream << "null";
	else {
		for (int i = 0, size = static_cast<int>(columns->size()); i < size; ++i) {
			stream << "[" << i << "]: " << columns->at(i);
			if (i < size - 1) stream << "; ";
		}
	}
	stream << "\nvalues: ";
	auto values = ob.GetValues();
	if (values == NULL) stream << "null";
	else {
		for (int i = 0, size = static_cast<int>(values->size()); i < size; ++i) {
			stream << "[" << i << "]: " << *(values->at(i));
			if (i < size - 1) stream << "; ";
		}
	}
	stream << "\nselect: ";
	if (select == NULL) stream << "null";
	return stream;
}

}