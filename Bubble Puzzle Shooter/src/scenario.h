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

	BubbleGenerator(const RandomBubbleModelSelector& arrowModels, const RandomBubbleModelSelector& boardModels, BubbleColor::Mask colors, RNG::Seed seed);

	BubbleColor::Mask getColors() const;
	void setColors(BubbleColor::Mask colors);

	const BubbleColorSelector& getColorSelector() const;
	BubbleColorSelector& getColorSelector();

	void setSeed(RNG::Seed seed);

	const RandomBubbleModelSelector& getModelSelector(bool arrow) const;
	RandomBubbleModelSelector& getModelSelector(bool arrow);

	const BubbleColor& getLastColor() const;

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
		HiddenBoard(HiddenBoard&&) = default;
		~HiddenBoard() = default;

		HiddenBoard& operator= (const HiddenBoard&) = default;
		HiddenBoard& operator= (HiddenBoard&&) = default;

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
};
