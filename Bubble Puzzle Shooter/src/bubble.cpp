#include "bubble.h"

#include "props.h"

#define BUBBLECOLOR_CODE_RED (0x1 << 0)
#define BUBBLECOLOR_CODE_ORANGE (0x1 << 1)
#define BUBBLECOLOR_CODE_YELLOW (0x1 << 2)
#define BUBBLECOLOR_CODE_GREEN (0x1 << 3)
#define BUBBLECOLOR_CODE_BLUE (0x1 << 4)
#define BUBBLECOLOR_CODE_PURPLE (0x1 << 5)
#define BUBBLECOLOR_CODE_GRAY (0x1 << 6)
#define BUBBLECOLOR_CODE_BLACK (0x1 << 7)



BubbleColor::BubbleColor() :
	_code{ 0 }
{}
BubbleColor::BubbleColor(UInt8 code) :
	_code{ code }
{}

BubbleColor::operator bool() const { return _code; }
bool operator! (const BubbleColor& left) { return !left._code; }

UInt8 BubbleColor::code() const { return _code; }
std::string BubbleColor::name() const
{
	switch (_code)
	{
		case BUBBLECOLOR_CODE_RED: return "red";
		case BUBBLECOLOR_CODE_ORANGE: return "orange";
		case BUBBLECOLOR_CODE_YELLOW: return "yellow";
		case BUBBLECOLOR_CODE_GREEN: return "green";
		case BUBBLECOLOR_CODE_BLUE: return "blue";
		case BUBBLECOLOR_CODE_PURPLE: return "purple";
		case BUBBLECOLOR_CODE_GRAY: return "gray";
		case BUBBLECOLOR_CODE_BLACK: return "black";
	}
	return "<invalid-color>";
}

BubbleColor::Mask BubbleColor::addToMask(BubbleColor::Mask mask) const { return mask | _code; }
BubbleColor::Mask BubbleColor::removeFromMask(BubbleColor::Mask mask) { return mask & ~_code; }
bool BubbleColor::hasInMask(BubbleColor::Mask mask) const { return mask & _code; }

BubbleColor::Mask operator+ (BubbleColor::Mask left, const BubbleColor& right) { return left | right._code; }
BubbleColor::Mask operator+ (const BubbleColor& left, BubbleColor::Mask right) { return right | left._code; }

BubbleColor::Mask operator- (BubbleColor::Mask left, const BubbleColor& right) { return left & ~right._code; }
BubbleColor::Mask operator- (const BubbleColor& left, BubbleColor::Mask right) { return right & ~left._code; }

bool operator& (BubbleColor::Mask left, const BubbleColor& right) { return (left & right._code) == left; }
bool operator& (const BubbleColor& left, BubbleColor::Mask right) { return (right & left._code) == right; }

BubbleColor::Mask& operator+= (BubbleColor::Mask& left, const BubbleColor& right) { return left = left + right; }
BubbleColor::Mask& operator-= (BubbleColor::Mask& left, const BubbleColor& right) { return left = left - right; }


const BubbleColor BubbleColor::Red{ BUBBLECOLOR_CODE_RED };
const BubbleColor BubbleColor::Orange{ BUBBLECOLOR_CODE_ORANGE };
const BubbleColor BubbleColor::Yellow{ BUBBLECOLOR_CODE_YELLOW };
const BubbleColor BubbleColor::Green{ BUBBLECOLOR_CODE_GREEN };
const BubbleColor BubbleColor::Blue{ BUBBLECOLOR_CODE_BLUE };
const BubbleColor BubbleColor::Purple{ BUBBLECOLOR_CODE_PURPLE };
const BubbleColor BubbleColor::Gray{ BUBBLECOLOR_CODE_GRAY };
const BubbleColor BubbleColor::Black{ BUBBLECOLOR_CODE_BLACK };

std::vector<BubbleColor> BubbleColor::all()
{
	return {
		BubbleColor::Red,
		BubbleColor::Orange,
		BubbleColor::Yellow,
		BubbleColor::Green,
		BubbleColor::Blue,
		BubbleColor::Purple,
		BubbleColor::Gray,
		BubbleColor::Black
	};
}
BubbleColor BubbleColor::defaultColor() { return BubbleColor::Blue; }




bool operator== (const BubbleModel& left, const BubbleModel& right) { return left.name == right.name; }
bool operator!= (const BubbleModel& left, const BubbleModel& right) { return left.name != right.name; }




BouncingBounds::BouncingBounds(Bubble& bubble) :
	_bubble{ bubble },
	_bounds{},
	_top{ false },
	_bottom{ false }
{}
BouncingBounds::~BouncingBounds() {}

void BouncingBounds::setBounds(const sf::IntRect& bounds) { _bounds = bounds; }
void BouncingBounds::setTopEnabled(bool flag) { _top = flag; }
void BouncingBounds::setBottomEnabled(bool flag) { _bottom = flag; }

BounceEdge BouncingBounds::check()
{
	Vec2f pos = _bubble.getPosition();
	Vec2f speed = _bubble.getSpeed();
	if (_top && pos.y - Bubble::Radius <= _bounds.top)
	{
		pos.y = static_cast<float>(_bounds.top + Bubble::Radius);
		speed.y *= -1;
		_bubble.setPosition(pos);
		_bubble.setSpeed(speed);
		return BounceEdge::Top;
	}
	if (_bottom && pos.y + Bubble::Radius >= _bounds.top + _bounds.height)
	{
		pos.y = static_cast<float>(_bounds.top + _bounds.height - Bubble::Radius);
		speed.y *= -1;
		_bubble.setPosition(pos);
		_bubble.setSpeed(speed);
		return BounceEdge::Bottom;
	}
	if (pos.x - Bubble::Radius <= _bounds.left)
	{
		pos.x = static_cast<float>(_bounds.left + Bubble::Radius);
		speed.x *= -1;
		_bubble.setPosition(pos);
		_bubble.setSpeed(speed);
		return BounceEdge::Left;
	}
	if (pos.x + Bubble::Radius >= _bounds.left + _bounds.width)
	{
		pos.x = static_cast<float>(_bounds.left + _bounds.width - Bubble::Radius);
		speed.x *= -1;
		_bubble.setPosition(pos);
		_bubble.setSpeed(speed);
		return BounceEdge::Right;
	}
	return BounceEdge::None;
}




Bubble::Bubble(const Ref<BubbleModel>& model, TextureManager& texs) :
	_model{ model },
	_exploited{ false },
	_speed{},
	_acceleration{},
	_allocPosition{},
	_allocCell{},
	_floatingCheckPhase{ false },
	_color{ BubbleColor::defaultColor() },
	_sprite{},
	_bounce{ *this },
	_localInts(model->localInts, 0),
	_localFloats(model->localFloats, 0.f),
	_localStrings(model->localStrings, "")
{}

Bubble::~Bubble() {}

Ref<BubbleModel> Bubble::getModel() const { return _model; }

bool Bubble::hasExploited() const { return _exploited; }

void Bubble::explode()
{
	//TODO: Implement
}

void Bubble::setSpeed(const Vec2f& speed) { _speed = speed; }
const Vec2f& Bubble::getSpeed() const { return _speed; }

void Bubble::setAcceleration(const Vec2f& acceleration) { _acceleration = acceleration; }
const Vec2f& Bubble::getAcceleration() const { return _acceleration; }

void Bubble::translate(const Vec2f& dp) { setPosition(getPosition() + dp); }
void Bubble::translate(float dx, float dy) { translate({ dx, dy }); }
void Bubble::move(const Vec2f& speed, const Vec2f& acceleration) { _speed = speed; _acceleration = acceleration; }

BubbleColor Bubble::getColor() const { return _color; }
BubbleColorType Bubble::getColorType() const { return _model->colorType; }

bool Bubble::colorMatch(const Ref<Bubble>& other) const
{
	switch (_model->colorType)
	{
		case BubbleColorType::Colorless:
			return false;

		case BubbleColorType::NormalColor:
			switch (other->_model->colorType)
			{
				case BubbleColorType::Colorless: return false;
				case BubbleColorType::MultiColor: return true;
				case BubbleColorType::NormalColor: return _color == other->_color;
			}

		case BubbleColorType::MultiColor:
			return other->_model->colorType != BubbleColorType::Colorless;
	}
	return false;
}

AnimatedSprite* Bubble::getSprite() { return &_sprite; }
const AnimatedSprite* Bubble::getSprite() const { return &_sprite; }


/* Model functions */
Int8 Bubble::getResistence() const { return _model->resistence; }
bool Bubble::isIndestructible() const { return _model->resistence < 0; }
bool Bubble::isFloating() const { return _model->floating; }
bool Bubble::destroyInBottom() const { return _model->destroyInBottom; }
bool Bubble::requireDestroyToClear() const { return _model->requireDestroyToClear; }
float Bubble::getPointsOfTurnsToDown() const { return _model->pointsOfTurnsToDown; }

Int32 Bubble::getLocalInt(UInt8 index) const { return _localInts[index]; }
float Bubble::getLocalFloat(UInt8 index) const { return _localFloats[index]; }
std::string Bubble::getLocalString(UInt8 index) const { return _localStrings[index]; }

void Bubble::setLocalInt(UInt8 index, Int32 value) { _localInts[index] = value; }
void Bubble::setLocalFloat(UInt8 index, const float& value) { _localFloats[index] = value; }
void Bubble::setLocalString(UInt8 index, const std::string& value) { _localStrings[index] = value; }

void Bubble::copyLocalInts(const std::vector<UInt32>& locals)
{
	size_t len = static_cast<size_t>(_model->localInts);
	if (locals.empty() || len < 1)
	{
		_localInts.clear();
		return;
	}

	size_t max = std::min(len, locals.size());
	_localInts.resize(max);
	std::copy(locals.begin(), locals.end(), _localInts.begin());
}
void Bubble::copyLocalFloats(const std::vector<float>& locals)
{
	size_t len = static_cast<size_t>(_model->localFloats);
	if (locals.empty() || len < 1)
	{
		_localFloats.clear();
		return;
	}

	size_t max = std::min(len, locals.size());
	_localFloats.resize(max);
	std::copy(locals.begin(), locals.end(), _localFloats.begin());
}
void Bubble::copyLocalStrings(const std::vector<std::string>& locals)
{
	size_t len = static_cast<size_t>(_model->localStrings);
	if (locals.empty() || len < 1)
	{
		_localStrings.clear();
		return;
	}

	size_t max = std::min(len, locals.size());
	_localStrings.resize(max);
	std::copy(locals.begin(), locals.end(), _localStrings.begin());
}





BubbleIdentifier::BubbleIdentifier() :
	_model{},
	_color{ BubbleColor::defaultColor() }
{}
BubbleIdentifier::BubbleIdentifier(const std::string& model, const BubbleColor& color) :
	_model{ model },
	_color{ color }
{}
BubbleIdentifier::~BubbleIdentifier() {}

BubbleIdentifier::operator bool() const { return !_model.empty(); }
bool operator! (const BubbleIdentifier & right) { return right._model.empty(); }

bool BubbleIdentifier::isInvalid() const { return _model.empty(); }

const std::string& BubbleIdentifier::model() const { return _model; }
void BubbleIdentifier::model(const std::string& model) { _model = model; }

const BubbleColor& BubbleIdentifier::color() const { return _color; }
void BubbleIdentifier::color(const BubbleColor& color) { _color = color; }

Ref<Bubble> BubbleIdentifier::createBubble(BubbleHeap& heap, TextureManager& textures, bool editorMode) const
{
	return heap.create(_model, textures, editorMode, _color);
}

BubbleIdentifier BubbleIdentifier::invalid() { return {}; }






BubbleModelManager::BubbleModelManager() :
	Manager{ nullptr },
	_defaultModel{}
{}
BubbleModelManager::~BubbleModelManager() {}

Ref<BubbleModel> BubbleModelManager::createModel(const std::string& name)
{
	auto model = Instance.create<BubbleModel>(name);
	model->name = name;
	return model;
}
Ref<BubbleModel> BubbleModelManager::getModel(const std::string& name)
{
	auto model = Instance.get(name);
	if (!model)
		return getDefaultModel();
	return model;
}
bool BubbleModelManager::hasModel(const std::string& name) { return Instance.has(name); }

Ref<BubbleModel> BubbleModelManager::getDefaultModel()
{
	if (Instance._defaultModel.empty())
		Instance._defaultModel = Instance.loadDefaultModel();
	return Instance.get(Instance._defaultModel);
}

std::string BubbleModelManager::loadDefaultModel() { return Props::getString("default_bubble_model", ""); }

BubbleModelManager BubbleModelManager::Instance{};

	





BubbleHeap::BubbleHeap() :
	MemoryAllocator{}
{}
BubbleHeap::~BubbleHeap() {}

Ref<Bubble> BubbleHeap::create(const std::string& modelName, TextureManager& textures, bool editorMode, const BubbleColor& color)
{
	auto model = BubbleModelManager::getModel(modelName);
	if (!model)
		return nullptr;

	auto bubble = alloc<Bubble>(model, textures);
	model->init(&bubble, color, editorMode);

	return bubble;
}
Ref<Bubble> BubbleHeap::create(const BubbleIdentifier& identifier, TextureManager& textures, bool editorMode)
{
	if (!identifier)
		return nullptr;

	return create(identifier.model(), textures, editorMode, identifier.color());
}
void BubbleHeap::destroy(const Ref<Bubble>& bub)
{
	free(bub);
}





void RandomBubbleModelSelector::setModelScore(const std::string& model, UInt16 score)
{
	if (score == 0)
	{
		auto it = _models.find(model);
		if (it != _models.end())
			_models.erase(it);
	}
	else _models[model] = score;
	_recompute = true;
}
UInt16 RandomBubbleModelSelector::getModelScore(const std::string& model) const
{
	auto it = _models.find(model);
	return it == _models.end() ? 0U : it->second;
}

Ref<BubbleModel> RandomBubbleModelSelector::selectModel(RNG& rand) const
{
	if (_score <= 0)
		return BubbleModelManager::getDefaultModel();

	RNG::RandomValue value = rand(0, _score);
	for (const auto& e : _models)
	{
		if (value < e.second)
			return BubbleModelManager::getModel(e.first);
		value -= e.second;
	}

	return BubbleModelManager::getDefaultModel();
}
void RandomBubbleModelSelector::computeScore() const
{
	if (_recompute)
	{
		RandomBubbleModelSelector& self = *const_cast<RandomBubbleModelSelector*>(this);

		RNG::RandomValue score = 0;
		for (const auto& e : _models)
			score += e.second;

		self._score = score;
		self._recompute = false;
	}
}
