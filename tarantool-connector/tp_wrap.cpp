#include "tp_wrap.h"

//~~~~~~~~~~~~~~~~~~~~~~~~ S E S S I O N ~~~~~~~~~~~~~~~~~~~~~~~~

Session::Session()  : connect_state(-1)
{
	tb_sesinit(&base_ses);
}

Session::Session(int port)
{
	tb_sesinit(&base_ses);
	tb_sesset(&base_ses, TB_PORT, port);
}

Session::Session(int port, const std::string &address)
{
	tb_sesinit(&base_ses);
	tb_sesset(&base_ses, TB_PORT, port);
	tb_sesset(&base_ses, TB_HOST, address.c_str());
}

int Session::Connect()
{
	connect_state = tb_sesconnect(&base_ses);
	return connect_state;
}

int Session::GetConnectState() const
{
	return connect_state;
}

int Session::GetErrorCode() const
{
	return base_ses.errno_;
}

struct tbses &Session::GetSessionObject()
{
	return base_ses;
}

int Session::Sync()
{
	return tb_sessync(&base_ses);
}

ssize_t Session::Send(DataStructure &data)
{
	return tb_sessend(&base_ses, data.Data(), data.Size());
}

ssize_t Session::Send(TP &data)
{
	if (!data.IsValid()) return 0;
	return tb_sessend(&base_ses, data.GetBuf(), data.Used());
}

TPResponse Session::SendRequest(TP_p &request)
{
	int ss = this->Send(*request);
	if (ss < 0) {
		LogFL(DEBUG) << "Session::SendRequest(): error while sending request was occured, return = " << ss << "\n";
		return TPResponse();
	}
	ss = this->Sync();
	if (ss < 0) {
		LogFL(DEBUG) << "Session::SendRequest(): error while syncing sesson was occured, return = " << ss << "\n";
		return TPResponse();
	}

	DataStructure response_buffer(MSG_HEADER_SIZE);
	int len = this->Receive(response_buffer);
	if (len < MSG_HEADER_SIZE) {
		LogFL(DEBUG) << "Session::SendRequest(): length of receive less than header size = " << len << "\n";
		return TPResponse();
	}
	if (mp_typeof(*response_buffer.Data()) != MP_UINT) {
		LogFL(DEBUG) << "Session::SendRequest(): bad reply length type = " << ((int)(mp_typeof(*response_buffer.Data()))) << "\n";
		return TPResponse();
	}
	int body = tp_get_uint(response_buffer.Data());
	response_buffer.Resize(MSG_HEADER_SIZE + body);
	len = this->Receive(response_buffer, MSG_HEADER_SIZE);
	if (len < body) {
		LogFL(DEBUG) << "Session::SendRequest(): receiving body failed, len = " << len << "\n";
		return TPResponse();
	}
	return TPResponse(response_buffer);
}

ssize_t Session::Receive(DataStructure &data, int from, int strict)
{
	return tb_sesrecv(&base_ses, data.Data() + from, data.Size() - from, strict);
}

int Session::SessionSet(enum tbsesopt o, ...)
{
	va_list args;
	va_start(args, o);
	switch (o) {
	case TB_HOST: {
		char *p = strdup(va_arg(args, char*));
		if (p == NULL) {
			va_end(args);
			base_ses.errno_ = ENOMEM;
			return -1;
		}
		free(base_ses.host);
		base_ses.host = p;
		break;
	}
	case TB_PORT:
		base_ses.port = va_arg(args, int);
		break;
	case TB_CONNECTTM:
		base_ses.tmc.tv_sec  = va_arg(args, int);
		base_ses.tmc.tv_usec = 0;
		break;
	case TB_SENDBUF:
		base_ses.sbuf = va_arg(args, int);
		break;
	case TB_READBUF:
		base_ses.rbuf = va_arg(args, int);
		break;
	default:
		va_end(args);
		base_ses.errno_ = EINVAL;
		return -1;
	}
	va_end(args);
	return 0;
}

Session::~Session()
{
	tb_sesfree(&base_ses);
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T P ~~~~~~~~~~~~~~~~~~~~~~~~

char *_reserve_(struct tp *p, size_t req, size_t *size)
{
	DataStructure *tmp = reinterpret_cast<DataStructure *>(p->obj);
	*size = tp_size(p) * 2;
	while (*size < tp_unused(p) + req) {
		*size *= 2;
	}

	tmp->Resize(*size);
	return tmp->Data();
}

TP::TP(const DataStructure &buf, tp_reserve reserve, void *obj) : source(buf), is_valid(true)
{
	tp_init(&base_tp, source.Data(), source.Size(), _reserve_, (&source));
}

TP::TP() : is_valid(false) { }

char *TP::GetBuf()
{	
	if (!is_valid) return NULL;
	return tp_buf(&base_tp);
}

size_t TP::Size()
{
	if (!is_valid) return 0;
	return tp_size(&base_tp);
}

size_t TP::Used()
{
	if (!is_valid) return 0;
	return tp_used(&base_tp);
}

size_t TP::Unused()
{
	if (!is_valid) return 0;
	return tp_unused(&base_tp);
}

bool TP::IsValid() const
{
	return is_valid;
}

void TP::Auth(const char *salt_base64, const std::string &user, const std::string &pass)
{
	if (!is_valid) return;
	tp_auth(&base_tp, salt_base64, user.data(), user.size(), pass.data(), pass.size());
}

//~~~~~~~~ R e q u e s t s ~~~~~~~~

void TP::AddPing()
{
	if (!is_valid) return;
	tp_ping(&base_tp);
}

void TP::AddSelect(uint32_t space, uint32_t index, uint32_t offset, enum tp_iterator_type iter, uint32_t limit)
{
	if (!is_valid) return;
	tp_select(&base_tp, space, index, offset, iter, limit);
	// if (this->Used() > source.Size()) {

	// }
}

void TP::AddInsert(uint32_t space)
{
	if (!is_valid) return;
	tp_insert(&base_tp, space);
}

void TP::AddReplace(uint32_t space)
{
	if (!is_valid) return;
	tp_replace(&base_tp, space);
}

void TP::AddDelete(uint32_t space)
{
	if (!is_valid) return;
	tp_delete(&base_tp, space);
}

void TP::AddUpdate(uint32_t space)
{
	if (!is_valid) return;
	tp_update(&base_tp, space);
}

void TP::ReserveUpdateOperations(uint32_t op_count)
{
	if (!is_valid) return;
	tp_updatebegin(&base_tp, op_count);
}

void TP::AddUpdateOperation(char op, uint32_t field_no)
{
	if (!is_valid) return;
	tp_op(&base_tp, op, field_no);
}

void TP::AddCall(const std::string &function)
{
	if (!is_valid) return;
	tp_call(&base_tp, function.data(), function.size());
}

void TP::AddEval(const std::string &function)
{
	if (!is_valid) return;
	tp_eval(&base_tp, function.data(), function.size());
	ReserveArray(0);
}

void TP::AddFormat(const char *format, ...)
{
	if (!is_valid) return;
	va_list args;
	va_start(args, format);
	size_t unused = this->Unused();
	size_t sz = mp_vformat(base_tp.p, unused, format, args);
	if (sz > unused) {
		if (tpunlikely(tp_ensure(&base_tp, sz) == -1)) {
			va_end(args);
			return;
		}
		mp_vformat(base_tp.p, unused, format, args);
	}
	va_end(args);
	tp_add(&base_tp, sz);
}

void TP::ReserveTupleFields(uint32_t field_count)
{
	if (!is_valid) return;
	tp_tuple(&base_tp, field_count);
}

void TP::ReserveKeyParts(uint32_t part_count)
{
	if (!is_valid) return;
	tp_key(&base_tp, part_count);

}

void TP::AddUint(uint64_t num)
{
	if (!is_valid) return;
	tp_encode_uint(&base_tp, num);
}

void TP::AddInt(int64_t num)
{
	if (!is_valid) return;
	if (num < 0) tp_encode_int(&base_tp, num);
	else tp_encode_uint(&base_tp, (uint64_t)(num));
}

void TP::AddString(const std::string &str)
{
	if (!is_valid) return;
	tp_encode_str(&base_tp, str.data(), str.size());
}

void TP::AddNil()
{
	if (!is_valid) return;
	tp_encode_nil(&base_tp);
}

void TP::AddBinary(const DataStructure &data)
{
	if (!is_valid) return;
	tp_encode_bin(&base_tp, data.Data(), data.Size());
}

void TP::ReserveArray(uint32_t size)
{
	if (!is_valid) return;
	tp_encode_array(&base_tp, size);
}

void TP::ReserveMap(uint32_t size)
{
	if (!is_valid) return;
	tp_encode_map(&base_tp, size);
}

void TP::AddBool(bool val)
{
	if (!is_valid) return;
	tp_encode_bool(&base_tp, val);
}

void TP::AddFloat(float num)
{
	if (!is_valid) return;
	tp_encode_float(&base_tp, num);
}

void TP::AddDouble(double num)
{
	if (!is_valid) return;
	tp_encode_double(&base_tp, num);
}

void TP::AddMValue(const MValue &val)
{
	switch(val.GetType()) {
		case TP_NIL: {
			this->AddNil();
			return;
		}
		case TP_UINT: {
			this->AddUint(val.GetUint());
			return;
		}
		case TP_INT: {
			this->AddInt(val.GetInt());
			return;
		}
		case TP_STR: {
			this->AddString(val.GetStr());
			return;
		}
		case TP_BIN: {
			this->AddBinary(val.GetBin());
			return;
		}
		case TP_BOOL: {
			this->AddBool(val.GetBool());
			return;
		}
		case TP_FLOAT: {
			this->AddFloat(val.GetFloat());
			return;
		}
		case TP_DOUBLE: {
			this->AddDouble(val.GetDouble());
			return;
		}
		case TP_ARRAY: {
			const MValueVector &arr = val.GetArray();
			this->ReserveArray(arr.size());
			for (size_t i = 0, size = arr.size(); i < size; ++i) {
				this->AddMValue(arr[i]);
			}
			return;
		}
		case TP_MAP: {
			const MValueMap &tmap = val.GetMap();
			this->ReserveMap(tmap.size());
			for (auto it = tmap.begin(); it != tmap.end(); ++it) {
				this->AddMValue(it->first);
				this->AddMValue(it->second);
			}
			return;
		}
		default: return;
	}
}

void TP::SetRequestID(uint32_t reqid)
{
	if (!is_valid) return;
	tp_reqid(&base_tp, reqid);
}

ssize_t TP::EnsureBuffer(size_t size)
{
	if (!is_valid) return 0;
	return tp_ensure(&base_tp, size);
}

void TP::AddCreateSpace(const std::string &name)
{
	if (!is_valid) return;
	std::string function("return ({box.schema.space.create(\"" + name + "\")})[2]");
	this->AddEval(function);
}

void TP::AddCreateIndex(const std::string &space_name, const std::string &index_name, const MValueMap &options)
{
	if (!is_valid) return;
	std::stringstream function;
	function << "return (box.space." << space_name << ":create_index(";
	function << "'" << index_name << "', {";
	auto it = options.find(MValue("type"));
	if (it != options.end()) {
		if (it->second.GetType() != TP_STR) {
			LogFL(DEBUG) << "TP::AddCreateIndex(): error with creating index\n";
			return;
		}
		function << "type = '" << it->second.GetStr() << "', ";
	} else {
		function << "type = 'TREE', ";
	}

	it = options.find(MValue("id"));
	if (it != options.end()) {
		if ((!(it->second.IsNumber())) || (it->second.GetType() == TP_FLOAT) || (it->second.GetType() == TP_DOUBLE)) {
			LogFL(DEBUG) << "TP::AddCreateIndex(): error with creating index\n";
			return;
		}
		function << "id = " << (it->second) << ", ";
	}
	it = options.find(MValue("unique"));
	if (it != options.end()) {
		if (it->second.GetType() != TP_BOOL) {
			LogFL(DEBUG) << "TP::AddCreateIndex(): error with creating index\n";
			return;
		}
		function << "unique = ";
		if (it->second.GetBool()) function << "true, ";
		else function << "false, ";
	}
	it = options.find(MValue("if_not_exists"));
	if (it != options.end()) {
		if (it->second.GetType() != TP_BOOL) {
			LogFL(DEBUG) << "TP::AddCreateIndex(): error with creating index\n";
			return;
		}
		function << "if_not_exists = ";
		if (it->second.GetBool()) function << "true, ";
		else function << "false, ";
	}
	it = options.find(MValue("parts"));
	if (it != options.end()) {
		if (it->second.GetType() != TP_ARRAY) {
			LogFL(DEBUG) << "TP::AddCreateIndex(): error with creating index\n";
			return;
		}
		function << "parts = {";
		const MValueVector &tmp = it->second.GetArray();
		for (int i = 0, size = static_cast<int>(tmp.size()); i < size; ++i) {
			if (tmp[i].IsNumber()) {
				function << tmp[i] << ", ";
			} else if (tmp[i].IsString()) {
				function << "'" << tmp[i].GetStr() << "', ";
			} else {
				LogFL(DEBUG) << "TP::AddCreateIndex(): error with creating index\n";
				return;
			}
		}
		function << "}, ";
	}
	function << "}))";
	this->AddEval(function.str());
}

void TP::AddSpaceFormat(const std::string &space_name, const std::vector<std::pair<std::string, std::string> > &cols)
{
	std::stringstream function;

	function << "format = {}\n";
	for (int i = 0, size = static_cast<int>(cols.size()); i < size; ++i) {
		function << "format[" << i + 1 << "] = {name='" << cols[i].first << "', type='" << cols[i].second << "'}\n"; 
	}
	function << "return (box.space." << space_name << ":format(format))";
	this->AddEval(function.str());
}

void TP::AddDropSpace(const std::string &name)
{
	std::string function;
	function = "return (box.space." + name + ":drop{})";
	this->AddEval(function);
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T P   R E S P O N S E ~~~~~~~~~~~~~~~~~~~~~~~~

TPResponse::TPResponse(const DataStructure &buf) : base_response(new struct tpresponse()), source(buf)
{
	state = tp_reply(base_response.get(), source.Data(), source.Size());
}

TPResponse::TPResponse() : base_response(new struct tpresponse()), source(DataStructure()) { }

uint32_t TPResponse::GetRequestID()
{
	return tp_getreqid(base_response.get());
}

uint32_t TPResponse::GetTupleCount() const
{
	return tp_tuplecount(base_response.get());
}

int64_t TPResponse::GetState() const
{
	return state;
}

uint32_t TPResponse::GetCode() const
{
	return base_response->code;
}

std::string TPResponse::GetError() const
{
	return std::string(base_response->error, static_cast<int>(base_response->error_end - base_response->error));
}

DataStructure TPResponse::GetData() const
{
	return DataStructure(base_response->data, static_cast<int>(base_response->data_end - base_response->data));
}

const char *TPResponse::GetCurrentTuple()
{
	return tp_gettuple(base_response.get());
}

uint32_t TPResponse::GetCurrentTupleSize()
{
	return tp_nextfield(base_response.get());
}

const char *TPResponse::GetCurrentField()
{
	return tp_getfield(base_response.get());
}

void TPResponse::ToBeginOfTuple()
{
	tp_rewindfield(base_response.get());
}

bool TPResponse::HasNextField()
{
	return static_cast<bool>(tp_hasnextfield(base_response.get()));
}

uint32_t TPResponse::GetCurrentFieldSize()
{
	return tp_getfieldsize(base_response.get());
}

bool TPResponse::HasData()
{
	return static_cast<bool>(tp_hasdata(base_response.get()));
}

bool TPResponse::HasNextTuple()
{
	return static_cast<bool>(tp_hasnext(base_response.get()));
}

void TPResponse::NextTuple()
{
	tp_next(base_response.get());
}

void TPResponse::ToBeginOfTuples()
{
	tp_rewind(base_response.get());
}

TPResponse &TPResponse::operator++()
{
	this->NextTuple();
	return *this;
}

//~~~~~~~~~~~~~~~~~~~~~~~~ T P   G R E E T I N G ~~~~~~~~~~~~~~~~~~~~~~~~

ssize_t TPGreeting::SetFromBuf(char *buf, size_t size)
{
	return tp_greeting(&base_greeting, buf, size);
}