#pragma once

#include <random>

#include "common.h"
#include "manager.h"


using sf::Texture;
using sf::Sprite;

class TextureManager : public Manager<Texture>
{
public:
	TextureManager(TextureManager* parent = nullptr);

	bool load(const std::string& filepath, const std::string& tag);
	bool load(const std::string& filepath, const std::string& tag, const sf::IntRect& dims);
	bool load(const std::string& filepath, const std::string& tag, int x, int y, int width, int height);

private:
	static TextureManager RootManager;

	explicit TextureManager(int);

public:
	static TextureManager& root();

public:
	NON_COPYABLE_MOVABLE(TextureManager);
};



class AnimatedSprite : public Sprite, public Renderable, public Updatable
{
public:
	enum class Mode { Static, Sequence, Loop, Random };

private:
	UInt32 _x;
	UInt32 _y;
	UInt32 _w;
	UInt32 _h;
	UInt32 _frames;

	Mode _mode;

	float _min;
	float _max;
	float _current;
	std::minstd_rand _rand;

	float _it;
	float _oldIt;
	float _speed;

	bool _end;

public:
	AnimatedSprite();
	virtual ~AnimatedSprite();

	void setFrameDimensions(UInt32 x, UInt32 y, UInt32 w, UInt32 h);
	void setFrameCount(UInt32 frames_count);

	void setStaticMode();
	void setSequenceMode();
	void setLoopMode();
	void setRandomMode(float min, float max);

	bool isStaticMode() const;
	bool isSequenceMode() const;
	bool isLoopMode() const;
	bool isRandomMode() const;

	bool hasEnded() const;

	void start();
	void stop();

	void setCurrentFrame(UInt32 frame);
	void setExactCurrentFrame(float frame);

	UInt32 getCurrentFrame() const;
	float getExactCurrentFrame() const;

	void rewind();
	void fastForward();

	UInt32 getFrameX() const;
	UInt32 getFrameY() const;
	UInt32 getFrameWidth() const;
	UInt32 getFrameHeight() const;
	UInt32 getFrameCount() const;

	void update(const sf::Time& delta) override;
	void render(sf::RenderTarget& canvas) override;

private:
	int state() const;
	void generateCurrent();
	void updateIterator();
};
