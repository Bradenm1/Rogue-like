#pragma once

//Included Classes
#include "GameSettings.h"

//Namespaces
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public enum ETileType { FLOOR, WALL ,VOID01, VOID02 };

//==================================================================================
// This class is used to find out what tile is on the map
// Such as if it's a Floor tile or a Wall tile
//==================================================================================
ref class TileDecipher
{
private:
	//The tiles position on the bitmap
	Point tileSheetPosition;
	//Type of tile
	ETileType tileType;
	//If true the block is shown in the world and has been seen by the user
	bool isShown;
	int unillHidden;
public:
	//Constructor
	TileDecipher(int startTileSheetPosY, int startTileSheetPosX, ETileType StartTileType);

	//Get/Sets
	void setTileType(ETileType newTileType) { tileType = newTileType; }
	//Set the map Position
	void setTileSheetPosition(Point newTileSheetPosition) { tileSheetPosition = newTileSheetPosition; }
	//Return the selected tiles position on the map
	Point getTileSheetPosition();

	void removeTime();

	//Return the selected tile type
	ETileType getTileType() { return tileType; }
	//Get and sets for the shown
	void setShown(bool newShown) { isShown = newShown; }
	bool getShown() { return isShown; }

	void untillHidden() { unillHidden = GameSettings::TIMEOUT_FOR_TILES; }
};

