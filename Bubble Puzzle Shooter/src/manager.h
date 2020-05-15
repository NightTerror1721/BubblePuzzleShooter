#pragma once

#include "memory.h"

template<typename _Base>
class Manager
{
public:
	typedef Ref<_Base> Resource;

private:
	MemoryAllocator<_Base> _alloc;
	std::map<std::string, Resource> _elems;
	Manager* const _parent;

public:
	Manager(Manager* parent = nullptr) :
		_alloc{},
		_elems{},
		_parent{ parent }
	{}
	~Manager() {}

	bool has(const std::string& name) const
	{
		if (_elems.find(name) != _elems.end())
			return true;
		return _parent && _parent->has(name);
	}

	Resource get(const std::string& name)
	{
		auto it = _elems.find(name);
		if (it == _elems.end())
		{
			if (!_parent)
				return nullptr;
			return _parent->get(name);
		}
		return it->second;
	}

	const Resource get(const std::string& name) const
	{
		auto it = _elems.find(name);
		if (it == _elems.end())
		{
			if (!_parent)
				return nullptr;
			return _parent->get(name);
		}
		return it->second;
	}

	bool destroy(const std::string& name)
	{
		auto it = _elems.find(name);
		if (it == _elems.end())
			return false;

		Resource res = it->second;
		_elems.erase(it);
		_alloc.free(res);
		return true;
	}

	void clear()
	{
		_elems.clear();
		_alloc.clear();
	}

	inline Resource operator[] (const std::string& name) { return get(name); }
	inline const Resource operator[] (const std::string& name) const { return get(name); }

protected:
	template<typename _Ty, typename...  _Args>
	Ref<_Ty> create(const std::string& name, _Args... args)
	{
		if (has(name))
			return nullptr;

		Ref<_Ty> ref = _alloc.alloc<_Ty>(std::forward<_Args>(args)...);
		_elems[name] = Ref<_Base>::upcast(ref);
		return ref;
	}
};
