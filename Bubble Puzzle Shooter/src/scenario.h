#pragma once

#include <deque>

#include "level.h"

class BubbleColorSelector
{
private:
	RNG _rand;
	BubbleColor::Mask _colors;
	std::vector<BubbleColor> _map;

public:
	BubbleColorSelector() = default;
	BubbleColorSelector(const BubbleColorSelector&) = default;
	BubbleColorSelector(BubbleColorSelector&&) = default;

	BubbleColorSelector& operator= (const BubbleColorSelector&) = default;
	BubbleColorSelector& operator= (BubbleColorSelector&&) = default;

	BubbleColorSelector(const RNG& rand);
	~BubbleColorSelector();

	const RNG& getRand() const;
	void setRand(const RNG& rand);

	BubbleColor::Mask getAvailableColors() const;
	void setAvailableColors(BubbleColor::Mask mask);

	bool isAvailableColor(const BubbleColor& color) const;
	void setAvailableColor(const BubbleColor& color, bool enabled);

	const BubbleColor& select();

private:
	void updateMap();
};




class BubbleGenerator
{
private:
	BubbleHeap _heap;
	BubbleColorSelector _colors;
	RNG _arrowRand;
	RNG _boardRand;
	RandomBubbleModelSelector _arrowModels;
	RandomBubbleModelSelector _boardModels;
	BubbleColor _lastColor = BubbleColor::defaultColor();

public:
	BubbleGenerator() = default;
	BubbleGenerator(const BubbleGenerator&) = default;
	BubbleGenerator(BubbleGenerator&&) = default;
	~BubbleGenerator() = default;

	BubbleGenerator& operator= (const BubbleGenerator&) = default;
	BubbleGenerator& operator= (BubbleGenerator&&) = default;

	void setup(LevelProperties& props);

	BubbleColor::Mask getColors() const;
	void setColors(BubbleColor::Mask colors);

	const BubbleColorSelector& getColorSelector() const;
	BubbleColorSelector& getColorSelector();

	const RandomBubbleModelSelector& getModelSelector(bool arrow) const;
	RandomBubbleModelSelector& getModelSelector(bool arrow);

	const BubbleColor& getLastColor() const;

	Ref<Bubble> generateFromIdentifier(const BubbleIdentifier& id, TextureManager& textures);

private:
	RNG& rand(bool arrow);
};




class HiddenBubbleContainer
{
private:
	class HiddenRow
	{
	private:
		std::vector<BubbleIdentifier> _bubbles;
		Int8 _count = -1;

	public:
		HiddenRow() = default;
		HiddenRow(const HiddenRow&) = default;
		HiddenRow(HiddenRow&&) = default;
		~HiddenRow() = default;

		HiddenRow& operator= (const HiddenRow&) = default;
		HiddenRow& operator= (HiddenRow&&) = default;

		HiddenRow(const std::vector<BubbleIdentifier>& bubbles);

		operator bool() const;
		bool operator! () const;

		bool empty() const;

		const std::vector<BubbleIdentifier>& getBubbles() const;
		void setBubbles(const std::vector<BubbleIdentifier>& bubbles);

		UInt8 getValidBubbleCount() const;

		std::vector<Ref<Bubble>> generate(BubbleHeap& heap, TextureManager& textures) const;
	};

	class HiddenBoard
	{
	private:
		std::deque<HiddenRow> _rows;
		UInt32 _count = 0;
		bool _modified = false;

	public:
		HiddenBoard() = default;
		HiddenBoard(const HiddenBoard&) = default;
		HiddenBoard(HiddenBoard&&) noexcept = default;
		~HiddenBoard() = default;

		HiddenBoard& operator= (const HiddenBoard&) = default;
		HiddenBoard& operator= (HiddenBoard&&) noexcept = default;

		operator bool() const;
		bool operator! () const;

		bool empty() const;

		UInt32 getValidBubbleCount() const;

		void trimTop();

		void addRow(const std::vector<BubbleIdentifier>& bubbles);

		std::vector<Ref<Bubble>> extractGeneratedRow(BubbleHeap& heap, TextureManager& textures);
		std::vector<std::vector<Ref<Bubble>>> extractAllGeneratedRows(BubbleHeap& heap, TextureManager& textures);

		inline void addEmptyRow() { addRow({}); }
	};

private:
	std::deque<HiddenBoard> _boards;
	ReferenceAllocator<HiddenBoard> _current;
	BoardColumnStyle _columns = BoardColumnStyle::Min;
	HiddenBubbleContainerType _type = HiddenBubbleContainerType::Continuous;
	RNG _rand;

public:
	HiddenBubbleContainer() = default;
	HiddenBubbleContainer(const HiddenBubbleContainer&) = default;
	HiddenBubbleContainer(HiddenBubbleContainer&&) = default;

	HiddenBubbleContainer& operator= (const HiddenBubbleContainer&) = default;
	HiddenBubbleContainer& operator= (HiddenBubbleContainer&&) = default;

	operator bool() const;
	bool operator! () const;

	bool empty() const;
	bool isDiscrete() const;

	void setup(LevelProperties& props);

	void fill(const std::vector<BinaryBubbleBoard>& boards);

	std::vector<std::vector<Ref<Bubble>>> generate(BubbleGenerator& bgen, TextureManager& textures);

	std::vector<std::vector<Ref<Bubble>>> generateBoard(BubbleGenerator& bgen, TextureManager& textures);

	std::vector<Ref<Bubble>> generateRow(BubbleGenerator& bgen, TextureManager& textures);

	UInt32 getValidBubbleCount() const;

private:
	void checkNext();
	void addHiddenBoard(std::vector<HiddenBoard>& aux, const BinaryBubbleBoard& bbb);
	void addHiddenRow(HiddenBoard& board, const std::vector<BubbleIdentifier>& brow, Row row);
};
