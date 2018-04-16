#pragma once

#include "Sprite.h"
#include "Actor.h"

public enum EItems
{
	//Money Items
	//COL 0
	MONEY_LARGE_BROZE, MONEY_LARGE_GOLD, MONEY_GEM01, MONEY_GOLD01, MONEY_GEM02, MONEY_DARK_GEM01, MONEY_DARK_GOLD01, MONEY_DARK_GEM02,
	MONEY_SMALL_BROZE, MONEY_SMALL_GOLD, MONEY_DIOMAND, MONEY_TEARDROP, MONEY_GEM03, MONEY_MONEY_DIOMAND, MONEY_DARK_TEARDROP, MONEY_DARK_GEM03,
	MONEY_TINY_BROZE, MONEY_TINY_GOLD, MONEY_RUBY, MONEY_GOLD02, MONEY_GEM04, MONEY_DARK_RUBY, MONEY_DARK_GOLD02, MONEY_DARK_GEM04,
	//COL 3
	MONEY_LARGE_PLATINUM, MONEY_LARGE_SILVER, MONEY_LARGE_ORANGE_COIN, MONEY_GEM05, MONEY_GEM06, MONEY_DARK_LARGE_ORANGE_COIN, MONEY_DARK_GEM05, MONEY_DARK_GEM06,
	MONEY_SMALL_PLATINUM, MONEY_SMALL_SILVER, MONEY_LARGE_PLATINUM_COIN, MONEY_GEM07, MONEY_GEM08, MONEY_DARK_LARGE_PLATINUM_COIN, MONEY_DARK_GEM07, MONEY_DARK_GEM08,
	MONEY_TINY_PLATINUM, MONEY_TINY_SILVER, MONEY_GEM09, MONEY_GEM10, MONEY_GEM11, MONEY_DARK_GEM09, MONEY_DARK_GEM10, MONEY_DARK_GEM11,
	//COL 6
	MONEY_GREEN_NOTES_LARGE, MONEY_RED_NOTES_LARGE, MONEY_GEM12, MONEY_GEM13, MONEY_BLANK01, MONEY_DARK_GEM12, MONEY_DARK_GEM13, MONEY_BLANK02,
	MONEY_GREEN_NOTE, MONEY_RED_NOTE, MONEY_GEM14, MONEY_GEM15, MONEY_BLANK03, MONEY_DARK_GEM14, MONEY_DARK_GEM15,

	//Money Items Length 
	MONEY_BLANK04,

	//Potions
	//COL 0
	POTION_RED, POTION_SMALL_DARK_BLUE, POTION_LARGE_GRAY, POTION_LARGE_GREEN_SQUARE, POTION_YELLOW_STINGS,
	POTION_PALE, POTION_SMALL_RED_AND_BLUE, POTION_LARGE_BLUE, POTION_HANDLE_BROWN, POTION_HANDLE_RED,
	POTION_ORANGE, POTION_SMALL_BLACK_RED, POTION_LARGE_BLACK, POTION_HANDLE_MAGIC_BLUE, POTION_HANDLE_GREY_AND_BLUE,
	//Col 3
	POTION_YELLOW, POTION_SMALL_PALE, POTION_LARGE_ORANGE, POTION_HANDLE_LIGHT, POTION_HANDLE_WATER,
	POTION_GREEN,

	//Enum Length
	ENUM_LENGTH_ITEMS
};

public enum EItemStats
{
	WORTH, HEALTH_INCRESS_FACTOR, HEALTH_DECRESS_FACTOR, SPEED_INCRESS_FACTOR, SPEED_DECRESS_FACTOR, INCRESS_ATTACK_DAMAGE, ENUM_LENGTH_ITEM_STATS
};

//==================================================================================
// This class is used for the items in the game and controls them
//==================================================================================
ref class Items : public Sprite
{
protected:
	EItems item;
	EItemStats itemEffect;
	array<Bitmap^>^ itemsSpriteSheets;
	array<int>^ stats;
	String^ name;
	bool isBeingGrabbed;
	bool isInInventory;
public:
	Items(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startItemsSpriteSheets, Logger ^ startLogger, EItems startItem);

	virtual void draw(int forcedX, int forcedY) override;
	virtual void actorPutUpItem();
	void actorDropItem(Point actorPosition);
	void actorGrabItem(Point actorPosition);

	array<int>^ getItemStats(EItems actor);
	bool getIsInventory() { return isInInventory; }
	EItems getItemType() { return item; }
	array<int>^ getItemStats() { return stats; }
	EItemStats getItemStatEfffect() { return itemEffect; }
	String^ getItemName() { return name; }
};

