#include "mvalue.h"

//~~~~~~~~ C o n s t r u c t o r s ~~~~~~~~

MValue::MValue() : val_type(TP_DEFAULT) { }

MValue::MValue(const char *val)
{
	SetValue(val);
}

//~~~~~~~~ S e t   m e t h o d s ~~~~~~~~

bool MValue::SetValue(const char *val)
{
	std::string tmp(val);
	return SetValue(tmp);
}

bool MValue::SetValue() { _new_val_type(TP_NIL); return true; }

//~~~~~~~~ G e t   M e t h o d s ~~~~~~~~

tp_type MValue::GetType() const { return val_type; }

#define _get_val_(_val_type_, _tp_type_, _val_postfix_) if (val_type == _tp_type_) val = val_##_val_postfix_; \
	else { \
		LogFL(DEBUG) << "MValue::GetValue(): val_type is not " #_val_type_ "\n"; \
		throw MakeDbgStr("MValue::GetValue(): val_type is not " #_val_type_); \
	} \

void MValue::GetValue(uint &val) const {
	_get_val_(uint, TP_UINT, uint);
}

void MValue::GetValue(int &val) const {
	_get_val_(int, TP_INT, int);
}

void MValue::GetValue(bool &val) const {
	_get_val_(bool, TP_BOOL, bool);
}

void MValue::GetValue(float &val) const {
	_get_val_(float, TP_FLOAT, float);
}

void MValue::GetValue(double &val) const {
	_get_val_(double, TP_DOUBLE, dbl);
}

#undef _get_val_
#define _get_val_(_val_type_, _tp_type_) if (val_type == _tp_type_) val = *reinterpret_cast<_val_type_ *>(val_obj.get()); \
	else { \
		LogFL(DEBUG) << "MValue::GetValue(): val_type is not " #_val_type_ "\n"; \
		throw MakeDbgStr("MValue::GetValue(): val_type is not " #_val_type_); \
	} \

void MValue::GetValue(std::string &val) const {
	_get_val_(std::string, TP_STR);
}

void MValue::GetValue(DataStructure &val) const {
	_get_val_(DataStructure, TP_BIN);
}

void MValue::GetValue(MValueVector &val) const {
	_get_val_(MValueVector, TP_ARRAY);
}

void MValue::GetValue(MValueMap &val) const {
	_get_val_(MValueMap, TP_MAP);
}

#undef _get_val_
#define _get_val_(_val_type_, _tp_type_, _val_postfix_) if (val_type == _tp_type_) return val_##_val_postfix_; \
	else { \
		LogFL(DEBUG) << "MValue::GetValue(): val_type is not " #_val_type_ "\n"; \
		throw MakeDbgStr("MValue::GetValue(): val_type is not " #_val_type_); \
	} \

uint MValue::GetUint() const {
	_get_val_(uint, TP_UINT, uint);
}

int MValue::GetInt() const {
	_get_val_(int, TP_INT, int);
}

bool MValue::GetBool() const {
	_get_val_(bool, TP_BOOL, bool);
}

float MValue::GetFloat() const {
	_get_val_(float, TP_FLOAT, float);
}

double MValue::GetDouble() const {
	_get_val_(double, TP_DOUBLE, dbl);
}

#undef _get_val_
#define _get_val_(_val_type_, _tp_type_) if (val_type == _tp_type_) return *reinterpret_cast<_val_type_ *>(val_obj.get()); \
	else { \
		LogFL(DEBUG) << "MValue::GetValue(): val_type is not " #_val_type_ "\n"; \
		throw MakeDbgStr("MValue::GetValue(): val_type is not " #_val_type_); \
	} \

const std::string &MValue::GetStr() const {
	_get_val_(std::string, TP_STR);
}

const DataStructure &MValue::GetBin() const {
	_get_val_(DataStructure, TP_BIN);
}

const MValueVector &MValue::GetArray() const {
	_get_val_(MValueVector, TP_ARRAY);
}

const MValueMap &MValue::GetMap() const {
	_get_val_(MValueMap, TP_MAP);
}

bool MValue::IsNumber() const
{
	return (GetType() == TP_INT) || (GetType() == TP_UINT) || (GetType() == TP_FLOAT) || (GetType() == TP_DOUBLE);
}

bool MValue::IsString() const
{
	return (GetType() == TP_STR);
}

#undef _get_val_

//~~~~~~~~ F a b r i c a l   m e t h o d s ~~~~~~~~

MValue MValue::FromMSGPack(const char **data)
{
	const char *tuple = *data;
	mp_type type = mp_typeof(*tuple);
	LogFL(DEBUG) << " MValue::FromMSGPack(): type = " << ToString(static_cast<tp_type>(type)) << ", value = \n";
	switch(type) {
		case MP_NIL:
			mp_decode_nil(&tuple);
			LogFL(DEBUG) << "nil\n";
			*data = tuple;
			return MValue(DataStructure());
		case MP_UINT: {
			uint val = mp_decode_uint(&tuple);
			LogFL(DEBUG) << val << "\n";
			*data = tuple;
			return MValue(val);
		}
		case MP_INT: {
			int val = mp_decode_int(&tuple);
			LogFL(DEBUG) << val << "\n";
			*data = tuple;
			return MValue(val);
		}
		case MP_STR: {
			uint32_t tmp_len = 0;
			const char *tmp_str = mp_decode_str(&tuple, &tmp_len);
			std::string val(tmp_str, tmp_len);
			LogFL(DEBUG) << "\"" << val << "\"\n";
			*data = tuple;
			return MValue(val);
		}
		case MP_BIN: {
			uint32_t tmp_len = 0;
			const char *tmp_bin = mp_decode_bin(&tuple, &tmp_len);
			LogFL(DEBUG) << "bin\n";
			*data = tuple;
			return MValue(DataStructure(tmp_bin, tmp_len));
		}
		case MP_BOOL: {
			bool val = mp_decode_bool(&tuple);
			LogFL(DEBUG) << val << "\n";
			*data = tuple;
			return MValue(val);
		}
		case MP_FLOAT: {
			float val = mp_decode_float(&tuple);
			LogFL(DEBUG) << val << "\n";
			*data = tuple;
			return MValue(val);
		}
		case MP_DOUBLE: {
			double val = mp_decode_double(&tuple);
			LogFL(DEBUG) << val << "\n";
			*data = tuple;
			return MValue(val);
		}

		//~~~~~~~~~~~~~~~~N o n t r i v i a l~~~~~~~~~~~~~~~~

		case MP_ARRAY: {
			uint32_t size = mp_decode_array(&tuple);
			LogFL(DEBUG) << "MValue::FromMSGPack(): size of array = " << size << "\n";
			MValueVector val;
			for (uint32_t i = 0; i < size; ++i) {
				val.push_back(MValue::FromMSGPack(&tuple));
			}
			LogFL(DEBUG) << "MValue::FromMSGPack(): array created\n";
			*data = tuple;
			return MValue(val);
		}

		case MP_MAP: {
			uint32_t size = mp_decode_map(&tuple);
			LogFL(DEBUG) << "MValue::FromMSGPack(): sizeof map = " << size << "\n";
			MValueMap val;
			for (uint32_t i = 0; i < size; ++i) {
				MValue key = MValue::FromMSGPack(&tuple);
				MValue value = MValue::FromMSGPack(&tuple);
				val.insert(std::make_pair(key, value));
			}
			LogFL(DEBUG) << "MValue::FromMSGPack(): map created\n";
			*data = tuple;
			return MValue(val);
		}
		
		default:
			LogFL(DEBUG) << "MValue::FromMSGPack(): default type, = " << ToString(static_cast<tp_type>(type)) << "\n";
			*data = tuple;
			return MValue(DataStructure());
	}
}

MValue MValue::FromMSGPack(const DataStructure &data)
{
	const char *tuple = data.Data();
	mp_type type = mp_typeof(*tuple);
	LogFL(DEBUG) << " MValue::FromMSGPack(): type = " << ToString(static_cast<tp_type>(type)) << ", value = \n";
	switch(type) {
		case MP_NIL:
			mp_decode_nil(&tuple);
			LogFL(DEBUG) << "nil\n";
			return MValue(DataStructure());
		case MP_UINT: {
			uint val = mp_decode_uint(&tuple);
			LogFL(DEBUG) << val << "\n";
			return MValue(val);
		}
		case MP_INT: {
			int val = mp_decode_int(&tuple);
			LogFL(DEBUG) << val << "\n";
			return MValue(val);
		}
		case MP_STR: {
			uint32_t tmp_len = 0;
			const char *tmp_str = mp_decode_str(&tuple, &tmp_len);
			std::string val(tmp_str, tmp_len);
			LogFL(DEBUG) << "\"" << val << "\"\n";
			return MValue(val);
		}
		case MP_BIN: {
			uint32_t tmp_len = 0;
			const char *tmp_bin = mp_decode_bin(&tuple, &tmp_len);
			LogFL(DEBUG) << "bin\n";
			return MValue(DataStructure(tmp_bin, tmp_len));
		}
		case MP_BOOL: {
			bool val = mp_decode_bool(&tuple);
			LogFL(DEBUG) << val << "\n";
			return MValue(val);
		}
		case MP_FLOAT: {
			float val = mp_decode_float(&tuple);
			LogFL(DEBUG) << val << "\n";
			return MValue(val);
		}
		case MP_DOUBLE: {
			double val = mp_decode_double(&tuple);
			LogFL(DEBUG) << val << "\n";
			return MValue(val);
		}

		//~~~~~~~~~~~~~~~~N o n t r i v i a l~~~~~~~~~~~~~~~~

		case MP_ARRAY: {
			uint32_t size = mp_decode_array(&tuple);
			LogFL(DEBUG) << "MValue::FromMSGPack(): size of array = " << size << "\n";
			MValueVector val;
			for (uint32_t i = 0; i < size; ++i) {
				val.push_back(MValue::FromMSGPack(&tuple));
			}
			LogFL(DEBUG) << "MValue::FromMSGPack(): array created\n";
			return MValue(val);
		}

		case MP_MAP: {
			uint32_t size = mp_decode_map(&tuple);
			LogFL(DEBUG) << "MValue::FromMSGPack(): sizeof map = " << size << "\n";
			MValueMap val;
			for (uint32_t i = 0; i < size; ++i) {
				MValue key = MValue::FromMSGPack(&tuple);
				MValue value = MValue::FromMSGPack(&tuple);
				val.insert(std::make_pair(key, value));
			}
			LogFL(DEBUG) << "MValue::FromMSGPack(): map created\n";
			return MValue(val);
		}
		
		default:
			LogFL(DEBUG) << "MValue::FromMSGPack(): default type, = " << ToString(static_cast<tp_type>(type)) << "\n";
			return MValue(DataStructure());
	}
}

//~~~~~~~~ O p e r a t o r s ~~~~~~~~

std::ostream &operator<<(std::ostream &stream, const MValue &ob)
{
	switch (ob.GetType()) {
		case TP_NIL: stream << "nil"; break;
		case TP_UINT: stream << ob.GetUint(); break;
		case TP_INT: stream << ob.GetInt(); break;
		case TP_STR: stream << "'" << ob.GetStr() << "'"; break;
		case TP_BIN: stream << ob.GetBin(); break;
		case TP_ARRAY: stream << ob.GetArray(); break;
		case TP_MAP: stream << ob.GetMap(); break;
		case TP_BOOL: stream << ob.GetBool(); break;
		case TP_FLOAT: stream << ob.GetFloat(); break;
		case TP_DOUBLE: stream << ob.GetDouble(); break;
		default: stream << "default"; break;
	}
	return stream;
}

std::ostream &VarDump(std::ostream &stream, const MValue &ob)
{
	stream << "tp_type: " << ToString(ob.GetType()) << "; value = " << ob;
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const MValueVector &ob)
{
	size_t size = ob.size();
	stream << "[";
	for (size_t i = 0; i < size; ++i) {
		stream << ob[i];
		if (i != size - 1) stream << ", ";
	}
	stream << "]";
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const MValueMap &ob)
{
	size_t size = ob.size(), i = 0;
	stream << "(";
	for (auto it = ob.begin(); it != ob.end(); ++it, ++i) {
		stream << "{" << it->first << " : " << it->second << "}";
		if (i != size - 1) stream << ", ";
	}
	stream << ")";
	return stream;
}

bool operator==(const MValue &left, const MValue &right)
{
	if ((left.GetType() == TP_BOOL) && (right.GetType() == TP_BOOL)) return left.GetBool() == right.GetBool();
	return (!(left < right)) && (!(left > right));
}

#define _get_val_(_left_val_) switch(right.GetType()) { \
	case TP_UINT: return _left_val_ != right.GetUint(); \
	case TP_INT: return _left_val_ != right.GetInt(); \
	case TP_FLOAT: return _left_val_ != right.GetFloat(); \
	case TP_DOUBLE: return _left_val_ != right.GetDouble(); \
	default: { \
		LogFL(DEBUG) << "operator!=(Mvalue): different types can not be compared\n"; \
		throw MakeDbgStr("operator!=(Mvalue): different types can not be compared"); \
	} \
} \

bool operator!=(const MValue &left, const MValue &right)
{
	switch(left.GetType()) {
		case TP_NIL: return right.GetType() != TP_NIL;
		case TP_UINT: {
			_get_val_(left.GetUint());
		}
		case TP_INT: {
			_get_val_(left.GetInt());
		}
		case TP_STR: {
			if (right.GetType() != TP_STR) {
				LogFL(DEBUG) << "operator!=(Mvalue): different types can not be compared\n";
				throw MakeDbgStr("operator!=(Mvalue): different types can not be compared");
			}
			return left.GetStr() != right.GetStr();
		}
		case TP_BIN: {
			LogFL(DEBUG) << "operator!=(Mvalue): bin type can not be compared\n";
			throw MakeDbgStr("operator!=(Mvalue): bin type can not be compared");
		}
		case TP_ARRAY: {
			LogFL(DEBUG) << "operator!=(Mvalue): array type can not be compared\n";
			throw MakeDbgStr("operator!=(Mvalue): array type can not be compared");
		}
		case TP_MAP: {
			LogFL(DEBUG) << "operator!=(Mvalue): map type can not be compared\n";
			throw MakeDbgStr("operator!=(Mvalue): map type can not be compared");
		}
		case TP_BOOL: {
			if (right.GetType() != TP_BOOL) {
				LogFL(DEBUG) << "operator!=(Mvalue): different types can not be compared\n";
				throw MakeDbgStr("operator!=(Mvalue): different types can not be compared");
			}
			return left.GetBool() != right.GetBool();
		}
		case TP_FLOAT: {
			_get_val_(left.GetFloat());
		}
		case TP_DOUBLE: {
			_get_val_(left.GetDouble());
		}
		default: {
			LogFL(DEBUG) << "operator!=(Mvalue): that types can not be compared\n";
			throw MakeDbgStr("operator!=(Mvalue): that types can not be compared");
		}
	}
}

#undef _get_val_
#define _get_val_(_left_val_, _left_type_) switch(right.GetType()) { \
	case TP_UINT: return _left_val_ < right.GetUint(); \
	case TP_INT: return _left_val_ < right.GetInt(); \
	case TP_STR: { \
		LogFL(DEBUG) << "operator<(MValue): str and " #_left_type_ " can't be comared\n"; \
		throw MakeDbgStr("operator<(Mvalue): str and " #_left_type_ " can't be comared"); \
	} \
	case TP_FLOAT: return _left_val_ < right.GetFloat(); \
	case TP_DOUBLE: return _left_val_ < right.GetDouble(); \
	default: { \
		LogFL(DEBUG) << "operator<(Mvalue): deault type ?!\n"; \
		throw MakeDbgStr("operator<(Mvalue): deault type ?!"); \
	} \
} \

bool operator<(const MValue &left, const MValue &right)
{
	if ((left.GetType() == TP_NIL) || (left.GetType() == TP_ARRAY) || (left.GetType() == TP_MAP) || (left.GetType() == TP_BIN) || (left.GetType() == TP_BOOL) || 
		(right.GetType() == TP_NIL) || (right.GetType() == TP_ARRAY) || (right.GetType() == TP_MAP) || (right.GetType() == TP_BIN) || (right.GetType() == TP_BOOL)) {
		LogFL(DEBUG) << "operator<(Mvalue): uncomparable types. left = " << ToString(left.GetType()) << ", right = " << ToString(right.GetType()) << "\n";
		throw MakeDbgStr("operator<(Mvalue): uncomparable types");
	}
	switch (left.GetType()) {
		case TP_UINT: {
			_get_val_(left.GetUint(), uint);
		}
		case TP_INT: {
			_get_val_(left.GetInt(), int);
		}
		case TP_STR: {
			if (right.GetType() == TP_STR) return left.GetStr() < right.GetStr();
			else {
				LogFL(DEBUG) << "operator<(MValue): str can't be compared with no str\n";
				throw MakeDbgStr("operator<(MValue): str can't be compared with no str");
			}
		}
		case TP_FLOAT: {
			_get_val_(left.GetFloat(), float);
		}
		case TP_DOUBLE: {
			_get_val_(left.GetDouble(), double);
		}
		default: {
			LogFL(DEBUG) << "operator<(Mvalue): deault type ?!\n";
			throw MakeDbgStr("operator<(Mvalue): deault type ?!");
		}
	}
}

bool operator>(const MValue &left, const MValue &right)
{
	return (!(left < right)) && (left != right);
}

bool operator<=(const MValue &left, const MValue &right)
{
	return !(left > right);
}

bool operator>=(const MValue &left, const MValue &right)
{
	return !(left < right);
}


bool operator&&(const MValue &left, const MValue &right)
{
	if (left.GetType() != right.GetType()) return false;
	switch(left.GetType()) {
		case TP_NIL: return false;
		case TP_UINT: return left.GetUint() && right.GetUint();
		case TP_INT: return left.GetInt() && right.GetInt();
		case TP_STR: return left.GetStr().length() && right.GetStr().length();
		case TP_BIN: return left.GetBin() && right.GetBin();
		case TP_ARRAY: return left.GetArray().size() && right.GetArray().size();
		case TP_MAP: return left.GetMap().size() && right.GetMap().size();
		case TP_BOOL: return left.GetBool() && right.GetBool();
		case TP_FLOAT: return left.GetFloat() && right.GetFloat();
		case TP_DOUBLE: return left.GetDouble() && right.GetDouble();
		default: return false;
	}
}

bool operator||(const MValue &left, const MValue &right)
{
	if (left.GetType() != right.GetType()) return false;
	switch(left.GetType()) {
		case TP_NIL: return false;
		case TP_UINT: return left.GetUint() || right.GetUint();
		case TP_INT: return left.GetInt() || right.GetInt();
		case TP_STR: return left.GetStr().length() || right.GetStr().length();
		case TP_BIN: return left.GetBin() || right.GetBin();
		case TP_ARRAY: return left.GetArray().size() || right.GetArray().size();
		case TP_MAP: return left.GetMap().size() || right.GetMap().size();
		case TP_BOOL: return left.GetBool() || right.GetBool();
		case TP_FLOAT: return left.GetFloat() || right.GetFloat();
		case TP_DOUBLE: return left.GetDouble() || right.GetDouble();
		default: return false;
	}
}

bool operator!(const MValue &ob)
{
	switch(ob.GetType()) {
		case TP_NIL: return false;
		case TP_UINT: return !ob.GetUint();
		case TP_INT: return !ob.GetInt();
		case TP_STR: return !ob.GetStr().length();
		case TP_BIN: return !ob.GetBin();
		case TP_ARRAY: return !ob.GetArray().size();
		case TP_MAP: return !ob.GetMap().size();
		case TP_BOOL: return !ob.GetBool();
		case TP_FLOAT: return !ob.GetFloat();
		case TP_DOUBLE: return !ob.GetDouble();
		default: return false;
	}
}

#define _make_concr_op(_val_type_, _mval_meth_, op) _val_type_ val = left._mval_meth_(); \
			switch(right.GetType()) { \
				case TP_UINT: { \
					if (left.GetType() == TP_INT) return MValue(val op static_cast<int>((right.GetUint()))); \
					else return MValue(val op right.GetUint()); \
				} \
				case TP_INT: { \
					if (left.GetType() == TP_UINT) return MValue(static_cast<int>(val) op right.GetInt()); \
					else return MValue(val op right.GetInt()); \
				} \
				case TP_FLOAT: return MValue(val op right.GetFloat()); \
				case TP_DOUBLE: return MValue(val op right.GetDouble()); \
				default: return MValue(); \
			} \
			break; \

#define _make_op_(op) switch(left.GetType()) { \
		case TP_UINT: { _make_concr_op(uint, GetUint, op) } \
		case TP_INT: { _make_concr_op(int, GetInt, op) } \
		case TP_FLOAT: { _make_concr_op(float, GetFloat, op) } \
		case TP_DOUBLE: { _make_concr_op(double, GetDouble, op) } \
		default: return MValue(); \
	} \
	return MValue(); \

MValue operator+(const MValue &left, const MValue &right)
{
	if ((left.GetType() == TP_NIL) || (left.GetType() == TP_BIN) || (left.GetType() == TP_ARRAY) || (left.GetType() == TP_MAP) || (left.GetType() == TP_DEFAULT) || (left.GetType() == TP_EXT) || (left.GetType() == TP_BOOL) ||
		(right.GetType() == TP_NIL) || (right.GetType() == TP_BIN) || (right.GetType() == TP_ARRAY) || (right.GetType() == TP_MAP) || (right.GetType() == TP_DEFAULT) || (right.GetType() == TP_EXT) || (right.GetType() == TP_BOOL)) {
		return MValue();
	}
	if (((left.GetType() == TP_STR) && (right.GetType() != TP_STR)) || ((left.GetType() != TP_STR) && (right.GetType() == TP_STR))) return MValue();
	else if ((left.GetType() == TP_STR) && (right.GetType() == TP_STR)) return MValue(left.GetStr() + right.GetStr());

	_make_op_(+)
}

MValue operator-(const MValue &left, const MValue &right)
{
	if ((left.GetType() == TP_NIL) || (left.GetType() == TP_BIN) || (left.GetType() == TP_ARRAY) || (left.GetType() == TP_STR) ||
		(left.GetType() == TP_MAP) || (left.GetType() == TP_DEFAULT) || (left.GetType() == TP_EXT) || (left.GetType() == TP_BOOL) ||
		(right.GetType() == TP_NIL) || (right.GetType() == TP_BIN) || (right.GetType() == TP_ARRAY) || (right.GetType() == TP_STR) ||
		(right.GetType() == TP_MAP) || (right.GetType() == TP_DEFAULT) || (right.GetType() == TP_EXT) || (right.GetType() == TP_BOOL)) {
		return MValue();
	}

	_make_op_(-)
}

MValue operator*(const MValue &left, const MValue &right)
{
	if ((left.GetType() == TP_NIL) || (left.GetType() == TP_BIN) || (left.GetType() == TP_ARRAY) || (left.GetType() == TP_STR) ||
		(left.GetType() == TP_MAP) || (left.GetType() == TP_DEFAULT) || (left.GetType() == TP_EXT) || (left.GetType() == TP_BOOL) ||
		(right.GetType() == TP_NIL) || (right.GetType() == TP_BIN) || (right.GetType() == TP_ARRAY) || (right.GetType() == TP_STR) ||
		(right.GetType() == TP_MAP) || (right.GetType() == TP_DEFAULT) || (right.GetType() == TP_EXT) || (right.GetType() == TP_BOOL)) {
		return MValue();
	}

	_make_op_(*)
}
MValue operator/(const MValue &left, const MValue &right)
{
	if ((left.GetType() == TP_NIL) || (left.GetType() == TP_BIN) || (left.GetType() == TP_ARRAY) || (left.GetType() == TP_STR) ||
		(left.GetType() == TP_MAP) || (left.GetType() == TP_DEFAULT) || (left.GetType() == TP_EXT) || (left.GetType() == TP_BOOL) ||
		(right.GetType() == TP_NIL) || (right.GetType() == TP_BIN) || (right.GetType() == TP_ARRAY) || (right.GetType() == TP_STR) ||
		(right.GetType() == TP_MAP) || (right.GetType() == TP_DEFAULT) || (right.GetType() == TP_EXT) || (right.GetType() == TP_BOOL)) {
		return MValue();
	}

	_make_op_(/)
}

MValue operator%(const MValue &left, const MValue &right)
{
	switch(left.GetType()) {
		case TP_UINT: {
			uint val = left.GetUint();
			switch(right.GetType()) {
				case TP_UINT: return MValue(val % right.GetUint());
				case TP_INT: return MValue(static_cast<int>(val) % right.GetInt());
				default: return MValue();
			}
		}
		case TP_INT: {
			int val = left.GetInt();
			switch(right.GetType()) {
				case TP_UINT: return MValue(val % static_cast<int>(right.GetUint()));
				case TP_INT: return MValue(val % right.GetInt());
				default: return MValue();
			}
		}
		default: return MValue();
	}
}