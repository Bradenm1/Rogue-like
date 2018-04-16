#pragma once

//Included Classes
#include "Actor.h"

//==================================================================================
// This class contains the controls for the input of the user, all input
// via the keyboard is sent to here which controls the in-game player
//==================================================================================
ref class Controls
{
private:
	array<bool>^ directions;
	Actor^ actor;
public:
	Controls(Actor^ mewActor);

	void controledMovement();
	void attack();
	void pickUpItem(Sprite ^ item);
	bool useObject(Sprite ^ object);
	void moveActor(EDirection direction);

	//Get/Sets
	void setDirections(EDirection direction, bool isMoving) { directions[direction] = isMoving; }
	void setNewControlableActor(Actor^ newActor) { actor = newActor; }
};

