#include "UI.h"
#include "Actor.h"

//==================================================================================
// Constructor
//==================================================================================
UI::UI(Graphics ^ startCanvas, Point startResolution, TileMap^ startTileMap, int startMiniMapX, int startMiniMapY, Sprite^ startPlayer, LoadImages^ startGuiImages, Random^ startRGen, Logger^ startLogger)
{
	canvas = startCanvas;
	resolution = startResolution;
	tileMap = startTileMap;
	guiImages = startGuiImages;
	rGen = startRGen;
	logger = startLogger;
	player = startPlayer;
	//Sets the font for the GUI
	font = gcnew Font("SDS_8x8", GameSettings::UI_FONT_SIZE);
	deathFont = gcnew Font("SDS_8x8", GameSettings::UI_DEATH_FONT_SIZE);

	mousePosition = Point(0, 0);
	mouseDown = false;
	mouseDownRight = false;
	rightPlacement = resolution.X - GameSettings::UI_RIGHT_COL;

	elements = gcnew array<UIElement^>(EUI::ENUM_LENGTH_EUI);

	//Create the Heart UI Elements
	for (int i = 0, o = 0; i < EUI::INVENOTRY_SLOT_01; i++, o += GameSettings::UI_ELEMENTS_SPACE_APART)
	{
		elements[i] = gcnew UIElement(canvas, tileMap, rGen, guiImages->getImages(ESpriteType::GUI, (int)EUIElements::ORANGE_HEART_FULL), logger, Point(rightPlacement + o, GameSettings::UI_HEARTS_Y));
	}

	//Create the Inventory UI Elements
	for (int i = EUI::INVENOTRY_SLOT_01, o = 0; i < EUI::ITEM_IN_SLOT_01; i++, o += GameSettings::UI_ELEMENTS_SPACE_APART)
	{
		elements[i] = gcnew UIElement(canvas, tileMap, rGen, guiImages->getImages(ESpriteType::GUI, (int)EUIElements::SMALL_YELLOW_BOX), logger, Point(rightPlacement + o, GameSettings::UI_INVENTORY_Y));
	}

	//Create the Inventory slot UI Elements
	for (int i = EUI::ITEM_IN_SLOT_01, o = 0; i < EUI::ENUM_LENGTH_EUI; i++, o += GameSettings::UI_ELEMENTS_SPACE_APART)
	{
		elements[i] = gcnew UIElement(canvas, tileMap, rGen, guiImages->getImages(ESpriteType::GUI, (int)EUIElements::UI_BLANK01), logger, Point(rightPlacement + o, GameSettings::UI_INVENTORY_Y));
		elements[i]->setIsVisible(false);
	}

	fullHeart = guiImages->getImages(ESpriteType::GUI, (int)EUIElements::ORANGE_HEART_FULL);
	halfEmptyHeart = guiImages->getImages(ESpriteType::GUI, (int)EUIElements::ORANGE_HEART_HALF_FULL);
	emptyHeart = guiImages->getImages(ESpriteType::GUI, (int)EUIElements::ORANGE_HEART_EMPTY);

	background = gcnew SolidBrush(Color::FromArgb(BACKGROUND_ALHA,0,0,0));
	playerOnMiniNap = gcnew SolidBrush(Color::FromArgb(GameSettings::COLOUR_255, 0, 0));
	textColour = gcnew SolidBrush(Color::FromArgb(GameSettings::COLOUR_255, GameSettings::COLOUR_255, GameSettings::COLOUR_255));
	bottomPanelColour = gcnew SolidBrush(Color::FromArgb(DEAD_PANEL_ALPHA, 0, 0, 0));
	miniMap = gcnew ViewPort(-GameSettings::MINI_MAP_TILE_SIZE, -GameSettings::MINI_MAP_TILE_SIZE, GameSettings::MINI_MAP_SIZE_X, GameSettings::MINI_MAP_SIZE_Y, tileMap,
		canvas, Point(GameSettings::RESOLUTION_X - GameSettings::MINI_MAP_X_POS, GameSettings::MINI_MAP_Y_POS), GameSettings::MINI_MAP_TILE_SIZE, false);
}//End Constructor

//==================================================================================
// Draws the hud to the canvas
//==================================================================================
void UI::drawHud()
{
	if (((Actor^)player)->getIsAlive() == true)
	{
		//Set the hearts relative to the players Health
		setHearts();
		//Draws rectangle around the map
		canvas->FillRectangle(background, rightPlacement, 15, (GameSettings::MINI_MAP_SIZE_X + 7) * GameSettings::MINI_MAP_TILE_SIZE, (GameSettings::MINI_MAP_SIZE_Y + 8) * GameSettings::MINI_MAP_TILE_SIZE);
		//Draws UI Elements
		drawUIElements();
		//Draw the minimap
		drawMiniMap();
		//Draws player dot on the map
		canvas->FillEllipse(playerOnMiniNap, (GameSettings::RESOLUTION_X - GameSettings::MINI_MAP_X_POS + player->getPositionX() / 16), GameSettings::MINI_MAP_Y_POS + player->getPositionY() / 16, GameSettings::UI_PLAYER_DOT_SIZE, GameSettings::UI_PLAYER_DOT_SIZE);
		//Displays the text log
		displayLoggerText();
		//Displays what's in the Players inventory
		displayPlayersInventory();
		//Check if the mouse if over any inventory items
		checkMouseOverItem();
		//Displays the amount of Money the player has
		drawText("Money: " + (((Actor^)player)->getActorStats())[EActorStats::MONEY].ToString(), rightPlacement, resolution.Y - GameSettings::UI_MONEY_Y_POS);
		mouseDown = false;
		mouseDownRight = false;
	}
	else
	{
		canvas->FillRectangle(bottomPanelColour, GameSettings::UI_DEAD_PANEL_X_POS, GameSettings::UI_DEAD_PANEL_Y_POS, resolution.X, GameSettings::UI_DEAD_PANEL_HEIGHT);
		canvas->DrawString("You Have Perished", deathFont, textColour, GameSettings::UI_DEAD_X_POS, GameSettings::UI_DEAD_Y_POS);
		canvas->DrawString("Final Score: " + (((Actor^)player)->getActorStats())[EActorStats::MONEY].ToString(), font, textColour, GameSettings::UI_FINAL_SCORE_X_POS, GameSettings::UI_FINAL_SCORE_Y_POS);
		canvas->DrawString("Press 'Esc' to Exit", font, textColour, GameSettings::UI_FINAL_SCORE_X_POS - 30, GameSettings::UI_FINAL_SCORE_Y_POS + 90);
	}
}//End drawHud

//==================================================================================
// Draws the minimap at the given position in the worldmap
//==================================================================================
void UI::drawMiniMap()
{
	//Draws the mini-map
	miniMap->viewportDraw(true, player->getPositionX(), player->getPositionY());
}//End drawMiniMap

//==================================================================================
// Draws all the UI elements to the canvas
//==================================================================================
void UI::drawUIElements()
{
	for (int i = 0; i < elements->Length; i++)
	{
		if (elements[i] != nullptr)
			elements[i]->draw();
	}
}//End drawUIElements

//==================================================================================
// Updates the animation for the elements if they have any
//==================================================================================
void UI::updateUIElementsFrames()
{
	for (int i = 0; i < elements->Length; i++)
	{
		if (elements[i] != nullptr)
			elements[i]->UpdateFrame();
	}
}//End updateUIElementsFrames

//==================================================================================
// Displays the items in the Players inventory
//==================================================================================
void UI::displayPlayersInventory()
{
	//Get the players items
	array<Sprite^>^ items = ((Actor^)player)->getActorInventory();

	//Create the Inventory UI Elements
	for (int i = EUI::ITEM_IN_SLOT_01, o = 0; i < EUI::ITEM_IN_SLOT_04 + 1; i++, o++)
	{
		//Check if the Player has any items
		if (items[o] != nullptr)
		{
			//Gets the item type
			EItems item = ((Items^)items[o])->getItemType();
			//Gets the items spriteSheet
			array<Bitmap^>^ spriteSheets = guiImages->getImages(ESpriteType::ITEM, (int)item);
			//Sets the element to the spriteSheet of the item
			elements[i]->setNewSpriteSheet(spriteSheets);
			//Allow the element to be displayed
			elements[i]->setIsVisible(true);
		}
	}
}//End displayPlayersInventory

//==================================================================================
// Updates the spritesheets for the Displayed Health, with the given health
//==================================================================================
void UI::setHearts()
{
	int health = (((Actor^)player)->getActorStats())[EActorStats::HEALTH];
	if (health <= ALL_HEARTS_EMPTY)
	{
		elements[EUI::HEALTH_HEART_04]->setNewSpriteSheet(emptyHeart);
		elements[EUI::HEALTH_HEART_03]->setNewSpriteSheet(emptyHeart);
		elements[EUI::HEALTH_HEART_02]->setNewSpriteSheet(emptyHeart);
		elements[EUI::HEALTH_HEART_01]->setNewSpriteSheet(emptyHeart);
	}
	else
	{
		if (health <= ONE_HALF_HEART)
		{
			elements[EUI::HEALTH_HEART_04]->setNewSpriteSheet(emptyHeart);
			elements[EUI::HEALTH_HEART_03]->setNewSpriteSheet(emptyHeart);
			elements[EUI::HEALTH_HEART_02]->setNewSpriteSheet(emptyHeart);
			elements[EUI::HEALTH_HEART_01]->setNewSpriteSheet(halfEmptyHeart);
		}
		else
		{
			if (health <= ONE_HALF_ONE_FULL)
			{
				elements[EUI::HEALTH_HEART_04]->setNewSpriteSheet(emptyHeart);
				elements[EUI::HEALTH_HEART_03]->setNewSpriteSheet(emptyHeart);
				elements[EUI::HEALTH_HEART_02]->setNewSpriteSheet(halfEmptyHeart);
				elements[EUI::HEALTH_HEART_01]->setNewSpriteSheet(fullHeart);
			}
			else
			{
				if (health <= ONE_HALF_TWO_FULL)
				{
					elements[EUI::HEALTH_HEART_04]->setNewSpriteSheet(emptyHeart);
					elements[EUI::HEALTH_HEART_03]->setNewSpriteSheet(halfEmptyHeart);
					elements[EUI::HEALTH_HEART_02]->setNewSpriteSheet(fullHeart);
					elements[EUI::HEALTH_HEART_01]->setNewSpriteSheet(fullHeart);
				}
				else
				{
					if (health < ONE_HALF_THREE_FULL)
					{
						elements[EUI::HEALTH_HEART_03]->setNewSpriteSheet(fullHeart);
						elements[EUI::HEALTH_HEART_02]->setNewSpriteSheet(fullHeart);
						elements[EUI::HEALTH_HEART_01]->setNewSpriteSheet(fullHeart);
						elements[EUI::HEALTH_HEART_04]->setNewSpriteSheet(halfEmptyHeart);
					}
					else
					{
						elements[EUI::HEALTH_HEART_03]->setNewSpriteSheet(fullHeart);
						elements[EUI::HEALTH_HEART_02]->setNewSpriteSheet(fullHeart);
						elements[EUI::HEALTH_HEART_01]->setNewSpriteSheet(fullHeart);
						elements[EUI::HEALTH_HEART_04]->setNewSpriteSheet(fullHeart);
					}
				}
			}
		}
	}
}//End setHearts

//==================================================================================
// Checks if the Mouse if hovering over an item
//==================================================================================
void UI::checkMouseOverItem()
{
	for (int i = EUI::ITEM_IN_SLOT_01, o = 0; i < EUI::ENUM_LENGTH_EUI; i++, o++)
	{
		if (elements[i]->getIsVisible() == true)
		{
			//Get the slots position on the canvas
			int inventorySlotX = elements[i]->getPositionX();
			int inventorySlotY = elements[i]->getPositionY();
			int inventorySlotXWidth = elements[i]->getPositionX() + elements[i]->getWidth();
			int inventorySlotYHeight = elements[i]->getPositionY() + elements[i]->getHeight();

			//Check if the mouse is within it
			if ((mousePosition.X > inventorySlotX) && (mousePosition.Y > inventorySlotY) &&
				(mousePosition.X < inventorySlotXWidth) && (mousePosition.Y < inventorySlotYHeight))
			{
				//Get the item at the given index in the Players inventory
				Sprite^ item = (((Actor^)player)->getActorInventory())[o];

				//Checks is user wants to use the item
				if (mouseDown == true)
				{
					//Gets a blank spriteSheet
					array<Bitmap^>^ spriteSheets = guiImages->getImages(ESpriteType::GUI, (int)EUIElements::UI_BLANK01);

					//Set the item to be used by the Player
					((Actor^)player)->useItem((Items^)item);
					//Remove the Iiem from the players inventory
					((Actor^)player)->markedItemForDeletion(o);
					((Actor^)player)->removeItem(o);

					//Set the slot to the given spriteSheet which is blank
					elements[i]->setNewSpriteSheet(spriteSheets);
					//Set its visibilty to false
					elements[i]->setIsVisible(false);
				}
				//If the mouse was not clicked
				else
				{
					if (mouseDownRight == true)
					{
						//Gets a blank spriteSheet
						array<Bitmap^>^ spriteSheets = guiImages->getImages(ESpriteType::GUI, (int)EUIElements::UI_BLANK01);

						((Actor^)player)->removeItem(o);
						((Items^)item)->actorDropItem(Point(player->getPositionX(), player->getPositionY()));

						//Set the slot to the given spriteSheet which is blank
						elements[i]->setNewSpriteSheet(spriteSheets);
						//Set its visibilty to false
						elements[i]->setIsVisible(false);

						logger->addText("Player dropped an Item");
					}
					//Draws the name of the item to the screen
					drawTextOffSet(((Items^)item)->getItemName(), mousePosition.X, mousePosition.Y);
				}
			}
		}
	}
}//End checkMouseOverItem

//==================================================================================
// Checks if the Mouse if hovering over an item
//==================================================================================
void UI::displayLoggerText()
{
	for (int i = 0, o = 0; i < logger->getArraySize(); i++, o += GameSettings::UI_LOGGER_ITTERATION_Y_POS)
	{
		drawText(logger->getString(i), GameSettings::UI_LOGGER_X_POS, resolution.Y - GameSettings::UI_LOGGER_Y_POS + o);
	}
}//End displayLoggerText

//==================================================================================
// Displays the given text at the given position to the canvas
//==================================================================================
void UI::drawText(String^ text, int xPos, int yPos)
{
	int textWidth = canvas->MeasureString(text, font).Width;
	int textHeight = canvas->MeasureString(text, font).Height;

	if (xPos + textWidth > resolution.X)
	{
		xPos = resolution.X - textWidth;
	}

	canvas->FillRectangle(bottomPanelColour, xPos, yPos, textWidth, textHeight);
	canvas->DrawString(text, font, textColour, Point(xPos, yPos));
}//End drawText

//==================================================================================
// Displays the given text at the given position to the canvas
//==================================================================================
void UI::drawTextOffSet(String^ text, int xPos, int yPos)
{
	int textWidth = canvas->MeasureString(text, font).Width;
	int textHeight = canvas->MeasureString(text, font).Height;

	canvas->FillRectangle(bottomPanelColour, xPos - textWidth, yPos, textWidth, textHeight);
	canvas->DrawString(text, font, textColour, Point(xPos - textWidth, yPos));
}//End drawText