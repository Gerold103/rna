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
	MValueVector values;

public:
	//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

	TupleObj();

	TupleObj(const MValueVector &_values);

	TupleObj(const MValue &source);

	//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

	const MValueVector &GetValues() const;
	size_t Size() const;
	MValue ToMValue() const;

	//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

	void PushBack(const MValue &val);

	//~~~~~~~~ F a b r i c a l   m e t h o d s ~~~~~~~~

	static TupleObj FromMSGPack(const char **data);
	static TupleObj FromMSGPack(const DataStructure &data);

	//~~~~~~~~ O p e r a t o r s ~~~~~~~~

	const MValue &operator[](int id) const;

	TupleObj operator+(const TupleObj &right) const;

	bool operator==(const TupleObj &ob);

	//~~~~~~~~ O t h e r ~~~~~~~~

	void Remove(int id);
};

//~~~~~~~~~~~~~~~~~~~~~~~~ S P A C E   O B J E C T ~~~~~~~~~~~~~~~~~~~~~~~~

class SpaceObject : public TarantoolObject {
private:
	std::vector<TupleObj> tuples;

	std::vector<std::string> names;

public:
	//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

	SpaceObject();

	SpaceObject(const std::vector<TupleObj> &_tuples);

	SpaceObject(const MValue &source);

	//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

	const std::vector<TupleObj> &GetTuples() const;
	const std::vector<std::string> &GetNames() const;

	size_t Size() const;

	size_t NamesCount() const;

	MValue ToMValue() const;

	//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

	void PushBack(const TupleObj &val);
	void PushBack(const SpaceObject &right);

	bool PushName(const std::string &str);

	void SetNames(const std::vector<std::string> &_names);

	//~~~~~~~~ S t a t i c   m e t h o d s ~~~~~~~~

	static SpaceObject CartesianProduct(const SpaceObject &left, const SpaceObject &right);

	//~~~~~~~~ F a b r i c a l   m e t h o d s ~~~~~~~~

	static SpaceObject FromMSGPack(const char **data);
	static SpaceObject FromMSGPack(const DataStructure &data);

	//~~~~~~~~ O p e r a t o r s ~~~~~~~~

	const TupleObj &operator[](int id) const;
	bool operator==(const SpaceObject &ob);

	//~~~~~~~~ O t h e r ~~~~~~~~

	void Remove(int id);
};

//~~~~~~~~~~~~~~~~~~~~~~~~ O P E R A T O R S ~~~~~~~~~~~~~~~~~~~~~~~~

std::string TarantoolObjTypeToString(TarantoolObjType tp);
std::ostream &operator<<(std::ostream &stream, const TupleObj &ob);
std::ostream &operator<<(std::ostream &stream, const TarantoolObject &ob);
std::ostream &operator<<(std::ostream &stream, const SpaceObject &ob);


#endif