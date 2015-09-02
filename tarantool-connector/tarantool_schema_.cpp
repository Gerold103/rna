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

std::vector<TColumn> TarantoolSpaceInfo::ColumnNames() const
{
	std::vector<TColumn> res;
	for (size_t i = 0, size = space_parts.size(); i < size; ++i) {
		res.push_back(TColumn(space_parts[i].name, this->Name()));
	}
	return res;
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