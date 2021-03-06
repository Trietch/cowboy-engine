#pragma once

#include <bitset>
#include <array>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

#include "utils.h"

class CameraComponent;
class Component;
class Entity;

using ID = std::uint64_t;

inline ID getComponentTypeID()
{
	static ID lastID = 0;	
	return lastID++;
}

template <typename T> inline ID getComponentTypeID() noexcept
{
	static ID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::uint64_t maxComponents = 64;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component *, maxComponents>;

class Component
{
public:
	Entity* entity = nullptr;

	Component()
	: _id { _counter++ }
	{}

	virtual void init() {}
	virtual void update([[maybe_unused]] double __deltaTime) {}
	virtual void draw() {}
	virtual ~Component() {}

	ID getComponentID() const { return _id; }

	static ID _counter;

private:
	ID _id;
};

class Entity
{
public:
	Entity();

	ID getEntityID() const;
	void update(double __deltaTime);
	void init();
	void draw();

	bool isActive() const;
	void destroy();

	template <typename T> bool hasComponent() const
	{
		return _componentBitSet[getComponentTypeID<T>()];
	}
	template <typename T, typename... TArgs>
	T& addComponent(TArgs... mArgs)
	{
		T* c {new T(std::forward<TArgs>(mArgs)...)};
		c->entity = this;
		std::unique_ptr<Component> uPtr {c};
		_components.emplace_back(std::move(uPtr));

		auto componentTypeID = getComponentTypeID<T>();
		_componentArray[componentTypeID] = c;
		_componentBitSet[componentTypeID] = true;

		c->init();
		return *c;
	}
	template <typename T> T& getComponent() const
	{
		auto ptr {_componentArray[getComponentTypeID<T>()]};
		return *static_cast<T*>(ptr);
	}

	static ID _counter;

private:
	bool _active = true;
	std::vector<std::unique_ptr<Component>> _components;

	ID _id;
	ComponentArray _componentArray;
	ComponentBitSet _componentBitSet;

};

class ECS_Manager
{
public:
	void update(double __deltaTime);
	void draw();
	void refresh();
	Entity & addEntity();

private:
	std::vector<std::unique_ptr<Entity>> _entities;
};
