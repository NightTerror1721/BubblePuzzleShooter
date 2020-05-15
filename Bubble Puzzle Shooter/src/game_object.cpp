#include "game_object.h"

GameObject::GameObject() :
	Object{},
	_tag{}
{}
GameObject::~GameObject() {}

const std::string& GameObject::tag() const { return _tag; }
void GameObject::tag(const std::string& tag) { _tag = tag; }

bool GameObject::hasAttached() const { return _gc; }

GameController& GameObject::getGameController() { return *_gc; }
const GameController& GameObject::getGameController() const { return *_gc; }
