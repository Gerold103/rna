#ifndef TARANTOOL_SCHEMA_H_INCLUDED
#define TARANTOOL_SCHEMA_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <ostream>
#include "utils.h"
#include "tp_wrap.h"
#include "logger.h"

#define TUPLE_LEN 6
#define FIRST_PACK_PART_SIZE 5

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   K E Y~~~~~~~~~~~~~~~~~~~~~~~~

enum FieldType {
	FT_STR = 0,
	FT_NUM = 1,
	FT_OTHER = 2,
};

std::string FieldTypeToString(FieldType tp);

std::ostream &operator<<(std::ostream &stream, FieldType tp);

struct SchemaKey {
private:
	std::vector<char> _data;
	FieldType tp;
public:
	SchemaKey();
	SchemaKey(uint32_t val);
	SchemaKey(const std::string &val);
	FieldType GetType() const { return tp; }

	uint32_t GetUInt32Val() const;
	std::string GetStrVal() const;

	void SetUInt32Val(uint32_t val);
	void SetStrVal(const std::string &val);

	char *data() { return _data.data(); }
	const char *data() const { return _data.data(); }
	size_t size() const { return _data.size(); }
};

bool operator<(const SchemaKey &left, const SchemaKey &right);
bool operator<=(const SchemaKey &left, const SchemaKey &right);
bool operator>(const SchemaKey &left, const SchemaKey &right);
bool operator>=(const SchemaKey &left, const SchemaKey &right);
bool operator==(const SchemaKey &left, const SchemaKey &right);
bool operator!=(const SchemaKey &left, const SchemaKey &right);
std::ostream &operator<<(std::ostream &stream, const SchemaKey &key);

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   V A L U E~~~~~~~~~~~~~~~~~~~~~~~~

struct SchemaValue {
	uint32_t number;
	std::string name;
	SchemaValue() : number(0), name("#undef#") { }
	virtual ~SchemaValue() { }
};

std::ostream &operator<<(std::ostream &stream, const SchemaValue &val);

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   F I E L D   V A L U E   :   SchemaValue~~~~~~~~~~~~~~~~~~~~~~~~

struct SchemaFieldValue : public SchemaValue {
	FieldType field_type;
};

std::ostream &operator<<(std::ostream &stream, const SchemaFieldValue &val);

std::ostream &operator<<(std::ostream &stream, const std::vector<SchemaFieldValue> &vec);

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   I N D E X   V A L U E   :   SchemaValue~~~~~~~~~~~~~~~~~~~~~~~~

struct SchemaIndexValue : public SchemaValue {
	SchemaKey key;
	std::vector<SchemaFieldValue> index_parts;
};

std::ostream &operator<<(std::ostream &stream, const SchemaIndexValue &val);

std::ostream &operator<<(std::ostream &stream, const std::map<SchemaKey, SchemaIndexValue> &val);

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   S P A C E   V A L U E   :   SchemaValue~~~~~~~~~~~~~~~~~~~~~~~~

struct SchemaSpaceValue : public SchemaValue {
	SchemaKey key;
	std::map<SchemaKey, SchemaIndexValue> indices;
	std::vector<SchemaFieldValue> schema_list;
};

std::ostream &operator<<(std::ostream &stream, const SchemaSpaceValue &val);

//~~~~~~~~~~~~~~~~~~~~~~~~T A R A N T O O L   S C H E M A~~~~~~~~~~~~~~~~~~~~~~~~

class TarantoolSchema;
std::ostream &operator<<(std::ostream &stream, const TarantoolSchema &schm);

class TarantoolSchema {
private:
	std::map<SchemaKey, SchemaSpaceValue> spaces;

	int add_space(const char **data);
	int add_index(const char **data);
public:
	TarantoolSchema(Session &ses, int number = 280);
	TarantoolSchema() {}

	//~~~~Set methods
	int AddSpaces(const DataStructure &data);
	int AddIndices(const DataStructure &data);

	//~~~~Get methods~~~~
	int GetSpaceIDByString(const std::string &space_name);
	int GetIndexIDByString(int space_id, const std::string &index_name);

	int GetColNumberByName(int space_id, const std::string &name) const;

	std::vector<std::pair<std::string, FieldType> > GetSpaceFormat(int space_id) const;

	std::string GetNameByColNumber(int space_id, int col_number) const;

	//~~~~Other~~~~
	void Flush();

	friend std::ostream &operator<<(std::ostream &stream, const TarantoolSchema &schm);
};

#endif