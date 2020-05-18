#include "level.h"


BinaryBubbleBoard::BinaryBubbleBoard(BoardColumnStyle columns) :
	_columns{ columns },
	_rows{ utils::static_matrix(utils::VisibleRows, utils::styleToColumn(columns), MetaBubble{}) }
{}
BinaryBubbleBoard::~BinaryBubbleBoard() {}

MetaBubble& BinaryBubbleBoard::insertBubble(Row row, Column column, const MetaBubble& bubble)
{
	return _rows[utils::clamp(row, 0U, utils::VisibleRows - 1)][utils::clamp(column, 0U, utils::styleToColumn(_columns))] = bubble;
}
MetaBubble& BinaryBubbleBoard::insertBubble(Row row, Column column, const std::string& model, const BubbleColor& color)
{
	MetaBubble& b = insertBubble(row, column, {});
	b.model(model);
	b.color(color);
	return b;
}

MetaBubble& BinaryBubbleBoard::seekBubble(Row row, Column column)
{
	return _rows[utils::clamp(row, 0U, utils::VisibleRows - 1)][utils::clamp(column, 0U, utils::styleToColumn(_columns))];
}
const MetaBubble& BinaryBubbleBoard::seekBubble(Row row, Column column) const
{
	return _rows[utils::clamp(row, 0U, utils::VisibleRows - 1)][utils::clamp(column, 0U, utils::styleToColumn(_columns))];
}

MetaBubble& BinaryBubbleBoard::operator[] (const std::pair<Row, Column>& index)
{
	return _rows[utils::clamp(index.first, 0U, utils::VisibleRows - 1)][utils::clamp(index.second, 0U, utils::styleToColumn(_columns))];
}
const MetaBubble& BinaryBubbleBoard::operator[] (const std::pair<Row, Column>& index) const
{
	return _rows[utils::clamp(index.first, 0U, utils::VisibleRows - 1)][utils::clamp(index.second, 0U, utils::styleToColumn(_columns))];
}
