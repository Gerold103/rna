#include "tarantool_objs.h"

TColumn::TColumn(const std::string &name_, const std::string &space_, const std::string &alias_, const MValue &val_)
	: name(name_), space(space_), alias(alias_), value(val_) { }

TColumn::TColumn(const MValue &val) : value(val) { }

bool TColumn::EqualCol(const TColumn &right) const
{
	return (name == right.name) && (space == right.space) && ((alias == right.alias) || (alias.length() == 0) || (right.alias.length() == 0));
}

bool TColumn::Equal(const TColumn &right) const
{
	bool r;
	try {
		r = (value == right.value);
	} catch(...) { return false; }
	return (this->EqualCol(right)) && r;
}

void TColumn::SetNames(const TColumn &col)
{
	name = col.name;
	space = col.space;
	alias = col.alias;
}

bool TColumn::Empty() const
{
	return (name.length() == 0) && (space.length() == 0) && (alias.length() == 0) && (value.GetType() == TP_DEFAULT);
}

std::ostream &operator<<(std::ostream &stream, const TColumn &col)
{
	if (col.space.length()) stream << col.space << ".";
	stream << col.name;
	if (col.alias.length()) stream << " as " << col.alias;
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const std::vector<TColumn> &col)
{
	for (size_t i = 0, size = col.size(); i < size; ++i) {
		stream << col[i];
		if (i < size - 1) stream << ", ";
	}
	return stream;
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   O B J E C T ~~~~~~~~~~~~~~~~~~~~~~~~

TarantoolObject::TarantoolObject(TarantoolObjType tp) : type(tp) { }

TarantoolObjType TarantoolObject::GetType() const { return type; }

const std::string TarantoolObject::GetError() const { return error; }

TarantoolObject::~TarantoolObject() { }

//~~~~~~~~~~~~~~~~~~~~~~~~ T U P L E ~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

TupleObj::TupleObj() : TarantoolObject(TUPLE_OBJ) { }

TupleObj::TupleObj(const std::vector<TColumn> &values_) : TarantoolObject(TUPLE_OBJ), values(values_) { }

TupleObj::TupleObj(const TColumn &value) : TarantoolObject(TUPLE_OBJ)
{
	values.push_back(value);
}

//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

const std::vector<TColumn> &TupleObj::GetValues() const
{
	return values;
}

size_t TupleObj::Size() const
{
	return values.size();
}

TColumn TupleObj::GetValue(const TColumn &col) const
{
	for (size_t i = 0, size = values.size(); i < size; ++i) {
		if (values[i].EqualCol(col)) {
			return values[i];
		}
	}
	return TColumn();
}

TColumn TupleObj::GetValue(const std::string &name) const
{
	int res = -1;
	for (size_t i = 0, size = values.size(); i < size; ++i) {
		if ((values[i].name == name) || (values[i].alias == name)) {
			if (res > 0) {
				LogFL(DEBUG) << "TupleObj::GetValue(): several TColumns with this name = " << name << "\n";
				return TColumn();
			}
			res = i;
		}
	}
	if (res < 0) return TColumn();
	return values[res];
}

TColumn TupleObj::GetValue(const std::string &name, const std::string &space)
{
	int res = -1;
	for (size_t i = 0, size = values.size(); i < size; ++i) {
		if ((values[i].name == name) && (values[i].space == space)) {
			if (res > 0) {
				LogFL(DEBUG) << "TupleObj::GetValue(): several TColumns with this name = " << name << " and space = " << space << "\n";
				return TColumn();
			}
			res = i;
		}
	}
	if (res < 0) return TColumn();
	return values[res];
}

MValueVector TupleObj::GetMValues() const
{
	MValueVector res;
	for (size_t i = 0, size = values.size(); i < size; ++i) {
		res.push_back(values[i].value);
	}
	return res;
}

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

void TupleObj::PushBack(const TColumn &val)
{
	values.push_back(val);
}

void TupleObj::SetNames(const std::vector<TColumn> &_names)
{
	if (_names.size() != values.size()) {
		LogFL(DEBUG) << "TupleObj::SetNames(): sizes of names are different\n";
		throw std::string("TupleObj::SetNames(): sizes of names are different");
	}
	for (size_t i = 0, size = values.size(); i < size; ++i) {
		values[i].SetNames(_names[i]);
	}
}

void TupleObj::Clear()
{
	values.clear();
}

//~~~~~~~~ F a b r i c a l   m e t h o d s ~~~~~~~~

TupleObj TupleObj::FromMSGPack(const char **data)
{
	const char *tuple = *data;
	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "TupleObj::FromMSGPack(): typeof raw tuple not equal to MP_ARRAY, but equal = " << mp_typeof(*tuple) << "\n";
		return TupleObj();
	}
	uint32_t tuple_len = mp_decode_array(&tuple);	
	TupleObj res;
	for (size_t i = 0; i < tuple_len; ++i) {
		res.PushBack(TColumn(MValue::FromMSGPack(&tuple)));
	}
	*data = tuple;
	return res;
}

TupleObj TupleObj::FromMSGPack(const DataStructure &data)
{
	const char *tuple = data.Data();
	if (mp_check(&tuple, tuple + data.Size())) {
		LogFL(DEBUG) << "TupleObj::FromMSGPack(): validating failed\n";
		return TupleObj();
	}
	tuple = data.Data();
	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "TupleObj::FromMSGPack(): typeof raw tuple not equal to MP_ARRAY, but equal = " << mp_typeof(*tuple) << "\n";
		return TupleObj();
	}
	uint32_t tuple_len = mp_decode_array(&tuple);	
	TupleObj res;
	for (size_t i = 0; i < tuple_len; ++i) {
		res.PushBack(TColumn(MValue::FromMSGPack(&tuple)));
	}
	return res;
}

//~~~~~~~~ O p e r a t o r s ~~~~~~~~

const TColumn &TupleObj::operator[](int id) const
{
	if ((id < 0) || (id >= static_cast<int>(values.size()))) throw std::string("TupleObj::operator[]: index out of range, i = ") + std::to_string(id);
	return values[id];
}

TColumn &TupleObj::operator[](int id)
{
	if ((id < 0) || (id >= static_cast<int>(values.size()))) throw std::string("TupleObj::operator[]: index out of range, i = ") + std::to_string(id);
	return values[id];
}

TupleObj TupleObj::operator+(const TupleObj &right) const
{
	TupleObj res(values);
	res.values.insert(res.values.end(), right.values.begin(), right.values.end());
	return res;
}

bool TupleObj::operator==(const TupleObj &ob)
{
	if (this->Size() != ob.Size()) return false;
	for (size_t i = 0, size = ob.Size(); i < size; ++i) {
		if (values[i].Equal(ob[i])) return false;
	}
	return true;
}

//~~~~~~~~ O t h e r ~~~~~~~~

void TupleObj::Remove(int id)
{
	values.erase(values.begin() + id);
}

//~~~~~~~~~~~~~~~~~~~~~~~~ S P A C E   O B J E C T ~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

SpaceObject::SpaceObject() : TarantoolObject(SPACE_OBJ) { }

SpaceObject::SpaceObject(const std::vector<TupleObj> &_tuples) : TarantoolObject(SPACE_OBJ), tuples(_tuples) { }

//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

const std::vector<TupleObj> &SpaceObject::GetTuples() const
{
	return tuples;
}

const std::vector<TColumn> &SpaceObject::GetNames() const
{
	return names;
}

size_t SpaceObject::Size() const
{
	return tuples.size();
}

size_t SpaceObject::ColumnsCount() const
{
	if (names.size()) return names.size();
	if (tuples.size()) return tuples[0].Size();
	return 0;
}

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

void SpaceObject::PushBack(const TupleObj &val)
{
	if ((tuples.size()) && (tuples[0].Size() != val.Size())) {
		LogFL(DEBUG) << "SpaceObject::PushBack(): sizes of tuples are different\n";
		throw std::string("SpaceObject::PushBack(): sizes of tuples are different"); 
	}
	tuples.push_back(val);
}

void SpaceObject::PushBack(const SpaceObject &right)
{
	if (names.size() != right.names.size()) {
		LogFL(DEBUG) << "SpaceObject::PushBack(): names size in SpaceObjects are different\n";
		throw std::string("SpaceObject::PushBack(): names size in SpaceObjects are different");
	}
	for (size_t i = 0, size = names.size(); i < size; ++i) {
		if (!names[i].EqualCol(right.names[i])) {
			LogFL(DEBUG) << "SpaceObject::PushBack(): names in SpaceObjects are different\n";
			throw std::string("SpaceObject::PushBack(): names in SpaceObjects are different");
		}
	}
	tuples.insert(tuples.end(), right.tuples.begin(), right.tuples.end());
}

void SpaceObject::SetNames(const std::vector<TColumn> &_names)
{
	if ((tuples.size()) && (tuples[0].Size() != _names.size())) {
		LogFL(DEBUG) << "SpaceObject::SetNames(): size of tuples and size of names are different\n";
		throw std::string("SpaceObject::SetNames(): size of tuples and size of names are different");
	}
	names = _names;
	for (size_t i = 0, size = tuples.size(); i < size; ++i) {
		tuples[i].SetNames(names);
	}
}

//~~~~~~~~ S t a t i c   m e t h o d s ~~~~~~~~

SpaceObject SpaceObject::CartesianProduct(const SpaceObject &left, const SpaceObject &right)
{
	SpaceObject res;
	for (size_t i = 0, size = left.Size(); i < size; ++i) {
		for (size_t j = 0, size2 = right.Size(); j < size2; ++j) {
			res.tuples.push_back(left[i] + right[j]);
		}
	}
	res.names.insert(res.names.end(), left.names.begin(), left.names.end());
	res.names.insert(res.names.end(), right.names.begin(), right.names.end());
	return res;
}

//~~~~~~~~ F a b r i c a l   m e t h o d s ~~~~~~~~

SpaceObject SpaceObject::FromMSGPack(const char **data)
{
	const char *tuple = *data;
	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "SpaceObject::FromMSGPack(): tuple is not MP_ARRAY, but is equal to " << mp_typeof(*tuple) << "\n";
		return SpaceObject();
	}
	SpaceObject res;
	uint32_t size = mp_decode_array(&tuple);
	for (uint32_t i = 0; i < size; ++i) {
		res.tuples.push_back(TupleObj::FromMSGPack(&tuple));
	}
	*data = tuple;
	return res;
}

SpaceObject SpaceObject::FromMSGPack(const DataStructure &data)
{
	const char *tuple = data.Data();
	if (mp_check(&tuple, tuple + data.Size())) {
		LogFL(DEBUG) << "SpaceObject::FromMSGPack(): validating failed\n";
		return SpaceObject();
	}
	tuple = data.Data();
	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "SpaceObject::FromMSGPack(): tuple is not MP_ARRAY, but is equal to " << mp_typeof(*tuple) << "\n";
		return SpaceObject();
	}
	SpaceObject res;
	uint32_t size = mp_decode_array(&tuple);
	for (uint32_t i = 0; i < size; ++i) {
		res.tuples.push_back(TupleObj::FromMSGPack(&tuple));
	}
	return res;
}

//~~~~~~~~ O p e r a t o r s ~~~~~~~~

const TupleObj &SpaceObject::operator[](int id) const
{
	if ((id < 0) || (id > static_cast<int>(tuples.size()))) throw std::string("SpaceObject::operator[]: index is out of range, i = ") + std::to_string(id);
	return tuples[id];
}

bool SpaceObject::operator==(const SpaceObject &ob) const
{
	if (names.size() != ob.names.size()) return false;
	if (tuples.size() != ob.tuples.size()) return false;
	for (size_t i = 0, size = names.size(); i < size; ++i) {
		if (!(names[i].EqualCol(ob.names[i]))) return false;
	}
	for (size_t i = 0, size = tuples.size(); i < size; ++i) {
		if (!(tuples[i] == ob.tuples[i])) return false;
	}
	return true;
}

//~~~~~~~~ O t h e r ~~~~~~~~

void SpaceObject::Remove(int id)
{
	tuples.erase(tuples.begin() + id);
}

//~~~~~~~~~~~~~~~~~~~~~~~~ O P E R A T O R S ~~~~~~~~~~~~~~~~~~~~~~~~

std::string TarantoolObjTypeToString(TarantoolObjType tp)
{
	switch(tp) {
		case SPACE_OBJ: return "SPACE OBJECT";
		case INDEX_OBJ: return "INDEX OBJECT";
		case TUPLE_OBJ: return "TUPLE OBJECT";
		default: return "DEFAULT";
	}
}

std::ostream &operator<<(std::ostream &stream, const TupleObj &ob)
{
	stream << ob.GetMValues();
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const std::vector<TupleObj> &ob)
{
	size_t size = ob.size();
	for (size_t i = 0; i < size; ++i) {
		stream << ob[i];
		if (i < size - 1) stream << std::endl << std::endl;
	}
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const TarantoolObject &ob)
{
	stream << "TarantoolObject: type = " << TarantoolObjTypeToString(ob.GetType());
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const SpaceObject &ob)
{
	stream << "|  ";
	for (int i = 0, size = ob.ColumnsCount(); i < size; ++i) {
		stream << ob.GetNames()[i];
		if (i < size - 1) stream << "  |  ";
	} 
	stream << "  |" << std::endl;
	stream << ob.GetTuples();
	return stream;
}

