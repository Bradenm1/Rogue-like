#include "TileMap.h"


//==================================================================================
// Constructor
//==================================================================================
TileMap::TileMap(array<TileList^>^ startTileLists, Graphics ^ startCanvas, int newNRows, int newNCols)
{
	tilelists = startTileLists;
	canvas = startCanvas;
	nRows = newNRows;
	nCols = newNCols;
	//Create a map array using the given cols and rows
	map = gcnew array<TileDecipher^, 2>(newNCols, newNRows);
	//Col
	for (int i = 0; i < nCols; i++)
	{
		//Row
		for (int o = 0; o < nRows; o++)
		{
			map[i, o] = gcnew TileDecipher(0, 0, ETileType::FLOOR);
		}
	}
}//End Constructor

 //==================================================================================
 // Sets the select col and row of the map to a certain index
 //==================================================================================
void TileMap::SetMapEntry(int col, int row, Point tileIndex, ETileType tileType)
{
	map[col, row]->setTileSheetPosition(tileIndex);
	map[col, row]->setTileType(tileType);
}//End SetMapEntry

 //==================================================================================
 // Gets the tile from the passed in col and row
 //==================================================================================
Point TileMap::GetMapEntry(int col, int row)
{
	if ((col >= nCols) || (row >= nRows) ||
		(col < 0) || (row < 0))
	{
		return Point(-1, -1);
	}
	else
	{
		return map[col, row]->getTileSheetPosition();
	}
}//End GetMapEntry

 //==================================================================================
 // Returns if the tile is walkable
 //==================================================================================
bool TileMap::IsMapEntryWalkable(int col, int row)
{
	Point tile = GetMapEntry(col, row);
	ETileType tileType = GetTileType(col, row);
	return tilelists[tileType]->IsTileWalkable(tile);
}//End IsMapEntryWalkable



 //==================================================================================
 // Returns the type of tile
 //==================================================================================
ETileType TileMap::GetTileType(int col, int row)
{
	if ((col >= nCols) || (row >= nRows) ||
		(col < 0) || (row < 0))
	{
		return ETileType::WALL;
	}
	else
	{
		return map[col, row]->getTileType();
	}
}//End GetTileType

 //==================================================================================
 // Get the bitmap for the tile
 //==================================================================================
Bitmap^ TileMap::getTileBitMap(Point tileIndex, ETileType tileType)
{
	return tilelists[tileType]->GetTileBitmap(tileIndex);
}//End getTileBitMap

 //==================================================================================
 // Draws the map
 //==================================================================================
void TileMap::DrawMap()
{
	for (int i = 0; i < nRows; i++)
	{
		for (int o = 0; o < nCols; o++)
		{
			Point tile = GetMapEntry(o, i);
			ETileType tileType = GetTileType(o, i);

			Bitmap^ temp = tilelists[tileType]->GetTileBitmap(tile);
			canvas->DrawImage(temp, temp->Height * i, temp->Height * o);
		}
	}
}//End DrawMap

 //==================================================================================
 // Set tile to be visible
 //==================================================================================
void TileMap::SetVisable(int col, int row, bool newVisable)
{
	map[col, row]->setShown(newVisable);
}//End SetVisable

 //==================================================================================
 // get tile visibilitiy
 //==================================================================================
bool TileMap::GetVisable(int col, int row)
{
	if ((col >= nCols) || (row >= nRows) ||
		(col < 0) || (row < 0))
	{
		return false;
	}
	else
	{
		return map[col, row]->getShown();
	}
	//return map[col, row]->getShown();
}//End GetVisable

 //==================================================================================
 // set tile visibilitiy false
 //==================================================================================
void TileMap::setAllNotVisable()
{
	for (int i = 0; i < nRows; i++)
	{
		for (int o = 0; o < nCols; o++)
		{
			map[o, i]->setShown(false);
		}
	}
}//End GetVisable

//==================================================================================
// Removes time from each tile
//==================================================================================
void TileMap::removeTime()
{
	for (int i = 0; i < nRows; i++)
	{
		for (int o = 0; o < nCols; o++)
		{
			map[o, i]->removeTime();
		}
	}
}//End removeTime

//==================================================================================
// Resets tiles time
//==================================================================================
void TileMap::resetTime(int col, int row)
{
	map[col, row]->untillHidden();
}//End resetTime

//==================================================================================
// Changes the animation of certain tiles - HAS TO BE CALLED FROM VIEWPORT
//==================================================================================
void TileMap::changeAnimation()
{
	TileList^ temp = tilelists[ETileType::VOID01];
	tilelists[ETileType::VOID01] = tilelists[ETileType::VOID02];
	tilelists[ETileType::VOID02] = temp;
}//End changeAnimation