#include "UIElement.h"



//==================================================================================
// Constructor
//==================================================================================
UIElement::UIElement(Graphics^ startCanvas, TileMap^ startTilemap, Random^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger^ startLogger, Point startPosition) : Sprite(startCanvas, startTilemap, startRGen, startSpriteSheets, startLogger)
{
	position = startPosition;
	nFrames = GameSettings::NUMBER_OF_FRAMES_AI;
}//End Constructor

//==================================================================================
// Draws the UI element
//==================================================================================
void UIElement::draw()
{
	//Set the position of x and y for the rectangle to use for the animation
	//Times current frame by frame width to change the frame, e.g the frame is 32X32 so the first frame would be 32, the second itteration would be 64, 92 and so fourth
	int startX = 0;
	int startY = 0;

	//Creates a rectangle at a certain size for the spritesheet to use to draw only certain parts of the image
	Rectangle pixelsToDraw = Rectangle(startX, startY, size.X, size.Y);

	//Draws the bitmap to the canvas but only draws a certain part of that bitmap using what was created in the pixelsToDraw above
	canvas->DrawImage(spriteSheets[currentFrame],
		position.X,				// on canvas
		position.Y,				// on canvas
		pixelsToDraw,			// on source
		GraphicsUnit::Pixel);	//Measurment
}//End Draw
