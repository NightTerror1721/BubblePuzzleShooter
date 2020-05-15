#include "audio.h"

#include "resources.h"

Music::Music() :
	_music{ nullptr }
{}
Music::~Music()
{
	close();
}

void Music::open(const std::string& filename)
{
	if (!_music)
	{
		sf::Music* m = new sf::Music{};
		if (!m->openFromFile(ResourcePoint::Musics + filename))
			delete m;
		else _music = m;
	}
}
void Music::close()
{
	if (_music)
	{
		delete _music;
		_music = nullptr;
	}
}
bool Music::isOpened() const { return _music; }

void Music::play()
{
	if (_music)
		_music->play();
}
void Music::stop()
{
	if (_music)
		_music->stop();
}
void Music::pause()
{
	if (_music)
		_music->pause();
}

Music::operator bool() const { return _music; }
bool Music::operator! () const { return !_music; }





SoundManager::SoundManager(int) :
	Manager{ nullptr }
{}
SoundManager::SoundManager(SoundManager* parent) :
	Manager{ parent ? parent : &RootManager }
{}
SoundManager::~SoundManager() {}

bool SoundManager::load(const std::string& filepath, const std::string& name)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(ResourcePoint::Sounds + filepath))
		return false;

	auto sound = create<Sound>(name);
	sound->setBuffer(std::move(buffer));
	return true;
}

SoundManager& SoundManager::root() { return RootManager; }

SoundManager SoundManager::RootManager(0);
