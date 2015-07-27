#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include "datastructure.h"
#include "logger.h"
#include <cstddef>
#include <cstdint>
extern "C" {
	#include "session.h"
	#include "tp.h"
}

std::string DebugStr(const std::string &file, int line, const std::string &msg);
#define MakeDbgStr(MSG) DebugStr(std::string(__FILE__), __LINE__, MSG)
#define ZValue_p std::shared_ptr<MSGValue>
#define ZValueVector std::vector<ZValue_p>
#define ZValueMap std::map<ZValue_p, ZValue_p>

//~~~~~~~~~~~~~~~~~~~~~~~~ M S G V A L U E ~~~~~~~~~~~~~~~~~~~~~~~~

class MSGValue {
private:
	tp_type type;
protected:
	MSGValue(tp_type tp);
	MSGValue();
public:
	tp_type GetType() const;
	virtual std::string ToString() = 0;
	virtual ~MSGValue();
};

//~~~~~~~~~~~~~~~~~~~~~~~~ T P   T Y P E   C O N V E R T E R ~~~~~~~~~~~~~~~~~~~~~~~~

template<class T>
inline tp_type ValueToTPType(const T &val)
{
	return TP_DEFAULT;
}

template<>
inline tp_type ValueToTPType<uint32_t>(const uint32_t &val) { return TP_UINT; }

template<>
inline tp_type ValueToTPType<uint64_t>(const uint64_t &val) { return TP_UINT; }

template<>
inline tp_type ValueToTPType<int>(const int &val) { return TP_INT; }

template<>
inline tp_type ValueToTPType<int64_t>(const int64_t &val) { return TP_INT; }

template<>
inline tp_type ValueToTPType<std::string>(const std::string &val) { return TP_STR; }

template<>
inline tp_type ValueToTPType<DataStructure>(const DataStructure &val) {
	if ((val.Data() == nullptr) || (val.Size() == 0)) return TP_NIL;
	return TP_BIN;
}

template<>
inline tp_type ValueToTPType<bool>(const bool &val) { return TP_BOOL; }

template<>
inline tp_type ValueToTPType<float>(const float &val) { return TP_FLOAT; }

template<>
inline tp_type ValueToTPType<double>(const double &val) { return TP_DOUBLE; }

template<>
inline tp_type ValueToTPType<ZValueVector>(const ZValueVector &val) { return TP_ARRAY; }



template<class T>
inline std::string ToString(const T &val)
{
	return "none";
}

template<>
inline std::string ToString(const int &val)
{
	return std::to_string(val);
}

template<>
inline std::string ToString(const int64_t &val)
{
	return std::to_string(val);
}

template<>
inline std::string ToString(const uint32_t &val)
{
	return std::to_string(val);
}

template<>
inline std::string ToString(const uint64_t &val)
{
	return std::to_string(val);
}

template<>
inline std::string ToString(const double &val)
{
	return std::to_string(val);
}

template<>
inline std::string ToString(const float &val)
{
	return std::to_string(val);
}

template<>
inline std::string ToString(const DataStructure &val)
{
	return "DataStructure: size = " + std::to_string(val.Size());
}

template<>
inline std::string ToString(const std::string &val)
{
	return val;
}

template<>
inline std::string ToString(const tp_type &val)
{
	std::string res("TP_");
	switch (val) {
		case TP_NIL:
			res += "NIL"; break;
		case TP_UINT:
			res += "UINT"; break;
		case TP_INT:
			res += "INT"; break;
		case TP_STR:
			res += "STR"; break;
		case TP_BIN:
			res += "BIN"; break;
		case TP_ARRAY:
			res += "ARRAY"; break;
		case TP_MAP:
			res += "MAP"; break;
		case TP_BOOL:
			res += "BOOL"; break;
		case TP_FLOAT:
			res += "FLOAT"; break;
		case TP_DOUBLE:
			res += "DOUBLE"; break;
		case TP_EXT:
			res += "EXT"; break;
		case TP_DEFAULT:
			res += "DEFAULT"; break;
	}
	return res;
}

//~~~~~~~~~~~~~~~~~~~~~~~~ Z V A L U E ~~~~~~~~~~~~~~~~~~~~~~~~

template<class T>
class ZValue : public MSGValue {
private:
	T value;
public:
	const T &GetValue() const 		{ return value; }
	void SetValue(const T &new_val) { value = new_val; }

	ZValue(const T &new_val) 	: MSGValue(ValueToTPType(new_val)), value(new_val) { }

	virtual std::string ToString() { return "type: " + ::ToString(this->GetType()) + ", value: " + ::ToString(value); }
};

template<class T>
std::shared_ptr<MSGValue> CreateZValue(const T &new_val) { return std::shared_ptr<MSGValue>(new ZValue<T>(new_val)); }

std::shared_ptr<MSGValue> ZValueFromMSGPack(const char **data);

template<class T>
ZValue<T> *GetZValue(const ZValue_p &p)
{
	return dynamic_cast<ZValue<T> *>(p.get());
}

//~~~~~~~~~~~~~~~~~~~~~~~~ O P E R A T O R S ~~~~~~~~~~~~~~~~~~~~~~~~

template<class T> bool operator==(const ZValue<T> &left, const ZValue<T> &right)
{
	return left.GetValue() == right.GetValue();
}

template<class T> bool operator!=(const ZValue<T> &left, const ZValue<T> &right)
{
	return left.GetValue() != right.GetValue();
}

template<class T> bool operator<(const ZValue<T> &left, const ZValue<T> &right)
{
	return left.GetValue() < right.GetValue();
}

template<class T> bool operator>(const ZValue<T> &left, const ZValue<T> &right)
{
	return left.GetValue() > right.GetValue();
}

template<class T> bool operator<=(const ZValue<T> &left, const ZValue<T> &right)
{
	return left.GetValue() <= right.GetValue();
}

template<class T> bool operator>=(const ZValue<T> &left, const ZValue<T> &right)
{
	return left.GetValue() >= right.GetValue();
}


inline bool operator<(const ZValue_p &left, const ZValue_p &right)
{
	if (left->GetType() != right->GetType()) {
		LogFL(DEBUG) << "operator<(ZValue_p): types is not equal: left = " << (int)(left->GetType()) << ", right = " << (int)(right->GetType()) << "\n";
		throw MakeDbgStr("operator<(ZValue_p): types is not equal: left = ") + std::to_string((int)(left->GetType())) + ", right = " + std::to_string((int)(right->GetType()));
	}
	tp_type tp = left->GetType();
	if ((tp == TP_NIL) || (tp == TP_ARRAY) || (tp == TP_MAP) || (tp == TP_EXT) || (tp == TP_DEFAULT)) {
		LogFL(DEBUG) << "operator<(ZValue_p): type is uncomparable = " << (int)(tp) << "\n";
		throw MakeDbgStr("operator<(ZValue_p): type is uncomparable = ") + std::to_string((int)(tp));
	}

	switch(left->GetType()) {
		case TP_UINT: 	return GetZValue<uint>(left)->GetValue() < GetZValue<uint>(right)->GetValue();
		case TP_INT: 	return GetZValue<int>(left)->GetValue() < GetZValue<int>(right)->GetValue();
		case TP_STR: 	return GetZValue<std::string>(left)->GetValue() < GetZValue<std::string>(right)->GetValue();
		case TP_BOOL: 	return GetZValue<bool>(left)->GetValue() < GetZValue<bool>(right)->GetValue();
		case TP_FLOAT: 	return GetZValue<float>(left)->GetValue() < GetZValue<float>(right)->GetValue();
		case TP_DOUBLE: return GetZValue<double>(left)->GetValue() < GetZValue<double>(right)->GetValue();
		default: 		return false;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif