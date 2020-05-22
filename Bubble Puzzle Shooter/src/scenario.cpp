#include "scenario.h"


BubbleColorSelector::BubbleColorSelector(const RNG& rand) :
	_rand{ rand },
	_colors{},
	_map{}
{}
BubbleColorSelector::~BubbleColorSelector() {}

const RNG& BubbleColorSelector::getRand() const { return _rand; }
void BubbleColorSelector::setRand(const RNG& rand) { _rand = rand; }

BubbleColor::Mask BubbleColorSelector::getAvailableColors() const { return _colors; }
void BubbleColorSelector::setAvailableColors(BubbleColor::Mask mask) { _colors = mask; }

bool BubbleColorSelector::isAvailableColor(const BubbleColor& color) const { return _colors & color; }
void BubbleColorSelector::setAvailableColor(const BubbleColor& color, bool enabled) { _colors = enabled ? _colors + color : _colors - color; }

const BubbleColor& BubbleColorSelector::select()
{
	return _map.empty()
		? BubbleColor::defaultColor()
		: _map[_rand(0, static_cast<RNG::RandomValue>(_map.size()))];
}

void BubbleColorSelector::updateMap()
{
	_map.clear();
	for (const auto& color : BubbleColor::all())
		_map.push_back(color);
}






BubbleGenerator::BubbleGenerator(const RandomBubbleModelSelector& arrowModels, const RandomBubbleModelSelector& boardModels, BubbleColor::Mask colors, RNG::Seed seed) :
	_colors{},
	_arrowRand{},
	_boardRand{},
	_arrowModels{ arrowModels },
	_boardModels{ boardModels },
	_lastColor{ BubbleColor::defaultColor() }
{
	setSeed(seed);
	setColors(colors);
}

BubbleColor::Mask BubbleGenerator::getColors() const { return _colors.getAvailableColors(); }
void BubbleGenerator::setColors(BubbleColor::Mask colors) { _colors.setAvailableColors(colors); }

const BubbleColorSelector& BubbleGenerator::getColorSelector() const { return _colors; }
BubbleColorSelector& BubbleGenerator::getColorSelector() { return _colors; }

void BubbleGenerator::setSeed(RNG::Seed seed)
{
	RNG rand{ seed };
	_colors.setRand(rand.randomRNG());
	_arrowRand = rand.randomRNG();
	_boardRand = rand.randomRNG();
}

const RandomBubbleModelSelector& BubbleGenerator::getModelSelector(bool arrow) const { return arrow ? _arrowModels : _boardModels; }
RandomBubbleModelSelector& BubbleGenerator::getModelSelector(bool arrow) { return arrow ? _arrowModels : _boardModels; }

const BubbleColor& BubbleGenerator::getLastColor() const { return _lastColor; }

RNG& BubbleGenerator::rand(bool arrow) { return arrow ? _arrowRand : _boardRand; }






HiddenBubbleContainer::HiddenRow::HiddenRow(const std::vector<BubbleIdentifier>& bubbles, UInt32 boardId, UInt8 row) :
	_bubbles{ bubbles },
	_count{ -1 },
	_row{ row },
	_boardId{ boardId }
{}
HiddenBubbleContainer::HiddenRow::HiddenRow(UInt32 boardId, UInt8 row) :
	HiddenRow{ {}, boardId, row }
{}

HiddenBubbleContainer::HiddenRow::operator bool() const { return !_bubbles.empty(); }
bool HiddenBubbleContainer::HiddenRow::operator! () const { _bubbles.empty(); }

bool HiddenBubbleContainer::HiddenRow::empty() const { return _bubbles.empty(); }

const std::vector<BubbleIdentifier>& HiddenBubbleContainer::HiddenRow::getBubbles() const { return _bubbles; }
void HiddenBubbleContainer::HiddenRow::setBubbles(const std::vector<BubbleIdentifier>& bubbles)
{
	_bubbles = bubbles;
	_count = -1;
}

UInt8 HiddenBubbleContainer::HiddenRow::getValidBubbleCount() const
{
	if (_count < 0)
	{
		Int8 ct = 0;
		for (const auto& bid : _bubbles)
			if (bid)
				ct++;
		const_cast<HiddenRow*>(this)->_count = ct;
	}
	return _count;
}

UInt8 HiddenBubbleContainer::HiddenRow::getRowId() const { return _row; }
void HiddenBubbleContainer::HiddenRow::setRowId(UInt8 row) { _row = row; }

UInt32 HiddenBubbleContainer::HiddenRow::getBoardId() const { return _boardId; }
void HiddenBubbleContainer::HiddenRow::setBoardId(UInt32 id) { _boardId = id; }

std::vector<Ref<Bubble>> HiddenBubbleContainer::HiddenRow::generate(BubbleHeap& heap, TextureManager& textures)
{
	if (!_bubbles.empty())
	{
		std::vector<Ref<Bubble>> bubs{ _bubbles.size() };
		for (const auto& bid : _bubbles)
		{
			bubs.push_back(heap.create(bid, textures, false));
		}
		return bubs;
	}
	return {};
}
