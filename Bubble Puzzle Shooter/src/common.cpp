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



RNG::RNG() :
	RNG{ utils::systemTime() }
{}
RNG::RNG(Seed seed, RandomValue min, RandomValue max) :
	_rand{ seed },
	_min{ min },
	_max{ max }
{}
RNG::~RNG() {}

RNG::RandomValue RNG::min() const { return _min; }
void RNG::min(RandomValue value) { minmax(value, _max); }

RNG::RandomValue RNG::max() const { return _max; }
void RNG::max(RandomValue value) { minmax(_min, value); }

RNG::RandomValue RNG::operator() (RandomValue __min, RandomValue __max)
{
	auto min = std::min(__min, __max);
	auto max = std::max(__min, __max);
	return (_rand() % (max - min)) + min;
}

float RNG::randomFloat() { return _rand() / static_cast<float>(std::minstd_rand::max()); }

void RNG::minmax(RandomValue min, RandomValue max)
{
	_min = std::min(min, max);
	_max = std::max(min, max);
}

RNG& operator>> (RNG& left, RNG::RandomValue& right) { right = left(); return left; }
RNG& operator>> (RNG& left, float& right) { right = left.randomFloat(); return left; }

bool operator== (RNG& left, RNG::RandomValue right) { return left() == right; }
bool operator== (RNG& left, float right) { return left.randomFloat() == right; }

bool operator!= (RNG& left, RNG::RandomValue right) { return left() != right; }
bool operator!= (RNG& left, float right) { return left.randomFloat() != right; }

bool operator> (RNG& left, RNG::RandomValue right) { return left() > right; }
bool operator> (RNG& left, float right) { return left.randomFloat() > right; }

bool operator< (RNG& left, RNG::RandomValue right) { return left() < right; }
bool operator< (RNG& left, float right) { return left.randomFloat() < right; }




namespace utils
{
	long long int systemTime()
	{
		return std::chrono::system_clock::now().time_since_epoch().count();
	}
}
