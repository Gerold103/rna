#include "datastructure.h"

//~~~~~~~~~~~~~~~~~~~~~~~~D A T A   S T R U C T U R E~~~~~~~~~~~~~~~~~~~~~~~~

DataStructure::DataStructure() : data(nullptr, std::default_delete<char[]>()), size(0) { }

DataStructure::DataStructure(const char *buf, size_t sz)
{
	data.reset(new char[sz], std::default_delete<char[]>());
	std::memcpy(data.get(), buf, sz);
	size = sz;
}

DataStructure::DataStructure(size_t sz)
{
	data.reset(new char[sz], std::default_delete<char[]>());
	size = sz;
}

size_t DataStructure::Size() const { return size; }

char *DataStructure::Data() const { return data.get(); }

void DataStructure::Resize(size_t sz)
{
	std::shared_ptr<char> new_mem_block(new char[sz], std::default_delete<char[]>());
	std::memcpy(new_mem_block.get(), data.get(), std::min(sz, size));
	data = new_mem_block;
	size = sz;
}

std::ostream &operator<<(std::ostream &stream, const DataStructure &ob)
{
	stream << "DataStructure: size = " << ob.Size();
	return stream;
}

bool operator&&(const DataStructure &left, const DataStructure &right)
{
	return ((left.Size() != 0) && (right.Size() != 0));
}

bool operator||(const DataStructure &left, const DataStructure &right)
{
	return ((left.Size() != 0) || (right.Size() != 0));
}

bool operator!(const DataStructure &ob)
{
	return !ob.Size();
}