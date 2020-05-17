#pragma once

#include <unordered_map>

#include "common.h"

enum class PropertyType
{
	Undefined,
	Integer,
	Float,
	Boolean,
	String,
	Array,
	Object
};

class Property
{
private:
	PropertyType _type = PropertyType::Undefined;
	union
	{
		int _undefined;
		UInt64 _integer;
		double _float;
		bool _boolean;
		std::string _string;
		std::vector<Property> _array;
		std::map<std::string, Property> _object;
	};

public:
	Property();
	Property(const Property& p);
	Property(Property&& p) noexcept;
	~Property();

	Property& operator= (const Property& p);
	Property& operator= (Property&& p) noexcept;

	friend bool operator== (const Property& left, const Property& right);
	friend bool operator!= (const Property& left, const Property& right);

	Property(UInt64 value);
	Property(double value);
	Property(bool value);
	Property(const std::string& value);
	Property(const std::vector<Property>& value);
	Property(const std::map<std::string, Property>& value);

	UInt64 asIntegerValue() const;
	double asFloatValue() const;
	bool asBooleanValue() const;
	std::string asStringValue() const;
	std::vector<Property> asArrayValue() const;
	std::map<std::string, Property> asObjectValue() const;

	inline operator UInt8() const { return static_cast<UInt8>(asIntegerValue()); }
	inline operator UInt16() const { return static_cast<UInt16>(asIntegerValue()); }
	inline operator UInt32() const { return static_cast<UInt32>(asIntegerValue()); }
	inline operator UInt64() const { return asIntegerValue(); }

	inline operator Int8() const { return static_cast<Int8>(asIntegerValue()); }
	inline operator Int16() const { return static_cast<Int16>(asIntegerValue()); }
	inline operator Int32() const { return static_cast<Int32>(asIntegerValue()); }
	inline operator Int64() const { return static_cast<Int64>(asIntegerValue()); }

	inline operator float() const { return static_cast<float>(asFloatValue()); }
	inline operator double() const { return asFloatValue(); }

	inline operator bool() const { return asBooleanValue(); }

	inline operator std::string() const { return asStringValue(); }

	inline operator std::vector<Property>() const { return asArrayValue(); }

	inline operator std::map<std::string, Property>() const { return asObjectValue(); }

	inline Property(UInt8 value) : Property{ static_cast<UInt64>(value) } {}
	inline Property(UInt16 value) : Property{ static_cast<UInt64>(value) } {}
	inline Property(UInt32 value) : Property{ static_cast<UInt64>(value) } {}

	inline Property(Int8 value) : Property{ static_cast<UInt64>(value) } {}
	inline Property(Int16 value) : Property{ static_cast<UInt64>(value) } {}
	inline Property(Int32 value) : Property{ static_cast<UInt64>(value) } {}
	inline Property(Int64 value) : Property{ static_cast<UInt64>(value) } {}

private:
	void _copy(const Property& p);
	void _move(Property&& p) noexcept;
};

class Properties
{
private:
	static std::unordered_map<std::string, Property> Props;

public:
	static void load();

	static const Property* get(const std::string& name);

private:
	template<typename _Ty>
	static inline _Ty genericGet(const std::string& name, const _Ty& default_value)
	{
		auto prop = get(name);
		if (!prop)
			return default_value;
		return *prop;
	}

public:

	static inline UInt8 getUInt8(const std::string& name, UInt8 default_value = 0) { return genericGet(name, default_value); }
	static inline UInt16 getUInt16(const std::string& name, UInt16 default_value = 0) { return genericGet(name, default_value); }
	static inline UInt32 getUInt32(const std::string& name, UInt32 default_value = 0) { return genericGet(name, default_value); }
	static inline UInt64 getUInt64(const std::string& name, UInt64 default_value = 0) { return genericGet(name, default_value); }

	static inline Int8 getInt8(const std::string& name, Int8 default_value = 0) { return genericGet(name, default_value); }
	static inline Int16 getInt16(const std::string& name, Int16 default_value = 0) { return genericGet(name, default_value); }
	static inline Int32 getInt32(const std::string& name, Int32 default_value = 0) { return genericGet(name, default_value); }
	static inline Int64 getInt64(const std::string& name, Int64 default_value = 0) { return genericGet(name, default_value); }

	static inline float getFloat(const std::string& name, float default_value = 0) { return genericGet(name, default_value); }
	static inline double getDouble(const std::string& name, double default_value = 0) { return genericGet(name, default_value); }

	static inline bool getBool(const std::string& name, bool default_value = false) { return genericGet(name, default_value); }

	static inline std::string getString(const std::string& name, const std::string& default_value = {})
	{
		return genericGet(name, default_value);
	}

	static std::vector<Property> getVector(const std::string& name, const std::vector<Property>& default_value = {})
	{
		return genericGet(name, default_value);
	}

	static std::map<std::string, Property> getMap(const std::string& name, const std::map<std::string, Property>& default_value = {})
	{
		return genericGet(name, default_value);
	}

public:
	Properties() = delete;
	Properties(const Properties&) = delete;

	Properties& operator= (const Properties&) = delete;
};

typedef Properties Props;

