#include "SpriteLinkedList.h"

//==================================================================================
// Constructor
//==================================================================================
SpriteLinkedList::SpriteLinkedList()
{
	tail = nullptr;
	head = nullptr;
}//End Constructor

//==================================================================================
// Deletes a given Sprite
//==================================================================================
void SpriteLinkedList::deleteOneActor(Sprite ^ ActorToDelete)
{
	Sprite^ nodeWalker;

	nodeWalker = head;

	//Checks if head is the pellet to delete
	if (nodeWalker == ActorToDelete)
	{
		//Checks if there are anymore pellets in the list, if not, delete remove the head and tail, since they are pointing to the same address
		if (nodeWalker->Next() == nullptr)
		{
			head = nullptr;
			tail = nullptr;
		}
		//If there are pellets assign head to the next pellet to remove the first pellet from the list
		else
		{
			head = nodeWalker->Next();
		}
	}
	//If the pellet is not the head to delete
	else
	{
		//Itterate through the list untill it hits the a node which points to the pellet to delete
		while (nodeWalker->Next() != ActorToDelete)
		{
			//If nodeWalker points at pellet to delete then assign address for pallet which contains pellet to delete to nodeWalker
			nodeWalker = nodeWalker->Next();
		}

		//
		if (ActorToDelete->Next() == nullptr)
		{
			tail = nodeWalker;
		}

		//Assign nodeWalks next potiner to the address which the pellet to delete holds
		nodeWalker->Next(ActorToDelete->Next());
	}
}//End deleteOneActor

//==================================================================================
// Deletes all the dead Sprites
//==================================================================================
void SpriteLinkedList::deleteAllDeadActors()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		deleteOneActor(nodeWalker);
		nodeWalker = nodeWalker->Next();
	}
}//End deleteAllDeadActors

//==================================================================================
// Checks if sprite wants to use magic and returns if it does
//==================================================================================
Sprite^ SpriteLinkedList::actorWantsToUseMagic()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		bool checkMagicStatus = ((Actor^)nodeWalker)->getMagicStatus();

		if (checkMagicStatus == true)
		{
			return nodeWalker;
		}
		nodeWalker = nodeWalker->Next();
	}
	return nullptr;
}//End actorWantsToUseMagic

//==================================================================================
// Deletes all Sprites
//==================================================================================
void SpriteLinkedList::deleteAllActors()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		deleteOneActor(nodeWalker);
		nodeWalker = nodeWalker->Next();
	}
}//End deleteAllActors


//==================================================================================
//Adds a Actor to the list
//==================================================================================
void SpriteLinkedList::addActor(Sprite ^ ActorToAdd)
{
	//If no pallets are in the list then make it head and tail
	if (tail == nullptr)
	{
		tail = ActorToAdd;
		head = ActorToAdd;
	}
	//If there is, then make the tail point to the new Actor and make the tail the new Actor
	else
	{
		tail->Next(ActorToAdd);
		tail = ActorToAdd;
	}
}//End addActor

//==================================================================================
// Updates the attack range position for the actors
//==================================================================================
void SpriteLinkedList::updateAtackRange()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		((Actor^)nodeWalker)->updateAttackRange();
		nodeWalker = nodeWalker->Next();
	}
}//End updateAtackRange

//==================================================================================
// Checks if AI is outside its bounds or touching a wall
//==================================================================================
void SpriteLinkedList::checkWithinBoundingArea()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		((Actor^)nodeWalker)->checkWithinBoundingArea();
		nodeWalker = nodeWalker->Next();
	}
}//End checkWithinBoundingArea

//==================================================================================
// Check if a Sprite needs to be deleted
//==================================================================================
void SpriteLinkedList::checkIfMarkedForDeletion()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		bool marked = nodeWalker->getMarkedForDeletion();

		if (marked == true)
		{
			deleteOneActor(nodeWalker);
		}
		nodeWalker = nodeWalker->Next();
	}
}//End checkIfMarkedForDeletion

//==================================================================================
// Check if Sprite is touching a wall and deletes if it's
//==================================================================================
void SpriteLinkedList::checkTouchingWall()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		bool isOnWalkable = nodeWalker->checkOnWalkable();

		if (isOnWalkable == false)
		{
			nodeWalker->setIsVisible(false);
			nodeWalker->setMarkedForDeletion(true);
		}
		nodeWalker = nodeWalker->Next();
	}
}//End checkWithinBoundingArea

//==================================================================================
// Checks if the player is within the items range
//==================================================================================
Sprite^ SpriteLinkedList::checkIfPlayerInGatheringRange(Actor^ player)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		if (nodeWalker->getIsVisible() == true)
		{
			bool spriteWithinRange = player->checkWithinGatheringRange(nodeWalker);
			if (spriteWithinRange == true)
			{
				return nodeWalker;
			}
		}
		nodeWalker = nodeWalker->Next();
	}
	return nullptr;
}//End checkIfPlayerInGatheringRange

//==================================================================================
// Checks if the player is within the items range
//==================================================================================
void SpriteLinkedList::checkProjectileTouchingActor(Actor^ player)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		if (nodeWalker->getIsVisible() == true)
		{
			bool isTouchingSprite = player->checkActorOntopOfSprite(nodeWalker);
			if (isTouchingSprite == true)
			{
				player->magicDamageActor();
				nodeWalker->setIsVisible(false);
				nodeWalker->setMarkedForDeletion(true);
			}
		}
		nodeWalker = nodeWalker->Next();
	}
}//End checkProjectileTouchingActor

//==================================================================================
// Updates the actors frame to the next one
//==================================================================================
void SpriteLinkedList::changeFrameActors()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		((Actor^)nodeWalker)->UpdateFrame();
		nodeWalker = nodeWalker->Next();
	}
}//End changeFrameActors

//==================================================================================
// Updates the objects frame to the next one
//==================================================================================
void SpriteLinkedList::changeFrameObjects()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		nodeWalker->UpdateFrame();
		nodeWalker = nodeWalker->Next();
	}
}//End changeFrameObjects

//==================================================================================
// Updates the Sprites state
//==================================================================================
void SpriteLinkedList::updateState()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		((Actor^)nodeWalker)->UpdateState();
		nodeWalker = nodeWalker->Next();
	}
}//End updateState

//==================================================================================
// Performs the Sprites Action
//==================================================================================
void SpriteLinkedList::performAction()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		((Actor^)nodeWalker)->PerformAction();
		nodeWalker = nodeWalker->Next();
	}
}//End performAction

//==================================================================================
// Move all the Actors
//==================================================================================
void SpriteLinkedList::moveActors()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		nodeWalker->move();
		nodeWalker = nodeWalker->Next();
	}
}//End moveActors

//==================================================================================
// Check Sprites Collisions
//==================================================================================
void SpriteLinkedList::updateActorsCollisions()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		//nodeWalker->updateCollision();
		nodeWalker = nodeWalker->Next();
	}
}//End updateActorsCollisions

//==================================================================================
// Draws Sprites collision boxes
//==================================================================================
void SpriteLinkedList::drawActorsCollisions(int xPos, int yPos, int width, int height)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		//Actors new position on the viewport
		int ActorViewPortX = nodeWalker->getPositionX() - xPos;
		int ActorViewPortY = nodeWalker->getPositionY() - yPos;

		//Check if the selected nodewalker is within the viewport, if so, display the Actor
		if ((nodeWalker->getPositionX() >= xPos - nodeWalker->getWidth()) && (nodeWalker->getPositionY() >= yPos - nodeWalker->getHeight()) &&
			(nodeWalker->getPositionX() <= xPos + width * GameSettings::TILE_SIZE) && (nodeWalker->getPositionY() <= yPos + height * GameSettings::TILE_SIZE))
		{
			nodeWalker->DrawCollisionBox(ActorViewPortX, ActorViewPortY);
		}
		nodeWalker = nodeWalker->Next();
	}
}//End drawActorsCollisions

//==================================================================================
// Update Tile Map for the Spites
//==================================================================================
void SpriteLinkedList::updateActorsTileMap(TileMap^ newTileMap)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		nodeWalker->updateTileMap(newTileMap);
		nodeWalker = nodeWalker->Next();
	}
}//End updateActorsTileMap

//==================================================================================
// Check if Actor is stuck
//==================================================================================
void SpriteLinkedList::checkIfActortuck()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		((Actor^)nodeWalker)->checkActorStuck();
		nodeWalker = nodeWalker->Next();
	}
}//End drawActors

//==================================================================================
// Draw what AI sees in the Fake position for the user to see
//==================================================================================
void SpriteLinkedList::drawFakeEyeSight(int xPos, int yPos, int width, int height)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		//Actors new position on the viewport
		int ActorViewPortX = (nodeWalker->getPositionX() + (nodeWalker->getWidth() / 2)) - xPos;
		int ActorViewPortY = (nodeWalker->getPositionY() + (nodeWalker->getHeight() / 2)) - yPos;

		//Check if the selected nodewalker is within the viewport, if so, display the Actor
		if ((nodeWalker->getPositionX() >= xPos - nodeWalker->getWidth()) && (nodeWalker->getPositionY() >= yPos - nodeWalker->getHeight()) &&
			(nodeWalker->getPositionX() <= xPos + width * GameSettings::TILE_SIZE) && (nodeWalker->getPositionY() <= yPos + height * GameSettings::TILE_SIZE))
		{
			// Get the Actor within detection
			Actor^ actor = ((Actor^)((Actor^)nodeWalker)->getActorWtihinDetection());
			if (actor != nullptr)
			{
				int otherActorViewPortX = actor->getPositionX() - xPos;
				int otherActorViewPortY = actor->getPositionY() - yPos;

				// Set the eyesight for the nodewalker on where to target given the x and y
				((AI^)nodeWalker)->eyeSight(otherActorViewPortX, otherActorViewPortY, ActorViewPortX, ActorViewPortY, xPos, yPos);
			}
		}
		nodeWalker = nodeWalker->Next();
	}
}//End drawFakeEyeSight

//==================================================================================
// Check if Actor is on an Item
//==================================================================================
void SpriteLinkedList::checkActorOnItem(SpriteLinkedList^ itemsList)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		int index = 0;
		while (index < (itemsList->countActors()))
		{
			((Actor^)nodeWalker)->checkWithinDetectRange((Sprite^)itemsList->getActor(index));
			index = index + 1;
		}
		nodeWalker = nodeWalker->Next();
	}
}//End checkActorOnItem

//==================================================================================
// Check if Actor is on an Item
//==================================================================================
void SpriteLinkedList::checkActorOnTrap(SpriteLinkedList^ objectList)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		int index = 0;
		while (index < (objectList->countActors()))
		{
			// Get a object from the list
			Objects^ object = (Objects^)objectList->getActor(index);

			// Get if the given object can damage the Actor
			bool canDamageActor = ((Actor^)nodeWalker)->checkTrapCanDamageActor(object);

			// Checking if the object can damage the Actor
			if (canDamageActor == true)
			{
				((Actor^)nodeWalker)->trapDamageActor(object);
			}
			index = index + 1;
		}
		nodeWalker = nodeWalker->Next();
	}
}//End checkActorOnItem

//==================================================================================
//Update Action Bounds
//==================================================================================
void SpriteLinkedList::updateActorsBoundActions(int newBoundAction)
{
	//Actor^ nodeWalker = head;

	//while (nodeWalker != nullptr)
	//{
	//	nodeWalker->SetBoundingAction(newBoundAction);
	//	nodeWalker = nodeWalker->Next;
	//}
}//End updateActorsBoundActions

//==================================================================================
// Force renders all the Sprites within the portview
//==================================================================================
void SpriteLinkedList::renderActors(int xPos, int yPos, int width, int height)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		//Actors new position on the viewport
		int ActorViewPortX = nodeWalker->getPositionX() - xPos;
		int ActorViewPortY = nodeWalker->getPositionY() - yPos;
		bool onVisableTile = checkIfOnVisableTile(nodeWalker);

		//Check if the selected nodewalker is within the viewport, if so, display the Actor
		if ((nodeWalker->getPositionX() >= xPos - nodeWalker->getWidth()) && (nodeWalker->getPositionY() >= yPos - nodeWalker->getHeight()) &&
			(nodeWalker->getPositionX() <= xPos + width * GameSettings::TILE_SIZE) && (nodeWalker->getPositionY() <= yPos + height * GameSettings::TILE_SIZE) &&
			(onVisableTile == true) && (nodeWalker->getIsVisible() == true))
		{
			nodeWalker->draw(ActorViewPortX, ActorViewPortY);
		}
		nodeWalker = nodeWalker->Next();
	}
}//End renderActors

//==================================================================================
// Checks if actors can log text in the logger
//==================================================================================
void SpriteLinkedList::canLogText(int xPos, int yPos, int width, int height)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		//Actors new position on the viewport
		int ActorViewPortX = nodeWalker->getPositionX() - xPos;
		bool onVisableTile = checkIfOnVisableTile(nodeWalker);

		//Check if the selected nodewalker is within the viewport, if so, display the Actor
		if ((nodeWalker->getPositionX() >= xPos - nodeWalker->getWidth()) && (nodeWalker->getPositionY() >= yPos - nodeWalker->getHeight()) &&
			(nodeWalker->getPositionX() <= xPos + width * GameSettings::TILE_SIZE) && (nodeWalker->getPositionY() <= yPos + height * GameSettings::TILE_SIZE) &&
			(onVisableTile == true))
		{
			((Actor^)nodeWalker)->setCanLog(true);
		}
		else // Else don't log
		{
			((Actor^)nodeWalker)->setCanLog(false);
		}
		nodeWalker = nodeWalker->Next();
	}
}//End canLogText

//==================================================================================
// Draws the health bars over the Actors
//==================================================================================
void SpriteLinkedList::drawHealthBars(int xPos, int yPos, int width, int height)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		bool onVisableTile = checkIfOnVisableTile(nodeWalker);
		int ActorViewPortX = nodeWalker->getPositionX() - xPos;
		int ActorViewPortY = nodeWalker->getPositionY() - yPos;

		//Check if the selected nodewalker is within the viewport, if so, display the Actor
		if ((nodeWalker->getPositionX() >= xPos - nodeWalker->getWidth()) && (nodeWalker->getPositionY() >= yPos - nodeWalker->getHeight()) &&
			(nodeWalker->getPositionX() <= xPos + width * GameSettings::TILE_SIZE) && (nodeWalker->getPositionY() <= yPos + height * GameSettings::TILE_SIZE) &&
			(onVisableTile == true))
		{
			((AI^)nodeWalker)->drawHealth(ActorViewPortX, ActorViewPortY);
		}
		nodeWalker = nodeWalker->Next();
	}
}

//==================================================================================
//Checks if Actor is on a visable tile
//==================================================================================
bool SpriteLinkedList::checkIfOnVisableTile(Sprite^ nodeWalker)
{
	return nodeWalker->checkIfOnVisableTile();
}//End checkIfOnVisableTile

//==================================================================================
// Checks if Sprite is within Actors detection
//==================================================================================
void SpriteLinkedList::checkWithinDetection(Sprite ^ otherguy)
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		bool checkCollision = ((Actor^)nodeWalker)->checkWithinDetectRange(((Actor^)otherguy));
		nodeWalker = nodeWalker->Next();
	}
}//End checkWithinDetection

//==================================================================================
// Updates the detection range for an Actor
//==================================================================================
void SpriteLinkedList::updateDetection()
{
	Sprite^ nodeWalker = head;

	while (nodeWalker != nullptr)
	{
		((Actor^)nodeWalker)->updateDetectRange();
		nodeWalker = nodeWalker->Next();
	}
}//End updateDetection

//==================================================================================
//Check amount of Actors in the list
//==================================================================================
int SpriteLinkedList::countActors()
{
	//Assign nodeWalker to first node in the list
	Sprite^ nodeWalker = head;

	int count = 0;
	//nodeWalker only equals nullptr if at the end of the list or no nodes in the list
	while (nodeWalker != nullptr)
	{
		nodeWalker = nodeWalker->Next();
		count++;
	}
	//Debug::WriteLine(count);
	return count;
}//End countActors

 //==================================================================================
 // Get a given Sprite in the list
 //==================================================================================
Sprite^ SpriteLinkedList::getActor(int index)
{
	//Assign nodeWalker to first node in the list
	Sprite^ nodeWalker = head;

	//nodeWalker only equals nullptr if at the end of the list or no nodes in the list
	for(int i = 0; i < index; i++)
	{
		nodeWalker = nodeWalker->Next();
	}
	//Debug::WriteLine(count);
	return nodeWalker;
}//End getActor