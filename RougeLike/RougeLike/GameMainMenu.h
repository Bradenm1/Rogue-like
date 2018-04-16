#pragma once

#include "GameSettings.h"
#include "Menu_Button.h"

#define DEVIDE_BY_2 2

//Namespaces
using namespace System;
using namespace System::Drawing;

public enum EMenuButton { GAME_NAME, START_BUTTON, HELP_BUTTON, EXIT_BUTTON, BACK_BUTTON, ENUM_MENU_BUTTONS_LENGTH};

//==================================================================================
// This class is used to control the Main Menu of the game
// It's where the user can select to play, exit or view help for controls
//==================================================================================
ref class GameMainMenu
{
private:
	Graphics^ canvas; //Canvas Pointer
	array<Menu_Button^>^ menuButtons;
	Bitmap^ background;
	Bitmap^ helpMenu;
	Point resolution;
	Point mousePosition;
	bool inHelpMenu;
	bool mouseDown;
	bool startGame;
public:
	GameMainMenu(Graphics^ startCanvas, Point startResolution);

	void DrawMenu();
	void draw();
	void clickedButtons();

	void setMousePosition(Point newPosition) { mousePosition = newPosition; }
	void setMouseDown(bool newMouseDown) { mouseDown = newMouseDown; }
	bool getGameStart() { return startGame; }
};

