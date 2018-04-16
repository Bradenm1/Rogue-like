#include "Player.h"


//==================================================================================
// Constructor
//==================================================================================
Player::Player(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger^ startLogger, int startNFrames, int startNDrections, int startSpeed, bool startIsplayer, EActor startActor)
	: Actor(startCanvas, startTilemap, startRGen, startSpriteSheets, startLogger, startNFrames, startNDrections, startIsplayer)
{
	stats = getPlayerStats();

	velocity.X = stats[EActorStats::SPEED];
	velocity.Y = stats[EActorStats::SPEED];

	canLogText = true;

	name = "Player";

	size.X = GameSettings::PLAYER_SIZE;
	size.Y = GameSettings::PLAYER_SIZE;

	nFrames = GameSettings::PLAYER_NFRAMES;
}//End Player

//==================================================================================
// Moves the Player
//==================================================================================
void Player::move()
{
	//Checks if tile the sprite is about to move on is walkable
	bool walkableCheck = UpdateTileArea();

	if ((walkableCheck == true) && (isAlive == true))
	{
		position.X += velocity.X * velocityDirections[spriteDirection].X;
		position.Y += velocity.Y * velocityDirections[spriteDirection].Y;
	}
}//End move

//=================================================================================
// Updates the state of the actor
//=================================================================================
void Player::UpdateState()
{
	//Empty
}//End UpdateState

//=================================================================================
// Performs the action of the state
//=================================================================================
void Player::PerformAction()
{
	//Empty
}//End PerformAction

//==================================================================================
// Draws sprite to canvas
//==================================================================================
void Player::draw(int forcedX, int forcedY)
{
	if (isAlive == true)
	{
		//Set the position of x and y for the rectangle to use for the animation
		//Times current frame by frame width to change the frame, e.g the frame is 32X32 so the first frame would be 32, the second itteration would be 64, 92 and so fourth
		int startX = currentFrame * size.X;
		int startY = 0;

		//Creates a rectangle at a certain size for the spritesheet to use to draw only certain parts of the image
		Rectangle pixelsToDraw = Rectangle(startX, startY, size.X, size.Y);

		//Choese the direction for the sprite to use depending one what way the sprite is walking
		Bitmap^ currentBitmap = spriteSheets[spriteDirection];

		//Draws the bitmap to the canvas but only draws a certain part of that bitmap using what was created in the pixelsToDraw above
		canvas->DrawImage(currentBitmap,
			forcedX,				// on canvas
			forcedY,				// on canvas
			pixelsToDraw,			// on source
			GraphicsUnit::Pixel);	//Measurment
	}
}//End Draw