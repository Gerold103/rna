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

enum FieldType {
	FT_STR = 0,
	FT_NUM = 1,
	FT_OTHER = 2,
};

struct SpacePart {
std::string name;
FieldType type;
SpacePart();
};

namespace Convert {

std::string ToString(FieldType tp);

}

std::ostream &operator<<(std::ostream &stream, FieldType tp);

class TarantoolInfo;

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   B A S E   I N F O ~~~~~~~~~~~~~~~~~~~~~~~~

struct TarantoolBaseInfo {
protected:
	std::string name;
	int id;
public:
	TarantoolBaseInfo(int id_, const std::string &name_);
	const std::string &Name() const;
	int ID() const;

	friend class TarantoolInfo;
};

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   I N D E X   I N F O ~~~~~~~~~~~~~~~~~~~~~~~~

class TarantoolIndexInfo : public TarantoolBaseInfo {
public:
	enum IndexType {
		HASH, TREE, BITSET, RTREE
	};

private:
	std::vector<SpacePart> index_parts;
	IndexType type;
	bool unique;
	int space_id;

public:
	TarantoolIndexInfo(int id_, const std::string &name_, IndexType type_, bool unique_, int space_id_);

	const std::vector<SpacePart> &Parts() const;

	friend class TarantoolInfo;
};

namespace Convert {
	std::string ToString(TarantoolIndexInfo::IndexType tp);
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   S P A C E   I N F O ~~~~~~~~~~~~~~~~~~~~~~~~

class TarantoolSpaceInfo : public TarantoolBaseInfo {
private:
	std::vector<std::shared_ptr<TarantoolIndexInfo> > infos;
	std::vector<SpacePart> space_parts;

	std::map<int, TarantoolIndexInfo *> indices_and_ids;
	std::map<std::string, TarantoolIndexInfo *> indices_and_names;

	int owner;
	std::string engine;
	std::string flags;

public:
	TarantoolSpaceInfo(int id_, const std::string &name_, int owner_, const std::string &engine_, const std::string &flags_);

	const SpacePart *ColumnBy(int number) const;
	const SpacePart *ColumnBy(const std::string &name_) const;

	int ColumnNumber(const std::string &name_) const;

	const TarantoolIndexInfo *IndexBy(int number) const;
	const TarantoolIndexInfo *IndexBy(const std::string &name_) const;

	const std::vector<std::shared_ptr<TarantoolIndexInfo> > Indices() const;

	friend class TarantoolInfo;
};

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   I N F O ~~~~~~~~~~~~~~~~~~~~~~~~

class TarantoolInfo {
private:
	std::vector<std::shared_ptr<TarantoolSpaceInfo> > infos;

	//id and pointer on space in infos vector
	std::map<int, TarantoolSpaceInfo *> spaces_and_ids;
	std::map<std::string, TarantoolSpaceInfo *> spaces_and_names;

	bool _add_spaces_(const DataStructure &data);
	bool _add_indices_(const DataStructure &data);

	TarantoolSpaceInfo *_space_by(const std::string &name);
	TarantoolSpaceInfo *_space_by(int id);

	Session *session;
	int service_space;

public:
	TarantoolInfo(Session &ses, int _space_id = 280);

	const TarantoolSpaceInfo *SpaceBy(const std::string &name) const;
	const TarantoolSpaceInfo *SpaceBy(int id) const;

	const std::vector<SpacePart> &SpaceFormat(int id) const;
	const std::vector<SpacePart> &SpaceFormat(const std::string &name) const;

	void Update();
};


//-------- old tarantool schema

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
	int AddIndices(const MValue &data);

	//~~~~Get methods~~~~
	int GetSpaceIDByString(const std::string &space_name);
	int GetIndexIDByString(int space_id, const std::string &index_name);

	int GetColNumberByName(int space_id, const std::string &name) const;

	std::vector<std::pair<std::string, FieldType> > GetSpaceFormat(int space_id) const;

	//vector of indices. First in pair - index name, second - index columns
	std::vector<std::pair<std::string, std::vector<std::string> > > GetIndexFormat(int space_id) const;

	std::vector<std::string> GetIndexFormat(int space_id, const std::string &index_name) const;


	std::string GetNameByColNumber(int space_id, int col_number) const;

	//~~~~Other~~~~
	void Flush();

	friend std::ostream &operator<<(std::ostream &stream, const TarantoolSchema &schm);
};

#endif