#include "Tile.h"


//==================================================================================
// Constructor
//==================================================================================
Tile::Tile(Bitmap^ startTileBitmap, bool startIsWalkable)
{
	TileBitmap = startTileBitmap;
	IsWalkable = startIsWalkable;
}//End Constructor