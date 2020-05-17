#include "props.h"

#include "resources.h"
#include "py.h"


Property::Property() :
	_type{ PropertyType::Undefined },
	_undefined{ 1 }
{}
Property::Property(const Property& p) :
	_type{ p._type }
{
	_copy(p);
}
Property::Property(Property&& p) noexcept :
	_type{ std::move(p._type) }
{
	_move(std::move(p));
}
Property::~Property()
{
	switch (_type)
	{
		case PropertyType::String:
			utils::destruct(_string);
			break;

		case PropertyType::Array:
			utils::destruct(_array);
			break;

		case PropertyType::Object:
			utils::destruct(_object);
			break;
	}
}

Property& Property::operator= (const Property& p)
{
	utils::destruct(*this);
	_type = p._type;
	_copy(p);
	return *this;
}
Property& Property::operator= (Property&& p) noexcept
{
	utils::destruct(*this);
	_type = std::move(p._type);
	_move(std::move(p));
	return *this;
}

void Property::_copy(const Property& p)
{
	switch (_type)
	{
		default:
		case PropertyType::Undefined:
			_undefined = 1;
			break;

		case PropertyType::Integer:
			_integer = p._integer;
			break;

		case PropertyType::Float:
			_float = p._float;
			break;

		case PropertyType::Boolean:
			_boolean = p._boolean;
			break;

		case PropertyType::String:
			utils::reconstruct(_string, p._string);
			break;

		case PropertyType::Array:
			utils::reconstruct(_array, p._array);
			break;

		case PropertyType::Object:
			utils::reconstruct(_object, p._object);
			break;
	}
}
void Property::_move(Property&& p) noexcept
{
	switch (_type)
	{
		default:
		case PropertyType::Undefined:
			_undefined = 1;
			break;

		case PropertyType::Integer:
			_integer = std::move(p._integer);
			break;

		case PropertyType::Float:
			_float = std::move(p._float);
			break;

		case PropertyType::Boolean:
			_boolean = std::move(p._boolean);
			break;

		case PropertyType::String:
			utils::reconstruct(_string, std::move(p._string));
			break;

		case PropertyType::Array:
			utils::reconstruct(_array, std::move(p._array));
			break;

		case PropertyType::Object:
			utils::reconstruct(_object, std::move(p._object));
			break;
	}
}

bool operator== (const Property& left, const Property& right)
{
	if (left._type != right._type)
		return false;

	switch (left._type)
	{
		default:
		case PropertyType::Undefined:
			return true;

		case PropertyType::Integer:
			return left._integer == right._integer;

		case PropertyType::Float:
			return left._float == right._float;

		case PropertyType::Boolean:
			return left._boolean == right._boolean;

		case PropertyType::String:
			return left._string == right._string;

		case PropertyType::Array:
			return left._array == right._array;

		case PropertyType::Object:
			return left._object == right._object;
	}
}
bool operator!= (const Property& left, const Property& right)
{
	if (left._type != right._type)
		return true;

	switch (left._type)
	{
		default:
		case PropertyType::Undefined:
			return false;

		case PropertyType::Integer:
			return left._integer != right._integer;

		case PropertyType::Float:
			return left._float != right._float;

		case PropertyType::Boolean:
			return left._boolean != right._boolean;

		case PropertyType::String:
			return left._string != right._string;

		case PropertyType::Array:
			return left._array != right._array;

		case PropertyType::Object:
			return left._object != right._object;
	}
}



Property::Property(UInt64 value) :
	_type{ PropertyType::Integer },
	_integer{ value }
{}
Property::Property(double value) :
	_type{ PropertyType::Float },
	_float{ value }
{}
Property::Property(bool value) :
	_type{ PropertyType::Boolean },
	_boolean{ value }
{}
Property::Property(const std::string& value) :
	_type{ PropertyType::String },
	_string{ value }
{}
Property::Property(const std::vector<Property>& value) :
	_type{ PropertyType::Array },
	_array{}
{
	_array = value;
}
Property::Property(const std::map<std::string, Property>& value) :
	_type{ PropertyType::Object },
	_object{ value }
{}



UInt64 Property::asIntegerValue() const
{
	UInt64 value;
	switch (_type)
	{
		default:
		case PropertyType::Undefined:
			return 0;

		case PropertyType::Integer:
			return _integer;

		case PropertyType::Float:
			return static_cast<UInt64>(_float);

		case PropertyType::Boolean:
			return static_cast<UInt64>(_boolean);

		case PropertyType::String:
			utils::from_string(_string, value);
			return value;

		case PropertyType::Array:
			return _array.empty() ? 0 : _array.front().asIntegerValue();

		case PropertyType::Object:
			return _object.empty() ? 0 : _object.begin()->second.asIntegerValue();
	}
}

double Property::asFloatValue() const
{
	double value;
	switch (_type)
	{
		default:
		case PropertyType::Undefined:
			return 0;

		case PropertyType::Integer:
			return static_cast<double>(_integer);

		case PropertyType::Float:
			return _float;

		case PropertyType::Boolean:
			return static_cast<double>(_boolean);

		case PropertyType::String:
			utils::from_string(_string, value);
			return value;

		case PropertyType::Array:
			return _array.empty() ? 0 : _array.front().asFloatValue();

		case PropertyType::Object:
			return _object.empty() ? 0 : _object.begin()->second.asFloatValue();
	}
}
bool Property::asBooleanValue() const
{
	bool value;
	switch (_type)
	{
		default:
		case PropertyType::Undefined:
			return false;

		case PropertyType::Integer:
			return static_cast<bool>(_integer);

		case PropertyType::Float:
			return static_cast<bool>(_float);

		case PropertyType::Boolean:
			return _boolean;

		case PropertyType::String:
			utils::from_string(_string, value);
			return value;

		case PropertyType::Array:
			return _array.empty() ? false : _array.front().asBooleanValue();

		case PropertyType::Object:
			return _object.empty() ? false : _object.begin()->second.asBooleanValue();
	}
}
std::string Property::asStringValue() const
{
	switch (_type)
	{
		default:
		case PropertyType::Undefined:
			return {};

		case PropertyType::Integer:
			return std::to_string(_integer);

		case PropertyType::Float:
			return std::to_string(_float);

		case PropertyType::Boolean:
			return std::to_string(_boolean);

		case PropertyType::String:
			return _string;

		case PropertyType::Array:
			return _array.empty() ? std::string{} : _array.front().asStringValue();

		case PropertyType::Object:
			return _object.empty() ? std::string{} : _object.begin()->second.asStringValue();
	}
}
std::vector<Property> Property::asArrayValue() const
{
	switch (_type)
	{
		default:
		case PropertyType::Undefined:
			return {};

		case PropertyType::Integer:
			return { { _integer } };

		case PropertyType::Float:
			return { { _float } };

		case PropertyType::Boolean:
			return { { _boolean } };

		case PropertyType::String:
			return { { _string } };;

		case PropertyType::Array:
			return _array;

		case PropertyType::Object:
			return { { _object } };
	}
}
std::map<std::string, Property> Property::asObjectValue() const
{
	switch (_type)
	{
		default:
		case PropertyType::Undefined:
			return {};

		case PropertyType::Integer:
			return { { "", { _integer } } };

		case PropertyType::Float:
			return { { "", { _float } } };

		case PropertyType::Boolean:
			return { { "", { _boolean } } };

		case PropertyType::String:
			return { { "", { _string } } };

		case PropertyType::Array:
			return { { "", { _array } } };

		case PropertyType::Object:
			return _object;
	}
}







std::unordered_map<std::string, Property> Properties::Props{};

const Property* Properties::get(const std::string& name)
{
	auto it = Props.find(name);
	return it == Props.end() ? nullptr : &it->second;
}



namespace py = pybind;

static Property loadProperty(const py::handle& value);

static Property loadArrayProperty(const py::list& value)
{
	std::vector<Property> props;
	for (auto raw : value)
		props.push_back(loadProperty(raw));
	return props;
}

static Property loadArrayProperty(const py::tuple& value)
{
	std::vector<Property> props;
	for (auto raw : value)
		props.push_back(loadProperty(raw));
	return props;
}

static Property loadArrayProperty(const py::set& value)
{
	std::vector<Property> props;
	for (auto raw : value)
		props.push_back(loadProperty(raw));
	return props;
}

static Property loadObjectProperty(const py::dict& value)
{
	std::map<std::string, Property> props;
	for (auto raw : value)
		props[raw.first.cast<std::string>()] = loadProperty(raw.second);
	return props;
}

static Property loadProperty(const py::handle& value)
{
	if (value.is_none())
		return {};

	if (py::isinstance<py::int_>(value))
		return value.cast<UInt64>();

	if (py::isinstance<py::float_>(value))
		return value.cast<double>();

	if (py::isinstance<py::bool_>(value))
		return value.cast<bool>();

	if (py::isinstance<py::str>(value))
		return value.cast<std::string>();

	if (py::isinstance<py::list>(value))
		return loadArrayProperty(py::cast<py::list>(value));

	if (py::isinstance<py::tuple>(value))
		return loadArrayProperty(py::cast<py::tuple>(value));

	if (py::isinstance<py::set>(value))
		return loadArrayProperty(py::cast<py::set>(value));

	if (py::isinstance<py::dict>(value))
		return loadObjectProperty(py::cast<py::dict>(value));

	return {};
}

void Properties::load()
{
	py::dict raw = pylib::executePropertiesPythonScript(ResourcePoint::Data + "config.py");
	Props.clear();

	for (const auto& rawProp : raw)
	{
		try
		{
			Props[rawProp.first.cast<std::string>()] = loadProperty(rawProp.second);
		}
		catch (std::exception& ex)
		{
			std::cout << "Unexpected exception during properties load occurs: " << ex.what() << std::endl;
		}
	}
}
