#pragma once

#include "Actor.h"

ref class Player : public Actor
{
public:
	Player(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger ^ startLogger, int startNFrames, int startNDrections, int startSpeed, bool startIsplayer, EActor startActor);
	
	virtual void draw(int forcedX, int forcedY) override;
	virtual void move() override;
	virtual void UpdateState() override;
	virtual void PerformAction() override;
};

