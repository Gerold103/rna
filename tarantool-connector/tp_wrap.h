#ifndef TP_WRAP_H_INCLUDED
#define TP_WRAP_H_INCLUDED

#include <cstddef>
#include <cstdint>
extern "C" {
	#include "session.h"
	#include "tp.h"
}
#include <string>
#include <memory>
#include <sstream>
#include "utils.h"
#include "logger.h"
#include "mvalue.h"

#define MSG_HEADER_SIZE 5
#define TP_p std::shared_ptr<TP>

class TPResponse;

//~~~~~~~~~~~~~~~~~~~~~~~~ T P ~~~~~~~~~~~~~~~~~~~~~~~~

class TP {
private:
	struct tp base_tp;
	DataStructure source;
	bool is_valid;

public:
	TP(const DataStructure &buf, tp_reserve reserve = NULL, void *obj = NULL);

	TP();

	char *GetBuf();
	size_t Size();
	size_t Used();
	size_t Unused();
	bool IsValid() const;

	void Auth(const char *salt_base64, const std::string &user, const std::string &pass);

	//~~~~ Requests ~~~~
	void AddPing();

	void AddSelect(uint32_t space, uint32_t index, uint32_t offset, enum tp_iterator_type iter = TP_ITERATOR_ALL, uint32_t limit = 1);

	void AddInsert(uint32_t space);

	void AddReplace(uint32_t space);

	void AddDelete(uint32_t space);

	void AddUpdate(uint32_t space);
	void ReserveUpdateOperations(uint32_t op_count);
	void AddUpdateOperation(char op, uint32_t field_no);

	void AddCall(const std::string &function);

	void AddEval(const std::string &function);

	void AddFormat(const char *format, ...);

	void ReserveTupleFields(uint32_t field_count);

	void ReserveKeyParts(uint32_t part_count = 0);

	void AddUint(uint64_t num);
	void AddInt(int64_t num);
	void AddString(const std::string &str);
	void AddNil();
	void AddBinary(const DataStructure &data);
	void ReserveArray(uint32_t size);
	void ReserveMap(uint32_t size);
	void AddBool(bool val);
	void AddFloat(float num);
	void AddDouble(double num);
	void AddMValue(const MValue &val);

	void AddValue(uint64_t num) { this->AddUint(num); }
	void AddValue(int64_t num) { this->AddInt(num); }
	void AddValue(const std::string &str) { this->AddString(str); }
	void AddValue() { this->AddNil(); }
	void AddValue(const DataStructure &data) { this->AddBinary(data); }
	void AddValue(const char *str) { this->AddString(std::string(str)); }
	void AddValue(bool val) { this->AddBool(val); }
	void AddValue(float num) { this->AddFloat(num); }
	void AddValue(double num) { this->AddDouble(num); }

	void SetRequestID(uint32_t reqid);

	ssize_t EnsureBuffer(size_t size);

	void AddCreateSpace(const std::string &name);
	void AddCreateIndex(const std::string &space_name, const std::string &index_name, const MValueMap &options);
	void AddSpaceFormat(const std::string &space_name, const std::vector<std::pair<std::string, std::string> > &cols);
	void AddDropSpace(const std::string &name);
};

//~~~~~~~~~~~~~~~~~~~~~~~~ S E S S I O N ~~~~~~~~~~~~~~~~~~~~~~~~

class Session {
private:
	struct tbses base_ses;
	int connect_state;
public:
	Session();
	Session(int port);
	Session(int port, const std::string &address);

	int Connect();
	int GetConnectState() const;
	int GetErrorCode() const;
	struct tbses &GetSessionObject();

	int Sync();

	ssize_t Send(DataStructure &data);
	ssize_t Send(TP &data);
	TPResponse SendRequest(TP_p &request);

	ssize_t Receive(DataStructure &data, int from = 0, int strict = 1);

	int SessionSet(enum tbsesopt o, ...);

	~Session();
};

//~~~~~~~~~~~~~~~~~~~~~~~~ T P   T U P L E ~~~~~~~~~~~~~~~~~~~~~~~~

class TPTuple {
private:

};

//~~~~~~~~~~~~~~~~~~~~~~~~ T P   R E S P O N S E ~~~~~~~~~~~~~~~~~~~~~~~~

class TPResponse {
private:
	std::shared_ptr<struct tpresponse> base_response;
	int64_t state;
	DataStructure source;

public:
	TPResponse(const DataStructure &buf);
	TPResponse();

	uint32_t GetRequestID();
	uint32_t GetTupleCount() const;
	int64_t GetState() const;
	uint32_t GetCode() const;
	std::string GetError() const;
	DataStructure GetData() const;

	const char *GetCurrentTuple();
	uint32_t GetCurrentTupleSize();

	const char *GetCurrentField();
	void ToBeginOfTuple();
	bool HasNextField();
	uint32_t GetCurrentFieldSize();

	bool HasData();
	bool HasNextTuple();

	void NextTuple();
	void ToBeginOfTuples();

	TPResponse &operator++(); //only prefix
};

//~~~~~~~~~~~~~~~~~~~~~~~~ T P   G R E E T I N G ~~~~~~~~~~~~~~~~~~~~~~~~

class TPGreeting {
private:
	struct tpgreeting base_greeting;
public:
	ssize_t SetFromBuf(char *buf, size_t size);
};

#endif