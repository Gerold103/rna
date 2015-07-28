#include "tarantool_objs.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   O B J E C T ~~~~~~~~~~~~~~~~~~~~~~~~

TarantoolObject::TarantoolObject(TarantoolObjType tp) : type(tp) { }

TarantoolObjType TarantoolObject::GetType() const { return type; }

const std::string TarantoolObject::GetError() const { return error; }

TarantoolObject::~TarantoolObject() { }

//~~~~~~~~~~~~~~~~~~~~~~~~ T U P L E ~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

TupleObj::TupleObj() : TarantoolObject(TUPLE_OBJ) { }

TupleObj::TupleObj(const MValueVector &_values) : TarantoolObject(TUPLE_OBJ), values(_values) { }

TupleObj::TupleObj(const MValue &source) : TarantoolObject(TUPLE_OBJ)
{
	switch(source.GetType()) {
		case TP_ARRAY: {
			values = source.GetArray();
			return;
		}
		case TP_MAP: {
			error = "TupleObj::TupleObj(): MValue type is map";
			LogFL(DEBUG) << error << "\n";
			return;
		}
		case TP_EXT: {
			error = "TupleObj::TupleObj(): MValue type is ext";
			LogFL(DEBUG) << error << "\n";
			return;
		}
		case TP_DEFAULT: {
			error = "TupleObj::TupleObj(): MValue type is default";
			LogFL(DEBUG) << error << "\n";
			return;
		}
		default: {
			values.push_back(source);
			return;
		}
	}
}

//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

const MValueVector &TupleObj::GetValues() const
{
	return values;
}

size_t TupleObj::Size() const
{
	return values.size();
}

MValue TupleObj::ToMValue() const
{
	return MValue(values);
}

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

void TupleObj::PushBack(const MValue &val)
{
	values.push_back(val);
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
		res.PushBack(MValue::FromMSGPack(&tuple));
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
		res.PushBack(MValue::FromMSGPack(&tuple));
	}
	return res;
}

//~~~~~~~~ O p e r a t o r s ~~~~~~~~

const MValue &TupleObj::operator[](int id) const
{
	if ((id < 0) || (id >= static_cast<int>(values.size()))) throw std::string("TupleObj::operator[]: index out of range, i = ") + std::to_string(id);
	return values[id];
}

TupleObj TupleObj::operator+(const TupleObj &right) const
{
	TupleObj res(values);
	res.values.insert(res.values.begin(), right.values.begin(), right.values.end());
	return res;
}

bool TupleObj::operator==(const TupleObj &ob)
{
	if (this->Size() != ob.Size()) return false;
	for (size_t i = 0, size = ob.Size(); i < size; ++i) {
		try {
			if (values[i] != ob[i]) return false;
		}
		catch(...) { return false; }
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

SpaceObject::SpaceObject(const MValue &source) : TarantoolObject(SPACE_OBJ)
{
	if (source.GetType() != TP_ARRAY) {
		error = "SpaceObject::SpaceObject(): type of MValue is not array";
		LogFL(DEBUG) << error << "\n";
		return;
	}
	const MValueVector &vec = source.GetArray();
	for (size_t i = 0, size = vec.size(); i < size; ++i) {
		tuples.push_back(TupleObj(vec[i]));
	}
}

//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

const std::vector<TupleObj> &SpaceObject::GetTuples() const
{
	return tuples;
}

const std::vector<std::string> &SpaceObject::GetNames() const
{
	return names;
}

size_t SpaceObject::Size() const
{
	return tuples.size();
}

size_t SpaceObject::NamesCount() const
{
	return names.size();
}

MValue SpaceObject::ToMValue() const
{
	MValueVector res;
	for (size_t i = 0, size = tuples.size(); i < size; ++i) {
		res.push_back(tuples[i].ToMValue());
	}
	return MValue(res);
}

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

void SpaceObject::PushBack(const TupleObj &val)
{
	tuples.push_back(val);
}

void SpaceObject::PushBack(const SpaceObject &right)
{
	tuples.insert(tuples.end(), right.tuples.begin(), right.tuples.end());
}

bool SpaceObject::PushName(const std::string &str)
{
	error.clear();
	if (tuples.size() == 0) {
		error = "SpaceObject::PushName(): there are no tuples, so name \"" + str + "\" can't be pushed";
		return false;
	}
	if (names.size() >= tuples[0].Size()) {
		error = "SpaceObject::PushName(): names count can't be bigger than objects in one tuple. names.size() = " + std::to_string(names.size());
		return false;
	}
	names.push_back(str);
	return true;
}

//~~~~~~~~ S t a t i c   m e t h o d s ~~~~~~~~

SpaceObject SpaceObject::CartesianProduct(const SpaceObject &left, const SpaceObject &right)
{
	SpaceObject res;
	for (size_t i = 0, size = left.Size(); i < size; ++i) {
		for (size_t j = 0, size2 = right.Size(); j < size2; ++j) {
			res.PushBack(left[i] + right[j]);
		}
	}
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
		res.PushBack(TupleObj::FromMSGPack(&tuple));
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
		res.PushBack(TupleObj::FromMSGPack(&tuple));
	}
	return res;
}

//~~~~~~~~ O p e r a t o r s ~~~~~~~~

const TupleObj &SpaceObject::operator[](int id) const
{
	if ((id < 0) || (id > static_cast<int>(tuples.size()))) throw std::string("SpaceObject::operator[]: index is out of range, i = ") + std::to_string(id);
	return tuples[id];
}

bool SpaceObject::operator==(const SpaceObject &ob)
{
	if (this->Size() != ob.Size()) return false;
	SpaceObject tmp(ob);
	bool res = true;
	for (int i = static_cast<int>(this->Size()) - 1; i >= 0; --i) {
		for (int j = static_cast<int>(tmp.Size()) - 1; j >= 0; --j) {
			if (tuples[i] == tmp[j]) {
				tmp.Remove(j);
				break;
			}
			if (j == 0) res = false;
		}
		if (!res) return false;
	}
	if (tmp.Size()) return false;
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
	stream << ob.GetValues();
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
	for (int i = 0, size = ob.NamesCount(); i < size; ++i) {
		stream << ob.GetNames()[i];
		if (i < size - 1) stream << "  |  ";
	} 
	stream << "  |" << std::endl;
	stream << ob.GetTuples();
	return stream;
}

