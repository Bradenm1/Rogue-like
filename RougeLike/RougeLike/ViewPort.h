#pragma once

//Included Classes
#include "TileMap.h"
#include "GameSettings.h"

#define DEVIDE_BY_2 2

//Namespaces
using namespace System;
using namespace System::Drawing;

//==================================================================================
// This class is ued to display the map onto the canvas at a given position
//==================================================================================
ref class ViewPort
{
private:
	int viewportWorldX;
	int viewportWorldY;

	int viewportTilesWide;
	int viewportTilesHigh;

	TileMap^ backgroundMap;
	Graphics^ canvas;
	Point viewPortPosition;
	bool isOffset;
	int tileSize;

public:
	ViewPort(int startX, int startY, int startTilesWide, int startTilesHigh,
		TileMap^ startBackGroundMap, Graphics^ startcanvas, Point startViewPortPosition, int startTileSize, bool startIsOffset);

	void viewportMove(int xMove, int yMove);
	void viewportDraw(bool filledVoid, int playerWorldX, int playerWorldY);

	void drawAnimation();

	void moveRelativeToPlayer(int playerWorldX, int playerWorldY);

	int getViewportWoldX() { return viewportWorldX; }
	int getViewportWoldY() { return viewportWorldY; }
	int getViewportWidth() { return viewportTilesWide; }
	int getViewportHigh() { return viewportTilesHigh; }
};

