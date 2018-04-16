#pragma once

//Included Classes
#include "GameSettings.h"
#include "Sprite.h"
#include "Actor.h"
#include "ViewPort.h"
#include "AI.h"
#include "Controls.h"
#include "DungeonDesigner.h"
#include "LoadImages.h"
#include "UI.h"
#include "SpriteLinkedList.h"
#include "Objects.h"
#include "Logger.h"
#include "GameMainMenu.h"
#include "Player.h"

#define DEVIED_BY_2 2

//Namespaces
using namespace System;
using namespace System::Drawing;

//==================================================================================
// The Game Manager is the controller which hold the entire game togeather
// It's used to control the game cycle
//==================================================================================
ref class GameManager
{
private:
	BufferedGraphicsContext^ context; //Double Buffering
	BufferedGraphics^ bufferGraphics; //Double Buffering
	SolidBrush^ backgroundColour; //Double Buffering background
	array<TileList^>^ tilelists; //Map tiles
	array<TileList^>^ miniMapTileLists; //Mini-map tiles
	ViewPort^ viewport; //Viewport
	Controls^ control; //Input for keyboard and player
	GameMainMenu^ mainMenu; //Main Menu
	UI^ gui; //In-game UI
	DungeonDesigner^ dungeon; //Creates the dungeon
	Point resolution; //Game Resolution
	Graphics^ canvas; //Canvas Pointer
	Point^ mapSize; //Size of the map
	TileMap^ tilemap; //Containts the map
	TileMap^ miniMapTilemap;
	Player^ player; //Player Actor
	Random^ rGen; //Random Pointer
	Logger^ logger; //Used for logging in-game changes
	SpriteLinkedList^ aiList; //Sprite linked lists
	SpriteLinkedList^ itemsList;
	SpriteLinkedList^ objectsList;
	SpriteLinkedList^ effectList;
	LoadImages^ spritemages; //Sprite images
	Point positionToDawPlayer;
	int frameInterval; //Interval which certain things update
	bool gameStarted;

	Player ^ GeneratePlayer();
	AI ^ generateActor(EActor actor);
	Objects ^ generateObject(EObjects object, EObjectType objectType);
	Items ^ generateItem(EItems item);
public:
	//Constructor
	GameManager(Graphics^ startCanvas, Random^ startRGen, Point^ startMapSize, Point startResolution);

	void generateWholeDungeon();

	void gameCycle();
	void populateMap();
	void drawMap();
	void playerControl();
	void AIControl();
	void projectilesControl();
	void createProjectile(Sprite ^ sprite);
	void ItemsControl();
	void ObjectsControl();
	void UpdateOnFrame();
	void spriteDirection(EDirection direction, bool isMoving);
	void checkPlayerOnTrap();
	void actorAttack();
	void actorInteract();
	void updateViewPort();
	void setMousePosition(Point newPosition);
	void setMouseDown();
	void setMouseDownRight();
};

