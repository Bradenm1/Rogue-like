#include "Objects.h"


//==================================================================================
// Constructor
//==================================================================================
Objects::Objects(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger^ startLogger, EObjects startObject, EObjectType startTypeObject) : Sprite(startCanvas, startTilemap, startRGen, startSpriteSheets, startLogger)
{
	object = startObject;
	typeObject = startTypeObject;

	// Get the objects stats
	getObjectStats();

	nFrames = GameSettings::NUMBER_OF_FRAMES_AI;

	// Makes the object not spawn on unwalkable areas
	NotSpawnOnWalkable();
}//End Constructor

//==================================================================================
// Draws the object to the screen
//==================================================================================
void Objects::draw(int forcedX, int forcedY)
{
	//Creates a rectangle at a certain size for the spritesheet to use to draw only certain parts of the image
	Rectangle pixelsToDraw = Rectangle(0, 0, size.X, size.Y);

	//Draws the bitmap to the canvas but only draws a certain part of that bitmap using what was created in the pixelsToDraw above
	canvas->DrawImage(spriteSheets[currentFrame],
		forcedX,				// on canvas
		forcedY,				// on canvas
		pixelsToDraw,			// on source
		GraphicsUnit::Pixel);	//Measurment
}//End draw

//==================================================================================
// Moves the object given the velocity
//==================================================================================
void Objects::move()
{	
	position.X += velocity.X;
	position.Y += velocity.Y;
}//End move

//==================================================================================
// Check if the object can be touched
//==================================================================================
bool Objects::objectTouch()
{
	// Check if it's a trap
	if (typeObject == EObjectType::TRAP)
	{
		// Check if it's on the right frame in-order to deal damage
		if (currentFrame == frameToDamageOn)
		{
			return true;
		}
	}
	return false;
}//End objectTouch

//==================================================================================
// Check if object can be used
//==================================================================================
bool Objects::objectUse()
{
	if (typeObject == EObjectType::DOOR)
	{
		return true;
	}
	return false;
}//End objectUse

//==================================================================================
// Gets the objects stats
//==================================================================================
void Objects::getObjectStats()
{
	frameToDamageOn = 0;

	switch (object)
	{
		case BASIC_WOOD_DOOR:
		case BASIC_STEEL_DOOR:
		case BASIC_JAIL_DOOR_STEEL:
		case BASIC_JAIL_DOOR_GOLD:
		case BLANK_01:
		case STEEL_PORTAL_DOOR_CLOSED:
		case SIDE_BASIC_WOOD_DOOR:
		case SIDE_BASIC_STEEL_DOOR:
		case SIDE_BASIC_JAIL_DOOR_STEEL:
		case SIDE_BASIC_JAIL_DOOR_GOLD:
		case BLANK_02:
		case STEEL_PORTAL_DOOR_OPEN:
		case STEEL_CHAINED_BASIC_WOOD_DOOR:
		case STEEL_CHAINED_BASIC_STEEL_DOOR:
		case STEEL_CHAINED_BASIC_JAIL_DOOR_STEEL:
		case STEEL_CHAINED_BASIC_JAIL_DOOR_GOLD:
		case STEEL_CHAINS:
		case GOLD_PORTAL_DOOR_CLOSED:
		case STEEL_CHAINED_SIDE_BASIC_WOOD_DOOR:
		case STEEL_CHAINED_SIDE_BASIC_STEEL_DOOR:
		case STEEL_CHAINED_SIDE_BASIC_JAIL_DOOR_STEEL:
		case STEEL_CHAINED_SIDE_BASIC_JAIL_DOOR_GOLD:
		case STEEL_SIDE_CHAINS:
		case GOLD_PORTAL_DOOR_OPEN:
		case GOLD_CHAINED_BASIC_WOOD_DOOR:
		case GOLD_CHAINED_BASIC_STEEL_DOOR:
		case GOLD_CHAINED_BASIC_JAIL_DOOR_STEEL:
		case GOLD_CHAINED_BASIC_JAIL_DOOR_GOLD:
		case GOLD_CHAINS:
		case SKY_PORTAL_DOOR_CLOSED:
		case GOLD_CHAINED_SIDE_BASIC_WOOD_DOOR:
		case GOLD_CHAINED_SIDE_BASIC_STEEL_DOOR:
		case GOLD_CHAINED_SIDE_BASIC_JAIL_DOOR_STEEL:
		case GOLD_CHAINED_SIDE_BASIC_JAIL_DOOR_GOLD:
		case GOLD_SIDE_CHAINS:
		case SKY_PORTAL_DOOR_OPEN:
		case BASIC_WOOD_DOOR_FRAME:
		case BASIC_STEEL_DOOR_FRAME:
		case BASIC_JAIL_DOOR_STEEL_FRAME:
		case BASIC_JAIL_DOOR_GOLD_FRAME:
		case HELL_PORTAL_DOOR_CLOSED:
		case BASIC_WOOD_DOOR_BROKEN:
		case BASIC_STEEL_DOOR_BROKEN:
		case BASIC_JAIL_DOOR_STEEL_BROKEN:
		case BASIC_JAIL_DOOR_GOLD_BROKEN:
		case HELL_PORTAL_DOOR_OPEN:
			DamageType = EDamageType::NON;
			break;
		case ARROW_TRAP_ICON:
		case WATER_TRAP_ICON:
		case ICEING_TRAP_ICON:
		case SHOCKING_TRAP_ICON:
		case HEATING_TRAP_ICON:
		case DONT_KNOW_TRAP_ICON01:
		case FALLING_TRAP_ICON:
		case DONT_KNOW_TRAP_ICON02:
			DamageType = EDamageType::NON;
		case UNDER_FLOOR_TRAP_NO_SPICKS:
		case SPARES_TRAP_FULL:
			DamageType = EDamageType::FULL;
			damageAmount = 2;
		case SPARES_TRAP_HALF:
			DamageType = EDamageType::HALF;
			damageAmount = 2;
			frameToDamageOn = 0;
			break;
		case SMALL_SPICKS_TRAP_FULL:
			DamageType = EDamageType::HALF;
			damageAmount = 2;
			frameToDamageOn = 0;
			break;
		case SMALL_SPICKS_TRAP_HALF:
			DamageType = EDamageType::FULL;
			damageAmount = 2;
			break;
		default:
			break;
	}
}
//End Draw
