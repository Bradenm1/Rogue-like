#include "AI.h"


//==================================================================================
// Constructor
//==================================================================================
AI::AI(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger^ startLogger, int startNFrames, int startNDrections, int startSpeed, bool startIsplayer, EActor startActor)
	: Actor(startCanvas, startTilemap, startRGen, startSpriteSheets, startLogger, startNFrames, startNDrections, startIsplayer)
{
	nTicks = rGen->Next(GameSettings::SPRITE_IDLE_MAX);

	// Set default number of frames for animations
	nFrames = GameSettings::NUMBER_OF_FRAMES_AI;

	// Set the actor
	actor = startActor;
	// Get the actors stats
	stats = getAIStats(actor);

	default_health = stats[EActorStats::HEALTH];

	// Set default speed
	velocity.X = stats[EActorStats::SPEED];
	velocity.Y = stats[EActorStats::SPEED];

	// Healthbar colours
	emptyHealth = gcnew Pen(Color::Red, 10);
	fullHealth = gcnew Pen(Color::Green, 10);
	behindHealth = gcnew Pen(Color::Black, 14);

	// Set bounding area for the AI
	setBoundingArea();
}//End Constructor

//==================================================================================
// Draws sprite to canvas
//==================================================================================
void AI::draw(int forcedX, int forcedY)
{
	if (isAlive == true)
	{
		//Set the position of x and y for the rectangle to use for the animation
		//Times current frame by frame width to change the frame, e.g the frame is 32X32 so the first frame would be 32, the second itteration would be 64, 92 and so fourth
		int startX = 0;
		int startY = 0;

		//Creates a rectangle at a certain size for the spritesheet to use to draw only certain parts of the image
		Rectangle pixelsToDraw = Rectangle(startX, startY, size.X, size.Y);

		//Draws the bitmap to the canvas but only draws a certain part of that bitmap using what was created in the pixelsToDraw above
		canvas->DrawImage(spriteSheets[currentFrame],
			forcedX,				// on canvas
			forcedY,				// on canvas
			pixelsToDraw,			// on source
			GraphicsUnit::Pixel);	//Measurment
	}
}//End Draw

//==================================================================================
// Moves the Player
//==================================================================================
void AI::move()
{
	//Checks if tile the sprite is about to move on is walkable
	bool walkableCheck = UpdateTileArea();

	// Check if that tile was walkable and Ai is still alive
	if ((walkableCheck == true) && (isAlive == true))
	{
		// Check if AI is seeking or avoiding
		if ((actorState == EActorState::SEEKING) || (actorState == EActorState::AVOIDING))
		{
			position.X += velocity.X;
			position.Y += velocity.Y;
		}
		else
		{
			position.X += velocity.X * velocityDirections[spriteDirection].X;
			position.Y += velocity.Y * velocityDirections[spriteDirection].Y;
		}
	}
}//End move

//==================================================================================
// Draws a health bar above the AI
//==================================================================================
void AI::drawHealth(int forcedX, int forcedY)
{
	forcedY = forcedY - GameSettings::HEALTH_BAR_OFFSET;
	forcedX = (forcedX - default_health / 2) + (size.X / 2);
	int emptyValue = default_health - stats[EActorStats::HEALTH];

	canvas->DrawLine(behindHealth, Point(forcedX - 2, forcedY - 2), Point(forcedX + default_health + 2, forcedY + 2));
	canvas->DrawLine(emptyHealth, Point(forcedX + stats[EActorStats::HEALTH], forcedY), Point(forcedX + default_health, forcedY));
	canvas->DrawLine(fullHealth, Point(forcedX, forcedY), Point(forcedX + stats[EActorStats::HEALTH], forcedY));
}//End drawHealth

//==================================================================================
// Draws fake eyesight for the AI seeing the player
//==================================================================================
void AI::eyeSight(int otherFakex, int otherFakeY, int fakeX, int fakeY, int xToAdd, int yToAdd)
{
	int posX = fakeX;
	int posY = fakeY;

	SolidBrush^ colour = gcnew SolidBrush(Color::Green);
	bool viewBlock = false;
	// Loop through every pixle from AI to thing is sight
	for (int i = 0; i < GameSettings::SPRITE_EYESIGHT_RANGE; i++)
	{
		int fakexDelta = otherFakex - posX;
		int fakeyDelta = otherFakeY - posY;

		Point fakePos;
		// Get fake angle to target allow the user to see the what the AI is seeing if needed
		int fakeangleToTarget = Math::Atan2(fakeyDelta, fakexDelta);

		// Set next point
		fakePos.X = Math::Cos(fakeangleToTarget) * stats[EActorStats::SPEED];
		fakePos.Y = Math::Sin(fakeangleToTarget) * stats[EActorStats::SPEED];

		// Add-on fake view
		posX += fakePos.X;
		posY += fakePos.Y;

		// Check if tile is walkable
		bool onWalkable = tilemap->IsMapEntryWalkable((posY + yToAdd) / GameSettings::TILE_SIZE, (posX + xToAdd) / GameSettings::TILE_SIZE);

		// Check if view is blocked
		if ((onWalkable == false) && (stats[EActorStats::CAN_NOCLIP] == 0))
		{
			viewBlock = true;
		}
	
		// If view is blocked change colour to red
		if (viewBlock == true)
		{
			colour->Color = Color::Red;
		}

		// Draw ellipse representing a the sight, which fakes a line
		canvas->FillEllipse(colour, posX, posY, GameSettings::AI_SIZE_OF_EYESIGHT, GameSettings::AI_SIZE_OF_EYESIGHT);
	}
}//End eyeSight

//=================================================================================
// Updates the state of the actor
//=================================================================================
void AI::UpdateState()
{
	switch (actorState)
	{
		case WANDERING:
			// Check if AI is alive
			if (isAlive == true)
			{
				//If the AI has been wandering too long
				if (nTicks >= GameSettings::SPRITE_WANDERING_MAX)
				{
					//Chose to change orientation or to idle
					if (rGen->Next(stats[EActorStats::ORIENTATION_CHANCE]) == 0)
					{
						actorState = EActorState::IDLE;
						nTicks = rGen->Next(0, GameSettings::SPRITE_IDLE_MAX);

					}
					else // Change the AIs orientation
					{
						actorState = EActorState::ORIENTATION;
						nTicks = 0;
					}
				}
				else
				{
					//If the AI detects an item
					if (itemWithinDetection != nullptr)
					{
						// Get the chance of 
						double gatherChance = ((double)stats[EActorStats::GATHER_ITEM_CHANCE]) / CHANGE_TO_DOUBLE_FROM_INT;

						if (rGen->NextDouble() < gatherChance)
						{
							//Try get the item
							actorState = EActorState::SEEKING;
							seekingState = ESeekingState::SEEKITEM;
							nTicks = GameSettings::CHECK_STUCK_MAX;
						}
						else
						{
							//Does not want the item
							itemWithinDetection = nullptr;
						}
					}
					{
						//If the AI detects an actor
						if (actorWtihinDetection != nullptr)
						{
							//Get the chance at which magic will be used by the AI
							double magicChance = ((double)stats[EActorStats::USE_MAGIC_CHANCE]) / CHANGE_TO_DOUBLE_FROM_INT;

							//If the AI wants to use magic
							if (rGen->NextDouble() < magicChance)
							{
								attackState = EActorAttackState::MAGIC;
								actorState = EActorState::ATTACKING;
							}
							//If the AI does not want to use magic
							else
							{
								EActorAttackState::HAND;
								//Get the chance at which voiding or seeking will happen to the AI
								double seekChance = ((double)stats[EActorStats::SEEK_CHANCE]) / CHANGE_TO_DOUBLE_FROM_INT;

								if (rGen->NextDouble() < seekChance)
								{
									//Seek actor
									actorState = EActorState::SEEKING;
									seekingState = ESeekingState::SEEKACTOR;
								}
								else
								{
									//Run from actor
									actorState = EActorState::AVOIDING;
								}
							}
						}
						else
						{
							// Get use chance given the AIs use stats
							double useChance = ((double)stats[EActorStats::USE_ITEM_CHANCE]) / CHANGE_TO_DOUBLE_FROM_INT;

							// Check if AIs health is low and it chance is within range
							// If so, try use health item
							if ((stats[EActorStats::HEALTH] < GameSettings::AI_HEALTH_LOW) && (rGen->NextDouble() < useChance))
							{
								bool itemFound = false;
								// Loop through inventory
								for (int i = 0; i < items->Length; i++)
								{
									// Check if item in inventory slot exists and item has not already been found
									if ((items[i] != nullptr) && (itemFound == false))
									{
										// Check if AI has health incress position, if so use it.
										if (((Items^)items[i])->getItemStatEfffect() == EItemStats::HEALTH_INCRESS_FACTOR)
										{
											itemToUse = items[i];
											actorState = EActorState::USE_ITEM;
											items[i] = nullptr;
											itemFound = true;
										}
									}
								}
							}
							else
							{
								//useChance = ((double)stats[EActorStats::USE_ITEM_CHANCE]) / CHANGE_TO_DOUBLE_FROM_INT;
								//if (rGen->NextDouble() > useChance)
								//{
								//	bool itemFound = false;
								//	for (int i = 0; i < items->Length; i++)
								//	{
								//		if ((items[i] != nullptr) && (itemFound == false))
								//		{
								//			if (((Items^)items[i])->getItemStatEfffect() != EItemStats::WORTH)
								//			{
								//				itemToUse = items[i];
								//				actorState = EActorState::USE_ITEM;
								//				items[i] = nullptr;
								//				itemFound = true;
								//			}
								//		}
								//	}
								//}
								//else
								//{
								//
								//}
							}
						}
					}
				}
			}
			else
			{
				actorState = EActorState::DEAD;
			}
			break;
		case SEEKING:
			if (isAlive == true)
			{
				switch (seekingState)
				{
				case SEEKITEM:
				{
					//If item is out of range or is in another players inventory run the below
					if ((checkWithinDetectRange(itemWithinDetection) == false) || (itemWithinDetection->getIsVisible() == false))
					{
						actorState = EActorState::WANDERING;
						// Set wandering time
						nTicks = rGen->Next(0, GameSettings::SPRITE_WANDERING_MAX);
						// Remove items if there was one within detection range
						itemWithinDetection = nullptr;
						// Reset speed
						velocity.X = stats[EActorStats::SPEED];
						velocity.Y = stats[EActorStats::SPEED];
					}
					else
					{
						//If item is within gathering range of the ai
						if (checkWithinGatheringRange(itemWithinDetection) == true)
						{
							actorState = EActorState::GATHERINGITEM;
						}
						else
						{
							//if (nTicks > 100)
							//{
							//	if (currentPositionCheck == Position)
							//	{
							//		Position = Point(itemWithinDetection->getPositionX(), itemWithinDetection->getPositionY());
							//	}
							//	else
							//	{
							//		nTicks = 0;
							//		currentPositionCheck = Position;
							//	}
							//}
							//else
							//{
							//	nTicks++;
							//}
						}
					}
				}
				break;
				case SEEKACTOR:
					//If the actor to seek is outside its detect range run the below
					if (checkWithinDetectRange(actorWtihinDetection) == false)
					{
						actorState = EActorState::WANDERING;
						nTicks = rGen->Next(0, GameSettings::SPRITE_WANDERING_MAX);
						actorWtihinDetection = nullptr;
						velocity.X = stats[EActorStats::SPEED];
						velocity.Y = stats[EActorStats::SPEED];
					}
					else
					{
						//If the actor is within the detect range run the below
						if (checkWithinAttackRange(actorWtihinDetection) == true)
						{
							// Check if AI wants to attack an actor
							if (rGen->Next(stats[EActorStats::ATTACK_CHANCE]) == 0)
								actorState = EActorState::ATTACKING;
						}
					}
					break;
				default:
					break;
				}
			}
			else
			{
				actorState = EActorState::DEAD;
			}

			break;
		case GATHERINGITEM:
			// Reset to wandering state
			actorState = EActorState::WANDERING;
			itemWithinDetection = nullptr;
			nTicks = rGen->Next(0, GameSettings::SPRITE_WANDERING_MAX);
			break;
		case USE_ITEM:
			// Reset to wandering state
			actorState = EActorState::WANDERING;
			itemToUse = nullptr;
			nTicks = rGen->Next(0, GameSettings::SPRITE_WANDERING_MAX);
			break;
		case ATTACKING:
			// Check if AI is dead
			if (isAlive == true)
			{
				// Check is Actor is using magic
				if (attackState == EActorAttackState::MAGIC)
				{
					actorState = EActorState::WANDERING;
					nTicks = rGen->Next(0, GameSettings::SPRITE_WANDERING_MAX);
					actorWtihinDetection = nullptr;
					wantsToUseMagic = false;
				}
				else // Not using magic
				{
					// Check if AI is within attacking range, if not reset to seeking
					if (checkWithinAttackRange(actorWtihinDetection) == false)
					{
						actorState = EActorState::SEEKING;
						actorWithinAttackRange = nullptr;
						wantsToUseMagic = false;
					}
				}
			}
			else // If AI is dead
			{
				actorState = EActorState::DEAD;
			}
			break;
		case AVOIDING:
			if (isAlive == true)
			{
				// Check if another actor is within detection range
				if (checkWithinDetectRange(actorWtihinDetection) == false)
				{
					actorState = EActorState::WANDERING;
					nTicks = rGen->Next(0, GameSettings::SPRITE_WANDERING_MAX);
					actorWtihinDetection = nullptr;
					velocity.X = stats[EActorStats::SPEED];
					velocity.Y = stats[EActorStats::SPEED];
				}
			}
			else
			{
				actorState = EActorState::DEAD;
			}
			break;
		case IDLE:
			// Check if AI is alive
			if (isAlive == true)
			{
				// Check if AI has idled for max amount of time
				if (nTicks >= GameSettings::GameSettings::SPRITE_IDLE_MAX)
				{
					// Reset to wandering
					actorState = EActorState::WANDERING;
					nTicks = 0;
				}
			}
			else
			{
				actorState = EActorState::DEAD;
			}
			break;
		case ORIENTATION:
			// Reset to wandering state
			actorState = EActorState::WANDERING;
			nTicks = rGen->Next(0, GameSettings::SPRITE_WANDERING_MAX);
			break;
		case DEAD:
			// If AI is dead marked for deletion
			markedForDeletion = true;
			break;
		default:
			break;
	}
}//End UpdateState

//=================================================================================
// Performs the action of the state
//=================================================================================
void AI::PerformAction()
{
	switch (actorState)
	{
		case WANDERING:
			move();
			nTicks++;
			break;
		case SEEKING:
			// Check what the AI is seeking
			switch (seekingState)
			{
				// Check if AI is seeking an Item
				case SEEKITEM:
					pointMeAt(itemWithinDetection, stats[EActorStats::SPEED]);
					break;
				// Check if AI is seeking an Actor
				case SEEKACTOR:
					pointMeAt(actorWtihinDetection, stats[EActorStats::SPEED]);
					break;
				default:
					break;
			}
			move();
			break;
		case GATHERINGITEM:
			// Gather a given item within range
			gatherItem(itemWithinDetection);
			break;
		case USE_ITEM:
			// Use the given stored item
			useItem(itemToUse);
			break;
		case ATTACKING:
			// Check if AI is using magic or not
			if (attackState == EActorAttackState::MAGIC)
			{
				attackMagic(); // Attack using magic
			}
			else
			{
				attackMelee(); // Attack using melee
			}
			break;
		case AVOIDING:
			// If avoiding point at actor to avoid, then invert direction giving off the impression of avoiding
			pointMeAt(actorWtihinDetection, stats[EActorStats::SPEED]);
			velocity.X = velocity.X * -1;
			velocity.Y = velocity.Y * -1;
			move();
			break;
		case IDLE:
			nTicks++;
			break;
		case ORIENTATION:
			// Changes the AIs direction randomly
			choseRandomDirection();
			break;
		case DEAD:
			// Drop AI inventory on death
			dropInventory();
			break;
		default:
			break;
	}
}//End PerformAction
