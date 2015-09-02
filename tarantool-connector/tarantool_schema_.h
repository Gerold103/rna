#ifndef TARANTOOL_SCHEMA_H_INCLUDED
#define TARANTOOL_SCHEMA_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <ostream>
#include "utils.h"
#include "tp_wrap.h"
#include "logger.h"
#include "tarantool_objs.h"

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

	std::vector<TColumn> ColumnNames() const;

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

#endif