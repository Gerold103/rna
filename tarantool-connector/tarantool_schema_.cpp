#include <algorithm>
#include <cstring>
#include <cinttypes>
#include <cctype>
#include <iostream>
#include "mvalue.h"
#include "tp_wrap.h"

#include "tarantool_schema_.h"
extern "C" {
	#include "msgpuck/msgpuck.h"
}

SpacePart::SpacePart() : type(FT_OTHER) { }

namespace Convert {

std::string ToString(FieldType tp)
{
	switch(tp) {
		case FT_NUM: return "FT_NUM";
		case FT_STR: return "FT_STR";
		case FT_OTHER: return "FT_OTHER";
		default: return "DEFAULT";
	}
}

}

std::ostream &operator<<(std::ostream &stream, FieldType tp)
{
	switch(tp) {
		case FT_NUM:
			stream << "FT_NUM";
			return stream;
		case FT_STR:
			stream << "FT_STR";
			return stream;
		case FT_OTHER:
			stream << "FT_OTHER";
			return stream;
		default:
			stream << "DEFAULT";
			return stream;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   B A S E   I N F O ~~~~~~~~~~~~~~~~~~~~~~~~

TarantoolBaseInfo::TarantoolBaseInfo(int id_, const std::string &name_) : id(id_), name(name_) { }

const std::string &TarantoolBaseInfo::Name() const
{
	return name;
}

int TarantoolBaseInfo::ID() const
{
	return id;
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   I N D E X   I N F O ~~~~~~~~~~~~~~~~~~~~~~~~

TarantoolIndexInfo::TarantoolIndexInfo(int id_, const std::string &name_, IndexType type_, bool unique_, int space_id_)
	: TarantoolBaseInfo(id_, name_), type(type_), unique(unique_), space_id(space_id_) { }

const std::vector<SpacePart> &TarantoolIndexInfo::Parts() const
{
	return index_parts;
}

namespace Convert {
	std::string ToString(TarantoolIndexInfo::IndexType tp)
	{
		switch(tp) {
			case TarantoolIndexInfo::HASH: return "HASH";
			case TarantoolIndexInfo::TREE: return "TREE";
			case TarantoolIndexInfo::BITSET: return "BITSET";
			case TarantoolIndexInfo::RTREE: return "RTREE";
			default: return "undefined";
		}
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   S P A C E   I N F O ~~~~~~~~~~~~~~~~~~~~~~~~

TarantoolSpaceInfo::TarantoolSpaceInfo(int id_, const std::string &name_, int owner_, const std::string &engine_, const std::string &flags_)
	: TarantoolBaseInfo(id_, name_), owner(owner_), engine(engine_), flags(flags_) { }

const SpacePart *TarantoolSpaceInfo::ColumnBy(int number) const
{
	if ((number >= static_cast<int>(space_parts.size())) || (number < 0)) return nullptr;
	return &(space_parts[number]);
}

const SpacePart *TarantoolSpaceInfo::ColumnBy(const std::string &name_) const
{
	for (size_t i = 0, size = space_parts.size(); i < size; ++i) {
		if (space_parts[i].name == name_) {
			return &(space_parts[i]);
		}
	}
	return nullptr;
}

int TarantoolSpaceInfo::ColumnNumber(const std::string &name_) const
{
	for (size_t i = 0, size = space_parts.size(); i < size; ++i) {
		if (space_parts[i].name == name_) {
			return i;
		}
	}
	return -1;
}

const TarantoolIndexInfo *TarantoolSpaceInfo::IndexBy(int number) const
{
	auto it = indices_and_ids.find(number);
	if (it == indices_and_ids.end()) return nullptr;
	return it->second;
}

const TarantoolIndexInfo *TarantoolSpaceInfo::IndexBy(const std::string &name_) const
{
	auto it = indices_and_names.find(name);
	if (it == indices_and_names.end()) return nullptr;
	return it->second;
}

const std::vector<std::shared_ptr<TarantoolIndexInfo> > TarantoolSpaceInfo::Indices() const
{
	return infos;
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T A R A N T O O L   I N F O ~~~~~~~~~~~~~~~~~~~~~~~~

bool TarantoolInfo::_add_spaces_(const DataStructure &data)
{
	spaces_and_names.clear();
	spaces_and_ids.clear();
	infos.clear();

	MValue t = MValue::FromMSGPack(data);
	if (t.GetType() != TP_ARRAY) {
		LogFL(DEBUG) << "TarantoolInfo::_add_spaces_(): data must be array\n";
		return false;
	}
	const MValueVector &spaces = t.GetArray();

	for (auto it = spaces.begin(); it != spaces.end(); ++it) {
		if (it->GetType() != TP_ARRAY) {
			LogFL(DEBUG) << "TarantoolInfo::_add_spaces_(): space tuple must be array\n";
			return false;
		}
		const MValueVector &cur_space = it->GetArray();
		int id = cur_space[0].GetUint();
		int owner = cur_space[1].GetUint();
		std::string name = cur_space[2].GetStr();
		std::string engine = cur_space[3].GetStr();
		//skip part count
		std::string flags = cur_space[5].GetStr();

		std::shared_ptr<TarantoolSpaceInfo> space_info(new TarantoolSpaceInfo(id, name, owner, engine, flags));

		const MValueVector &columns = cur_space[6].GetArray();
		for (int i = 0, size = columns.size(); i < size; ++i) {
			const MValueMap &cur_part = columns[i].GetMap();
			SpacePart tmp;
			tmp.name = cur_part.at(MValue("name")).GetStr();
			tmp.type = (cur_part.at(MValue("type")) == MValue("str")) ? FT_STR : FT_NUM;
			space_info->space_parts.push_back(tmp);
		}

		infos.push_back(space_info);
		spaces_and_ids.insert(std::make_pair(id, space_info.get()));
		spaces_and_names.insert(std::make_pair(name, space_info.get()));
	}
	return true;
}

bool TarantoolInfo::_add_indices_(const DataStructure &data)
{
	MValue t = MValue::FromMSGPack(data);
	if (t.GetType() != TP_ARRAY) {
		LogFL(DEBUG) << "TarantoolInfo::_add_indices_(): data must be array\n";
		return false;
	}
	const MValueVector &indices = t.GetArray();

	for (auto it = indices.begin(); it != indices.end(); ++it) {
		if (it->GetType() != TP_ARRAY) {
			LogFL(DEBUG) << "TarantoolInfo::_add_indices_(): index tuple must be array\n";
			return false;
		}
		const MValueVector &cur_index = it->GetArray();
		int space_id = cur_index[0].GetUint();
		int iid = cur_index[1].GetUint();
		std::string name = cur_index[2].GetStr();
		TarantoolIndexInfo::IndexType type = TarantoolIndexInfo::TREE;
		{
			std::string _type = cur_index[3].GetStr();
			std::transform(_type.begin(), _type.end(), _type.begin(), ::tolower);
			if (_type == "hash") {
				type = TarantoolIndexInfo::HASH;
			} else if (_type == "bitset") {
				type = TarantoolIndexInfo::BITSET;
			} else if (_type == "rtree") {
				type = TarantoolIndexInfo::RTREE;
			} else if (_type != "tree") {
				LogFL(DEBUG) << "TarantoolInfo::_add_indices_(): error with reading type of index\n";
				return false;
			}
		}
		bool unique = !!cur_index[4].GetUint();
		int part_count = cur_index[5].GetUint();
		TarantoolSpaceInfo *space = this->_space_by(space_id);
		std::shared_ptr<TarantoolIndexInfo> index(new TarantoolIndexInfo(iid, name, type, unique, space_id));
		for (int i = 0; i < part_count * 2; i += 2) {
			index->index_parts.push_back(*(space->ColumnBy(cur_index[6 + i].GetUint())));
		}
		space->infos.push_back(index);
		space->indices_and_ids.insert(std::make_pair(iid, index.get()));
		space->indices_and_names.insert(std::make_pair(name, index.get()));
	}
	return true;
}

DataStructure internal_execute_select(TP_p &request, int _space_id, Session &ses)
{
	request->AddSelect(_space_id, 0, 0, TP_ITERATOR_ALL, UINT32_MAX);
	request->ReserveKeyParts();

	int ss = ses.Send(*request);
	if (ss < 0) {
		LogFL(DEBUG) << "internal_execute_select(): error while sending request was occured, return = "<< ss << "\n";
		return DataStructure();
	}

	ss = ses.Sync();
	if (ss < 0) {
		LogFL(DEBUG) << "internal_execute_select(): error while syncing sesson was occured, return = " << ss << "\n";
		return DataStructure();
	}

	//------------------------ R E P L Y ------------------------

	DataStructure response_buffer(FIRST_PACK_PART_SIZE);
	ssize_t len = ses.Receive(response_buffer);
	ses.Sync();
	if (len < FIRST_PACK_PART_SIZE) {
		LogFL(DEBUG) << "internal_execute_select(): length of receive less than 5 = " << len << "\n";
		return DataStructure();
	}
	if (mp_typeof(*response_buffer.Data()) != MP_UINT) {
		LogFL(DEBUG) << "internal_execute_select(): bad reply length type = "  << mp_typeof(*response_buffer.Data()) << "\n";
		return DataStructure();
	}
	uint32_t body = tp_get_uint(response_buffer.Data());

	response_buffer.Resize(FIRST_PACK_PART_SIZE + body);
	len = ses.Receive(response_buffer, FIRST_PACK_PART_SIZE);
	ses.Sync();
	if (len < body) {
		LogFL(DEBUG) << "internal_execute_select(): receive failed, len = " << len << "\n";
		return DataStructure();
	}
	TPResponse resp(response_buffer);
	if (resp.GetState() == -1) {
		LogFL(DEBUG) <<"internal_execute_select(): failed to parse response\n";
		return DataStructure();
	}
	if (resp.GetCode() != 0) {
		LogFL(DEBUG) << "internal_execute_select(): server respond: " << resp.GetCode() << ", " << resp.GetError() << "\n";
		return DataStructure();
	}
	return resp.GetData();
}

TarantoolInfo::TarantoolInfo(Session &ses, int _space_id) : session(&ses), service_space(_space_id)
{
	size_t msg_size = 128;
	size_t msg_max_size = 1048576;

	TP_p request(new TP(DataStructure(msg_size)));
	DataStructure res = internal_execute_select(request, _space_id, ses);

	if (!this->_add_spaces_(res)) {
		LogFL(DEBUG) << "TarantoolInfo::TarantoolInfo(): error with adding spaces\n";
		return;
	}

	_space_id = this->SpaceBy("_index")->ID();

	request.reset(new TP(DataStructure(msg_size)));
	res = internal_execute_select(request, _space_id, ses);

	if (!this->_add_indices_(res)) {
		LogFL(DEBUG) << "TarantoolInfo::TarantoolInfo(): error with adding indices\n";
		return;
	}
}

TarantoolSpaceInfo *TarantoolInfo::_space_by(const std::string &name)
{
	auto it = spaces_and_names.find(name);
	if (it == spaces_and_names.end()) return nullptr;
	return it->second;
}

TarantoolSpaceInfo *TarantoolInfo::_space_by(int id)
{
	auto it = spaces_and_ids.find(id);
	if (it == spaces_and_ids.end()) return nullptr;
	return it->second;
}

const TarantoolSpaceInfo *TarantoolInfo::SpaceBy(const std::string &name) const
{
	auto it = spaces_and_names.find(name);
	if (it == spaces_and_names.end()) return nullptr;
	return it->second;
}

const TarantoolSpaceInfo *TarantoolInfo::SpaceBy(int id) const
{
	auto it = spaces_and_ids.find(id);
	if (it == spaces_and_ids.end()) return nullptr;
	return it->second;
}

const std::vector<SpacePart> &TarantoolInfo::SpaceFormat(int id) const
{
	return this->SpaceBy(id)->space_parts;
}

const std::vector<SpacePart> &TarantoolInfo::SpaceFormat(const std::string &name) const
{
	return this->SpaceBy(name)->space_parts;
}

void TarantoolInfo::Update()
{
	size_t msg_size = 128;
	size_t msg_max_size = 1048576;

	TP_p request(new TP(DataStructure(msg_size)));
	DataStructure res = internal_execute_select(request, service_space, *session);

	if (!this->_add_spaces_(res)) {
		LogFL(DEBUG) << "TarantoolInfo::Update(): error with adding spaces\n";
		return;
	}

	int tmp = this->SpaceBy("_index")->ID();

	request.reset(new TP(DataStructure(msg_size)));
	res = internal_execute_select(request, tmp, *session);

	if (!this->_add_indices_(res)) {
		LogFL(DEBUG) << "TarantoolInfo::Update(): error with adding indices\n";
		return;
	}
}

SchemaKey::SchemaKey() : tp(FT_OTHER) { }

SchemaKey::SchemaKey(uint32_t val)
{
	SetUInt32Val(val);
}

SchemaKey::SchemaKey(const std::string &val)
{
	SetStrVal(val);
}

uint32_t SchemaKey::GetUInt32Val() const
{
	if (tp != FieldType::FT_NUM) {
		LogFL(DEBUG) << "SchemaKey::GetIntVal(): typeof key not equal to FT_NUM\n";
		throw MakeDbgStr("SchemaKey::GetIntVal(): typeof key not equal to FT_NUM");
	}
	uint32_t res;
	std::memcpy(&res, _data.data(), sizeof(uint32_t));
	return res;
}

std::string SchemaKey::GetStrVal() const
{
	if (tp != FieldType::FT_STR) {
		LogFL(DEBUG) << "SchemaKey::GetIntVal(): typeof key not equal to FT_STR\n";
		MakeDbgStr("SchemaKey::GetIntVal(): typeof key not equal to FT_STR");
	}
	return std::string(_data.data());
}

void SchemaKey::SetUInt32Val(uint32_t val)
{
	tp = FieldType::FT_NUM;
	const char *tmp = (const char *)(&val);
	_data.clear();
	for (size_t i = 0; i < sizeof(uint32_t); ++i) _data.push_back(tmp[i]);
}

void SchemaKey::SetStrVal(const std::string &val)
{
	tp = FieldType::FT_STR;
	_data.clear();
	_data.insert(_data.begin(), val.begin(), val.end());
}

bool operator<(const SchemaKey &left, const SchemaKey &right)
{
	size_t left_sz = left.size();
	size_t right_sz = right.size();
	size_t min_size = std::min(left_sz, right_sz);
	int res = std::memcmp(left.data(), right.data(), min_size);
	if (res < 0) return true;
	if (res > 0) return false;
	return left_sz < right_sz;
}

bool operator<=(const SchemaKey &left, const SchemaKey &right)
{
	return (left < right) || (left == right);
}

bool operator>(const SchemaKey &left, const SchemaKey &right)
{
	return !(left <= right);
}

bool operator>=(const SchemaKey &left, const SchemaKey &right)
{
	return !(left < right);
}

bool operator==(const SchemaKey &left, const SchemaKey &right)
{
	size_t left_sz = left.size();
	return (left_sz == right.size()) && (std::memcmp(left.data(), right.data(), left_sz) == 0);
}

bool operator!=(const SchemaKey &left, const SchemaKey &right)
{
	return !(left == right);
}

std::ostream &operator<<(std::ostream &stream, const SchemaKey &key)
{
	stream << "SchemaKey: type = " << key.GetType() << std::endl << "value: ";
	switch(key.GetType()) {
		case FT_NUM:
			stream << key.GetUInt32Val();
			break;
		case FT_STR:
			stream << key.GetStrVal();
			break;
		case FT_OTHER:
			stream << "ft_other";
			break;
		default:
			stream << "default";
			break;
	}
	stream << std::endl;
	return stream;
}

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   V A L U E~~~~~~~~~~~~~~~~~~~~~~~~

std::ostream &operator<<(std::ostream &stream, const SchemaValue &val)
{
	stream << "SchemaValue: number = " << val.number << std::endl;
	stream << "name = " << val.name << std::endl;
	return stream; 
}

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   F I E L D   V A L U E   :   SchemaValue~~~~~~~~~~~~~~~~~~~~~~~~

std::ostream &operator<<(std::ostream &stream, const SchemaFieldValue &val)
{
	stream << "SchemaFieldValue: number = " << val.number << std::endl;
	stream << "name = " << val.name << std::endl;
	stream << "type = " << val.field_type << std::endl;
	return stream; 
}

std::ostream &operator<<(std::ostream &stream, const std::vector<SchemaFieldValue> &vec)
{
	stream << "vector<SchemaFieldValue>: " << std::endl;
	if (vec.size() == 0) stream << "empty" << std::endl;
	else for (auto it = vec.begin(); it != vec.end(); ++it) {
		stream << *it << std::endl;
	}
	return stream;
}

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   I N D E X   V A L U E   :   SchemaValue~~~~~~~~~~~~~~~~~~~~~~~~

std::ostream &operator<<(std::ostream &stream, const SchemaIndexValue &val)
{
	stream << "SchemaIndexValue: number = " << val.number << std::endl;
	stream << "name = " << val.name << std::endl;
	stream << "key = " << val.key << std::endl;
	stream << "index_parts: " << val.index_parts;
	return stream; 
}

std::ostream &operator<<(std::ostream &stream, const std::map<SchemaKey, SchemaIndexValue> &val)
{
	stream << "map<SchemaKey, SchemaIndexValue>: " << std::endl;
	if (val.size() == 0) stream << "empty" << std::endl;
	else for (auto it = val.begin(); it != val.end(); ++it) {
		stream << it->first << std::endl << it->second << std::endl;
	}
	return stream;
}

//~~~~~~~~~~~~~~~~~~~~~~~~S C H E M A   S P A C E   V A L U E   :   SchemaValue~~~~~~~~~~~~~~~~~~~~~~~~

std::ostream &operator<<(std::ostream &stream, const SchemaSpaceValue &val)
{
	stream << "SchemaSpaceValue: number = " << val.number << std::endl;
	stream << "name = " << val.name << std::endl;
	stream << "key = " << val.key << std::endl;
	stream << "schema_list: " << val.schema_list;
	stream << "indices: " << val.indices;
	return stream; 
}

//~~~~~~~~~~~~~~~~~~~~~~~~T A R A N T O O L   S C H E M A~~~~~~~~~~~~~~~~~~~~~~~~

TarantoolSchema::TarantoolSchema(Session &ses, int number)
{
	//------------------------ R E Q U E S T ------------------------

	TP_p request(new TP(DataStructure(1024)));
	request->AddSelect(number, 0, 0, TP_ITERATOR_ALL, UINT32_MAX);
	request->ReserveKeyParts();
	if (request->Used() > 1024) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): used more than was allocated;\n";
		return;
	}

	int ss = ses.Send(*request);
	if (ss < 0) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): error while sending request was occured, return = "<< ss << "\n";
		return;
	}

	ss = ses.Sync();
	if (ss < 0) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): error while syncing sesson was occured, return = " << ss << "\n";
		return;
	}

	//------------------------ R E P L Y ------------------------

	DataStructure response_buffer(FIRST_PACK_PART_SIZE);
	ssize_t len = ses.Receive(response_buffer);
	ses.Sync();
	if (len < FIRST_PACK_PART_SIZE) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): length of receive less than 5 = " << len << "\n";
		return;
	}
	if (mp_typeof(*response_buffer.Data()) != MP_UINT) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): bad reply length type = "  << mp_typeof(*response_buffer.Data()) << "\n";
		return;
	}
	uint32_t body = tp_get_uint(response_buffer.Data());

	response_buffer.Resize(FIRST_PACK_PART_SIZE + body);
	len = ses.Receive(response_buffer, FIRST_PACK_PART_SIZE);
	ses.Sync();
	if (len < body) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): receive failed, len = " << len << "\n";
		return;
	}
	TPResponse resp(response_buffer);
	if (resp.GetState() == -1) {
		LogFL(DEBUG) <<"TarantoolSchema::TarantoolSchema(): failed to parse response\n";
		return;
	}
	if (resp.GetCode() != 0) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): server respond: " << resp.GetCode() << ", " << resp.GetError() << "\n";
	}
	try {
		this->AddSpaces(resp.GetData());
	} catch(std::string msg)
	{
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): error: " << msg << "\n";
	}

	//------------------------ R E Q U E S T ------------------------

	request.reset(new TP(DataStructure(1024)));
	number = this->GetSpaceIDByString("_index");
	request->AddSelect(number, 0, 0, TP_ITERATOR_ALL, UINT32_MAX);
	request->ReserveKeyParts();
	if (request->Used() > 1024) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): used more than was allocated;\n";
		return;
	}

	ss = ses.Send(*request);
	if (ss < 0) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): error while sending request was occured, return = "<< ss << "\n";
		return;
	}

	ss = ses.Sync();
	if (ss < 0) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): error while syncing sesson was occured, return = " << ss << "\n";
		return;
	}

	//------------------------ R E P L Y ------------------------

	response_buffer = DataStructure(FIRST_PACK_PART_SIZE);
	len = ses.Receive(response_buffer);
	ses.Sync();
	if (len < FIRST_PACK_PART_SIZE) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): length of receive less than 5 = " << len << "\n";
		return;
	}
	if (mp_typeof(*response_buffer.Data()) != MP_UINT) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): bad reply length type = "  << mp_typeof(*response_buffer.Data()) << "\n";
		return;
	}
	body = tp_get_uint(response_buffer.Data());

	response_buffer.Resize(FIRST_PACK_PART_SIZE + body);
	len = ses.Receive(response_buffer, FIRST_PACK_PART_SIZE);
	ses.Sync();
	if (len < body) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): receive failed, len = " << len << "\n";
		return;
	}
	resp = TPResponse(response_buffer);
	if (resp.GetState() == -1) {
		LogFL(DEBUG) <<"TarantoolSchema::TarantoolSchema(): failed to parse response\n";
		return;
	}
	if (resp.GetCode() != 0) {
		LogFL(DEBUG) << "TarantoolSchema::TarantoolSchema(): server respond: " << resp.GetCode() << ", " << resp.GetError() << "\n";
	}
	

	MValue indices = MValue::FromMSGPack(resp.GetData());

	this->AddIndices(indices);
}

//~~~~Set methods

int TarantoolSchema::AddSpaces(const DataStructure &data)
{
	const char *tuple = data.Data();
	if (mp_check(&tuple, tuple + data.Size()))
		return -1;
	tuple = data.Data();
	if (mp_typeof(*tuple) != MP_ARRAY)
		return -1;
	uint32_t space_count = mp_decode_array(&tuple);
	while (space_count-- > 0) {
		if (this->add_space(&tuple))
			return -1;
	}
	return 0;
}

int TarantoolSchema::AddIndices(const MValue &data)
{
	const MValueVector &indices = data.GetArray();
	for (size_t i = 0, size = indices.size(); i < size; ++i) {
		uint32_t space_number = indices[i][0].GetUint();

		SchemaKey space_key(space_number);
		if (spaces.find(space_key) == spaces.end()) return -1;
		SchemaSpaceValue &space = spaces[space_key];

		SchemaIndexValue index_string;
		index_string.number = indices[i][1].GetUint();
		index_string.name = indices[i][2].GetStr();
		size_t part_count = indices[i][5].GetUint();
		for (size_t j = 0; j < part_count * 2; j += 2) {
			SchemaFieldValue val;
			val.number = indices[i][6 + j].GetUint();
			val.field_type = ((indices[i][6 + j + 1].GetStr()[0] == 'S') ? FT_STR : FT_NUM);
			index_string.index_parts.push_back(val);
		}

		SchemaIndexValue index_number = index_string;
		index_string.key.SetStrVal(index_string.name);
		index_number.key.SetUInt32Val(index_number.number);
		space.indices.insert(std::make_pair(index_string.key, index_string));
		space.indices.insert(std::make_pair(index_number.key, index_number));
	}
	return 0;
}

//~~~~Get methods~~~~

int TarantoolSchema::GetSpaceIDByString(const std::string &space_name)
{
	SchemaKey space_key(space_name);
	if (spaces.find(space_key) == spaces.end()) return -1;
	return spaces[space_key].number;
}

int TarantoolSchema::GetIndexIDByString(int space_id, const std::string &index_name)
{
	SchemaKey space_key(space_id);
	if (spaces.find(space_key) == spaces.end()) return -1;
	SchemaSpaceValue space = spaces[space_key];

	SchemaKey index_key(index_name);
	if (space.indices.find(index_key) == space.indices.end()) return -1;
	return space.indices[index_key].number;
}

int TarantoolSchema::GetColNumberByName(int space_id, const std::string &name) const
{
	SchemaKey space_key(space_id);
	auto it = spaces.find(space_key);
	if (it == spaces.end()) return -1;
	SchemaSpaceValue space = it->second;

	for (size_t i = 0, size = space.schema_list.size(); i < size; ++i) {
		if (space.schema_list[i].name == name) return i;
	}
	return -1;
}

std::vector<std::pair<std::string, FieldType> > TarantoolSchema::GetSpaceFormat(int space_id) const
{
	SchemaKey space_key(space_id);
	auto it = spaces.find(space_key);
	if (it == spaces.end()) return std::vector<std::pair<std::string, FieldType> >();
	SchemaSpaceValue space = it->second;

	std::vector<std::pair<std::string, FieldType> > res;
	for (size_t i = 0, size = space.schema_list.size(); i < size; ++i) {
		res.push_back(std::make_pair(space.schema_list[i].name, space.schema_list[i].field_type));
	}
	return res;
}

//vector of indices. First in pair - index name, second - index columns
std::vector<std::pair<std::string, std::vector<std::string> > > TarantoolSchema::GetIndexFormat(int space_id) const
{
	std::vector<std::pair<std::string, std::vector<std::string> > > res;
	SchemaKey space_key(space_id);
	auto it = spaces.find(space_key);
	if (it == spaces.end()) return res;
	SchemaSpaceValue space = it->second;

	for (auto it = space.indices.begin(); it != space.indices.end(); ++it) {
		const SchemaIndexValue &index = it->second;
		std::string index_name = it->second.name;
		std::vector<std::string> index_parts;
		for (size_t i = 0, size = index.index_parts.size(); i < size; ++i) {
			index_parts.push_back(this->GetNameByColNumber(space_id, index.index_parts[i].number));
		}
		res.push_back(std::make_pair(index_name, index_parts));
	}
	return res;
}

std::vector<std::string> TarantoolSchema::GetIndexFormat(int space_id, const std::string &index_name) const
{
	auto index_format = this->GetIndexFormat(space_id);
	for (size_t i = 0, size = index_format.size(); i < size; ++i) {
		if (index_format[i].first == index_name) return index_format[i].second;
	}
	return std::vector<std::string>();
}

std::string TarantoolSchema::GetNameByColNumber(int space_id, int col_number) const
{
	SchemaKey space_key(space_id);
	auto it = spaces.find(space_key);
	if (it == spaces.end()) return std::string();
	SchemaSpaceValue space = it->second;

	if ((col_number < 0) || (col_number >= space.schema_list.size())) return std::string();
	return space.schema_list[col_number].name;
}

//~~~~Other~~~~

void TarantoolSchema::Flush()
{
	spaces.clear();
	return;
}

std::string MpDecodeStr(const char **p)
{
	uint32_t len = 0;
	const char *tmp = mp_decode_str(p, &len);
	return std::string(tmp, len);
}

int TarantoolSchema::add_space(const char **data)
{
	const char *tuple = *data;
	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "TarantoolSchema::add_space(): typeof raw tuple not equal to MP_ARRAY, but equal = " << mp_typeof(*tuple) << "\n";
		throw MakeDbgStr("TarantoolSchema::add_space(): typeof raw tuple not equal to MP_ARRAY, but equal = " + std::to_string(mp_typeof(*tuple)));
	}

	uint32_t tuple_len = mp_decode_array(&tuple);
	if (tuple_len < TUPLE_LEN) {
		LogFL(DEBUG) << "TarantoolSchema::add_space(): tuple_len not equal to " << TUPLE_LEN << " but equal = " << tuple_len << "\n";
		throw MakeDbgStr("TarantoolSchema::add_space(): tuple_len not equal to " + std::to_string(TUPLE_LEN) + " but equal = " + std::to_string(tuple_len));
	}
	if (mp_typeof(*tuple) != MP_UINT) {
		LogFL(DEBUG) << "TarantoolSchema::add_space(): typeof decoded tuple not equal to MP_UINT\n";
		throw MakeDbgStr("TarantoolSchema::add_space(): typeof decoded tuple not equal to MP_UINT");
	}

	SchemaSpaceValue space_string;
	space_string.number = mp_decode_uint(&tuple);

	/* skip owner id */
	mp_next(&tuple);

	if (mp_typeof(*tuple) != MP_STR) {
		LogFL(DEBUG) << "TarantoolSchema::add_space(): typeof second field in tuple not equal to MP_STR\n";
		throw MakeDbgStr("TarantoolSchema::add_space(): typeof second field in tuple not equal to MP_STR");
	}
	space_string.name = MpDecodeStr(&tuple);
	/* skip engine name */
	mp_next(&tuple);
	/* skip field count */
	mp_next(&tuple);
	/* skip format */
	mp_next(&tuple);
	/* parse format */

	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "TarantoolSchema::add_space(): typeof field in tuple not equal to MP_ARRAY\n";
		throw MakeDbgStr("TarantoolSchema::add_space(): typeof field in tuple not equal to MP_ARRAY");
	}
	uint32_t fmt_len = mp_decode_array(&tuple);

	while (fmt_len-- > 0) {
		if (mp_typeof(*tuple) != MP_MAP) {
			LogFL(DEBUG) << "TarantoolSchema::add_space(): typeof field in tuple not equal to MP_MAP. fmt_len = " << fmt_len << "\n";
			throw MakeDbgStr("TarantoolSchema::add_space(): typeof field in tuple not equal to MP_MAP. fmt_len = " + std::to_string(fmt_len));
		}
		uint32_t arrsz = mp_decode_map(&tuple);
		SchemaFieldValue val;

		while (arrsz-- > 0) {
			if (mp_typeof(*tuple) != MP_STR) {
				LogFL(DEBUG) << "TarantoolSchema::add_space(): typeof field in tuple not equal to MP_STR. fmt_len = " << fmt_len << ", arrsz = " << arrsz << "\n";
				throw MakeDbgStr("TarantoolSchema::add_space(): typeof field in tuple not equal to MP_STR. fmt_len = " + std::to_string(fmt_len) + ", arrsz = " + std::to_string(arrsz));
			}
			std::string sfield = MpDecodeStr(&tuple);
			if (sfield == "name") {
				if (mp_typeof(*tuple) != MP_STR) {
					LogFL(DEBUG) << "TarantoolSchema::add_space(): typeof field in tuple not equal to MP_STR. fmt_len = " << fmt_len << ", arrsz = " << arrsz << "\n";
					throw MakeDbgStr("TarantoolSchema::add_space(): typeof field in tuple not equal to MP_STR. fmt_len = " + std::to_string(fmt_len) + ", arrsz = " + std::to_string(arrsz));
				}
				val.name = MpDecodeStr(&tuple);
			} else if (sfield == "type") {
				if (mp_typeof(*tuple) != MP_STR) {
					LogFL(DEBUG) << "TarantoolSchema::add_space(): typeof field in tuple not equal to MP_STR. fmt_len = " << fmt_len << ", arrsz = " << arrsz << "\n";
					throw MakeDbgStr("TarantoolSchema::add_space(): typeof field in tuple not equal to MP_STR. fmt_len = " + std::to_string(fmt_len) + ", arrsz = " + std::to_string(arrsz));
				}
				sfield = MpDecodeStr(&tuple);
				switch(std::tolower(sfield[0])) {
					case ('s'):
						val.field_type = FieldType::FT_STR;
						break;
					case ('n'):
						val.field_type = FieldType::FT_NUM;
						break;
					default:
						val.field_type = FieldType::FT_OTHER;
				}
			}
		}
		space_string.schema_list.push_back(val);
	}

	SchemaSpaceValue space_number = space_string;
	space_string.key.SetStrVal(space_string.name);
	space_number.key.SetUInt32Val(space_number.number);

	spaces.insert(std::make_pair(space_string.key, space_string));
	spaces.insert(std::make_pair(space_number.key, space_number));
	*data = tuple;
	return 0;
}

int TarantoolSchema::add_index(const char **data)
{
	const char *tuple = *data;
	if (mp_typeof(*tuple) != MP_ARRAY) {
		LogFL(DEBUG) << "TarantoolSchema::add_index(): typeof raw tuple not equal to MP_ARRAY\n";
		throw MakeDbgStr("TarantoolSchema::add_index(): typeof raw tuple not equal to MP_ARRAY");
	}

	int64_t tuple_len = mp_decode_array(&tuple);
	if (tuple_len < TUPLE_LEN) {
		LogFL(DEBUG) << "TarantoolSchema::add_index(): tuple_len not equal to " << TUPLE_LEN << "\n";
		throw MakeDbgStr("TarantoolSchema::add_index(): tuple_len not equal to " + std::to_string(TUPLE_LEN));
	}
	if (mp_typeof(*tuple) != MP_UINT) {
		LogFL(DEBUG) << "TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_UINT\n";
		throw MakeDbgStr("TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_UINT");
	}

	uint32_t space_number = mp_decode_uint(&tuple);
	SchemaKey space_key(space_number);
	if (spaces.find(space_key) == spaces.end()) return -1;
	SchemaSpaceValue space = spaces[space_key];

	SchemaIndexValue index_string;
	if (mp_typeof(*tuple) != MP_UINT) {
		LogFL(DEBUG) << "TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_UINT\n";
		throw MakeDbgStr("TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_UINT");
	}
	index_string.number = mp_decode_uint(&tuple);
	if (mp_typeof(*tuple) != MP_STR) {
		LogFL(DEBUG) << "TarantoolSchema::add_index(): typeof field in tuple not equal to MP_STR\n";
		throw MakeDbgStr("TarantoolSchema::add_index(): typeof field in tuple not equal to MP_STR");
	}
	{
		uint32_t tmp;
		index_string.name = mp_decode_str(&tuple, &tmp);
	}
	/* skip index type */
	mp_next(&tuple);
	/* skip unique flag */
	mp_next(&tuple);
	uint32_t part_count = mp_decode_uint(&tuple);
	if (mp_typeof(*tuple) != MP_UINT) {
		LogFL(DEBUG) << "TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_UINT\n";
		throw MakeDbgStr("TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_UINT");
	}

	if (tuple_len - part_count * 2 != TUPLE_LEN) {
		LogFL(DEBUG) << "TarantoolSchema::add_index(): tuple_len - part_count * 2 != " << TUPLE_LEN << "\n";
		throw MakeDbgStr("TarantoolSchema::add_index(): tuple_len - part_count * 2 != " + std::to_string(TUPLE_LEN));
	}
	while (part_count--) {
		if (mp_typeof(*tuple) != MP_UINT) {
			LogFL(DEBUG) << "TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_UINT, part_count = " << part_count << "\n";
			throw MakeDbgStr("TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_UINT, part_count = " + std::to_string(part_count));
		}
		SchemaFieldValue val;
		val.number = mp_decode_uint(&tuple);
		if (mp_typeof(*tuple) != MP_STR) {
			LogFL(DEBUG) << "TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_STR, part_count = " << part_count << "\n";
			throw MakeDbgStr("TarantoolSchema::add_index(): typeof decoded tuple not equal to MP_STR, part_count = " + std::to_string(part_count));
		}
		{
			uint32_t sfield_len = 0;
			std::string sfield(mp_decode_str(&tuple, &sfield_len));
			switch(std::tolower(sfield[0])) {
				case ('s'):
					val.field_type = FT_STR;
					break;
				case ('n'):
					val.field_type = FT_NUM;
					break;
				default:
					val.field_type = FT_OTHER;
			}
		}
		index_string.index_parts.push_back(val);
	}

	SchemaIndexValue index_number = index_string;
	index_string.key.SetStrVal(index_string.name);
	index_number.key.SetUInt32Val(index_number.number);
	space.indices.insert(std::make_pair(index_string.key, index_string));
	space.indices.insert(std::make_pair(index_number.key, index_number));
	*data = tuple;
	return 0;
}

std::ostream &operator<<(std::ostream &stream, const TarantoolSchema &schm)
{
	stream << "TarantoolSchema: " << std::endl;
	for (auto it = schm.spaces.begin(); it != schm.spaces.end(); ++it) {
		stream << it->first << std::endl << it->second << std::endl;
	}
	return stream;
}