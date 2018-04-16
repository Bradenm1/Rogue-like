#include "ViewPort.h"


//==================================================================================
// Constructor
//==================================================================================
ViewPort::ViewPort(int startX, int startY, int startTilesWide, int startTilesHigh, TileMap ^ startBackGroundMap, Graphics ^ startcanvas, Point startViewPortPosition, int startTileSize, bool startIsOffset)
{
	viewportWorldX = startX;
	viewportWorldY = startY;
	viewportTilesHigh = startTilesHigh;
	viewportTilesWide = startTilesWide;
	canvas = startcanvas;
	backgroundMap = startBackGroundMap;
	viewPortPosition = startViewPortPosition;
	tileSize = startTileSize;
	isOffset = startIsOffset;
}//End Constructor

//==================================================================================
// Moves the viewport using a passed in value
//==================================================================================
void ViewPort::viewportMove(int xMove, int yMove)
{
	//Statment to stop the viewport from going of the edge of the map
	if ((!viewportWorldX <= 0) || (!viewportWorldY <= 0))
	{
		viewportWorldX += xMove;
		viewportWorldY += yMove;
	}
}//End viewportMove

//==================================================================================
// Draws the viewport
//==================================================================================
void ViewPort::viewportDraw(bool filledVoid, int playerWorldX, int playerWorldY)
{
	//Get the tile at the top lefthand conor of the viewport
	int directionCornerX = viewportWorldX / tileSize;
	int directionCornerY = viewportWorldY / tileSize;

	//Create the offset for the draw to use in order to place the block correctly while moving
	int tileOffsetX = viewportWorldX % tileSize;
	int tileOffsetY = viewportWorldY % tileSize;

	int drawTiles = GameSettings::NUMBER_OF_VISIBLE_TILES * tileSize;

	//Cal position of the drawviewport
	int drawviewportWorldX = (playerWorldX - viewportWorldX) - drawTiles / DEVIDE_BY_2;
	int drawviewportWorldY = (playerWorldY - viewportWorldY) - drawTiles / DEVIDE_BY_2;

	Rectangle newRec;
	Rectangle shownArea = Rectangle(drawviewportWorldX, drawviewportWorldY, drawTiles, drawTiles);
	//Create the Bitmap not within the loops to save memory instead of creating a new bitmap every itteration
	Bitmap^ temp;
	ETileType tileType;
	Point tile;
	//Loops though the current position the viewport is at in the map array in order to display the correct tiles, one is subtracted to stop clipping issues at less then 0 at x and y
	for (int i = directionCornerX - 1; i < directionCornerX + viewportTilesWide + 1; i++)
	{
		for (int o = directionCornerY - 1; o < directionCornerY + viewportTilesHigh + 2; o++)
		{
			int screenX = ((i - directionCornerX) * tileSize);
			int screenY = ((o - directionCornerY) * tileSize);

			//Returns -1 if there is no tile or some error occurs
			tile = backgroundMap->GetMapEntry(o, i);
			//Gets the type of tile of the inputted tile from above
			tileType = backgroundMap->GetTileType(o, i);

			//Stops the program from trying to draw blocks in the void which would cause a crash
			if ((tile.Y != -1) || (tile.X != -1))
			{
				//tile = backgroundMap->GetMapEntry(o, i);
				//Checks if the tile is within the viewport
				if ((screenX - tileOffsetX > shownArea.X) && (screenX - tileOffsetX < shownArea.X + shownArea.Width) &&
					(screenY - tileOffsetY > shownArea.Y) && (screenY - tileOffsetY < shownArea.Y + shownArea.Height))
				{
					backgroundMap->SetVisable(o, i, true);
					//Resets tiles to be visible, stops flicker
					backgroundMap->resetTime(o, i);
				}
			}
			//Else cover the void in a certain block type
			else
			{
				//Checks if the void should be filled with a certain block type
				if (filledVoid)
				{
					//Tile to used to fill the void
					tileType = ETileType::VOID01;
					tile = Point(0,0);
				}

			}
			//Gets the bitmap from the given tile and tiletype
			temp = backgroundMap->getTileBitMap(tile, tileType);

			//Create the position where the tile will be displayed on the canvas
			if (isOffset == true)
			{
				newRec = Rectangle((screenX - tileOffsetX) + viewPortPosition.X, (screenY - tileOffsetY) + viewPortPosition.Y, tileSize, tileSize);
			}
			else
			{
				newRec = Rectangle(screenX + viewPortPosition.X, screenY + viewPortPosition.Y, tileSize, tileSize);
			}
			//Draw the tile to the canvas
			canvas->DrawImage(temp, newRec);
		}
	}
}//End viewportDraw

//==================================================================================
// Changes animations for certain tiles
//==================================================================================
void ViewPort::drawAnimation()
{
	backgroundMap->changeAnimation();
}//End drawAnimation

//==================================================================================
// Method used to compute the view point onto the player and allow it to follow the player
//==================================================================================
void ViewPort::moveRelativeToPlayer(int playerWorldX, int playerWorldY)
{
	viewportWorldX = playerWorldX - (((viewportTilesWide) * tileSize) / 2);
	viewportWorldY = playerWorldY - (((viewportTilesHigh) * tileSize) / 2);
}//End moveRelativeToPlayer
