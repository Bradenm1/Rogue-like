#pragma once

#include "Actor.h"

#define CHANGE_TO_DOUBLE_FROM_INT 100

//==================================================================================
// This class contains everything an AI needs in-order to run, 
// it's a child of Actor and it can also be used as playable class
//==================================================================================
ref class AI : public Actor
{
private:
	Pen^ emptyHealth;
	Pen^ fullHealth;
	Pen^ behindHealth;
	int nTicks;

public:
	AI(Graphics ^ startCanvas, TileMap ^ startTilemap, Random ^ startRGen, array<Bitmap^>^ startSpriteSheets, Logger ^ startLogger, int startNFrames, int startNDrections, int startSpeed, bool startIsplayer, EActor startActor);

	virtual void draw(int forcedX, int forcedY) override;
	virtual void move() override;
	void drawHealth(int forcedX, int forcedY);
	void eyeSight(int otherFakex, int otherFakeY, int fakeX, int fakeY, int xToAdd, int yToAdd);
	virtual void UpdateState() override;
	virtual void PerformAction() override;
};

