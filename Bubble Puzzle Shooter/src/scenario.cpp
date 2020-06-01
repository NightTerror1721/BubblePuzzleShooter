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
	static const auto defaultColor = BubbleColor::defaultColor();
	return _map.empty()
		? defaultColor
		: _map[_rand(0, static_cast<RNG::RandomValue>(_map.size()))];
}

void BubbleColorSelector::updateMap()
{
	_map.clear();
	for (const auto& color : BubbleColor::all())
		_map.push_back(color);
}






void BubbleGenerator::setup(LevelProperties& props)
{
	_colors.setRand(props.generateRNG());
	_arrowRand = props.generateRNG();
	_boardRand = props.generateRNG();
	_arrowModels = props.getArrowModelSelector();
	_boardModels = props.getBoardModelSelector();
	_lastColor = BubbleColor::defaultColor();
	setColors(props.getEnabledColors());
}

BubbleColor::Mask BubbleGenerator::getColors() const { return _colors.getAvailableColors(); }
void BubbleGenerator::setColors(BubbleColor::Mask colors) { _colors.setAvailableColors(colors); }

const BubbleColorSelector& BubbleGenerator::getColorSelector() const { return _colors; }
BubbleColorSelector& BubbleGenerator::getColorSelector() { return _colors; }

const RandomBubbleModelSelector& BubbleGenerator::getModelSelector(bool arrow) const { return arrow ? _arrowModels : _boardModels; }
RandomBubbleModelSelector& BubbleGenerator::getModelSelector(bool arrow) { return arrow ? _arrowModels : _boardModels; }

const BubbleColor& BubbleGenerator::getLastColor() const { return _lastColor; }

RNG& BubbleGenerator::rand(bool arrow) { return arrow ? _arrowRand : _boardRand; }






HiddenBubbleContainer::HiddenRow::HiddenRow(const std::vector<BubbleIdentifier>& bubbles) :
	_bubbles{ bubbles },
	_count{ -1 }
{}

HiddenBubbleContainer::HiddenRow::operator bool() const { return !_bubbles.empty(); }
bool HiddenBubbleContainer::HiddenRow::operator! () const { return _bubbles.empty(); }

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

std::vector<Ref<Bubble>> HiddenBubbleContainer::HiddenRow::generate(BubbleHeap& heap, TextureManager& textures) const
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




HiddenBubbleContainer::HiddenBoard::operator bool() const { return !_rows.empty(); }
bool HiddenBubbleContainer::HiddenBoard::operator! () const { return _rows.empty(); }

bool HiddenBubbleContainer::HiddenBoard::empty() const { return _rows.empty(); }

UInt32 HiddenBubbleContainer::HiddenBoard::getValidBubbleCount() const
{
	if (_modified)
	{
		UInt32 count = 0;
		for (const auto& row : _rows)
			count += static_cast<UInt32>(row.getValidBubbleCount());
		const_cast<HiddenBoard*>(this)->_count = count;
		const_cast<HiddenBoard*>(this)->_modified = false;
	}
	return _count;
}

void HiddenBubbleContainer::HiddenBoard::trimTop()
{
	while (!_rows.empty() && _rows.back().empty())
		_rows.pop_back();
}

void HiddenBubbleContainer::HiddenBoard::addRow(const std::vector<BubbleIdentifier>& bubbles)
{
	if (_rows.size() < utils::VisibleRows)
	{
		_modified = true;
		_rows.emplace_back(bubbles);
	}
}

std::vector<Ref<Bubble>> HiddenBubbleContainer::HiddenBoard::extractGeneratedRow(BubbleHeap& heap, TextureManager& textures)
{
	if (!_rows.empty())
	{
		_modified = true;
		auto row = _rows.front().generate(heap, textures);
		_rows.pop_front();
		return row;
	}
	return {};
}
std::vector<std::vector<Ref<Bubble>>> HiddenBubbleContainer::HiddenBoard::extractAllGeneratedRows(BubbleHeap& heap, TextureManager& textures)
{
	if (!_rows.empty())
	{
		std::vector<std::vector<Ref<Bubble>>> board{ _rows.size() };
		_modified = true;
		for (const auto& hrow : _rows)
			board.push_back(hrow.generate(heap, textures));
		_rows.clear();
		return board;
	}
	return {};
}







HiddenBubbleContainer::operator bool() const { return !_boards.empty() || (_current && !_current->empty()); }
bool HiddenBubbleContainer::operator! () const { return _boards.empty() && (!_current || _current->empty()); }

bool HiddenBubbleContainer::empty() const { return _boards.empty() && (!_current || _current->empty()); }
bool HiddenBubbleContainer::isDiscrete() const { return utils::isDiscrete(_type); }

void HiddenBubbleContainer::setup(LevelProperties& props)
{

}

void HiddenBubbleContainer::fill(const std::vector<BinaryBubbleBoard>& boards);

std::vector<std::vector<Ref<Bubble>>> HiddenBubbleContainer::generate(BubbleHeap& heap, TextureManager& textures);

std::vector<std::vector<Ref<Bubble>>> HiddenBubbleContainer::generateBoard(BubbleHeap& heap, TextureManager& textures);

std::vector<Ref<Bubble>> HiddenBubbleContainer::generateRow(BubbleHeap& heap, TextureManager& textures);

UInt32 HiddenBubbleContainer::getValidBubbleCount() const;

void HiddenBubbleContainer::checkNext();
void HiddenBubbleContainer::addHiddenRow(HiddenBoard& board, const BinaryBubbleBoard& bbb);
