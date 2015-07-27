#include "utils.h"
#include <algorithm>

std::string DebugStr(const std::string &file, int line, const std::string &msg)
{
	return "File: " + file + ", line: " + std::to_string(line) + ": " + msg;
}

//~~~~~~~~~~~~~~~~~~~~~~~~ M S G V A L ~~~~~~~~~~~~~~~~~~~~~~~

tp_type MSGValue::GetType() const
{
	return type;
}

MSGValue::MSGValue(tp_type tp) : type(tp) { }

MSGValue::MSGValue() : type(TP_NIL) { }

MSGValue::~MSGValue() { }

//~~~~~~~~~~~~~~~~~~~~~~~~ Z V A L U E ~~~~~~~~~~~~~~~~~~~~~~~~

std::shared_ptr<MSGValue> ZValueFromMSGPack(const char **data)
{
	mp_type type = mp_typeof(**data);
	std::cout << MakeDbgStr("ZValueFromMSGPack(): type = ") << ToString(static_cast<tp_type>(type)) << ", value = ";
	switch(type) {
		case MP_NIL:
			mp_decode_nil(data);
			LogFL(DEBUG) << "nil\n";
			return CreateZValue(DataStructure());
		case MP_UINT: {
			uint val = mp_decode_uint(data);
			LogFL(DEBUG) << val << "\n";
			return CreateZValue(val);
		}
		case MP_INT: {
			int val = mp_decode_int(data);
			LogFL(DEBUG) << val << "\n";
			return CreateZValue(val);
		}
		case MP_STR: {
			uint32_t tmp_len = 0;
			const char *tmp_str = mp_decode_str(data, &tmp_len);
			std::string val(tmp_str, tmp_len);
			LogFL(DEBUG) << "\"" << val << "\"\n";
			return CreateZValue(val);
		}
		case MP_BIN: {
			uint32_t tmp_len = 0;
			const char *tmp_bin = mp_decode_bin(data, &tmp_len);
			LogFL(DEBUG) << "bin\n";
			return CreateZValue(DataStructure(tmp_bin, tmp_len));
		}
		case MP_BOOL: {
			bool val = mp_decode_bool(data);
			LogFL(DEBUG) << val << "\n";
			return CreateZValue(val);
		}
		case MP_FLOAT: {
			float val = mp_decode_float(data);
			LogFL(DEBUG) << val << "\n";
			return CreateZValue(val);
		}
		case MP_DOUBLE: {
			double val = mp_decode_double(data);
			LogFL(DEBUG) << val << "\n";
			return CreateZValue(val);
		}

		//~~~~~~~~~~~~~~~~N o n t r i v i a l~~~~~~~~~~~~~~~~

		case MP_ARRAY: {
			uint32_t size = mp_decode_array(data);
			LogFL(DEBUG) << "ZValueFromMSGPack: size of array = " << size << "\n";
			ZValueVector val;
			for (uint32_t i = 0; i < size; ++i) {
				val.push_back(ZValueFromMSGPack(data));
			}
			LogFL(DEBUG) << "ZValueFromMSGPack: array created\n";
			return CreateZValue(val);
		}
		
		default:
			LogFL(DEBUG) << "ZValueFromMSGPack(): default type, = " << ToString(static_cast<tp_type>(type)) << "\n";
			return CreateZValue(DataStructure());
	}
}