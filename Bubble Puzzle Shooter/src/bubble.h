#pragma once

#include "assets.h"
#include "game_object.h"


class Bubble;
class BubbleHeap;

enum class BubbleColorType
{
	Colorless,
	NormalColor,
	MultiColor
};

enum class BounceEdge
{
	None,
	Top,
	Bottom,
	Left,
	Right
};



class BubbleColor
{
private:
	UInt8 _code;

public:
	BubbleColor();
	BubbleColor(const BubbleColor&) = default;
	BubbleColor(BubbleColor&&) = default;

	BubbleColor& operator= (const BubbleColor&) = default;
	BubbleColor& operator= (BubbleColor&&) = default;

	operator bool() const;
	friend bool operator! (const BubbleColor& left);

	auto operator<=> (const BubbleColor&) const = default;
	bool operator== (const BubbleColor&) const = default;

	bool isInvalid() const;
	bool isRandom() const;

	UInt8 code() const;
	std::string name() const;

public:
	typedef UInt8 Mask;

	Mask addToMask(Mask mask) const;
	Mask removeFromMask(Mask mask);
	bool hasInMask(Mask mask) const;

	friend Mask operator+ (Mask left, const BubbleColor& right);
	friend Mask operator+ (const BubbleColor& left, Mask right);

	friend Mask operator- (Mask left, const BubbleColor& right);
	friend Mask operator- (const BubbleColor& left, Mask right);

	friend bool operator& (Mask left, const BubbleColor& right);
	friend bool operator& (const BubbleColor& left, Mask right);

private:
	BubbleColor(UInt8 code);

public:
	static const BubbleColor Red;
	static const BubbleColor Orange;
	static const BubbleColor Yellow;
	static const BubbleColor Green;
	static const BubbleColor Blue;
	static const BubbleColor Purple;
	static const BubbleColor Gray;
	static const BubbleColor Black;

	static std::vector<BubbleColor> all();
	static BubbleColor defaultColor();
};

BubbleColor::Mask& operator+= (BubbleColor::Mask& left, const BubbleColor& right);
BubbleColor::Mask& operator-= (BubbleColor::Mask& left, const BubbleColor& right);




struct BubbleModel
{
	/* PROPERTIES */
	std::string name;

	BubbleColorType colorType;

	bool floating;
	bool destroyInBottom;
	bool requireDestroyToClear;
	bool onlyBoardColorInArrowGen;

	Int8 resistence;

	float pointsOfTurnsToDown;

	UInt8 localInts;
	UInt8 localFloats;
	UInt8 localStrings;


	/* FUNCTIONS */
	std::function<void(Bubble*, BubbleColor, bool)> init;

	std::function<void(Bubble*, Bubble*)> onCollide;
	std::function<void(Bubble*)> onInserted;
	std::function<void(Bubble*)> onExplode;
	std::function<void(Bubble*, Bubble*)> onNeighborInserted;
	std::function<void(Bubble*, Bubble*)> onNeighborExplode;

	friend bool operator== (const BubbleModel& left, const BubbleModel& right);
	friend bool operator!= (const BubbleModel& left, const BubbleModel& right);
};




class BouncingBounds
{
private:
	Bubble& _bubble;
	sf::IntRect _bounds;
	bool _top;
	bool _bottom;

public:
	BouncingBounds(Bubble& bubble);
	~BouncingBounds();

	void setBounds(const sf::IntRect& bounds);
	void setTopEnabled(bool flag);
	void setBottomEnabled(bool flag);

	BounceEdge check();
};




class Bubble : public Object, public sf::Transformable
{
public:
	static constexpr UInt32 Radius = 32;
	static constexpr UInt32 HitboxWith = 64;
	static constexpr UInt32 HitboxHeight = 54;

private:
	Ref<BubbleModel> _model;

	bool _exploited;

	Vec2f _speed;
	Vec2f _acceleration;

	Vec2f _allocPosition;
	Vec2i _allocCell;
	bool _floatingCheckPhase;

	BubbleColor _color;

	AnimatedSprite _sprite;

	BouncingBounds _bounce;

	std::vector<Int32> _localInts;
	std::vector<float> _localFloats;
	std::vector<std::string> _localStrings;

public:
	Bubble(const Ref<BubbleModel>& model, TextureManager& textures = TextureManager::root());
	virtual ~Bubble();

	Ref<BubbleModel> getModel() const;

	bool hasExploited() const;
	void explode();

	void setSpeed(const Vec2f& speed);
	const Vec2f& getSpeed() const;

	void setAcceleration(const Vec2f& acceleration);
	const Vec2f& getAcceleration() const;

	void translate(const Vec2f& dp);
	void translate(float dx, float dy);
	void move(const Vec2f& speed, const Vec2f& acceleration = {});

	BubbleColor getColor() const;
	BubbleColorType getColorType() const;

	bool colorMatch(const Ref<Bubble>& other) const;

	AnimatedSprite* getSprite();
	const AnimatedSprite* getSprite() const;


	/* Model functions */
	Int8 getResistence() const;
	bool isIndestructible() const;
	bool isFloating() const;
	bool destroyInBottom() const;
	bool requireDestroyToClear() const;
	float getPointsOfTurnsToDown() const;

	Int32 getLocalInt(UInt8 index) const;
	float getLocalFloat(UInt8 index) const;
	std::string getLocalString(const UInt8 index) const;

	void setLocalInt(UInt8 index, const Int32 value);
	void setLocalFloat(UInt8 index, const float& value);
	void setLocalString(UInt8 index, const std::string& value);

	void copyLocalInts(const std::vector<UInt32>& locals);
	void copyLocalFloats(const std::vector<float>& locals);
	void copyLocalStrings(const std::vector<std::string>& locals);
};




class BubbleIdentifier
{
private:
	std::string _model;
	BubbleColor _color;

public:
	BubbleIdentifier();
	BubbleIdentifier(const std::string& model, const BubbleColor& color);
	BubbleIdentifier(const BubbleIdentifier&) = default;
	BubbleIdentifier(BubbleIdentifier&&) = default;
	~BubbleIdentifier();

	BubbleIdentifier& operator= (const BubbleIdentifier&) = default;
	BubbleIdentifier& operator= (BubbleIdentifier&&) = default;

	operator bool() const;
	friend bool operator! (const BubbleIdentifier& right);

	bool operator== (const BubbleIdentifier&) const = default;
	auto operator<=> (const BubbleIdentifier&) const = default;

	bool isInvalid() const;

	const std::string& model() const;
	void model(const std::string& model);

	const BubbleColor& color() const;
	void color(const BubbleColor& color);

	Ref<Bubble> createBubble(BubbleHeap& heap, TextureManager& textures, bool editorMode) const;

	static BubbleIdentifier invalid();
};




class BubbleModelManager : private Manager<BubbleModel>
{
private:
	std::string _defaultModel;

public:
	~BubbleModelManager();

	static Ref<BubbleModel> createModel(const std::string& name);
	static Ref<BubbleModel> getModel(const std::string& name);
	static bool hasModel(const std::string& name);
	static Ref<BubbleModel> getDefaultModel();

private:
	static BubbleModelManager Instance;

	BubbleModelManager();

	static std::string loadDefaultModel();
};




class BubbleHeap : private MemoryAllocator<Bubble>
{
public:
	BubbleHeap();
	~BubbleHeap();

	Ref<Bubble> create(const std::string& modelName, TextureManager& textures, bool editorMode, const BubbleColor& color = BubbleColor::defaultColor());
	Ref<Bubble> create(const BubbleIdentifier& identifier, TextureManager& textures, bool editorMode);
	void destroy(const Ref<Bubble>& bub);
};




class RandomBubbleModelSelector
{
public:
	static constexpr UInt16 MaxScore = 1000;
	static constexpr UInt16 MinScore = 0;

private:
	std::map<std::string, UInt16> _models;
	RNG::RandomValue _score = 0;
	bool _recompute = true;

public:
	RandomBubbleModelSelector() = default;
	RandomBubbleModelSelector(const RandomBubbleModelSelector&) = default;
	RandomBubbleModelSelector(RandomBubbleModelSelector&&) = default;
	~RandomBubbleModelSelector() = default;

	RandomBubbleModelSelector& operator= (const RandomBubbleModelSelector&) = default;
	RandomBubbleModelSelector& operator= (RandomBubbleModelSelector&&) = default;

	void setModelScore(const std::string& model, UInt16 score);
	UInt16 getModelScore(const std::string& model) const;

	Ref<BubbleModel> selectModel(RNG& rand) const;

private:
	void computeScore() const;
};
