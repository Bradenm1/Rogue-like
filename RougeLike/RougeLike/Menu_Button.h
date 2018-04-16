#pragma once

#include "GameSettings.h"

#define BACKGROUND_ALPHA 170

//Namespaces
using namespace System;
using namespace System::Drawing;

//==================================================================================
// This class is used for the Menu buttons for the Main Menu
//==================================================================================
ref class Menu_Button
{
private:
	Graphics^ canvas; //Canvas Pointer
	Point position;
	String^ buttonName;
	Point textSize;
	Font^ font;
	SolidBrush^ buttonBackgroundColour;
	SolidBrush^ textColour;
	int fontSize;
	bool background;
public:
	Menu_Button(Graphics^ startCanvas, Point startPosition, String^ startButtonText, int startFontSize, bool startBackground);

	void draw();
	bool checkWithin(Point checkPosition);
};

