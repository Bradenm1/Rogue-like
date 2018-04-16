#pragma once

//Included Classes
#include "GameSettings.h"
#include "TileMap.h"
#include "Logger.h"

#define DEVIDE_BY_2 2

//Name Spaces
using namespace System;
using namespace System::Drawing;

//==================================================================================
// This class is used to hold all the sprites in the game as a parent abstract class
// This is what the items, objects, actors use as their main class
//==================================================================================
ref class Sprite abstract
{
protected:
	Graphics^ canvas; //Canvas Pointer
	TileMap^ tilemap; //Tilemap Pointer
	array<Bitmap^>^ spriteSheets; //Pointer to SpriteSheets
	Random^ rGen;
	Logger^ logger; //Used for logging in-game changes

	//Properties
	Point velocity;
	Point position; //Position on Map
	Point size; //Size of Sprite
	bool visible; //Sprite is visible on canvas or not
	bool markedForDeletion; //Marks Sprite for deletion
	int currentFrame; //Current frame of animation
	int nFrames; //Number of frames

	Sprite^ next;
public:
	//Constructor
	Sprite(Graphics^ startCanvas, TileMap^ startTilemap, Random^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger^ startLogger);

	//virtual void updateCollision() abstract;
	virtual void draw(int forcedX, int forcedY);
	virtual void move();
	void pointMeAt(Sprite ^ otherActor, int speed);
	bool checkIfOnVisableTile();
	void NotSpawnOnWalkable();
	bool checkOnWalkable();
	void UpdateFrame();
	void DrawCollisionBox();
	void DrawCollisionBox(int forcedX, int forcedY);

	//Sets and Gets
	void updateTileMap(TileMap^ newTileMap) { tilemap = newTileMap; }
	int getPositionX() { return position.X; }
	int getPositionY() { return position.Y; }
	int getWidth() { return size.X; }
	int getHeight() { return size.Y; }
	void setPositionX(int positionX) { position.X = positionX; }
	void setPositionY(int positionY) { position.Y = positionY; }
	bool getIsVisible() { return visible; }
	bool getMarkedForDeletion() { return markedForDeletion; }
	void setIsVisible(bool newVisible) { visible = newVisible; }
	Sprite^ Next() { return next; }
	void Next(Sprite^ newNext) { next = newNext; }
	void setNewSpriteSheet(array<Bitmap^>^ newSpriteSheets) { spriteSheets = newSpriteSheets; }
	void setMarkedForDeletion(bool setDeletion) { markedForDeletion = setDeletion; }
};

