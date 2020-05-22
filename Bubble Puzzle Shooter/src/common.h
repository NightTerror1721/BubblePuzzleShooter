#pragma once

#include <cstdint>
#include <cstring>

#include <type_traits>
#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <compare>
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


enum class PlayerId : UInt8 { First, Second, Single = First };
enum class TimerMode : UInt8 { TURN = 0x1, END = 0x2, TURN_AND_END = TURN | END };


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

	Seed randomSeed();

	RNG randomRNG();

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
	template<typename _Ty>
	using matrix = std::vector<std::vector<_Ty>>;

	extern const std::string EmptyString;
	long long int systemTime();

	
	template<typename _Ty, typename _MinTy, typename _MaxTy>
	constexpr _Ty clamp(_Ty value, _MinTy min, _MaxTy max)
	{
		static_assert(std::is_integral<_Ty>::value || std::is_floating_point<_Ty>::value);
		static_assert(std::is_integral<_MinTy>::value || std::is_floating_point<_MinTy>::value);
		static_assert(std::is_integral<_MaxTy>::value || std::is_floating_point<_MaxTy>::value);

		if constexpr (std::is_same<_MinTy, _Ty>::value)
		{
			if constexpr (std::is_same<_MaxTy, _Ty>::value)
			{
				return std::min<_Ty>(max, std::max<_Ty>(min, value));
			}
			else
			{
				return std::min<_Ty>(static_cast<_Ty>(max), std::max<_Ty>(min, value));
			}
		}
		else
		{
			if constexpr (std::is_same<_MaxTy, _Ty>::value)
			{
				return std::min<_Ty>(max, std::max<_Ty>(static_cast<_Ty>(min), value));
			}
			else
			{
				return std::min<_Ty>(static_cast<_Ty>(max), std::max<_Ty>(static_cast<_Ty>(min), value));
			}
		}
	}


	template<typename _Ty>
	constexpr bool is_pair(_Ty value)
	{
		static_assert(std::is_integral<_Ty>::value);
		return (value % 2) == 0;
	}

	template<typename _Ty, typename... _Args>
	inline _Ty& reconstruct(_Ty& object, _Args&&... args)
	{
		new(&object) _Ty(std::forward<_Args>(args)...);
		return object;
	}

	template<typename _Ty>
	inline void destruct(_Ty& object)
	{
		object.~_Ty();
	}

	template<typename _Ty>
	inline _Ty& from_string(const std::string& string, _Ty& value)
	{
		std::stringstream ss(string);
		ss >> value;
		return value;
	}

	template<typename _Ty>
	inline std::string to_string(const _Ty& value)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	template<typename _Ty, typename _SizeType>
	inline std::vector<_Ty> static_vector(_SizeType size, const _Ty& template_value)
	{
		if constexpr (std::is_same<_SizeType, size_t>::value)
		{
			return std::vector(size, template_value);
		}
		else
		{
			return std::vector(static_cast<size_t>(size), template_value);
		}
	}

	template<typename _Ty, typename _RowSizeType, typename _ColumnSizeType>
	inline std::vector<std::vector<_Ty>> static_matrix(_RowSizeType rows, _ColumnSizeType columns, const _Ty& template_value)
	{
		if constexpr (std::is_same<_RowSizeType, size_t>::value)
		{
			return std::vector(rows, static_vector<_Ty, _ColumnSizeType>(columns, template_value));
		}
		else
		{
			return std::vector(static_cast<size_t>(rows), static_vector<_Ty, _ColumnSizeType>(columns, template_value));
		}
	}
}
