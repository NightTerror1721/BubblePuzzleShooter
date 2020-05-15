#pragma once

#include "common.h"
#include "memory.h"

class GameController;

class GameObject : public Object, public Renderable, public Updatable, public EventDispatcher
{
private:
	GameController* _gc;
	std::string _tag;

public:
	GameObject();
	virtual ~GameObject();

	const std::string& tag() const;
	void tag(const std::string& tag);

	bool hasAttached() const;

	virtual void render(sf::RenderTarget& canvas) override = 0;
	virtual void update(const sf::Time& delta) override = 0;
	virtual void dispatchEvent(const sf::Event& event) override = 0;

protected:
	GameController& getGameController();
	const GameController& getGameController() const;
};


template<class _Base>
class GameObjectContainer
{
	static_assert(std::is_base_of<GameObject, _Base>::value);

private:
	MemoryAllocator<_Base> _alloc;

protected:
	virtual void onCreateGameObject(_Base& obj) {}
	virtual void onDestroyGameObject(_Base& obj) {}

public:
	GameObjectContainer() :
		_alloc{}
	{}
	~GameObjectContainer() {}

	GameObjectContainer(const GameObjectContainer&) = delete;

	GameObjectContainer& operator= (const GameObjectContainer&) = delete;

	template<typename _Ty, typename... _Args>
	Ref<_Ty> createGameObject(_Args&&... args)
	{
		Ref<_Ty> ref = _alloc.alloc<_Ty>(args);
		onCreateGameObject(reinterpret_cast<_Base&>(*ref));
		return ref;
	}

	std::vector<Ref<_Base>> findGameObject(const std::function<bool(const _Base&)>& criteria)
	{
		return _alloc.find(criteria);
	}

	const std::vector<Ref<_Base>> findGameObject(const std::function<bool(const _Base&)>& criteria) const
	{
		return _alloc.find(criteria);
	}

	template<typename _Ty>
	Ref<_Ty> getGameObjectById(const ID& id)
	{
		static_assert(std::is_base_of<_Base, _Ty>::value);
		auto result = find([&id](const _Base& obj) { return obj.id() == id; });
		if (!result.empty())
		{
			if constexpr (std::is_same<_Base, _Ty>::value)
			{
				return result[0];
			}
			else
			{
				return Ref<_Ty>::downcast(result[0]);
			}
		}

		return nullptr;
	}

	template<typename _Ty>
	inline const Ref<_Ty> getGameObjectById(const ID& id) const { return const_cast<GameObjectContainer*>(this)->getGameObjectById<_Ty>(id); }

	bool containsGameObject(const ID& id) const
	{
		return !_alloc.find([&id](const _Base& obj) { return obj.id() == id; }).empty();
	}

	template<typename _Ty>
	void destroyGameObject(Ref<_Ty>& ref)
	{
		static_assert(std::is_base_of<_Base, _Ty>::value);
		onDestroyGameObject(ref);
		_alloc.free(ref);
	}

	void forEachGameObject(const std::function<void(_Base&)>& action) { _alloc.forEach(action); }
	void forEachGameObject(const std::function<void(const _Base&)>& action) const { _alloc.forEach(action); }



	template<typename _Ty>
	inline Ref<_Ty> operator[] (const ID& id) { return getGameObjectById<_Ty>(id); }

	template<typename _Ty>
	inline Ref<_Ty> operator[] (const ID& id) const { return getGameObjectById<_Ty>(id); }

protected:
	MemoryAllocator<_Base>& gameObjectAllocator() { return _alloc; }
	const MemoryAllocator<_Base>& gameObjectAllocator() const { return _alloc; }
};

