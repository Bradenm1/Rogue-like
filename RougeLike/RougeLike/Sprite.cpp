#include "Sprite.h"


//==================================================================================
// Constructor
//==================================================================================
Sprite::Sprite(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger^ startLogger)
{
	canvas = startCanvas;
	tilemap = startTilemap;
	rGen = startRGen;
	spriteSheets = startSpriteSheets;
	logger = startLogger;

	size.X = spriteSheets[0]->Width;
	size.Y = spriteSheets[0]->Height;

	nFrames = spriteSheets[0]->Width / size.Y;

	//Set visible by default for all sprites as true
	visible = true;

	markedForDeletion = false;

}//End Contructor

//==================================================================================
// Draws sprite to canvas
//==================================================================================
void Sprite::draw(int forcedX, int forcedY)
{
	//Child classes add their own code
}//End Draw

//==================================================================================
// Moves sprite on canvas
//==================================================================================
void Sprite::move()
{
	//Child classes add their own code
}//End move

//=================================================================================
// Points the Actor at the given Sprite
//=================================================================================
void Sprite::pointMeAt(Sprite^ otherActor, int speed)
{
	int xDelta = otherActor->getPositionX() - position.X;
	int yDelta = otherActor->getPositionY() - position.Y;

	int angleToTarget = Math::Atan2(yDelta, xDelta);

	bool viewBlock = false;

	velocity.X = Math::Cos(angleToTarget) * speed;
	velocity.Y = Math::Sin(angleToTarget) * speed;

}//End pointMeAt

//==================================================================================
// Checks if tile under the sprite is visible or not
//==================================================================================
bool Sprite::checkIfOnVisableTile()
{
	//Get coordinates or tile under sprite
	int tileX = position.X / GameSettings::TILE_SIZE;
	int tileY = position.Y / GameSettings::TILE_SIZE;

	//Gets if tile is visible
	bool isTileVisible = false;

	//Takes into account the offset fixes a bug with them drawing in non-visable tiles
	if (tilemap->GetVisable(tileY, tileX) && tilemap->GetVisable(tileY + 1, tileX + 1))
	{
		isTileVisible = true;
	}

	return isTileVisible;
}//End checkIfOnVisableTile

 //==================================================================================
 //Checks if the sprite is spawning on a unwalkable tile and this method has to fix it
 //==================================================================================
void Sprite::NotSpawnOnWalkable()
{
	int temp01;
	int temp02;
	bool walkableCheck = true;

	int mapSizeY = tilemap->nCols;
	int mapSizeX = tilemap->nRows;

	//Creates a random position at which the sprite is used to be placed randomly
	//Note some will be offset by a block, which means some may still spawn and get stuck in walls
	do
	{
		//Generates the next position which will be tested for the placement of the actor
		temp01 = rGen->Next(mapSizeX);
		temp02 = rGen->Next(mapSizeY);

		//Convert the above position to number of tiles from the top and left of the canvas
		int directionCornerX = temp01;
		int directionCornerY = temp02;

		//Checks it the block the actor has been placed on is walkable
		walkableCheck = tilemap->IsMapEntryWalkable(directionCornerY, directionCornerX);

	} while (walkableCheck == false);

	//Places the actor onto the middle of the block, this stops actors from spawning on blocks which are not walkable
	position.X = temp01 * GameSettings::TILE_SIZE;
	position.Y = temp02 * GameSettings::TILE_SIZE;
}//End NotSpawnOnWalkable

 //==================================================================================
 //Checks if the sprite is spawning on a unwalkable tile and this method has to fix it
 //==================================================================================
bool Sprite::checkOnWalkable()
{
	bool walkableCheck;

	int xPos = (position.X + (size.X / DEVIDE_BY_2)) / GameSettings::TILE_SIZE;
	int yPos = (position.Y + (size.Y / DEVIDE_BY_2)) / GameSettings::TILE_SIZE;

	walkableCheck = tilemap->IsMapEntryWalkable(yPos, xPos);

	return walkableCheck;
}//End NotSpawnOnWalkable

//==================================================================================
// Updates the current frame on the Actor to the next frame
//==================================================================================
void Sprite::UpdateFrame()
{
	//This line of code adds 1 to the frame each time the method is called.
	//Once the currentframe equals nFrames which would be '0' sets it to '0'
	currentFrame = (currentFrame + 1) % nFrames;
}//End UpdateFrame