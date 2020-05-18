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
	//std::map<BubbleIdentifier, >
};



class BinaryBubbleBoard
{
private:
	utils::matrix<MetaBubble> _rows;
	BoardColumnStyle _columns;

public:
	BinaryBubbleBoard(BoardColumnStyle columns);
	BinaryBubbleBoard(const BinaryBubbleBoard&) = default;
	BinaryBubbleBoard(BinaryBubbleBoard&&) = default;
	~BinaryBubbleBoard();

	BinaryBubbleBoard& operator= (const BinaryBubbleBoard&) = default;
	BinaryBubbleBoard& operator= (BinaryBubbleBoard&&) = default;

	MetaBubble& insertBubble(Row row, Column column, const MetaBubble& bubble);
	MetaBubble& insertBubble(Row row, Column column, const std::string& model, const BubbleColor& color);

	MetaBubble& seekBubble(Row row, Column column);
	const MetaBubble& seekBubble(Row row, Column column) const;

	MetaBubble& operator[] (const std::pair<Row, Column>& index);
	const MetaBubble& operator[] (const std::pair<Row, Column>& index) const;
};

