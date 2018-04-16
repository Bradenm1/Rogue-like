#pragma once

//Included Classes
#include "Tile.h"
#include "GameSettings.h"

//Namespaces
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

//==================================================================================
// TileList contains the array of tile images used by the map
//==================================================================================
ref class TileList
{
private:
	array<Tile^, 2>^ tileSheetMap;
	Bitmap^ tileSheet;
	int tileSize;

public:
	TileList(String ^ newTileSheet, int startTileSize, bool Collisions);

	void removeColour();

	Bitmap ^ GetTileBitmap(Point pos);
	Bitmap ^ GetSpriteBitmap(Point pos);
	void SetTileArrayEntry(int row, int col, Tile ^ tileToEnter);
	bool IsTileWalkable(Point pos);

	//Sets/Gets
	int getTileSheetHeight() { return tileSheet->Height; }
	int getTileSheetWidth() { return tileSheet->Width; }
	int getTileLstSizeX() { return tileSheetMap->GetLength(0); }
	int getTileLstSizeY() { return tileSheetMap->GetLength(1); }

};