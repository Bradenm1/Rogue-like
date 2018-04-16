#include "GameMainMenu.h"

//==================================================================================
// Constructor
//==================================================================================
GameMainMenu::GameMainMenu(Graphics^ startCanvas, Point startResolution)
{
	canvas = startCanvas;
	resolution = startResolution;

	background = gcnew Bitmap("images/MenuBackGround.bmp");
	helpMenu = gcnew Bitmap("images/Controls.bmp");

	menuButtons = gcnew array<Menu_Button^>(EMenuButton::ENUM_MENU_BUTTONS_LENGTH);

	menuButtons[EMenuButton::GAME_NAME] = gcnew Menu_Button(canvas, Point((startResolution.X / DEVIDE_BY_2) - GameSettings::MAIN_MENU_TITLE_X_POS, GameSettings::MAIN_MENU_TITLE_Y_POS), "Roguelike", GameSettings::MAIN_MENU_TITLE_SIZE, true);
	menuButtons[EMenuButton::START_BUTTON] = gcnew Menu_Button(canvas, Point((startResolution.X / DEVIDE_BY_2) - GameSettings::MAIN_MENU_START_BUTTON_X_POS, GameSettings::MAIN_MENU_START_BUTTON_Y_POS), "Start Game", GameSettings::MAIN_MENU_BUTTON_FONT_SIZE, true);
	menuButtons[EMenuButton::HELP_BUTTON] = gcnew Menu_Button(canvas, Point((startResolution.X / DEVIDE_BY_2) - GameSettings::MAIN_MENU_HELP_BUTTON_X_POS, GameSettings::MAIN_MENU_HELP_BUTTON_Y_POS), "Help", GameSettings::MAIN_MENU_BUTTON_FONT_SIZE, true);
	menuButtons[EMenuButton::EXIT_BUTTON] = gcnew Menu_Button(canvas, Point((startResolution.X / DEVIDE_BY_2) - GameSettings::MAIN_MENU_EXIT_BUTTON_X_POS, GameSettings::MAIN_MENU_EXIT_BUTTON_Y_POS), "Exit", GameSettings::MAIN_MENU_BUTTON_FONT_SIZE, true);
	menuButtons[EMenuButton::BACK_BUTTON] = gcnew Menu_Button(canvas, Point((startResolution.X / DEVIDE_BY_2) - GameSettings::MAIN_MENU_BACK_BUTTON_X_POS, GameSettings::MAIN_MENU_BACK_BUTTON_Y_POS), "Back", GameSettings::MAIN_MENU_BUTTON_FONT_SIZE, true);

	mousePosition = Point(0, 0);
	mouseDown = false;
	startGame = false;
	inHelpMenu = false;
}//End Constructor

//==================================================================================
// Runtime for the main menu
//==================================================================================
void GameMainMenu::DrawMenu()
{
	clickedButtons();
	draw();
	mouseDown = false;
}//End DrawMenu

//==================================================================================
// Draws the main menu
//==================================================================================
void GameMainMenu::draw()
{
	if (inHelpMenu != true)
	{
		canvas->DrawImage(background, 0, 0, resolution.X, resolution.Y);
		for (int i = 0; i < EMenuButton::BACK_BUTTON; i++)
		{
			menuButtons[i]->draw();
		}
	}
	else
	{
		canvas->DrawImage(helpMenu, 0, 0, resolution.X, resolution.Y);
		menuButtons[EMenuButton::BACK_BUTTON]->draw();
	}
}//End draw

//==================================================================================
// Checks if the user has pressed a button
//==================================================================================
void GameMainMenu::clickedButtons()
{
	if (mouseDown == true)
	{
		if (inHelpMenu != true)
		{
			bool startButtonPressed = menuButtons[EMenuButton::START_BUTTON]->checkWithin(mousePosition);
			if (startButtonPressed == true)
			{
				startGame = true;
			}

			bool helpButtonPressed = menuButtons[EMenuButton::HELP_BUTTON]->checkWithin(mousePosition);
			if (helpButtonPressed == true)
			{
				inHelpMenu = true;
			}

			bool exitButtonPressed = menuButtons[EMenuButton::EXIT_BUTTON]->checkWithin(mousePosition);
			if (exitButtonPressed == true)
			{
				System::Windows::Forms::Application::Exit();
			}
		}
		else
		{
			bool backButtonPressed = menuButtons[EMenuButton::BACK_BUTTON]->checkWithin(mousePosition);
			if (backButtonPressed == true)
			{
				inHelpMenu = false;
			}
		}
	}
}//End clickedButtons
