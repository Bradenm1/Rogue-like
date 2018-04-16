#include "TileDecipher.h"


//==================================================================================
// Constructor
//==================================================================================
TileDecipher::TileDecipher(int startTileSheetPosY, int startTileSheetPosX, ETileType StartTileType)
{
	tileSheetPosition = Point(startTileSheetPosY, startTileSheetPosX);
	tileType = StartTileType;
	unillHidden = GameSettings::TIMEOUT_FOR_TILES;
	isShown = false;

}//End Constructor

//==================================================================================
// Returns the position of the tile on the map
//==================================================================================
Point TileDecipher::getTileSheetPosition()
{
	//Checks if the tile is visible on the map
	if (isShown == true)
	{
		return tileSheetPosition;
	}
	else
	{
		//If tile is not visible return a static type
		return Point(0, 0);
	}
}//End getTileSheetPosition

//==================================================================================
// Removes time from shown tiles
//==================================================================================
void TileDecipher::removeTime()
{
	if (GameSettings::TILES_TIMEOUT == 1)
	{
		if (unillHidden <= 0)
		{
			isShown = false;
			unillHidden = GameSettings::TIMEOUT_FOR_TILES;
		}
		else
		{
			if (isShown == true)
				unillHidden -= 1;
		}
	}
}//End removeTime