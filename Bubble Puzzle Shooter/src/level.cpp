#include "level.h"


BinaryBubbleBoard::BinaryBubbleBoard(BoardColumnStyle columns) :
	_columns{ columns },
	_rows{ utils::static_matrix(utils::VisibleRows, utils::styleToColumn(columns), BubbleIdentifier::invalid()) }
{}
BinaryBubbleBoard::~BinaryBubbleBoard() {}

void BinaryBubbleBoard::setColumnStyle(BoardColumnStyle style)
{
	if (_columns != style)
	{
		_columns = style;
		size_t newsize = static_cast<size_t>(utils::styleToColumn(style));
		for (auto& row : _rows)
			row.resize(newsize, BubbleIdentifier::invalid());
	}
}
BoardColumnStyle BinaryBubbleBoard::getColumnStyle() const { return _columns; }

BubbleIdentifier& BinaryBubbleBoard::insertBubble(Row row, Column column, const BubbleIdentifier& bubble)
{
	return _rows[utils::clamp(row, 0U, utils::VisibleRows - 1)][utils::clamp(column, 0U, utils::styleToColumn(_columns))] = bubble;
}
BubbleIdentifier& BinaryBubbleBoard::insertBubble(Row row, Column column, const std::string& model, const BubbleColor& color)
{
	BubbleIdentifier& id = insertBubble(row, column, {});
	id.model(model);
	id.color(color);
	return id;
}

BubbleIdentifier& BinaryBubbleBoard::peekBubble(Row row, Column column)
{
	return _rows[utils::clamp(row, 0U, utils::VisibleRows - 1)][utils::clamp(column, 0U, utils::styleToColumn(_columns))];
}
const BubbleIdentifier& BinaryBubbleBoard::peekBubble(Row row, Column column) const
{
	return _rows[utils::clamp(row, 0U, utils::VisibleRows - 1)][utils::clamp(column, 0U, utils::styleToColumn(_columns))];
}

BubbleIdentifier& BinaryBubbleBoard::operator[] (const std::pair<Row, Column>& index)
{
	return _rows[utils::clamp(index.first, 0U, utils::VisibleRows - 1)][utils::clamp(index.second, 0U, utils::styleToColumn(_columns))];
}
const BubbleIdentifier& BinaryBubbleBoard::operator[] (const std::pair<Row, Column>& index) const
{
	return _rows[utils::clamp(index.first, 0U, utils::VisibleRows - 1)][utils::clamp(index.second, 0U, utils::styleToColumn(_columns))];
}

const std::vector<BubbleIdentifier>& BinaryBubbleBoard::peekRow(Row row) const { return _rows[utils::clamp(row, 0U, utils::VisibleRows - 1)]; }

const std::vector<BubbleIdentifier>& BinaryBubbleBoard::operator[] (Row row) const { return _rows[utils::clamp(row, 0U, utils::VisibleRows - 1)]; }





MetaGoals::MetaGoals() :
	_bubgoals{},
	_clearedBoardCount{ 0 }
{}
MetaGoals::~MetaGoals() {}

bool MetaGoals::getCleanedBoard() const { return _clearedBoardCount > 0; }
UInt32 MetaGoals::getCleanedBoardCount() const { return _clearedBoardCount; }
void MetaGoals::setCleanedBoardCount(UInt32 amount) { _clearedBoardCount = amount; }

UInt32 MetaGoals::getBubbleGoals(const BubbleIdentifier& id) const
{
	auto it = _bubgoals.find(id);
	return it == _bubgoals.end() ? 0U : it->second;
}
void MetaGoals::setBubbleGoals(const BubbleIdentifier& id, UInt32 amount)
{
	_bubgoals[id] = amount;
}

void MetaGoals::forEachBubbleGoal(const std::function<void(std::pair<const BubbleIdentifier, UInt32>)>& action)
{
	for (const auto& p : _bubgoals)
		action(p);
}







BoardColumnStyle LevelProperties::getColuns() const { return _columns; }
void LevelProperties::setColuns(BoardColumnStyle columns)
{
	_columns = columns;
	for (auto& bubs : _bubbles)
		bubs.setColumnStyle(columns);
}

PlayerId LevelProperties::getPlayer() const { return _playerid; }
void LevelProperties::setPlayer(PlayerId player) { _playerid = player; }

UInt32 LevelProperties::getBubbleBoardCount() const { return static_cast<UInt32>(_bubbles.size()); }
void LevelProperties::setBubbleBoardCount(UInt32 count)
{
	_bubbles.clear();
	_bubbles.resize(static_cast<size_t>(count), { _columns });
}

const BinaryBubbleBoard& LevelProperties::getBubbleBoard(UInt32 index) const { return _bubbles[static_cast<size_t>(index)]; }
BinaryBubbleBoard& LevelProperties::peekBubbleBoard(UInt32 index) { return _bubbles[static_cast<size_t>(index)]; }

HiddenBubbleContainerType LevelProperties::getHiddenBubbleContainerType() const { return _hideType; }
void LevelProperties::setHiddenBubbleContainerType(HiddenBubbleContainerType type) { _hideType = type; }

UInt32 LevelProperties::getClearedBoardRequiredCount() const { return _clearBoardsRequired; }
void LevelProperties::setClearedBoardRequiredCount(UInt32 amount) { _clearBoardsRequired = amount; }

BubbleColor::Mask LevelProperties::getEnabledColors() const { return _availableColors; }
bool LevelProperties::isColorEnabled(const BubbleColor& color) const { return _availableColors & color; }
void LevelProperties::setColorEnabled(const BubbleColor& color, bool enabled) { _availableColors = enabled ? _availableColors + color : _availableColors - color; }

RNG::Seed LevelProperties::getSeed() const { return _seed; }
bool LevelProperties::isRandomSeed() const { return _seed == 0; }
void LevelProperties::setSeed(RNG::Seed seed) { _seed = seed; _randReady = false; }
void LevelProperties::setSeedRandom() { _seed = 0; _randReady = false; }
RNG LevelProperties::generateRNG()
{
	if (!_randReady)
	{
		_rand = _seed;
		_randReady = true;
	}
	return _rand.randomRNG();
}

UInt32 LevelProperties::getInitialFilledRows() const { return _initialBubbles; }
void LevelProperties::setInitialFilledRows(UInt32 count) { _initialBubbles = count; }

bool LevelProperties::isBubbleGenerationEnabled() const { return _generateUpBubbles; }
void LevelProperties::setBubbleGenerationEnabled(bool enabled) { _generateUpBubbles = enabled; }

const RandomBubbleModelSelector& LevelProperties::getArrowModelSelector() const { return _arrowBubbleSelector; }
const RandomBubbleModelSelector& LevelProperties::getBoardModelSelector() const { return _boardBubbleSelector; }
RandomBubbleModelSelector& LevelProperties::peekArrowModelSelector() { return _arrowBubbleSelector; }
RandomBubbleModelSelector& LevelProperties::peekBoardModelSelector() { return _boardBubbleSelector; }

bool LevelProperties::isRoofEnabled() const { return _roof; }
void LevelProperties::setRoofEnabled(bool enabled) { _roof = enabled; }

bool LevelProperties::isRemoteBubblesEnabled() const { return _remote; }
void LevelProperties::setRemoteBubblesEnabled(bool enabled) { _remote = enabled; }

bool LevelProperties::isHideTimer() const { return _hideTimer; }
void LevelProperties::setHideTimer(bool enabled) { _hideTimer = enabled; }

UInt32 LevelProperties::getTimerTurnTime() const { return _timerTurnTime; }
void LevelProperties::setTimerTurnTime(UInt32 seconds) { _timerTurnTime = seconds; }

UInt32 LevelProperties::getTimerEndTime() const { return _timerEndTime; }
void LevelProperties::setTimerEndTime(UInt32 seconds) { _timerEndTime = seconds; }

TimerMode LevelProperties::getTimerMode() const { return _timerMode; }
void LevelProperties::setTimerMode(TimerMode mode) { _timerMode = mode; }

bool LevelProperties::isBubbleSwapEnabled() const { return _enableBubbleSwap; }
void LevelProperties::setBubbleSwapEnabled(bool enabled) { _enableBubbleSwap = enabled; }

const std::string& LevelProperties::getBackground() const { return _background; }
void LevelProperties::setBackground(const std::string& textureName) { _background = textureName; }

const MetaGoals& LevelProperties::getGoals() const { return _goals; }
MetaGoals& LevelProperties::peekGoals() { return _goals; }
