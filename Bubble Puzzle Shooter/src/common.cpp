#include "common.h"

#include <chrono>

UInt64 ID::_gen{ 0 };

ID ID::generate()
{
	return { ++_gen };
}

bool operator== (const ID& id0, const ID& id1) { return id0._id == id1._id; }
bool operator!= (const ID& id0, const ID& id1) { return id0._id != id1._id; }
bool operator> (const ID& id0, const ID& id1) { return id0._id > id1._id; }
bool operator< (const ID& id0, const ID& id1) { return id0._id < id1._id; }
bool operator>= (const ID& id0, const ID& id1) { return id0._id >= id1._id; }
bool operator<= (const ID& id0, const ID& id1) { return id0._id <= id1._id; }

ID::operator bool() const { return _id; }
bool operator! (const ID& id) { return !id._id; }

std::ostream& operator<< (std::ostream& os, const ID& id) { return os << id._id; }
std::istream& operator>> (std::istream& is, ID& id) { return is >> id._id; }

size_t ID::Hash::operator() (const ID& id)
{
	return std::hash<UInt64>()(id._id);
}



Object::Object() :
	_id{}
{}
Object::~Object() {}

const ID& Object::id() const { return _id; }

bool operator== (const Object& left, const Object& right) { return left._id == right._id; }
bool operator!= (const Object& left, const Object& right) { return left._id != right._id; }



namespace utils
{
	long long int systemTime()
	{
		return std::chrono::system_clock::now().time_since_epoch().count();
	}
}