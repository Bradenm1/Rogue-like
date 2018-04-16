#pragma once

#include "GameSettings.h"
#include "TileList.h"
#include "StoreImages.h"

//Namespaces
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

public enum ESpriteType { ACTOR, OBJECT, ITEM, TRAPS, GUI, ENUM_LENGTH_SPRITETYPE };

//==================================================================================
//  This class is used to pre-load all the sprites for the game. 
//  Ready to be used.
//==================================================================================
ref class LoadImages
{
private:
	int index;
	array<StoreImages^>^ sprites;
	//Actors
	array<TileList^>^ undead;
	//Objects
	array<TileList^>^ doors;
	array<TileList^>^ traps;
	array<TileList^>^  effects;
	//Items
	TileList^ money;
	TileList^ potion;
	//GUI
	array<TileList^>^ gui;
	//Player spriteSheets
	Bitmap^ playerSpriteSheet;

public:
	LoadImages();

	void loadUndeadImages();
	void loadDoorImages();
	void loadTrapImages();
	void loadEffectsImages();
	void loadMoneyImages();
	void loadPotionImages();
	void loadguimages();

	array<Bitmap^>^ getImages(ESpriteType sprite, int actor);
	Bitmap ^ LoadActor(Bitmap ^ spriteSheet, Rectangle croppedArea);
	array<Bitmap^>^ LoadPlayerImage();
	array<Bitmap^>^ CreateBitMapSpriteArray(Bitmap ^ east, Bitmap ^ south, Bitmap ^ west, Bitmap ^ north, int directions);
	Rectangle CropSection(int xPos, int yPos, int Width, int Height);
};

