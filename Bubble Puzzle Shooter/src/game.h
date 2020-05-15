#pragma once

#include "common.h"
#include "game_object.h"
#include "py.h"

typedef decltype(sf::Style::Default) WindowStyle;

class GameController : public GameObjectContainer<GameObject>
{
private:
	bool _close;
	sf::RenderWindow _window;

	PyInterpreter _py;

	sf::Clock _deltaClock;
	sf::Time _phAccumulator;
	sf::Time _phUps;

	std::string _name;
	sf::VideoMode _vmode;
	WindowStyle _wstyle;

public:
	GameController(const std::string& name);
	~GameController();

	void start();

	void close();

	void setVideoMode(sf::VideoMode mode, bool apply = true);
	void setStyle(WindowStyle style, bool apply = true);
	void resetWindow();

	bool isFullscreen() const;

private:
	void loop();

	void init();
	void update(const sf::Time& delta);
	void render();
	void processEvents();
};
