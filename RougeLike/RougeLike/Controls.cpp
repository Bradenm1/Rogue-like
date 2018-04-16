#include "Controls.h"


//==================================================================================
// Constructor
//==================================================================================
Controls::Controls(Actor ^ startActor)
{
	directions = gcnew array<bool>(GameSettings::NUMBER_OF_DRECTIONS);
	actor = startActor;
}//End Constructor

//==================================================================================
// Method which checks which controls are currently running
//==================================================================================
void Controls::controledMovement()
{
	//An switch or if-elses are not used here because we want all the controls to run, not just one every game cycle
	if (directions[EDirection::NORTH] == true)
	{
		moveActor(EDirection::NORTH);
	}
	if (directions[EDirection::SOUTH] == true)
	{
		moveActor(EDirection::SOUTH);
	}
	if (directions[EDirection::WEST] == true)
	{
		moveActor(EDirection::WEST);
	}
	if (directions[EDirection::EAST] == true)
	{
		moveActor(EDirection::EAST);
	}
}//End controledMovement

//==================================================================================
// Method used when controled actor attacks
//==================================================================================
void Controls::attack()
{
	actor->attackMelee();
}//End attack

//==================================================================================
// Nethod used for allowing the actor to pick up the item
//==================================================================================
void Controls::pickUpItem(Sprite^ item)
{
	actor->gatherItem(item);
}//End pickUpItem

//==================================================================================
// Nethod used for allowing the actor to use an object
//==================================================================================
bool Controls::useObject(Sprite^ object)
{
	return actor->useObject(object);
}//End useObject

//==================================================================================
// Method used to control the movement of the actor which is currently controlable
//==================================================================================
void Controls::moveActor(EDirection direction)
{
	//Sets the direction of the player
	actor->SpriteDirection(direction);
	//Update the player
	//actor->UpdateFrame();
	//Move the player
	actor->move();
}//End moveActor
