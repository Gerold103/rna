#ifndef MVALUE_H_INCLUDED
#define MVALUE_H_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstring>
#include <iostream>
#include <ostream>
#include "utils.h"
#include "logger.h"
#include <cstddef>
#include <cstdint>
extern "C" {
	#include "session.h"
	#include "tp.h"
}


#define MValueVector std::vector<MValue>
#define MValueMap std::map<MValue, MValue>

class MValue {
private:
	union {
		uint val_uint;
		int val_int;
		bool val_bool;
		float val_float;
		double val_dbl;
	};
	std::shared_ptr<void> val_obj;

	tp_type val_type;

	void _new_val_type(tp_type new_type) { val_obj = nullptr; val_type = new_type; }

	template<class T> void _reset_val_obj(const T &val) { val_obj.reset(new T(val)); }

public:
	//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

	MValue();

	MValue(const char *val);
	
	template<class T> MValue(const T &val) { SetValue(val); }

	//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

	template<class T> bool SetValue(const T &val) { return false; }

	bool SetValue(const char *val);

	bool SetValue();

	//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

	tp_type GetType() const;

	void GetValue(uint &val) const;
	void GetValue(int &val) const;
	void GetValue(bool &val) const;
	void GetValue(float &val) const;
	void GetValue(double &val) const;

	void GetValue(std::string &val) const;
	void GetValue(DataStructure &val) const;
	void GetValue(MValueVector &val) const;
	void GetValue(MValueMap &val) const;


	uint GetUint() const;
	int GetInt() const;
	bool GetBool() const;
	float GetFloat() const;
	double GetDouble() const;
	const std::string &GetStr() const;
	const DataStructure &GetBin() const;
	const MValueVector &GetArray() const;
	const MValueMap &GetMap() const;

	bool IsNumber() const;
	bool IsString() const;

	//~~~~~~~~ F a b r i c a l   m e t h o d s ~~~~~~~~

	static MValue FromMSGPack(const char **data);
	static MValue FromMSGPack(const DataStructure &data);
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

	template<> inline MValue::MValue(const std::string &val) : val_type(TP_STR) 	{ _reset_val_obj(val); }

	template<> inline MValue::MValue(const DataStructure &val) : val_type(TP_BIN) 	{ _reset_val_obj(val); }

	template<> inline MValue::MValue(const MValueVector &val) : val_type(TP_ARRAY)  { _reset_val_obj(val); }

	template<> inline MValue::MValue(const MValueMap &val): val_type(TP_MAP) 		{ _reset_val_obj(val); }

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

	template<> inline bool MValue::SetValue(const uint &val) 	{ _new_val_type(TP_UINT); val_uint = val; return true; }
	template<> inline bool MValue::SetValue(const int &val)		{ _new_val_type(TP_INT); val_int = val; return true; }
	template<> inline bool MValue::SetValue(const bool &val)	{ _new_val_type(TP_BOOL); val_bool = val; return true; }
	template<> inline bool MValue::SetValue(const float &val)	{ _new_val_type(TP_FLOAT); val_float = val; return true; }
	template<> inline bool MValue::SetValue(const double &val)	{ _new_val_type(TP_DOUBLE); val_dbl = val; return true; }

	template<> inline bool MValue::SetValue(const std::string &val)	{ _new_val_type(TP_STR); _reset_val_obj(val); return true; }
	template<> inline bool MValue::SetValue(const DataStructure &val)	{ _new_val_type(TP_BIN); _reset_val_obj(val); return true; }
	template<> inline bool MValue::SetValue(const MValueVector &val)	{ _new_val_type(TP_ARRAY); _reset_val_obj(val); return true; }
	template<> inline bool MValue::SetValue(const MValueMap &val)		{ _new_val_type(TP_MAP); _reset_val_obj(val); return true; }

//~~~~~~~~ O p e r a t o r s ~~~~~~~~

std::ostream &operator<<(std::ostream &stream, const MValue &ob);

std::ostream &VarDump(std::ostream &stream, const MValue &ob);

std::ostream &operator<<(std::ostream &stream, const MValueVector &ob);
std::ostream &operator<<(std::ostream &stream, const MValueMap &ob);

bool operator==(const MValue &left, const MValue &right);
bool operator!=(const MValue &left, const MValue &right);
bool operator<(const MValue &left, const MValue &right);
bool operator>(const MValue &left, const MValue &right);
bool operator<=(const MValue &left, const MValue &right);
bool operator>=(const MValue &left, const MValue &right);

bool operator&&(const MValue &left, const MValue &right);
bool operator||(const MValue &left, const MValue &right);
bool operator!(const MValue &ob);

MValue operator+(const MValue &left, const MValue &right);
MValue operator-(const MValue &left, const MValue &right);
MValue operator*(const MValue &left, const MValue &right);
MValue operator/(const MValue &left, const MValue &right);
MValue operator%(const MValue &left, const MValue &right);

#endif