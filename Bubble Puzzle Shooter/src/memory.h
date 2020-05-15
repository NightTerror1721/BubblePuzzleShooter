#pragma once

#include "common.h"

namespace memory
{
	template<typename _Base>
	class AllocatorList;

	template<typename _Base>
	struct Allocator final
	{
		AllocatorList<_Base>* const list;
		_Base* const data;
		Allocator* next;
		Allocator* prev;

		Allocator(AllocatorList<_Base>* list, _Base* data) :
			list{ list },
			data{ data },
			next{ nullptr },
			prev{ nullptr }
		{}
		~Allocator() { delete data; }

		Allocator(const Allocator&) = delete;
		Allocator& operator= (const Allocator&) = delete;
	};
}
#define ALLOCATOR_FRIENDLY friend memory::Allocator


template<class _Ty>
class AllocatorIterator
{
private:
	using alloc_t = memory::Allocator<_Ty>*;

	alloc_t& _alloc;
	bool _end;

public:
	bool operator== (const AllocatorIterator& it) const { return _end == it._end && _alloc == it._alloc; }
	bool operator!= (const AllocatorIterator& it) const { return _end != it._end && _alloc != it._alloc; }

	bool operator! () const { return _end; }
	operator bool() { return !_end; }

	AllocatorIterator operator++ ()
	{
		if (_end)
			throw no_such_element();

		if (!_alloc->next)
			_end = true;
		else _alloc = _alloc->next;

		return *this;
	}
	AllocatorIterator operator++ (int)
	{
		AllocatorIterator old{ _alloc, _end };
		operator++();
		return old;
	}

	AllocatorIterator operator-- ()
	{
		if (_alloc->prev)
		{
			_alloc = _alloc->prev;
			_end = false;
		}
		else throw no_such_element();

		return *this;
	}
	AllocatorIterator operator-- (int)
	{
		AllocatorIterator old{ _alloc, _end };
		operator--();
		return old;
	}

	_Ty& operator* ()
	{
		return *_alloc->data;
	}
	const _Ty& operator* () const
	{
		return *_alloc->data;
	}

	_Ty* operator->()
	{
		return _alloc->data;
	}
	const _Ty* operator->() const
	{
		return _alloc->data;
	}

	template<class _Ty>
	friend class memory::AllocatorList;


private:
	AllocatorIterator(alloc_t& alloc, bool end) : _alloc(alloc), _end(end) {}

public:
	class no_such_element : public std::exception {};
};


namespace memory
{
	template<typename _Base>
	class AllocatorList
	{
	public:
		typedef Allocator<_Base> Node;

	private:
		Node* _head;
		Node* _tail;
		size_t _size;

	public:
		AllocatorList() :
			_head{ nullptr },
			_tail{ nullptr },
			_size{ 0 },
			_begin{ _head, false },
			_end{ _tail, true }
		{}
		~AllocatorList() { clear(); }

		template<typename _Ty, typename... _Args>
		Node* create(_Args&&... args)
		{
			_Base* data = new _Ty(std::forward<_Args>(args)...);

			if (!_head)
			{
				_head = new Node(this, data);
				_tail = _head;
				_size++;
				return _head;
			}
			Node* node = new Node(this, data);
			node->prev = _tail;
			_tail->next = node;
			_tail = node;
			_size++;
			return node;
		}

		void destroy(Node* const& allocator)
		{
			if (!_head || allocator->list != this)
				return;

			Node* current = allocator;

			if (_head == _tail)
			{
				delete _head;
				_head = nullptr;
				_tail = nullptr;
				_size = 0;
			}
			else
			{
				Node* prev = current->prev;
				Node* next = current->next;

				if (current == _head)
					_head = _head->next;
				if (current == _tail)
					_tail = _tail->prev;

				prev->next = next;
				next->prev = prev;
				_size--;

				delete current;
			}
		}

		void clear()
		{
			Node* current = _head;
			while (current)
			{
				Node* next = current->next;
				delete current;
				current = next;
			}
			_head = nullptr;
			_tail = nullptr;
			_size = 0;
		}

		void forEach(std::function<void(_Base&)> action)
		{
			for (Node* current = _head; current; current = current->next)
				action(current->data);
		}

		void forEach(std::function<void(const _Base&)> action) const
		{
			for (Node* current = _head; current; current = current->next)
				action(current->data);
		}


		/* Iterable part */
	private:
		AllocatorIterator<_Base> _begin;
		AllocatorIterator<_Base> _end;

	public:
		AllocatorIterator<_Base> begin() { return _begin; }
		const AllocatorIterator<_Base> begin() const { return _begin; }
		AllocatorIterator<_Base> end() { return _end; }
		const AllocatorIterator<_Base> end() const { return _end; }
	};
}



template<typename _Base>
class MemoryAllocator;

template<typename _Ty>
class Ref final
{
private:
	_Ty* _ptr;
	void* _alloc;

public:
	template<typename _Base>
	Ref(memory::Allocator<_Base>& alloc) :
		_ptr{ reinterpret_cast<_Ty*>(alloc.data) },
		_alloc{ reinterpret_cast<void*>(&alloc) }
	{
		static_assert(std::is_base_of<_Base, _Ty>::value);
	}
	Ref() :
		_ptr{ nullptr },
		_alloc{ nullptr }
	{}
	Ref(decltype(nullptr)) :
		_ptr{ nullptr },
		_alloc{ nullptr }
	{}
	Ref(const Ref&) = default;
	Ref(Ref&&) = default;

	Ref& operator= (const Ref&) = default;
	Ref& operator= (Ref&&) = default;

	operator bool() const { return _ptr; }
	bool operator! () const { return !_ptr; }
	
	bool operator== (const Ref& right) const { return _ptr == right._ptr; }
	bool operator!= (const Ref& right) const { return _ptr != right._ptr; }
	bool operator> (const Ref& right) const { return _ptr > right._ptr; }
	bool operator< (const Ref& right) const { return _ptr < right._ptr; }
	bool operator>= (const Ref& right) const { return _ptr >= right._ptr; }
	bool operator<= (const Ref& right) const { return _ptr <= right._ptr; }

	_Ty& operator* () { return *_ptr; }
	const _Ty& operator* () const { return *_ptr; }

	_Ty* operator& () { return _ptr; }
	const _Ty* operator& () const { return _ptr; }

	_Ty* operator->() { return _ptr; }
	const _Ty* operator->() const { return _ptr; }

private:
	explicit Ref(_Ty* ptr, void* alloc) :
		_ptr{ ptr },
		_alloc{ alloc }
	{}

public:
	template<typename _Base>
	static Ref<_Ty> downcast(Ref<_Base>& ref)
	{
		static_assert(std::is_base_of<_Base, _Ty>::value);
		return Ref<_Ty>(dynamic_cast<_Ty*>(ref._ptr), ref._alloc);
	}

	template<typename _Sub>
	static Ref<_Ty> upcast(Ref<_Sub>& ref)
	{
		static_assert(std::is_base_of<_Ty, _Sub>::value);
		return Ref<_Ty>(reinterpret_cast<_Ty*>(ref._ptr), ref._alloc);
	}

	template<typename _Base>
	friend class MemoryAllocator;
};



template<typename _Base>
class MemoryAllocator
{
public:
	using alloc_t = memory::Allocator<_Base>;
	using iterator = AllocatorIterator<_Base>;
	using const_iterator = const AllocatorIterator<_Base>;

private:
	memory::AllocatorList<_Base> _mem;

public:
	MemoryAllocator() :
		_mem{}
	{}
	~MemoryAllocator() {}

	template<typename _Ty, typename... _Args>
	Ref<_Ty> alloc(_Args&&... args)
	{
		static_assert(std::is_base_of<_Base, _Ty>::value);
		return Ref<_Ty>(*_mem.create<_Ty>(std::forward<_Args>(args)...));
	}

	template<typename _Ty>
	inline void free(Ref<_Ty> ptr)
	{
		static_assert(std::is_base_of<_Base, _Ty>::value);
		_mem.destroy(reinterpret_cast<memory::Allocator<_Base>*>(ptr._alloc));
	}

	inline void clear()
	{
		_mem.clear();
	}

	std::vector<Ref<_Base>> find(std::function<bool(const _Base&)> criteria)
	{
		std::vector<Ref<_Base>> vec;
		for (alloc_t* alloc = _mem._head; alloc; alloc = alloc->next)
			if (criteria(alloc->data))
				vec.push_back({ alloc });
		return std::move(vec);
	}

	const std::vector<Ref<_Base>> find(std::function<bool(const _Base&)> criteria) const
	{
		std::vector<Ref<_Base>> vec;
		for (alloc_t* alloc = _mem._head; alloc; alloc = alloc->next)
			if (criteria(alloc->data))
				vec.push_back({ alloc });
		return std::move(vec);
	}

	inline void forEach(std::function<void(_Base&)> action) { _mem.forEach(action); }
	inline void forEach(std::function<void(const _Base&)> action) const { _mem.forEach(action); }

	/*template<typename _Ty>
	inline Ref<_Ty> operator[] (const ID& id) 
	{
		static_assert(std::is_base_of<_Base, _Ty>::value);
		return find(id);
	}
	template<typename _Ty>
	inline const Ref<_Ty> operator[] (const ID& id) const
	{
		static_assert(std::is_base_of<_Base, _Ty>::value);
		return find(id);
	}*/

	iterator begin() { return _mem.begin(); }
	const_iterator begin() const { return _mem.begin(); }
	const_iterator cbegin() const { return _mem.begin(); }
	iterator end() { return _mem.end(); }
	const_iterator end() const { return _mem.end(); }
	const_iterator cend() const { return _mem.end(); }
};
