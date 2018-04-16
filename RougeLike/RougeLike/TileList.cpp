#include "TileList.h"


//==================================================================================
// Constructor
//==================================================================================
TileList::TileList(String^ newTileSheet, int startTileSize, bool Collisions)
{
	tileSize = startTileSize;

	Bitmap^ tile;
	Rectangle croppedtile;

	tileSheet = gcnew Bitmap(newTileSheet);
	//DO NOT DELETE THIS!
	//This is used to convert the image, sometimes images are not a good format and cause
	//the program to come to a crawl this fixes that issue
	croppedtile = Rectangle(0, 0, tileSheet->Width, tileSheet->Height);
	tileSheet = tileSheet->Clone(croppedtile, PixelFormat::Format32bppPArgb);
	
	//Removes Purple
	Color transparentColor = Color::FromArgb(255, 0, 255);
	tileSheet->MakeTransparent(transparentColor);

	//Creates an array holding tiles which is the size of the given bitmap
	tileSheetMap = gcnew array<Tile^, 2>(tileSheet->Width / tileSize, tileSheet->Height / tileSize);
	//Populates the array chopping up the given bitmap into small sizes of a given size.
	for (int r = 0, k = 0; r < tileSheet->Width; r += tileSize, k++)
	{
		//row
		for (int c = 0, i = 0; c < tileSheet->Height; c += tileSize, i++)
		{
			//col
			croppedtile = Rectangle(r, c, tileSize, tileSize);
			tile = tileSheet->Clone(croppedtile, tileSheet->PixelFormat);
			tileSheetMap[k, i] = gcnew Tile(tile, Collisions);
		}
	}
}//End Contructor

//==================================================================================
// Removes colour from bitmaps
//==================================================================================
void TileList::removeColour()
{
	//Removes Purple
	Color transparentColor = Color::FromArgb(255, 0, 255);

	for (int r = 0, k = 0; r < tileSheet->Width; r += tileSize, k++)
	{
		for (int c = 0, i = 0; c < tileSheet->Height; c += tileSize, i++)
		{
			tileSheetMap[k, i]->TileBitmap->MakeTransparent(transparentColor);
		}
	}
}//End removeColour

//==================================================================================
// Returns the bitmap from the given position relative to the map
//==================================================================================
Bitmap^ TileList::GetTileBitmap(Point pos)
{
	pos = Point(pos.X / tileSize, pos.Y / tileSize);
	return tileSheetMap[pos.Y, pos.X]->TileBitmap;
}//End GetTileBitmap

 //==================================================================================
 // Returns the bitmap from the given position relative to the map
 //==================================================================================
Bitmap^ TileList::GetSpriteBitmap(Point pos)
{
	return tileSheetMap[pos.X, pos.Y]->TileBitmap;
}//End GetTileBitmap

//==================================================================================
// Sets a tile in the tileArray to a new tile
//==================================================================================
void TileList::SetTileArrayEntry(int row, int col, Tile ^ tileToEnter)
{
	tileSheetMap[col, row] = tileToEnter;
}//End SetTileArrayEntry


//==================================================================================
//Returns if the current indexed tile is walkable
//==================================================================================
bool TileList::IsTileWalkable(Point pos)
{
	//Checks if the given is not null
	if ((pos.Y != -1) && (pos.X != -1))
	{
		return tileSheetMap[pos.Y / tileSize, pos.X / tileSize]->IsWalkable;
	}
	else
	{
		//If the tile is null it returns false, since the sprites can't walk on the void
		return false;
	}
}//End IsTileWalkable