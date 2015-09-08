#ifndef TARANTOOL_OBJS_H_INCLUDED
#define TARANTOOL_OBJS_H_INCLUDED

#include <ostream>
#include "utils.h"
#include "mvalue.h"
#include "logger.h"
#include "datastructure.h"


enum TarantoolObjType {
	SPACE_OBJ,
	INDEX_OBJ,
	TUPLE_OBJ
};

struct TColumn {
public:
	std::string name;
	std::string space;
	std::string alias;
	MValue value;

	TColumn(const std::string &name_ = std::string(""), const std::string &space_ = std::string(""), const std::string &alias_ = std::string(""), const MValue &val_ = MValue());
	TColumn(const MValue &val);
	bool EqualCol(const TColumn &right) const;
	bool Equal(const TColumn &right) const;
	void SetNames(const TColumn &col);
	bool Empty() const;
};

std::ostream &operator<<(std::ostream &stream, const TColumn &col);
std::ostream &operator<<(std::ostream &stream, const std::vector<TColumn> &col);

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   O B J E C T ~~~~~~~~~~~~~~~~~~~~~~~~

class TarantoolObject {
private:
	TarantoolObjType type;

protected:

	std::string error;

	//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

	TarantoolObject(TarantoolObjType tp);

public:
	//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

	TarantoolObjType GetType() const;

	const std::string GetError() const;

	//~~~~~~~~ O t h e r ~~~~~~~~
	virtual ~TarantoolObject();
};

//~~~~~~~~~~~~~~~~~~~~~~~~ T U P L E ~~~~~~~~~~~~~~~~~~~~~~~~

class TupleObj : public TarantoolObject {
private:
	std::vector<TColumn> values;

public:
	//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

	TupleObj();

	TupleObj(const std::vector<TColumn> &values_);

	TupleObj(const TColumn &value);

	//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

	const std::vector<TColumn> &GetValues() const;
	size_t Size() const;
	TColumn GetValue(const TColumn &col) const; //get tcolumn with value from tcolumn with only name
	TColumn GetValue(const std::string &name) const;
	TColumn GetValue(const std::string &name, const std::string &space);
	MValueVector GetMValues() const;

	//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

	void PushBack(const TColumn &val);
	void SetNames(const std::vector<TColumn> &_names);
	void Clear();

	//~~~~~~~~ F a b r i c a l   m e t h o d s ~~~~~~~~

	static TupleObj FromMSGPack(const char **data);
	static TupleObj FromMSGPack(const DataStructure &data);

	//~~~~~~~~ O p e r a t o r s ~~~~~~~~

	const TColumn &operator[](int id) const;
	TColumn &operator[](int id);

	TupleObj operator+(const TupleObj &right) const;

	bool operator==(const TupleObj &ob) const;

	//~~~~~~~~ O t h e r ~~~~~~~~

	void Remove(int id);
};

//~~~~~~~~~~~~~~~~~~~~~~~~ S P A C E   O B J E C T ~~~~~~~~~~~~~~~~~~~~~~~~

class SpaceObject : public TarantoolObject {
private:
	std::vector<TupleObj> tuples;

	std::vector<TColumn> names;

public:
	//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

	SpaceObject();

	SpaceObject(const std::vector<TupleObj> &_tuples);

	//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

	const std::vector<TupleObj> &GetTuples() const;
	const std::vector<TColumn> &GetNames() const;

	size_t Size() const;

	size_t ColumnsCount() const;

	int FindTuple(const TupleObj &tuple) const;

	//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

	void PushBack(const TupleObj &val);
	void PushBack(const SpaceObject &right);

	void SetNames(const std::vector<TColumn> &_names);

	//~~~~~~~~ S t a t i c   m e t h o d s ~~~~~~~~

	static SpaceObject CartesianProduct(const SpaceObject &left, const SpaceObject &right);

	//~~~~~~~~ F a b r i c a l   m e t h o d s ~~~~~~~~

	static SpaceObject FromMSGPack(const char **data);
	static SpaceObject FromMSGPack(const DataStructure &data);

	//~~~~~~~~ O p e r a t o r s ~~~~~~~~

	const TupleObj &operator[](int id) const;
	bool operator==(const SpaceObject &ob) const;

	//~~~~~~~~ O t h e r ~~~~~~~~

	void Remove(int id);
};

//~~~~~~~~~~~~~~~~~~~~~~~~ O P E R A T O R S ~~~~~~~~~~~~~~~~~~~~~~~~

std::string TarantoolObjTypeToString(TarantoolObjType tp);
std::ostream &operator<<(std::ostream &stream, const TupleObj &ob);
std::ostream &operator<<(std::ostream &stream, const TarantoolObject &ob);
std::ostream &operator<<(std::ostream &stream, const SpaceObject &ob);


#endif