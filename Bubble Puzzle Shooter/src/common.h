#pragma once

#include <cstdint>
#include <cstring>

#include <type_traits>
#include <functional>
#include <algorithm>
#include <iostream>
#include <utility>
#include <random>
#include <string>
#include <vector>
#include <map>


#include <SFML/Graphics.hpp>

#define NON_COPYABLE(_Class) \
	_Class(const _Class&) = delete; \
	_Class& operator= (const _Class&) = delete
#define NON_COPYABLE_MOVABLE(_Class) NON_COPYABLE(_Class); \
	_Class(_Class&&) = delete; \
	_Class& operator= (_Class&&) = delete


typedef std::uint8_t UInt8;
typedef std::uint16_t UInt16;
typedef std::uint32_t UInt32;
typedef std::uint64_t UInt64;

typedef std::int8_t Int8;
typedef std::int16_t Int16;
typedef std::int32_t Int32;
typedef std::int64_t Int64;

typedef sf::Vector2f Vec2f;
typedef sf::Vector2i Vec2i;
typedef sf::Vector2u Vec2u;

using std::size_t;


class ID
{
private:
	UInt64 _id;
	static UInt64 _gen;

	constexpr ID(UInt64 code) : _id{ code } {}

public:
	constexpr ID() : _id{ 0 } {}
	ID(const ID&) = default;
	ID(ID&&) noexcept = default;

	ID& operator= (const ID&) = default;
	ID& operator= (ID&&) = default;

	static ID generate();

	friend bool operator== (const ID& id0, const ID& id1);
	friend bool operator!= (const ID& id0, const ID& id1);
	friend bool operator> (const ID& id0, const ID& id1);
	friend bool operator< (const ID& id0, const ID& id1);
	friend bool operator>= (const ID& id0, const ID& id1);
	friend bool operator<= (const ID& id0, const ID& id1);

	operator bool() const;
	friend bool operator! (const ID& id);

	friend std::ostream& operator<< (std::ostream& os, const ID& id);
	friend std::istream& operator>> (std::istream& is, ID& id);

public:
	class Hash
	{
		size_t operator() (const ID& id);
	};
	friend class ID::Hash;
};


class Object
{
private:
	ID _id;

public:
	explicit Object();
	virtual ~Object();

	const ID& id() const;

	friend bool operator== (const Object& left, const Object& right);
	friend bool operator!= (const Object& left, const Object& right);
};


class Renderable
{
public:
	virtual void render(sf::RenderTarget& canvas) = 0;
};

class Updatable
{
public:
	virtual void update(const sf::Time& delta) = 0;
};

class EventDispatcher
{
public:
	virtual void dispatchEvent(const sf::Event& event) = 0;
};


class RNG
{
public:
	typedef unsigned int RandomValue;
	typedef unsigned int Seed;

private:
	std::minstd_rand _rand;
	RandomValue _min;
	RandomValue _max;

public:
	RNG();
	RNG(Seed seed, RandomValue min = std::minstd_rand::min(), RandomValue max = std::minstd_rand::max());
	RNG(const RNG&) = default;
	RNG(RNG&&) = default;
	~RNG();

	RNG& operator= (const RNG&) = default;
	RNG& operator= (RNG&&) = default;

	RandomValue min() const;
	void min(RandomValue value);

	RandomValue max() const;
	void max(RandomValue value);

	RandomValue operator() (RandomValue min, RandomValue max);

	float randomFloat();

	friend RNG& operator>> (RNG& left, RandomValue& right);
	friend RNG& operator>> (RNG& left, float& right);

	friend bool operator== (RNG& left, RandomValue right);
	friend bool operator== (RNG& left, float right);

	friend bool operator!= (RNG& left, RandomValue right);
	friend bool operator!= (RNG& left, float right);

	friend bool operator> (RNG& left, RandomValue right);
	friend bool operator> (RNG& left, float right);

	friend bool operator< (RNG& left, RandomValue right);
	friend bool operator< (RNG& left, float right);

	inline RandomValue operator() (RandomValue max) { return (*this)(_min, max); }
	inline RandomValue operator() () { return (*this)(_min, _max); }

private:
	void minmax(RandomValue min, RandomValue max);
};


namespace utils
{
	long long int systemTime();
}
