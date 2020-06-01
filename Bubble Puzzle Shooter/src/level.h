#pragma once

#include "common.h"
#include "bubble.h"


typedef UInt32 Column;
typedef UInt32 Row;

namespace utils
{
	constexpr Column MinColumnCount = 8;
	constexpr Column MaxColumnCount = 16;

	constexpr Row VisibleRows = 14;
	constexpr Row BottomRows = 3;
	constexpr Row TopRows = 1;
	constexpr Row HiddenRows = (BottomRows + TopRows);
	constexpr Row TotalRows = VisibleRows + HiddenRows;
}

enum class BoardColumnStyle : Column
{
	Min = 8,
	VerySmall = 9,
	Small = 10,
	SmallMedium = 11,
	Medium = 12,
	LargeMedium = 13,
	Large = 14,
	VeryLarge = 15,
	Max = 16
};

namespace utils
{
	constexpr Column styleToColumn(BoardColumnStyle style) { return utils::clamp(static_cast<Column>(style), MinColumnCount, MaxColumnCount); }
	constexpr BoardColumnStyle columnToStyle(Column columnCount) { return static_cast<BoardColumnStyle>(utils::clamp(columnCount, MaxColumnCount, MaxColumnCount)); }

	constexpr bool isPairColumn(Column column) { return is_pair(column); }
	constexpr bool isPairColumn(BoardColumnStyle style) { return is_pair(styleToColumn(style)); }
}

constexpr BoardColumnStyle operator"" _cs(unsigned long long int value) { return utils::columnToStyle(static_cast<Column>(value)); }

enum class HiddenBubbleContainerType : UInt8
{
	Continuous = 0x0,
	Discrete = 0x1,
	Random_Continuous = Continuous | 0x2,
	Random_Discrete = Random_Continuous | Discrete,
	Endless_Continuous = Continuous | 0x4,
	Endless_Discrete = Endless_Continuous | Discrete,
	Endless_Random_Continuous = Random_Continuous | Endless_Continuous,
	Endless_Random_Discrete = Endless_Random_Continuous | Discrete
};
constexpr HiddenBubbleContainerType operator| (HiddenBubbleContainerType left, HiddenBubbleContainerType right)
{
	return static_cast<HiddenBubbleContainerType>(static_cast<UInt8>(left) | static_cast<UInt8>(right));
}
constexpr HiddenBubbleContainerType operator& (HiddenBubbleContainerType left, HiddenBubbleContainerType right)
{
	return static_cast<HiddenBubbleContainerType>(static_cast<UInt8>(left)& static_cast<UInt8>(right));
}

namespace utils
{
	constexpr bool containsType(HiddenBubbleContainerType base, HiddenBubbleContainerType typesToCheck) { return (base & typesToCheck) == typesToCheck; }
	constexpr bool isDiscrete(HiddenBubbleContainerType type) { return containsType(type, HiddenBubbleContainerType::Discrete); }
	constexpr bool isRandom(HiddenBubbleContainerType type) { return containsType(type, HiddenBubbleContainerType::Random_Continuous); }
	constexpr bool isEndless(HiddenBubbleContainerType type) { return containsType(type, HiddenBubbleContainerType::Endless_Continuous); }
}



class MetaGoals
{
private:
	std::map<BubbleIdentifier, UInt32> _bubgoals;
	UInt32 _clearedBoardCount;

public:
	MetaGoals();
	MetaGoals(const MetaGoals&) = default;
	MetaGoals(MetaGoals&&) = default;
	~MetaGoals();

	MetaGoals& operator= (const MetaGoals&) = default;
	MetaGoals& operator= (MetaGoals&&) = default;

	bool getCleanedBoard() const;
	UInt32 getCleanedBoardCount() const;
	void setCleanedBoardCount(UInt32 amount);

	UInt32 getBubbleGoals(const BubbleIdentifier& id) const;
	void setBubbleGoals(const BubbleIdentifier& id, UInt32 amount);

	void forEachBubbleGoal(const std::function<void(std::pair<const BubbleIdentifier, UInt32>)>& action);

	inline UInt32 getBubbleGoals(const std::string& model, const BubbleColor& color) { return getBubbleGoals({ model, color }); }
};



class BinaryBubbleBoard
{
private:
	utils::matrix<BubbleIdentifier> _rows;
	BoardColumnStyle _columns;

public:
	BinaryBubbleBoard(BoardColumnStyle columns);
	BinaryBubbleBoard(const BinaryBubbleBoard&) = default;
	BinaryBubbleBoard(BinaryBubbleBoard&&) = default;
	~BinaryBubbleBoard();

	BinaryBubbleBoard& operator= (const BinaryBubbleBoard&) = default;
	BinaryBubbleBoard& operator= (BinaryBubbleBoard&&) = default;

	void setColumnStyle(BoardColumnStyle style);
	BoardColumnStyle getColumnStyle() const;

	BubbleIdentifier& insertBubble(Row row, Column column, const BubbleIdentifier& bubble);
	BubbleIdentifier& insertBubble(Row row, Column column, const std::string& model, const BubbleColor& color);

	BubbleIdentifier& peekBubble(Row row, Column column);
	const BubbleIdentifier& peekBubble(Row row, Column column) const;

	BubbleIdentifier& operator[] (const std::pair<Row, Column>& index);
	const BubbleIdentifier& operator[] (const std::pair<Row, Column>& index) const;
};



class LevelProperties
{
private:
	BoardColumnStyle _columns = BoardColumnStyle::Min;
	PlayerId _playerid = PlayerId::Single;
	std::vector<BinaryBubbleBoard> _bubbles;
	HiddenBubbleContainerType _hideType = HiddenBubbleContainerType::Continuous;
	UInt32 _clearBoardsRequired = 0U;
	BubbleColor::Mask _availableColors = 0xFFU;
	RNG::Seed _seed = 0;
	UInt32 _initialBubbles = 0;
	bool _generateUpBubbles = false;
	RandomBubbleModelSelector _arrowBubbleSelector;
	RandomBubbleModelSelector _boardBubbleSelector;
	bool _roof = false;
	bool _remote = false;
	bool _enableTimer = true;
	bool _hideTimer = true;
	UInt32 _timerTurnTime = 10;
	UInt32 _timerEndTime = 90;
	TimerMode _timerMode = TimerMode::TURN;
	bool _enableBubbleSwap = true;
	std::string _background = "";
	std::string _music = "";
	MetaGoals _goals;

	RNG _rand;
	bool _randReady = false;

public:
	LevelProperties() = default;
	LevelProperties(const LevelProperties&) = default;
	LevelProperties(LevelProperties&&) = default;
	~LevelProperties() = default;

	LevelProperties& operator= (const LevelProperties&) = default;
	LevelProperties& operator= (LevelProperties&&) = default;

	BoardColumnStyle getColuns() const;
	void setColuns(BoardColumnStyle columns);

	PlayerId getPlayer() const;
	void setPlayer(PlayerId player);

	UInt32 getBubbleBoardCount() const;
	void setBubbleBoardCount(UInt32 count);
	
	const BinaryBubbleBoard& getBubbleBoard(UInt32 index) const;
	BinaryBubbleBoard& peekBubbleBoard(UInt32 index);

	HiddenBubbleContainerType getHiddenBubbleContainerType() const;
	void setHiddenBubbleContainerType(HiddenBubbleContainerType type);

	UInt32 getClearedBoardRequiredCount() const;
	void setClearedBoardRequiredCount(UInt32 amount);

	BubbleColor::Mask getEnabledColors() const;
	bool isColorEnabled(const BubbleColor& color) const;
	void setColorEnabled(const BubbleColor& color, bool enabled);

	RNG::Seed getSeed() const;
	bool isRandomSeed() const;
	void setSeed(RNG::Seed seed);
	void setSeedRandom();
	RNG generateRNG();

	UInt32 getInitialFilledRows() const;
	void setInitialFilledRows(UInt32 count);

	bool isBubbleGenerationEnabled() const;
	void setBubbleGenerationEnabled(bool enabled);

	const RandomBubbleModelSelector& getArrowModelSelector() const;
	const RandomBubbleModelSelector& getBoardModelSelector() const;
	RandomBubbleModelSelector& peekArrowModelSelector();
	RandomBubbleModelSelector& peekBoardModelSelector();

	bool isRoofEnabled() const;
	void setRoofEnabled(bool enabled);

	bool isRemoteBubblesEnabled() const;
	void setRemoteBubblesEnabled(bool enabled);

	bool isHideTimer() const;
	void setHideTimer(bool enabled);

	UInt32 getTimerTurnTime() const;
	void setTimerTurnTime(UInt32 seconds);

	UInt32 getTimerEndTime() const;
	void setTimerEndTime(UInt32 seconds);

	TimerMode getTimerMode() const;
	void setTimerMode(TimerMode mode);

	bool isBubbleSwapEnabled() const;
	void setBubbleSwapEnabled(bool enabled);

	const std::string& getBackground() const;
	void setBackground(const std::string& textureName);

	const MetaGoals& getGoals() const;
	MetaGoals& peekGoals();
};

