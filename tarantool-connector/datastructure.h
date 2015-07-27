#ifndef DATASTRUCTURE_H_INCLUDED
#define DATASTRUCTURE_H_INCLUDED

#include <memory>
#include <ostream>
#include <cstring>

//~~~~~~~~~~~~~~~~~~~~~~~~D A T A   S T R U C T U R E~~~~~~~~~~~~~~~~~~~~~~~~

class DataStructure {
private:
	mutable std::shared_ptr<char> data;
	size_t size;
public:
	DataStructure();
	DataStructure(const char *buf, size_t sz);
	DataStructure(size_t sz);
	size_t Size() const;
	char *Data() const;
	void Resize(size_t sz);
};

std::ostream &operator<<(std::ostream &stream, const DataStructure &ob);

bool operator&&(const DataStructure &left, const DataStructure &right);
bool operator||(const DataStructure &left, const DataStructure &right);
bool operator!(const DataStructure &ob);
//bool operator!=(const DataStructure &left, const DataStructure &right);

#endif