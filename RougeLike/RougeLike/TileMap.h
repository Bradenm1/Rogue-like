#pragma once

//Included Classes
#include "TileList.h"
#include "TileDecipher.h"

//Name Spaces
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//==================================================================================
// This class is used to hold the map for the game
//==================================================================================
ref class TileMap
{
private:
	array<TileList^>^ tilelists;
	array<TileDecipher^, 2>^ map;
	Graphics^ canvas;
public:
	property int nRows;
	property int nCols;
public:
	TileMap(array<TileList^>^ startTileLists, Graphics^ startCanvas, int newNRows, int newNCols);

	void SetMapEntry(int col, int row, Point tileIndex, ETileType tileType);
	Point GetMapEntry(int col, int row);
	bool IsMapEntryWalkable(int col, int row);
	ETileType GetTileType(int col, int row);
	Bitmap ^ getTileBitMap(Point tileIndex, ETileType tileType);
	void DrawMap();
	void SetVisable(int col, int row, bool newVisable);
	bool GetVisable(int col, int row);
	void setAllNotVisable();
	void removeTime();
	void resetTime(int col, int row);
	void changeAnimation();

	void setMap(array<TileDecipher^, 2>^ newMap) { map = newMap; }
	array<TileDecipher^, 2>^ getMap() { return map; }
};

