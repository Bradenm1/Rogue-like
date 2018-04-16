#include "Actor.h"

//==================================================================================
// Constructor
//==================================================================================
Actor::Actor(Graphics^ startCanvas, TileMap^ startTilemap, Random^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger^ startLogger, int startNFrames, int startNDrections, bool startIsplayer) : Sprite(startCanvas, startTilemap, startRGen, startSpriteSheets, startLogger)
{
	//Sets the spritesheet for the actor
	spriteSheets = startSpriteSheets;

	//Sets number of frames
	nFrames = startNFrames;

	//Set the number of directions the Actor has
	nDirections = startNDrections;

	//Gets the sprite shet and divides it by the number of frames to get the frame width
	//size.X = spriteSheets[0]->Width / (nFrames);
	//size.Y = spriteSheets[0]->Height;

	//FIlls the veloctiy with the correct directions
	velocityDirections = gcnew array<Point>(nDirections);
	velocityDirections[0] = Point(1, 0);
	velocityDirections[1] = Point(0, 1);
	velocityDirections[2] = Point(-1, 0);
	velocityDirections[3] = Point(0, -1);

	//If the Actor can log text by default
	canLogText = false;
	//Sets default direction to be moving east
	spriteDirection = EDirection::EAST;

	//Sets Default Stats
	isAlive = true;
	isPlayer = startIsplayer;
	nTicksTillAttack = 0;
	//The frame the animation will start on
	currentFrame = 0;
	inventorySlotsFill = 0;
	nTicksTillUnstuck = 0;
	//Sets the states
	actorState = EActorState::IDLE;
	seekingState = ESeekingState::SEEKACTOR;
	attackState = EActorAttackState::HAND;
	boundingAction = EBoundsAction::BOUNCE;

	//Creates the inventory slots
	items = gcnew array<Sprite^>(4);

	//Places the Actor on a walkable tile
	NotSpawnOnWalkable();

	//Asigns Next to nullptr
	next = nullptr;
	actorWtihinDetection = nullptr;
	actorWithinAttackRange = nullptr;
	//itemWithinDetection = nullptr;
}//End Contructor

//==================================================================================
// Updates the detect range for the Actor
//==================================================================================
void Actor::updateDetectRange()
{
	nTicksTillAttack++;
	int newX = position.X - GameSettings::SPRITE_DETECT_RANGE;
	int newY = position.Y - GameSettings::SPRITE_DETECT_RANGE;
	int newWidth = size.X + GameSettings::SPRITE_DETECT_RANGE * 2;
	int newHeight = size.Y + GameSettings::SPRITE_DETECT_RANGE * 2;

	//New Detect Range
	detectRange = Rectangle(newX, newY, newWidth, newHeight);
}//End updateDetectRange

//==================================================================================
// Updates the detect range for the Actor
//==================================================================================
void Actor::updateAttackRange()
{
	int newX = position.X - GameSettings::SPRITE_ATTACK_RANGE;
	int newY = position.Y - GameSettings::SPRITE_ATTACK_RANGE;
	int newWidth = size.X + GameSettings::SPRITE_ATTACK_RANGE * 2;
	int newHeight = size.Y + GameSettings::SPRITE_ATTACK_RANGE * 2;

	//New Detect Range
	attackRange = Rectangle(newX, newY, newWidth, newHeight);
}//End updateDetectRange

//==================================================================================
// Makes the actor go into a attack
//==================================================================================
void Actor::attackMelee()
{
	//Checks if the Actor can attack again from the delay
	if (nTicksTillAttack >= GameSettings::SPRITE_ATTACK_DELAY)
	{
		//Checks if the Actor it wants to attack is actually there
		if (actorWithinAttackRange != nullptr)
		{
			//Checks if the actor it wants to attack is alive
			if (actorWithinAttackRange->stats[EActorStats::HEALTH] > 0)
			{
				//If so attacks them
				actorWithinAttackRange->stats[EActorStats::HEALTH] -= 10;

				if (canLogText == true)
				{
					logger->addText(name + " attcked the " + actorWithinAttackRange->name);
				}
			}
			//Else the Actors health is less then zero and should be killed
			else
			{
				actorWithinAttackRange->setIsAlive(false);
			}
		}
		//Resets he delay for the Actor to attack again
		nTicksTillAttack = 0;
	}
}//End attack

//==================================================================================
// Makes the actor attack using magic
//==================================================================================
void Actor::attackMagic()
{
	//Checks if the Actor can attack again from the delay
	if (nTicksTillAttack >= GameSettings::SPRITE_ATTACK_DELAY)
	{
		wantsToUseMagic = true;

		//Resets he delay for the Actor to attack again
		nTicksTillAttack = 0;
	}
}//End attackMagic

//=================================================================================
// Checks if the arg1 actor is within the detection distance
//=================================================================================
bool Actor::checkWithinDetectRange(Actor^ otherActor)
{
	if ((otherActor != nullptr) && (otherActor->getIsVisible() == true) && (otherActor->getIsAlive() == true))
	{
		if ((otherActor->getPositionX() > detectRange.X) && (otherActor->getPositionY() > detectRange.Y) &&
			(otherActor->getPositionX() < detectRange.X + detectRange.Width) && (otherActor->getPositionY() < detectRange.Y + detectRange.Height))
		{
			if ((canSeeSprite(otherActor) == true) || (stats[EActorStats::CAN_NOCLIP] == 1))
			{
				actorWtihinDetection = otherActor;
				return true;
			}
		}
	}
	return false;

}//End checkWithinDetectRange

//=================================================================================
// Checks if the arg1 actor is within the detection distance
//=================================================================================
bool Actor::checkWithinDetectRange(Sprite^ otherActor)
{	
	// Check if other sprite is not null, is visible
	if ((otherActor != nullptr) && (otherActor->getIsVisible() == true) && (inventorySlotsFill != items->Length))
	{
		// Check if Actor is within distance of other Sprite
		if ((otherActor->getPositionX() > detectRange.X) && (otherActor->getPositionY() > detectRange.Y) &&
			(otherActor->getPositionX() < detectRange.X + detectRange.Width) && (otherActor->getPositionY() < detectRange.Y + detectRange.Height))
		{
			// Check if the Actor is nocliping or can see other Sprite
			if ((canSeeSprite(otherActor) == true) || (stats[EActorStats::CAN_NOCLIP] == 1))
			{
				itemWithinDetection = otherActor;
				return true;
			}
		}
	}
	return false;

}//End checkWithinDetectRange

//=================================================================================
// Unstuck actor if stuck, place them on the nearest walkable tile
//=================================================================================
bool Actor::unStuckActor()
{
	bool onWalkable = false;

	int yPos = (position.Y + (size.Y / 2)) / GameSettings::TILE_SIZE;
	int xPos = (position.X + (size.X / 2)) / GameSettings::TILE_SIZE;

	onWalkable = tilemap->IsMapEntryWalkable(position.Y / GameSettings::TILE_SIZE, position.X / GameSettings::TILE_SIZE);

	// A check radius around the actor grows bigger untill it finds
	// a walkable tile which then places the actor onto it
	if ((onWalkable == false) && (stats[EActorStats::CAN_NOCLIP] == 0))
	{
		for (int range = 0; range < GameSettings::SPRITE_STUCK_CHECK_RADIUS; range++)
		{
			//Creates the range of the X and Y at which the game will check
			//if the tiles are walkable around the Actor
			Point rangePos = Point(xPos + range, yPos + range);
			Point rangeNeg = Point(xPos - range, yPos - range);

			for (int i = rangeNeg.X; i < rangePos.X; i++)
			{
				for (int o = rangeNeg.Y; o < rangePos.Y; o++)
				{
					//Convert the tile position of the for loops into actual positions
					Point newPosition = Point(i * GameSettings::TILE_SIZE, o * GameSettings::TILE_SIZE);
					//Check if the new position is walkable
					onWalkable = tilemap->IsMapEntryWalkable(newPosition.Y / GameSettings::TILE_SIZE, newPosition.X / GameSettings::TILE_SIZE);
					//If the tile is, sets the Actors position to that tile
					if (onWalkable == true)
					{
						position = newPosition;
						//Settings the Velocity to the other directon will try attempt to stop bad AIs from getting unstuck then stuck again in an endless loop
						velocity.X += velocity.X * -1;
						velocity.Y += velocity.Y * -1;
						return true;
					}
				}
			}
		}
	}
	return false;
}//End unStuckActor

//=================================================================================
// IF an actor has ben in the same area for too long it assumes they are stuck
// and unstucks them
//=================================================================================
void Actor::checkActorStuck()
{
	// Checks if number of ticks have passed to do the checking
	if (nTicksTillUnstuck > GameSettings::SPRITE_STUCK_CHECK_MAX)
	{
		// If AI is still in the same position unstuck them
		if (currentPositionCheck == position)
		{
			unStuckActor();
		}
		else // Else set current position as new checking position
		{
			currentPositionCheck = position;
		}
		// Reset the timer
		nTicksTillUnstuck = 0;
	}
	else // Else the AI is not stuck and should increment the ticks
	{
		nTicksTillUnstuck++;
	}
}//End checkActorStuck

//=================================================================================
// Checks if the actor can see the other sptire
//=================================================================================
bool Actor::canSeeSprite(Sprite^ otherSprite)
{
	// Set the position to the center of the Actor
	int posX = position.X + (size.X / 2);
	int posY = position.Y + (size.X / 2);
	Point fakePos;

	for (int i = 0; i < detectRange.Width; i++)
	{
		int xDelta = (otherSprite->getPositionX() + (otherSprite->getWidth() / 2)) - posX;
		int yDelta = (otherSprite->getPositionY() + (otherSprite->getHeight() / 2)) - posY;

		int angleToTarget = Math::Atan2(yDelta, xDelta);

		bool viewBlock = false;

		fakePos.X = Math::Cos(angleToTarget) * stats[EActorStats::SPEED];
		fakePos.Y = Math::Sin(angleToTarget) * stats[EActorStats::SPEED];

		posX += fakePos.X;
		posY += fakePos.Y;

		// Check if tile is walkable
		bool onWalkable = tilemap->IsMapEntryWalkable(posY / GameSettings::TILE_SIZE, posX / GameSettings::TILE_SIZE);

		// If the Actor is not on a walkable tile return false
		if (onWalkable == false)
		{
			return false;
		}
	}
	// The Actor is on a walkable tile
	return true;
}//End canSeeSprite

//=================================================================================
// Checks if the arg1 actor is within the attacking range
//=================================================================================
bool Actor::checkWithinAttackRange(Actor^ otherActor)
{
	if (otherActor != nullptr)
	{
		if ((otherActor->getPositionX() > attackRange.X) && (otherActor->getPositionY() > attackRange.Y) &&
			(otherActor->getPositionX() < attackRange.X + attackRange.Width) && (otherActor->getPositionY() < attackRange.Y + attackRange.Height))
		{
			if ((canSeeSprite(otherActor) == true) || (stats[EActorStats::CAN_NOCLIP] == 1))
			{
				actorWithinAttackRange = otherActor;
				return true;
			}
		}
	}
	return false;

}//End checkWithinAttackRange

//=================================================================================
// Checks if the arg1 item is within the gatherting range
//=================================================================================
bool Actor::checkWithinGatheringRange(Sprite^ item)
{
	if ((item != nullptr) && (item->getIsVisible() == true))
	{
		if ((item->getPositionX() > attackRange.X) && (item->getPositionY() > attackRange.Y) &&
			(item->getPositionX() < attackRange.X + attackRange.Width) && (item->getPositionY() < attackRange.Y + attackRange.Height))
		{
			if ((canSeeSprite(item) == true) || (stats[EActorStats::CAN_NOCLIP] == 1))
			{
				return true;
			}
		}
	}
	return false;

}//End checkWithinGatheringRange

//=================================================================================
// Checks if the arg1 sprite is within the gatherting range
//=================================================================================
bool Actor::checkActorOntopOfSprite(Sprite^ sprite)
{
	if ((sprite != nullptr) && (sprite->getIsVisible() == true))
	{
		if ((position.X > sprite->getPositionX() - GameSettings::TILE_SIZE) && (position.Y > sprite->getPositionY() - GameSettings::TILE_SIZE) &&
			(position.X < sprite->getPositionX() + GameSettings::TILE_SIZE) && (position.Y < sprite->getPositionY() + GameSettings::TILE_SIZE))
		{
			return true;
		}
	}
	return false;
}//End checkWithinGatheringRange

//=================================================================================
// Removes a item at a given index from the actors inventory
//=================================================================================
void Actor::removeItem(int index)
{
	items[index] = nullptr;
}//End removeItem

void Actor::markedItemForDeletion(int index)
{
	items[index]->setMarkedForDeletion(true);
}

//=================================================================================
// Picks up the inputted item and puts in the the actors inventory
// IF it's money and the actor is the player it just adds it to the score and
// does not add it to the inventory.
//=================================================================================
bool Actor::gatherItem(Sprite^ item)
{
	if ((item != nullptr) && (((Items^)item)->getIsVisible() == true))
	{
		EItems itemType = ((Items^)item)->getItemType();
		String^ itemName = ((Items^)item)->getItemName();

		//Check if the Actor wanting to pick up the Item is the player
		// And check if what they want to pick up is Money
		if ((itemType < EItems::MONEY_BLANK04) && (name->Equals("Player")))
		{
			//Gets the items value
			int moneyAmount = (((Items^)item)->getItemStats())[EItemStats::WORTH];

			//Adds that value to the Actor
			stats[EActorStats::MONEY] += moneyAmount;
			//Tells the Item it has been picked up
			((Items^)item)->actorPutUpItem();
			if (canLogText == true)
			{
				logger->addText(name + " has picked up " + moneyAmount.ToString() + " Money");
			}
			return true;
		}
		//Else it's a AI
		else
		{
			for (int i = 0; i < items->Length; i++)
			{
				//Check for an empty slot
				//If non the Actors inventory is full
				if (items[i] == nullptr)
				{
					//When a empty slot is found store the item into the Actors inventory
					items[i] = item;
					//Tell the item it has een picked up
					((Items^)item)->actorPutUpItem();
					//Add a slot as beening used
					inventorySlotsFill++;
					if (canLogText == true)
					{
						logger->addText(name + " Picked up " + itemName);
					}
					return true;
				}
			}
		}
	}
	return false;
}//End gatherItem

//=================================================================================
// Uses the given item
// This does not remove the item from the Actors inventory BUT removes it from a slot
//=================================================================================
void Actor::useItem(Sprite^ itemToUse)
{
	Items^ item = (Items^)itemToUse;
	//Get the name of the item
	String^ itemName = item->getItemName();
	//Get the type of item
	EItems itemType = item->getItemType();
	//Get the items effect
	EItemStats itemEffect = item->getItemStatEfffect();
	//Get the items stats
	array<int>^ itemStats = item->getItemStats(itemType);

	//Checks the effect on the item and adjusts the Actors stats
	//According to the effect
	switch (itemEffect)
	{
	case HEALTH_INCRESS_FACTOR:
		if (stats[EActorStats::HEALTH] < GameSettings::SPRITE_HEALTH_DEFAULT)
		{
			stats[EActorStats::HEALTH] += itemStats[itemEffect];
		}
		break;
	case HEALTH_DECRESS_FACTOR:
		stats[EActorStats::HEALTH] -= itemStats[itemEffect];
		break;
	case SPEED_INCRESS_FACTOR:
		stats[EActorStats::SPEED] += itemStats[itemEffect];
		velocity.X = stats[EActorStats::SPEED];
		velocity.Y = stats[EActorStats::SPEED];
		break;
	case SPEED_DECRESS_FACTOR:
		stats[EActorStats::SPEED] -= itemStats[itemEffect];
		velocity.X = stats[EActorStats::SPEED];
		velocity.Y = stats[EActorStats::SPEED];
		break;
	case INCRESS_ATTACK_DAMAGE:
		stats[EActorStats::ATTACK_DAMAGE] += itemStats[itemEffect];
		break;
	default:
		break;
	}
	if (canLogText == true)
	{
		logger->addText(name + " Used Item " + itemName);
	}
	//Remove the items from a slot
	item->setMarkedForDeletion(true);
	inventorySlotsFill--;
}//End useItems

//=================================================================================
// Uses the given object
//=================================================================================
bool Actor::useObject(Sprite^ objectToUse)
{
	Objects^ object = (Objects^)objectToUse;
	//Get the type of object
	EObjectType objectType = object->getObjectType();

	switch (objectType)
	{
	case DOOR:
		return true;
		break;
	case TRAP:
		//Do nothing
		break;
	default:
		break;
	}

	return false;
}//End useItem


//=================================================================================
// Damages the actor with the given trap
//=================================================================================
bool Actor::checkTrapCanDamageActor(Objects^ trap)
{
	if (trap->getObjectType() == EObjectType::TRAP)
	{
		bool inRange = checkActorOntopOfSprite((Sprite^)trap);

		if (inRange == true)
		{
			bool canDamageActor = trap->objectTouch();

			if (canDamageActor == true)
			{
				return true;
			}
		}
	}
	return false;
}//End checkTrapCanDamageActor

//=================================================================================
// Damages the actor with the given trap
//=================================================================================
void Actor::trapDamageActor(Objects^ trap)
{
	int damageAmount = trap->getDamageAmount();

	stats[EActorStats::HEALTH] -= damageAmount;

	if (stats[EActorStats::HEALTH] <= 0)
	{
		isAlive = false;
	}
	else
	{
		if (canLogText == true)
		{
			logger->addText(name + " was damaged by a Trap");
		}
	}
}//End trapDamageActor

//=================================================================================
// Damages the actor
//=================================================================================
void Actor::magicDamageActor()
{
	int damageAmount = GameSettings::MAGIC_DAMAGE_AMOUNT;

	stats[EActorStats::HEALTH] -= damageAmount;

	if (stats[EActorStats::HEALTH] <= 0)
	{
		isAlive = false;
	}
	else
	{
		if (canLogText == true)
		{
			logger->addText(name + " was damaged by some magic");
		}
	}
}//End trapDamageActor

//=================================================================================
// Drops all items in Actors inventory
//=================================================================================
void Actor::dropInventory()
{
	for (int i = 0; i < items->Length; i++)
	{
		dropInventorySlot(i);
	}
}//End dropInventory

//=================================================================================
// Drops Item at certain slot in the Actor inventory
//=================================================================================
void Actor::dropInventorySlot(int slot)
{
	if (items[slot] != nullptr)
	{
		//Drop the item at the actors position
		((Items^)items[slot])->actorDropItem(position);
		//Set the item as Nullptr in the Actors inventory, this frees up the space
		items[slot] = nullptr;
		if (canLogText == true)
		{
			logger->addText(name + " dropped an Item");
		}
	}
}//End dropInventorySlot

//==================================================================================
// Makes the actor chose a random direction to walk
//==================================================================================
void Actor::choseRandomDirection()
{
	spriteDirection = (EDirection)(rGen->Next(nDirections));
}//End choseRandomDirection

//==================================================================================
// Updates the new tile the sprite is about to walk onto
//==================================================================================
bool Actor::UpdateTileArea()
{
	bool walkableCheck = true;
	if (stats[EActorStats::CAN_NOCLIP] == 0)
	{
		//Check move ahead in the future
		int newX = position.X + (stats[EActorStats::SPEED] * velocityDirections[spriteDirection].X);
		int newY = position.Y + (stats[EActorStats::SPEED] * velocityDirections[spriteDirection].Y);

		int directionCornerX;
		int directionCornerY;

		switch (spriteDirection)
		{
		case (NORTH): //W
			directionCornerX = (newX + size.X / 2) / GameSettings::TILE_SIZE;	// directionCornerX is halfway across the image
			directionCornerY = newY / GameSettings::TILE_SIZE;								// directionCornerY is the top edge
			walkableCheck = tilemap->IsMapEntryWalkable(directionCornerY, directionCornerX);
			break;
		case(EAST): //D
			directionCornerX = (newX + size.X) / GameSettings::TILE_SIZE;		// directionCornerX is the right-hand edge
			directionCornerY = (newY + size.Y / 2) / GameSettings::TILE_SIZE;	// directionCornerY is halfway down the image
			walkableCheck = tilemap->IsMapEntryWalkable(directionCornerY, directionCornerX);
			break;
		case(SOUTH): //S
			directionCornerX = (newX + size.X / 2) / GameSettings::TILE_SIZE;	// directionCornerX is halfway across the image
			directionCornerY = (newY + size.Y) / GameSettings::TILE_SIZE;		// directionCornerY is the bottom edge
			walkableCheck = tilemap->IsMapEntryWalkable(directionCornerY, directionCornerX);
			break;
		case(WEST): //A
			directionCornerX = newX / GameSettings::TILE_SIZE;								// directionCornerX is the left-hand edge
			directionCornerY = (newY + size.Y / 2) / GameSettings::TILE_SIZE;	// direcitonCornerY is halfway down the image
			walkableCheck = tilemap->IsMapEntryWalkable(directionCornerY, directionCornerX);
			break;
		}
		if (walkableCheck == true)
		{
			//Checks if the block it was testing was null
			if ((directionCornerY == -1) || (directionCornerX == -1))
			{
				walkableCheck = false;
			}
		}
	}
	return walkableCheck;

}//End UdpateTileArea

//==================================================================================
// Updates the bounding area for the Actor
//==================================================================================
void Actor::setBoundingArea()
{
	int newX = position.X - GameSettings::SPRITE_AREA_BOUND_RANGE;
	int newY = position.Y - GameSettings::SPRITE_AREA_BOUND_RANGE;
	int newWidth = size.X + GameSettings::SPRITE_AREA_BOUND_RANGE * 2;
	int newHeight = size.Y + GameSettings::SPRITE_AREA_BOUND_RANGE * 2;

	//New Area Range
	boundingArea = Rectangle(newX, newY, newWidth, newHeight);
}//End updateDetectRange

//=================================================================================
// Checks if AI is about to walk into a wall or is outside its bounding area
//=================================================================================
void Actor::checkWithinBoundingArea()
{
	if ((position.X > boundingArea.X) && (position.Y > boundingArea.Y) &&
		(position.X < boundingArea.Left) && (position.Y < boundingArea.Top) ||
		UpdateTileArea() == false)
	{
		boundsAction();
	}
}//End checkWithinBoundingArea

//=================================================================================
// Bounding actions for the AI
//=================================================================================
void Actor::boundsAction()
{
	switch (boundingAction)
	{
	//Stops the volocity for both x,y for the sprite
	case EBoundsAction::STOP:
		velocity.X = 0;
		velocity.Y = 0;
		break;
	//Sets Alive to false if touches edge
	case EBoundsAction::DIE:
		isAlive = false;
		break;
	//Checks if the sprite is touching an edge and wraps the sprite to the other side
	case EBoundsAction::WRAP:
		//Checks if sprite is past the right or left of the boundry
		if ((position.X < boundingArea.X) || ((position.X + size.X) > boundingArea.Width - GameSettings::SPRITE_CHIKEN_XACCOUNT_OF_BOUNDS))
		{
			//Checks if the sprite is past the left of the boundry
			if (position.X < boundingArea.X)
			{
				position.X = boundingArea.X + boundingArea.Width - size.X - GameSettings::SPRITE_CHIKEN_XACCOUNT_OF_BOUNDS;
			}
			//If not past the left of the boundry is past the right of the boundry
			else
			{
				position.X = boundingArea.X;
			}
		}
		//Checks if the sprite is past the top or bottom of the boundry
		if ((position.Y < boundingArea.Y) || ((position.Y + size.Y) > boundingArea.Height))
		{
			//Checks if the sprite is past the top of the boundry
			if (position.Y < boundingArea.Y)
			{
				position.Y = boundingArea.Y + boundingArea.Height - size.Y - 1;
			}
			//If not past the top, the sprite is under the boundry
			else
			{
				position.Y = boundingArea.Y;
			}
		}
		break;
	//Checks if a sprite is touching an edge and bounces the sprite in the other dir
	case EBoundsAction::BOUNCE:
		//Cal used to change the directions of the sprites using the bounce
		spriteDirection = (EDirection)((((int)spriteDirection) + (nDirections / 2)) % nDirections);
		break;
	default:
		//Default
		break;
	}
}//End boundsAction

//==================================================================================
// Draws the collision box for the Sprite
//==================================================================================
void Sprite::DrawCollisionBox()
{
	Pen^ pen = gcnew Pen(Color::White);
	//Draws the border around the image
	canvas->DrawRectangle(pen, position.X, position.Y, size.X, size.Y);
}//End DrawCollisionBox


//==================================================================================
// Draws the collision box for the sprite at fake locations
//==================================================================================
void Sprite::DrawCollisionBox(int forcedX, int forcedY)
{
	Pen^ pen = gcnew Pen(Color::Red);
	int newX = position.X - GameSettings::SPRITE_ATTACK_RANGE;
	int newY = position.Y - GameSettings::SPRITE_ATTACK_RANGE;
	int newWidth = size.X + GameSettings::SPRITE_ATTACK_RANGE * 2;
	int newHeight = size.Y + GameSettings::SPRITE_ATTACK_RANGE * 2;

	//New Detect Range
	Rectangle something = Rectangle(newX, newY, newWidth, newHeight);
	//Draws the border around the image
	canvas->DrawRectangle(pen, something);
	DrawCollisionBox();
}//End DrawCollisionBox

//==================================================================================
// Returns the players default stats
//==================================================================================
array<int>^ Actor::getPlayerStats()
{
	array<int>^ stats = gcnew array<int>(EActorStats::ENUM_LENGTH_ACTOR_STATS);

	stats[EActorStats::HEALTH] = GameSettings::SPRITE_HEALTH_DEFAULT;
	stats[EActorStats::ARMOR] = 0;
	stats[EActorStats::SEEK_CHANCE] = 0;
	stats[EActorStats::SPEED] = GameSettings::SPRITE_SPEED_PLAYER;
	stats[EActorStats::ATTACK_CHANCE] = 0;
	stats[EActorStats::CAN_NOCLIP] = 0;
	stats[EActorStats::ATTACK_DAMAGE] = 13;

	return stats;
}//End getPlayerStats

//==================================================================================
// Returns the AI stats for the given AI
//==================================================================================
array<int>^ Actor::getAIStats(EActor actor)
{
	//Creates the array for the stats to be held
	array<int>^ stats = gcnew array<int>(EActorStats::ENUM_LENGTH_ACTOR_STATS);

	//Defaults incase some don't get used
	//DO NOT SET THE SPEED TOO HIGH OR THEY CAN WALK INTO WALLS FROM GOING TO FAST
	name = "Enemy";
	stats[EActorStats::HEALTH] = GameSettings::SPRITE_HEALTH_DEFAULT;
	stats[EActorStats::ARMOR] = GameSettings::SPRITE_ARMOR_DEFAULT;
	//Seek chance higher is more chance
	stats[EActorStats::SEEK_CHANCE] = GameSettings::SPRITE_SEEK_CHANCE_DEFAULT;
	stats[EActorStats::SPEED] = GameSettings::SPRITE_SPEED_DEFAULT;
	//Gather item higher is more chance
	stats[EActorStats::GATHER_ITEM_CHANCE] = GameSettings::SPRITE_GATHER_ITEM_CHANCE_DEFAULT;
	//One means the actor can noclip
	stats[EActorStats::CAN_NOCLIP] = GameSettings::SPRITE_CAN_NOCLIP_DEFAULT;
	//Use item, magic Higher more chance
	stats[EActorStats::USE_ITEM_CHANCE] = GameSettings::SPRITE_USE_ITEM_CHANCE_DEFAULT;
	stats[EActorStats::USE_MAGIC_CHANCE] = GameSettings::SPRITE_USE_MAGIC_CHANCE_DEFAULT;
	//Attack Chance lower is more chance
	stats[EActorStats::ATTACK_CHANCE] = GameSettings::SPRITE_ATTACK_CHANCE_DEFAULT;
	//Orientation higher is more orienations
	stats[EActorStats::ORIENTATION_CHANCE] = GameSettings::SPRITE_ORIENTATION_CHANCE_DEAULT;
	stats[EActorStats::ATTACK_DAMAGE] = 10;

	switch (actor)
	{
	case ZOMBIE_BASIC04:
	case ZOMBIE_RABBIT:
	case ZOMBIE_GIRL:
	case ZOMBIE_KNIFE:
	case ZOMBIE_WIZARD:
	case ZOMBIE_TWOHEADS:
	case ZOMBIE_BOY:
	case ZOMBIE_TOUGH:
		name = "Zombie";
		stats[EActorStats::HEALTH] = 60;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 50;
		stats[EActorStats::SPEED] = 3;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 0;
		break;
	case MUMMY_RABBIT:
	case MUMMY_BASIC04:
	case MUMMY_KNIFE:
	case MUMMY_WIZARD:
	case MUMMY_GIRL:
	case MUMMY_TWOHEADS:
	case MUMMY_BOY:
	case MUMMY_TOUGH:
		name = "Mummy";
		stats[EActorStats::HEALTH] = 50;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 70;
		stats[EActorStats::SPEED] = 3;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 0;
		stats[EActorStats::USE_MAGIC_CHANCE] = 30;
		break;
	case NORMAL_SKELETON:
	case ORANGE_SKELETON:
	case GREEN_SKELETON:
	case DARK_SKELETON:
		name = "Sekeleton";
		stats[EActorStats::HEALTH] = 70;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 60;
		stats[EActorStats::SPEED] = 6;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 40;
		break;
	case NORMAL_SKELETON_CLOTHING:
	case GOLD_SKELETON_CLOTHING:
		name = "Skeleton Clothing";
		stats[EActorStats::HEALTH] = 70;
		stats[EActorStats::ARMOR] = 20;
		stats[EActorStats::SEEK_CHANCE] = 100;
		stats[EActorStats::SPEED] = 6;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 80;
		break;
	case KING_SKELETON_CLOTHING:
		name = "Skeleton King";
		stats[EActorStats::HEALTH] = 90;
		stats[EActorStats::ARMOR] = 30;
		stats[EActorStats::SEEK_CHANCE] = 100;
		stats[EActorStats::SPEED] = 7;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 90;
		stats[EActorStats::USE_MAGIC_CHANCE] = 100;
		break;
	case BASIC01:
	case BASIC02:
	case BASIC03:
		name = "Basic";
		stats[EActorStats::HEALTH] = 50;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 50;
		stats[EActorStats::SPEED] = 6;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 50;
		break;
	case SMALL_GHOST:
	case NORMAL_GHOST:
	case CRASY_GHOST:
		name = "Ghost";
		stats[EActorStats::HEALTH] = 60;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 70;
		stats[EActorStats::SPEED] = 5;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 0;
		stats[EActorStats::CAN_NOCLIP] = 1;
		stats[EActorStats::USE_MAGIC_CHANCE] = 50;
		break;
	case NORMAL_REAPER:
	case SICK_REAPER:
	case STICK_REAPER:
		name = "Reaper";
		stats[EActorStats::HEALTH] = 90;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 60;
		stats[EActorStats::SPEED] = 6;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 20;
		break;
	case GRIM_REAPER01:
	case GRIM_REAPER02:
		name = "Grim Reaper";
		stats[EActorStats::HEALTH] = 70;
		stats[EActorStats::ARMOR] = 20;
		stats[EActorStats::SEEK_CHANCE] = 70;
		stats[EActorStats::SPEED] = 6;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 20;
		stats[EActorStats::USE_MAGIC_CHANCE] = 1;
		break;
	case WALKING_MOUSE_SMALL:
	case WALKING_MOUSE_BIG:
		name = "Walking Mouse";
		stats[EActorStats::HEALTH] = 40;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 30;
		stats[EActorStats::SPEED] = 13;
		break;
	case BASIC01_01:
	case BASIC02_01:
	case BASIC01_02:
	case BASIC02_02:
	case BASIC01_03:
		name = "Basic";
		stats[EActorStats::HEALTH] = 50;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 50;
		stats[EActorStats::SPEED] = 6;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 50;
		break;
	case SHAODW_GUY:
	case SMALL_SHADOW_GUY:
		name = "Shadow Guy";
		stats[EActorStats::HEALTH] = 80;
		stats[EActorStats::ARMOR] = 40;
		stats[EActorStats::SEEK_CHANCE] = 40;
		stats[EActorStats::SPEED] = 4;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 0;
		break;
	case OLD_MAN:
		name = "Old Man";
		stats[EActorStats::HEALTH] = 30;
		stats[EActorStats::ARMOR] = 0;
		stats[EActorStats::SEEK_CHANCE] = 0;
		stats[EActorStats::SPEED] = 3;
		break;
	case EVIL_GUY:
		name = "Evil Guy";
		stats[EActorStats::HEALTH] = 200;
		stats[EActorStats::ARMOR] = 100;
		stats[EActorStats::SEEK_CHANCE] = 100;
		stats[EActorStats::SPEED] = 16;
		stats[EActorStats::GATHER_ITEM_CHANCE] = 100;
		stats[EActorStats::USE_MAGIC_CHANCE] = 50;
		stats[EActorStats::ATTACK_CHANCE] = 0;
		break;
	default:
		//Don't put defaults in here since some Actor may not have fields for certain stats
		break;
	}

	return stats;
}//End getAIStats