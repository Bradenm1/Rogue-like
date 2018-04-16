#pragma once

#include "ViewPort.h"
#include "GameSettings.h"
#include "LoadImages.h"
#include "UIElement.h"

#define ALL_HEARTS_EMPTY 0
#define ONE_HALF_HEART 10
#define ONE_HALF_ONE_FULL 20
#define ONE_HALF_TWO_FULL 40
#define ONE_HALF_THREE_FULL 60
#define BACKGROUND_ALHA 180
#define DEAD_PANEL_ALPHA 170

//Namespaces
using namespace System;
using namespace System::Drawing;

public enum EUI
{
	HEALTH_HEART_01, HEALTH_HEART_02, HEALTH_HEART_03, HEALTH_HEART_04, INVENOTRY_SLOT_01, INVENOTRY_SLOT_02, INVENOTRY_SLOT_03, INVENOTRY_SLOT_04, 
	ITEM_IN_SLOT_01, ITEM_IN_SLOT_02, ITEM_IN_SLOT_03, ITEM_IN_SLOT_04, ENUM_LENGTH_EUI
};

public enum EUIElements 
{
	//First COL
	WHITE_CLUE, ORANGE_HEART_FULL, GREEN_HEART_FULL, WHITE_HEART_FULL, BLACK_HEART_FULL, ANIMATED_HEART,
	ANIMATED_DOWN_ARROW_SMALL, SMALL_RED_BOX, UI_BLANK01, UI_BLANK02, SMALL_BLUE_BOX, UI_BLANK03, UI_BLANK04,
	SMALL_GREEN_BOX, UI_BLANK05, UI_BLANK06, SMALL_YELLOW_BOX, UI_BLANK07, UI_BLANK08,
	//Second COL
	YELLOW_CLUE, ORANGE_HEART_HALF_FULL, GREEN_HEART_HALF_FULL, WHITE_HEART_HALF_FULL, BLACK_HEART_HALF_FULL, ANIMATED_DONT_KNOW01, ANIMATED_UP_ARROW_SMALL, 
	LEFT_TOP_RED_BOX, LEFT_MIDDLE_RED_BOX, LEFT_BOTTOM_RED_BOX, LEFT_TOP_BLUE_BOX, LEFT_MIDDLE_BLUE_BOX, LEFT_BOTTOM_BLUE_BOX,
	LEFT_TOP_GREEN_BOX, LEFT_MIDDLE_GREEN_BOX, LEFT_BOTTOM_GREEN_BOX, LEFT_TOP_YELLOW_BOX, LEFT_MIDDLE_YELLOW_BOX, LEFT_BOTTOM_YELLOW_BOX,
	//Third COL
	ORNAGE_CLUE, ORANGE_HEART_NEAR_EMPTY, GREEN_HEART_NEAR_EMPTY, WHITE_HEART_NEAR_EMPTY, BLACK_HEART_NEAR_EMPTY, ANIMATED_DONT_KNOW02, UI_BLANK09,
	MIDDLE_TOP_RED_BOX, MIDDLE_MIDDLE_RED_BOX, MIDDLE_BOTTOM_RED_BOX, MIDDLE_TOP_BLUE_BOX, MIDDLE_MIDDLE_BLUE_BOX, MIDDLE_BOTTOM_BLUE_BOX,
	MIDDLE_TOP_GREEN_BOX, MIDDLE_MIDDLE_GREEN_BOX, MIDDLE_BOTTOM_GREEN_BOX, MIDDLE_TOP_YELLOW_BOX, MIDDLE_MIDDLE_YELLOW_BOX, MIDDLE_BOTTOM_YELLOW_BOX,
	//Fourth COL
	RED_CLUE, ORANGE_HEART_NEAR_NEAR_EMPTY, GREEN_HEART_NEAR_NEAR_EMPTY, WHITE_HEART_NEAR_NEAR_EMPTY, BLACK_HEART_NEAR_NEAR_EMPTY, ANIMATED_DONT_KNOW03, UI_BLANK10,
	RIGHT_TOP_RED_BOX, RIGHT_MIDDLE_RED_BOX, RIGHT_BOTTOM_RED_BOX, RIGHT_TOP_BLUE_BOX, RIGHT_MIDDLE_BLUE_BOX, RIGHT_BOTTOM_BLUE_BOX,
	RIGHT_TOP_GREEN_BOX, RIGHT_MIDDLE_GREEN_BOX, RIGHT_BOTTOM_GREEN_BOX, RIGHT_TOP_YELLOW_BOX, RIGHT_MIDDLE_YELLOW_BOX, RIGHT_BOTTOM_YELLOW_BOX,
	//
	BLACK_CLUE, ORANGE_HEART_EMPTY, GREEN_HEART_EMPTY, WHITE_HEART_EMPTY, BLACK_HEART_EMPTY
};

//==================================================================================
// This class is used for the in-game GUI of the game and controls it
//==================================================================================
ref class UI
{
private:
	Point resolution; //Game Resolution
	Graphics^ canvas;
	TileMap^ tileMap;
	LoadImages^ guiImages;
	ViewPort^ miniMap;
	array<UIElement^>^ elements;
	SolidBrush^ background;
	SolidBrush^ playerOnMiniNap;
	SolidBrush^ textColour;
	SolidBrush^ bottomPanelColour;
	array<Bitmap^>^ fullHeart;
	array<Bitmap^>^ halfEmptyHeart;
	array<Bitmap^>^ emptyHeart;
	Sprite^ player;
	Random^ rGen;
	Logger^ logger;
	Font^ font;
	Font^ deathFont;
	Point mousePosition;
	bool mouseDown;
	bool mouseDownRight;
	int rightPlacement;

public:
	UI(Graphics ^ startCanvas, Point startResolution, TileMap^ startTileMap, int startMiniMapX, int startMiniMapY, Sprite^ startPlayer, LoadImages^ startGuiImages, Random^ startRGen, Logger^ startLogger);

	void drawHud();
	void drawMiniMap();
	void drawUIElements();
	void updateUIElementsFrames();
	void displayPlayersInventory();
	void setHearts();
	void checkMouseOverItem();
	void displayLoggerText();
	void drawText(String ^ text, int xPos, int yPos);
	void drawTextOffSet(String ^ text, int xPos, int yPos);
	void setMousePosition(Point newMousePotition) { mousePosition = newMousePotition; }
	void setMouseDown(bool newMouseDown) { mouseDown = newMouseDown; }
	void setMouseDownRight(bool newMouseDown) { mouseDownRight = newMouseDown; }
};

