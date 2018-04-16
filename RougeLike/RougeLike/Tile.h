#pragma once

#include "GameSettings.h"

//Namespaces
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//==================================================================================
// This class is used for the tiles in the game and holds the images of them
//==================================================================================
ref class Tile
{
private:

public:
	property Bitmap^ TileBitmap;
	property bool IsWalkable;
public:
	Tile(Bitmap^ startTileBitmap, bool startIsWalkable);

};

