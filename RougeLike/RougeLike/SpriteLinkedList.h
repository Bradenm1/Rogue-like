#pragma once

#include "Actor.h"
#include "AI.h"
#include "Objects.h"

using namespace System::Diagnostics;

//==================================================================================
// This class is a linked list and can contain any class decsended from Sprite
// Items, Objects, Actors, UIElement.
//==================================================================================
ref class SpriteLinkedList
{
private:
	Sprite^ head;
	Sprite^ tail;
public:
	SpriteLinkedList(void);
	void deleteOneActor(Sprite ^ ActorToDelete);
	void deleteAllDeadActors();
	Sprite^ actorWantsToUseMagic();
	void deleteAllActors();
	void addActor(Sprite^ ActorToDelete);
	void updateAtackRange();
	void checkWithinBoundingArea();
	void checkIfMarkedForDeletion();
	void checkTouchingWall();
	Sprite^ checkIfPlayerInGatheringRange(Actor ^ player);
	void checkProjectileTouchingActor(Actor ^ player);
	void changeFrameActors();
	void changeFrameObjects();
	void updateState();
	void performAction();
	void moveActors();
	void updateActorsCollisions();
	void drawActorsCollisions(int xPos, int yPos, int width, int height);
	void updateActorsTileMap(TileMap ^ newTileMap);
	void checkIfActortuck();
	void drawFakeEyeSight(int xPos, int yPos, int width, int height);
	void checkActorOnItem(SpriteLinkedList ^ itemsList);
	void checkActorOnTrap(SpriteLinkedList ^ objectList);
	void updateActorsBoundActions(int newBoundAction);
	void renderActors(int xPos, int yPos, int width, int height);
	void canLogText(int xPos, int yPos, int width, int height);
	void drawHealthBars(int xPos, int yPos, int width, int height);
	bool checkIfOnVisableTile(Sprite ^ nodeWalker);
	void checkWithinDetection(Sprite ^ otherguy);
	void updateDetection();
	int countActors();
	Sprite ^ getActor(int index);
};

