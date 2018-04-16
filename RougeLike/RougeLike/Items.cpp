#include "Items.h"


//==================================================================================
// Constructor
//==================================================================================
Items::Items(Graphics^ startCanvas, TileMap^ startTilemap, Random^ startRGen, array<Bitmap^>^ startItemsSpriteSheets, Logger^ startLogger, EItems startItem) : Sprite(startCanvas, startTilemap, startRGen, startItemsSpriteSheets, startLogger)
{
	itemsSpriteSheets = startItemsSpriteSheets;
	item = startItem;
	stats = getItemStats(item);

	NotSpawnOnWalkable();
}//End Constructor

//==================================================================================
// Draws sprite to canvas
//==================================================================================
void Items::draw(int forcedX, int forcedY)
{
	//Does not draw the item if it's in someones inventory
	if (isInInventory == false)
	{
		//Creates a rectangle at a certain size for the spritesheet to use to draw only certain parts of the image
		Rectangle pixelsToDraw = Rectangle(0, 0, size.X, size.Y);

		//Draws the bitmap to the canvas but only draws a certain part of that bitmap using what was created in the pixelsToDraw above
		canvas->DrawImage(itemsSpriteSheets[currentFrame],
			forcedX,				// on canvas
			forcedY,				// on canvas
			pixelsToDraw,			// on source
			GraphicsUnit::Pixel);	//Measurment
	}
}

//==================================================================================
// Tells the pick it has been picked up
//==================================================================================
void Items::actorPutUpItem()
{
	isInInventory = true;
	visible = false;
}//End actorPutUpItem

//==================================================================================
// Tells the item it has been dropped and drops it at a given location
//==================================================================================
void Items::actorDropItem(Point actorPosition)
{
	position.X = actorPosition.X;
	position.Y = actorPosition.Y;

	isInInventory = false;
	visible = true;
}//End actorDropItem

//==================================================================================
// Tells the item it is being grabbed
//==================================================================================
void Items::actorGrabItem(Point actorPosition)
{
	isBeingGrabbed = true;
}//End actorGrabItem

//==================================================================================
// Gets the stats for the given item
//==================================================================================
array<int>^ Items::getItemStats(EItems item)
{
	array<int>^ stats = gcnew array<int>(EItemStats::ENUM_LENGTH_ITEM_STATS);

	// Defaults
	name = "Un-Named";
	stats[EItemStats::WORTH] = GameSettings::ITEM_WORTH_DEFAULT;
	stats[EItemStats::HEALTH_DECRESS_FACTOR] = 0;
	stats[EItemStats::HEALTH_INCRESS_FACTOR] = 0;
	stats[EItemStats::SPEED_DECRESS_FACTOR] = 0;
	stats[EItemStats::SPEED_INCRESS_FACTOR] = 0;
	stats[EItemStats::INCRESS_ATTACK_DAMAGE] = 0;
	itemEffect = EItemStats::WORTH;

	switch (item)
	{
	//MONEY
	case MONEY_LARGE_BROZE:
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 50;
		break;
	case MONEY_LARGE_GOLD:
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 100;
		break;
	case MONEY_GEM01:
	case MONEY_GEM02:
	case MONEY_GEM03:
	case MONEY_GEM04:
	case MONEY_GEM05:
	case MONEY_GEM06:
	case MONEY_GEM07:
	case MONEY_GEM08:
	case MONEY_GEM09:
	case MONEY_GEM10:
	case MONEY_GEM11:
	case MONEY_GEM12:
	case MONEY_GEM13:
	case MONEY_GEM14:
	case MONEY_GEM15:
	case MONEY_TEARDROP:
		name = "Gem";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 22;
		break;
	case MONEY_GOLD01:
	case MONEY_GOLD02:
		name = "Gold";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 30;
		break;
	case MONEY_DARK_GEM01:
	case MONEY_DARK_GEM02:
	case MONEY_DARK_GEM03:
	case MONEY_DARK_GEM04:
	case MONEY_DARK_GEM05:
	case MONEY_DARK_GEM06:
	case MONEY_DARK_GEM07:
	case MONEY_DARK_GEM08:
	case MONEY_DARK_GEM09:
	case MONEY_DARK_GEM10:
	case MONEY_DARK_GEM11:
	case MONEY_DARK_GEM12:
	case MONEY_DARK_GEM13:
	case MONEY_DARK_GEM14:
	case MONEY_DARK_GEM15:
	case MONEY_DARK_TEARDROP:
		name = "Dark Gem";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 12;
		break;
	case MONEY_DARK_GOLD01:
	case MONEY_DARK_GOLD02:
		name = "Dark Gold";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 15;
		break;
	case MONEY_SMALL_BROZE:
		name = "Bronze Coins";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 15;
		break;
	case MONEY_SMALL_GOLD:
		name = "Gold Coins";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 50;
		break;
	case MONEY_DIOMAND:
		name = "Diomand";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 1000;
		break;
	case MONEY_MONEY_DIOMAND:
		name = "Diomand";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 50;
		break;
	case MONEY_TINY_BROZE:
		name = "Bronze Coin";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 5;
		break;
	case MONEY_TINY_GOLD:
		name = "Gold Coin";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 15;
		break;
	case MONEY_RUBY:
		name = "Ruby";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 25;
		break;
	case MONEY_DARK_RUBY:
		name = "Dark Ruby";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 10;
		break;
	case MONEY_LARGE_PLATINUM:
		name = "Platinum Coin";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 200;
		break;
	case MONEY_LARGE_SILVER:
		name = "Sliver Coins";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 70;
		break;
	case MONEY_LARGE_ORANGE_COIN:
		name = "Orange Notes";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 24;
		break;
	case MONEY_DARK_LARGE_ORANGE_COIN:
		name = "Orange Coin";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 12;
		break;
	case MONEY_SMALL_PLATINUM:
		name = "Platinum Coins";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 25;
		break;
	case MONEY_SMALL_SILVER:
		name = "Sliver Coins";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 15;
		break;
	case MONEY_LARGE_PLATINUM_COIN:
		name = "Large Platinum Coin";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 75;
		break;
	case MONEY_DARK_LARGE_PLATINUM_COIN:
		name = "Large Dark Platinum Coin";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 45;
		break;
	case MONEY_TINY_PLATINUM:
		name = "Platinum Coin";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 15;
		break;
	case MONEY_TINY_SILVER:
		name = "Sliver Coin";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 5;
		break;
	case MONEY_GREEN_NOTES_LARGE:
		name = "Green Notes";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 100;
		break;
	case MONEY_RED_NOTES_LARGE:
		name = "Red Notes";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 100;
		break;
	case MONEY_GREEN_NOTE:
		name = "Green Note";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 10;
		break;
	case MONEY_RED_NOTE:
		name = "Red Note";
		itemEffect = EItemStats::WORTH;
		stats[EItemStats::WORTH] = 10;
		break;

	//POITIONS
	case POTION_RED:
	case POTION_LARGE_GRAY:
	case POTION_LARGE_GREEN_SQUARE:
	case POTION_YELLOW_STINGS:
	case POTION_PALE:
		name = "Incress Attack Damage";
		stats[EItemStats::INCRESS_ATTACK_DAMAGE] = 3;
		break;
	case POTION_SMALL_RED_AND_BLUE:
	case POTION_LARGE_BLUE:
	case POTION_HANDLE_BROWN:
	case POTION_HANDLE_RED:
	case POTION_HANDLE_WATER:
	case POTION_GREEN:
		name = "Healing Potion";
		itemEffect = EItemStats::HEALTH_INCRESS_FACTOR;
		stats[EItemStats::HEALTH_INCRESS_FACTOR] = 40;
		break;
	case POTION_SMALL_DARK_BLUE:
	case POTION_SMALL_BLACK_RED:
		name = "Decress Health Potion";
		itemEffect = EItemStats::HEALTH_DECRESS_FACTOR;
		stats[EItemStats::HEALTH_DECRESS_FACTOR] = 50;
		break;
	case POTION_YELLOW:
	case POTION_SMALL_PALE:
	case POTION_LARGE_ORANGE:
	case POTION_HANDLE_LIGHT:
		name = "Speed Incress Potion";
		itemEffect = EItemStats::SPEED_INCRESS_FACTOR;
		stats[EItemStats::SPEED_INCRESS_FACTOR] = 1;
		break;
	case POTION_ORANGE:
	case POTION_LARGE_BLACK:
	case POTION_HANDLE_MAGIC_BLUE:
	case POTION_HANDLE_GREY_AND_BLUE:
		name = "Speed Decress Potion";
		itemEffect = EItemStats::SPEED_DECRESS_FACTOR;
		stats[EItemStats::SPEED_DECRESS_FACTOR] = 1;
		break;
	default:
		break;
	}

	return stats;
}//End getItemStats