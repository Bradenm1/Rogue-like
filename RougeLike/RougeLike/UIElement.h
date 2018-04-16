#pragma once

#include "Sprite.h"

//==================================================================================
// This class is used to contain the UI elements for the in-game menu
// It's a decendent of Sprite
//==================================================================================
ref class UIElement : public Sprite
{
private:
public:
	UIElement(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger ^ startLogger, Point startPosition);

	void draw();
};

