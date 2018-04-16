#include "GameManager.h"


//==================================================================================
// Constructor
//==================================================================================
GameManager::GameManager(Graphics ^ startCanvas, Random ^ startRGen, Point^ startMapSize, Point startResolution)
{
	canvas = startCanvas;
	rGen = startRGen;
	mapSize = startMapSize;
	resolution = startResolution;

	//Create the double buffering
	context = BufferedGraphicsManager::Current;
	context->MaximumBuffer = System::Drawing::Size(resolution.X, resolution.Y);
	bufferGraphics = context->Allocate(canvas, Rectangle(0, 0, resolution.X, resolution.Y));

	mainMenu = gcnew GameMainMenu(bufferGraphics->Graphics, resolution);

	//Background colour for the canvas (The Void)
	backgroundColour = gcnew SolidBrush(Color::FromArgb(GameSettings::COLOUR_255, 0, GameSettings::COLOUR_255));

	logger = gcnew Logger();

	//We use tilelist in the GameManager and not just the tilemap so we can call from gamemanager to tilelist and use tilemap
	tilelists = gcnew array<TileList^>(GameSettings::NUMBER_OF_TILESETS);
	//Loading tile images
	tilelists[ETileType::FLOOR] = gcnew TileList("images/Objects/Floor.bmp", GameSettings::TILE_SIZE, true);
	tilelists[ETileType::WALL] = gcnew TileList("images/Objects/Wall.bmp", GameSettings::TILE_SIZE, false);
	tilelists[ETileType::VOID01] = gcnew TileList("images/Objects/Pit0.bmp", GameSettings::TILE_SIZE, false);
	tilelists[ETileType::VOID02] = gcnew TileList("images/Objects/Pit1.bmp", GameSettings::TILE_SIZE, false);
	//Creates another set of lists used for the mini-map used as an optmization technique
	miniMapTileLists = gcnew array<TileList^>(GameSettings::NUMBER_OF_TILESETS);
	//Loading tile images
	miniMapTileLists[ETileType::FLOOR] = gcnew TileList("images/Objects/Floor_MiniMap.bmp", GameSettings::MINI_MAP_TILE_SIZE, true);
	miniMapTileLists[ETileType::WALL] = gcnew TileList("images/Objects/Wall_MiniMap.bmp", GameSettings::MINI_MAP_TILE_SIZE, false);
	miniMapTileLists[ETileType::VOID01] = gcnew TileList("images/Objects/Pit0_MiniMap.bmp", GameSettings::MINI_MAP_TILE_SIZE, false);
	miniMapTileLists[ETileType::VOID02] = gcnew TileList("images/Objects/Pit1_MiniMap.bmp", GameSettings::MINI_MAP_TILE_SIZE, false);

	//Creates the tilemap which holds the map information
	tilemap = gcnew TileMap(tilelists, bufferGraphics->Graphics, mapSize->X, mapSize->Y);
	miniMapTilemap = gcnew TileMap(tilelists, bufferGraphics->Graphics, mapSize->X, mapSize->Y);
	spritemages = gcnew LoadImages();
	dungeon = gcnew DungeonDesigner(tilemap, rGen);
	//Generates an instance of the player
	player = GeneratePlayer();
	//Creates the control class used to control actors using inputs
	control = gcnew Controls(player);

	//Sets count to zero for use in the animations
	frameInterval = 0;

	positionToDawPlayer.X = (resolution.X - GameSettings::TILE_SIZE) / DEVIED_BY_2;
	positionToDawPlayer.Y = (resolution.Y - GameSettings::TILE_SIZE) / DEVIED_BY_2;

	//Width and Height of the viewport converted from the resolution to tiles
	//Taking into account the spawning in of the tiles on the edges of the map
	int viewPortWidth = resolution.X / GameSettings::TILE_SIZE;
	int viewPortHeight = resolution.Y / GameSettings::TILE_SIZE;

	//PositionX, PositionY, Width, Height, TileSide for the below viewport
	viewport = gcnew ViewPort(player->getPositionX(), player->getPositionX(), viewPortWidth, viewPortHeight, tilemap,
		bufferGraphics->Graphics, Point(GameSettings::PLAYER_VIEWPORT_X, GameSettings::PLAYER_VIEWPORT_Y), GameSettings::VIEW_PORT_TILE_SIZE, true);
	gui = gcnew UI(bufferGraphics->Graphics, resolution, miniMapTilemap, player->getPositionX(), player->getPositionY(), player, spritemages, rGen, logger);

	//Starts the game
	generateWholeDungeon();

	gameStarted = false;
	//bufferGraphics->Graphics->RotateTransform(23);
} //End Constructor

//==================================================================================
// Generates a new dungeon
//==================================================================================
void GameManager::generateWholeDungeon()
{
	tilemap->setAllNotVisable();
	//Creates the dungeon
	populateMap();

	//Place the player
	player->NotSpawnOnWalkable();

	aiList = gcnew SpriteLinkedList();
	itemsList = gcnew SpriteLinkedList();
	objectsList = gcnew SpriteLinkedList();
	effectList = gcnew SpriteLinkedList();

	int actorsSize = EActor::ENUM_LENGTH_ACTORS;
	int moneySize = EItems::MONEY_BLANK04;
	int positionSize = EItems::POTION_GREEN;

	//Generates the Actors
	for (int i = 0; i < GameSettings::NUMBER_OF_STARTING_SPTIRES; i++)
	{
		int actorFromSeection = rGen->Next(actorsSize);
		bool foundSprite = false;

		//Checks if the Actor is an empty image
		while (foundSprite == false)
		{
			switch (EActor(actorFromSeection))
			{
			case BLANK01:
			case BLANK02:
			case BLANK03:
			case BLANK04:
			case BLANK05:
			case BLANK06:
			case BLANK07:
			case BLANK08:
			case BLANK09:
			case BLANK10:
			case BLANK11:
			case BLANK12:
			case BLANK13:
			case BLANK14:
			case BLANK15:
			case BLANK16:
			case BLANK17:
			case BLANK18:
			case BLANK19:
			case BLANK20:
			case BLANK21:
			case BLANK22:
			case BLANK23:
			case BLANK24:
			case BLANK25:
			case BLANK26:
			case BLANK27:
			case BLANK28:
				actorFromSeection = rGen->Next(actorsSize);
				break;
			default:
				foundSprite = true;
				break;
			}
		}
		//Randomly picks which sprite to generate
		Actor^ actor = generateActor((EActor)actorFromSeection);
		//Adds the sprite to the linked list
		aiList->addActor(actor);
	}

	//Generate the money
	for (int i = 0; i < GameSettings::NUMBER_OF_MONEY; i++)
	{
		int itemFromSeection = rGen->Next(moneySize);

		bool foundSprite = false;
		//Checks if the Actor is an empty image
		while (foundSprite == false)
		{
			switch ((EItems)itemFromSeection)
			{
			case MONEY_BLANK01:
			case MONEY_BLANK02:
			case MONEY_BLANK03:
			case MONEY_BLANK04:
				itemFromSeection = rGen->Next(moneySize);
				break;
			default:
				foundSprite = true;
				break;
			}
		}
		Items^ item = generateItem((EItems)itemFromSeection);
		itemsList->addActor(item);
	}

	//Generate the potions
	for (int i = 0; i < GameSettings::NUMBER_OF_POTIONS; i++)
	{
		int itemFromSeection = rGen->Next(moneySize, positionSize);

		Items^ item = generateItem((EItems)itemFromSeection);
		itemsList->addActor(item);
	}

	//Creates the exit for the game
	//Generate the door first before the traps so it's always at the head of the linked list
	Objects^ exit = generateObject(EObjects::STEEL_PORTAL_DOOR_OPEN, EObjectType::DOOR);
	objectsList->addActor(exit);

	//Generate traps
	for (int i = 0; i < GameSettings::NUMBER_OF_TRAPS; i++)
	{
		int trapFromSelection = rGen->Next(GameSettings::NUMBER_OF_TRAPS_OB);
		switch (trapFromSelection)
		{
		case 0:
			trapFromSelection = (int)EObjects::SPARES_TRAP_HALF;
			break;
		case 1:
			trapFromSelection = (int)EObjects::SMALL_SPICKS_TRAP_FULL;
			break;
		case 2:
			trapFromSelection = (int)EObjects::SMALL_SPICKS_TRAP_HALF;
			break;
		default:
			break;
		}
		Objects^ object = generateObject((EObjects)trapFromSelection, EObjectType::TRAP);

		objectsList->addActor(object);
	}

}//End generateWholeDungeon

//==================================================================================
// Cycles through the game by a frame
//==================================================================================
void GameManager::gameCycle()
{
	//Create a memory and store all the below code into buffer graphics, then render to canvas at buffergraphics->
	bufferGraphics->Graphics->FillRectangle(backgroundColour, 0, 0, resolution.X, resolution.Y);
	if (gameStarted == true)
	{
		//Updates viewport last
		updateViewPort();
		//Draws the map
		drawMap();
		//Controls the sprites
		ItemsControl();
		ObjectsControl();
		playerControl();
		AIControl();
		projectilesControl();
		//Updates things such as animations at certain intervals
		UpdateOnFrame();
		//Draw the GUI
		//if (gameStarted == true)
		gui->drawHud();
	}
	else
	{
		mainMenu->DrawMenu();
		gameStarted = mainMenu->getGameStart();
	}
	//Render the buffer graphics stored in memoery
	bufferGraphics->Render(canvas);
} //End gameCycle

//==================================================================================
// Populates the map using a generation algorithm
//==================================================================================
void GameManager::populateMap()
{
	dungeon->makeNewDungeon(GameSettings::NUMBER_OF_ROOMS);
	//Set the minimap to the main tilemap
	miniMapTilemap->setMap(tilemap->getMap());
} //End populateMap

 //==================================================================================
 // Draws the map to the screen
 //==================================================================================
void GameManager::drawMap()
{
	bool fillVoid = true;
	viewport->viewportDraw(fillVoid, player->getPositionX(), player->getPositionY());
}//End drawMap

//==================================================================================
// Controls the runtime of the Player Actor
//==================================================================================
void GameManager::playerControl()
{
	player->updateAttackRange();
	player->updateDetectRange();
	checkPlayerOnTrap();
	control->controledMovement();
	//player->DrawCollisionBox(((resolution.X - GameSettings::TILE_SIZE) / 2), ((resolution.Y - GameSettings::TILE_SIZE) / 2));
	player->draw(positionToDawPlayer.X, positionToDawPlayer.Y);
}//End playerControl

//==================================================================================
// Controls the runtime of the AIs Actors
//==================================================================================
void GameManager::AIControl()
{
	//aiList->moveActors();
	Sprite^ wantsToUseMagic = aiList->actorWantsToUseMagic();
	if (wantsToUseMagic != nullptr) createProjectile(wantsToUseMagic);
	aiList->checkIfMarkedForDeletion();
	aiList->checkWithinDetection(player);
	aiList->checkWithinBoundingArea();
	aiList->checkActorOnItem(itemsList);
	aiList->checkIfActortuck();
	aiList->checkActorOnTrap(objectsList);
	aiList->updateAtackRange();
	aiList->updateDetection();
	aiList->updateState();
	aiList->performAction();
	//aiList->drawActorsCollisions(viewport->getViewportWoldX(), viewport->getViewportWoldY(), viewport->getViewportWidth(), viewport->getViewportHigh());
	aiList->canLogText(viewport->getViewportWoldX(), viewport->getViewportWoldY(), viewport->getViewportWidth(), viewport->getViewportHigh());
	aiList->drawHealthBars(viewport->getViewportWoldX(), viewport->getViewportWoldY(), viewport->getViewportWidth(), viewport->getViewportHigh());
	aiList->renderActors(viewport->getViewportWoldX(), viewport->getViewportWoldY(), viewport->getViewportWidth(), viewport->getViewportHigh());
	//aiList->drawFakeEyeSight(viewport->getViewportWoldX(), viewport->getViewportWoldY(), viewport->getViewportWidth(), viewport->getViewportHigh());
}//End playerControl

//==================================================================================
// Controls the runtime of the projectiles
//==================================================================================
void GameManager::projectilesControl()
{
	effectList->checkIfMarkedForDeletion();
	effectList->checkTouchingWall();
	effectList->checkProjectileTouchingActor(player);
	effectList->moveActors();
	effectList->renderActors(viewport->getViewportWoldX(), viewport->getViewportWoldY(), viewport->getViewportWidth(), viewport->getViewportHigh());
}//End projectilesControl

//==================================================================================
// Controls the runtime of the Items
//==================================================================================
void GameManager::ItemsControl()
{
	itemsList->checkIfMarkedForDeletion();
	itemsList->renderActors(viewport->getViewportWoldX(), viewport->getViewportWoldY(), viewport->getViewportWidth(), viewport->getViewportHigh());
}//End playerControl

//==================================================================================
// Controls the runtime of the Objects
//==================================================================================
void GameManager::ObjectsControl()
{
	objectsList->checkIfMarkedForDeletion();
	objectsList->renderActors(viewport->getViewportWoldX(), viewport->getViewportWoldY(), viewport->getViewportWidth(), viewport->getViewportHigh());
}//End ObjectsControl

//==================================================================================
// Updates certain things at a certain frame interval
//==================================================================================
void GameManager::UpdateOnFrame()
{
	//Delays the update frame to allow lower cycle for more frames to be displayed on the game, essentially making it not laggy
	if (frameInterval == 0)
	{
		aiList->changeFrameActors();
		objectsList->changeFrameObjects();
		//viewport->drawAnimation();
		gui->updateUIElementsFrames();
		player->UpdateFrame();
		tilemap->removeTime();
	}
	//Adds 1 to the frameInterval anc checks if it's over the a certain amount if so reset it
	frameInterval = (frameInterval + 1) % GameSettings::FRAMEUPDATE_SPEED;
}

//==================================================================================
// Creates a projectile object and adjusts it to the actors position
//==================================================================================
void GameManager::createProjectile(Sprite^ sprite)
{
	array<Bitmap^>^ images = spritemages->getImages(ESpriteType::OBJECT, GameSettings::FIRE_BALL);

	Objects^ projectile = gcnew Objects(bufferGraphics->Graphics, tilemap, rGen, images, logger, EObjects::ARROW_TRAP_ICON, EObjectType::EFFECTS);

	int spriteXPos = sprite->getPositionX();
	int spriteYPos = sprite->getPositionY();

	projectile->setPositionX(spriteXPos);
	projectile->setPositionY(spriteYPos);

	projectile->pointMeAt(player, GameSettings::MAGIC_SPEED);

	effectList->addActor(projectile);
}//End createProjectile


//==================================================================================
// Creates an instance of the player
//==================================================================================
Player^ GameManager::GeneratePlayer()
{
	//Set speed for the sprite
	int nFrames = GameSettings::NUMBER_OF_FRAMES_PLAYER;
	int speed = GameSettings::SPRITE_SPEED_PLAYER;
	int nDirections = GameSettings::NUMBER_OF_DRECTIONS;

	//Creates an instance of the sprite
	Player^ actor = gcnew Player(bufferGraphics->Graphics, tilemap, rGen, spritemages->LoadPlayerImage(), logger, nFrames, nDirections, speed, true, EActor::BLANK01);
	//Return the sprite to be used
	return actor;

}//End GenerateNakedManSprite

//==================================================================================
// Creates an instance of an actor
//==================================================================================
AI ^ GameManager::generateActor(EActor actorType)
{
	//Sets the number of frames for the chickens
	int framesInChickenSheet = GameSettings::NUMBER_OF_FRAMES_AI;
	int nFrames = 1;
	int nDirections = GameSettings::NUMBER_OF_DRECTIONS;
	//Set speed for the sprite
	int speed = GameSettings::SPRITE_SPEED_AI;

	array<Bitmap^>^ imageSet = spritemages->getImages(ESpriteType::ACTOR, (int)actorType);
	array<Bitmap^>^ images = spritemages->CreateBitMapSpriteArray(imageSet[0], imageSet[1], imageSet[0], imageSet[1], nDirections);

	//Creates an instance of the sprite
	AI^ actor = gcnew AI(bufferGraphics->Graphics, tilemap, rGen, images, logger, nFrames, nDirections, speed, false, actorType);
	//Return the sprite to be used
	return actor;
}//End generateActor

//==================================================================================
// Creates an instance of an object
//==================================================================================
Objects ^ GameManager::generateObject(EObjects object, EObjectType objectType)
{
	//Sets the number of frames for the chickens
	int framesInChickenSheet = GameSettings::NUMBER_OF_FRAMES_AI;
	int nFrames = GameSettings::NUMBER_OF_FRAMES_AI;
	int nDirections = GameSettings::NUMBER_OF_DRECTIONS;
	//Set speed for the sprite
	int speed = GameSettings::SPRITE_SPEED_AI;

	array<Bitmap^>^ imageSet = spritemages->getImages(ESpriteType::OBJECT, (int)object);

	//Creates an instance of the sprite
	Objects^ objectInstance = gcnew Objects(bufferGraphics->Graphics, tilemap, rGen, imageSet, logger, object, objectType);
	//Return the sprite to be used
	return objectInstance;
}//End generateObject

//==================================================================================
// Creates an instance of an item
//==================================================================================
Items ^ GameManager::generateItem(EItems item)
{
	//Sets the number of frames for the chickens
	int framesInChickenSheet = GameSettings::NUMBER_OF_FRAMES_AI;
	int nFrames = GameSettings::NUMBER_OF_FRAMES_AI;
	int nDirections = GameSettings::NUMBER_OF_DRECTIONS;
	//Set speed for the sprite
	int speed = GameSettings::SPRITE_SPEED_AI;

	array<Bitmap^>^ imageSet = spritemages->getImages(ESpriteType::ITEM, (int)item);

	//Creates an instance of the sprite
	Items^ objectInstance = gcnew Items(bufferGraphics->Graphics, tilemap, rGen, imageSet, logger, item);
	//Return the sprite to be used
	return objectInstance;
}//End generateItem

//==================================================================================
// Changes direction of Player sprite
//==================================================================================
void GameManager::spriteDirection(EDirection direction, bool isMoving)
{
	control->setDirections(direction, isMoving);
}//End spriteDirection

//==================================================================================
// Check if player it on a Trap
//==================================================================================
void GameManager::checkPlayerOnTrap()
{
	int index = 0;
	while (index < (objectsList->countActors()))
	{
		Objects^ object = (Objects^)objectsList->getActor(index);

		bool canDamageActor = ((Actor^)player)->checkTrapCanDamageActor(object);

		if (canDamageActor == true)
		{
			((Actor^)player)->trapDamageActor(object);
		}
		index++;
	}
}//End checkPlayerOnTrap

//==================================================================================
// Tells the player to attack and checks if it can attack anything
//==================================================================================
void GameManager::actorAttack()
{
	bool canAttacKActor = false;

	for (int i = 0; i < aiList->countActors(); i++)
	{
		Actor^ actor = (Actor^)aiList->getActor(i);
		if (actor->getIsAlive() == true)
		{
			canAttacKActor = player->checkWithinAttackRange(actor);
			if (canAttacKActor == true)
			{
				control->attack();
			}
			else
			{
				canAttacKActor = false;
			}
		}
	}
}//End actorAttack

 //==================================================================================
 // When the player wants to interact with things in the game
 //==================================================================================
void GameManager::actorInteract()
{
	// Checks if the player can collect the item
	Sprite^ canCollectItem = itemsList->checkIfPlayerInGatheringRange(player);
	if (canCollectItem != nullptr)
	{
		control->pickUpItem(canCollectItem);
	}

	// Checks if the player can use the item
	Sprite^ canUseObject = objectsList->checkIfPlayerInGatheringRange(player);
	if (canUseObject != nullptr)
	{
		bool isDoor = control->useObject(canUseObject);

		if (isDoor == true)
		{
			generateWholeDungeon();
		}
	}
}//End actorInteract

//==================================================================================
// Updates the viewports position on the map releative to the player
//==================================================================================
void GameManager::updateViewPort()
{
	viewport->moveRelativeToPlayer(player->getPositionX() + (player->getWidth() / DEVIED_BY_2), player->getPositionY());
}//End updateViewPort


//==================================================================================
// Tells the GUI to update the mouse position to the new position
//==================================================================================
void GameManager::setMousePosition(Point newPosition)
{
	mainMenu->setMousePosition(newPosition);
	gui->setMousePosition(newPosition);
}//End setMousePosition

//==================================================================================
// Tells the GUI the mouse button has been clicked
//==================================================================================
void GameManager::setMouseDown()
{
	mainMenu->setMouseDown(true);
	gui->setMouseDown(true);
}//End setMouseDown

 //==================================================================================
 // Tells the GUI the mouse right button has been clicked
 //==================================================================================
void GameManager::setMouseDownRight()
{
	gui->setMouseDownRight(true);
}//End setMouseDownRight
