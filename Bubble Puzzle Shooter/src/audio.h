#pragma once

#include <SFML/Audio.hpp>

#include "manager.h"

using sf::Sound;

class Music
{
private:
	sf::Music* _music;

public:
	Music();
	~Music();

	Music(const Music&) = delete;
	Music(Music&&) = default;

	Music& operator= (const Music&) = delete;
	Music& operator= (Music&&) = default;

	void open(const std::string& filename);
	void close();
	bool isOpened() const;

	void play();
	void stop();
	void pause();

	operator bool() const;
	bool operator! () const;
};


class SoundManager : public Manager<Sound>
{
public:
	SoundManager(SoundManager* parent = nullptr);
	~SoundManager();

	bool load(const std::string& filepath, const std::string& name);

private:
	static SoundManager RootManager;

	explicit SoundManager(int);

public:
	static SoundManager& root();

public:
	NON_COPYABLE_MOVABLE(SoundManager);
};
