#pragma once

//==================================================================================
// This class is used to contain the setting for the game
// These settings effect the game and is used by most classes
//==================================================================================
ref class GameSettings
{
public:
	//==================================================================================
	// Display Settings
	//==================================================================================
	//Resolution
	static const int RESOLUTION_X = 1920; //Wide
	static const int RESOLUTION_Y = 1080; //High
	//Frame-rate
	static const int FRAME_RATE = 60;
	//The rate at which the frames for certain things update
	static const int FRAMEUPDATE_SPEED = 19;
	//Player Viewport Pos
	static const int PLAYER_VIEWPORT_X = 0;
	static const int PLAYER_VIEWPORT_Y = 0;

	//==================================================================================
	// GUI Settings
	//==================================================================================
	//Mini-Map Size - In tiles
	static const int MINI_MAP_SIZE_X = 64; //Wide
	static const int MINI_MAP_SIZE_Y = 64; //High
	//Mini-map Viewport Pos
	static const int MINI_MAP_VIEWPORT_X = 100;
	static const int MINI_MAP_VIEWPORT_Y = 100;
	//Mini-map position
	static const int MINI_MAP_X_POS = 300;
	static const int MINI_MAP_Y_POS = 30;
	//Mini-map offset
	static const int MINI_MAP_OFFSET = 8;
	//Right Colum for UI Elements
	static const int UI_RIGHT_COL = 315;
	//Hearts Y Placement
	static const int UI_HEARTS_Y = 394;
	//Inventory Y Placement
	static const int UI_INVENTORY_Y = 522;
	//Font size
	static const int UI_FONT_SIZE = 14;
	//Death font size
	static const int UI_DEATH_FONT_SIZE = 32;
	//Player Mini-map dot size
	static const int UI_PLAYER_DOT_SIZE = 10;
	//Player Mini-map dot position
	static const int UI_PLAYER_DOT_X_POS = 175;
	static const int UI_PLAYER_DOT_Y_POS = 155;
	//Logging text Y position itteration
	static const int UI_LOGGER_ITTERATION_Y_POS = 24;
	//Logging text Position
	static const int UI_LOGGER_X_POS = 0;
	static const int UI_LOGGER_Y_POS = 150;
	//Elements to space apart
	static const int UI_ELEMENTS_SPACE_APART = 64;
	//Money Position
	static const int UI_MONEY_X_POS = 0;
	static const int UI_MONEY_Y_POS = 150;
	//Logging text string size
	static const int LOGGING_ARRAY_SIZE = 6;
	//Death Text Postiion
	static const int UI_DEAD_X_POS = 570;
	static const int UI_DEAD_Y_POS = 400;
	//Final Score Position
	static const int UI_FINAL_SCORE_X_POS = 800;
	static const int UI_FINAL_SCORE_Y_POS = 464;
	//Dead Panel Position
	static const int UI_DEAD_PANEL_X_POS = 0;
	static const int UI_DEAD_PANEL_Y_POS = 350;
	//Dead Panel Width and Height
	static const int UI_DEAD_PANEL_WIDTH = 0;
	static const int UI_DEAD_PANEL_HEIGHT = 250;
	//Colour 255
	static const int COLOUR_255 = 255;

	//==================================================================================
	// Main Menu Settings
	//==================================================================================
	//Tile Font Size
	static const int MAIN_MENU_TITLE_SIZE = 64;
	//Button Font Sizes
	static const int MAIN_MENU_BUTTON_FONT_SIZE = 32;
	//Title Position
	static const int MAIN_MENU_TITLE_X_POS = 450;
	static const int MAIN_MENU_TITLE_Y_POS = 150;
	//Start Button Position
	static const int MAIN_MENU_START_BUTTON_X_POS = 250;
	static const int MAIN_MENU_START_BUTTON_Y_POS = 600;
	//Help Button Position
	static const int MAIN_MENU_HELP_BUTTON_X_POS = 140;
	static const int MAIN_MENU_HELP_BUTTON_Y_POS = 700;
	//Exit button Position
	static const int MAIN_MENU_EXIT_BUTTON_X_POS = 140;
	static const int MAIN_MENU_EXIT_BUTTON_Y_POS = 800;
	//Back button Position
	static const int MAIN_MENU_BACK_BUTTON_X_POS = 140;
	static const int MAIN_MENU_BACK_BUTTON_Y_POS = 1000;

	//==================================================================================
	// Map Settings
	//==================================================================================
	//Number of tileSets
	static const int NUMBER_OF_TILESETS = 4;
	//Tiles to draw in viewport size
	static const int NUMBER_OF_VISIBLE_TILES = 12;
	//Wall spawn amount
	static const int CHANCE_OF_WALL = 10;
	//Size of the tile sprites
	static const int TILE_SIZE = 64;
	static const int VIEW_PORT_TILE_SIZE = 64;
	static const int MINI_MAP_TILE_SIZE = 4;
	//Map Size
	//This can be changed to anything, the map generates from these numbers
	static const int MAP_ROW = 60;
	static const int MAP_COL = 60;
	//Number of rooms
	static const int NUMBER_OF_ROOMS = 15;
	//Room Sizes
	static const int ROOM_MAX_SIZE = 15;
	static const int ROOM_MIN_SIZE = 8;
	//Corridor Size
	static const int CORRIDOR_MAX_SIZE = 2;
	static const int CORRIDOR_MIN_SIZE = 1;
	//Timeout for tiles to be hidden
	static const int TIMEOUT_FOR_TILES = 50;
	static const int TILES_TIMEOUT = 0;

	//==================================================================================
	// Sprite Spawns
	//==================================================================================
	//The number of AIs which are created
	static const int NUMBER_OF_STARTING_SPTIRES = 20;
	//Number of moneys
	static const int NUMBER_OF_MONEY = 25;
	//Number of Potions
	static const int NUMBER_OF_POTIONS = 30;
	//Number of traps
	static const int NUMBER_OF_TRAPS = 10;

	//==================================================================================
	// Actor Settings
	//==================================================================================
	//Number of directions for the spritesheet for all sprites
	static const int NUMBER_OF_DRECTIONS = 4;
	//Number of frames in the spritesheets for all sprites
	static const int NUMBER_OF_FRAMES_PLAYER = 3;
	static const int NUMBER_OF_FRAMES_AI = 2;
	//Sprite Speeds
	static const int SPRITE_SPEED_PLAYER = 7;
	static const int SPRITE_SPEED_AI = 7;
	//The random chance at which the AIs will change directions
	static const int WANDER_PROB = 5;
	//Default Settings
	static const int SPRITE_SPEED_DEFAULT = 7;
	static const int SPRITE_HEALTH_DEFAULT = 50;
	static const int SPRITE_ARMOR_DEFAULT = 0;
	static const int SPRITE_SEEK_CHANCE_DEFAULT = 50;
	static const int SPRITE_GATHER_ITEM_CHANCE_DEFAULT = 50;
	static const int SPRITE_USE_ITEM_CHANCE_DEFAULT = 50;
	static const int SPRITE_CAN_NOCLIP_DEFAULT = 0;
	static const int SPRITE_USE_MAGIC_CHANCE_DEFAULT = 0;
	static const int SPRITE_ATTACK_CHANCE_DEFAULT = 65;
	static const int SPRITE_ORIENTATION_CHANCE_DEAULT = 2;
	//Detect Range
	static const int SPRITE_DETECT_RANGE = 170;
	//Attck Range
	static const int SPRITE_ATTACK_RANGE = 64;
	//Wanrdering Maxinum ticks
	static const int SPRITE_WANDERING_MAX = 15;
	//Idling Maxinum ticks
	static const int SPRITE_IDLE_MAX = 80;
	//Bounding area size
	static const int SPRITE_AREA_BOUND_RANGE = 300;
	//Number of Ticks till can attack again
	static const int SPRITE_ATTACK_DELAY = 15;
	//Actor stuck check radius IN TILES
	static const int SPRITE_STUCK_CHECK_RADIUS = 100;
	//nTicks to check if actor is stuck at
	static const int SPRITE_STUCK_CHECK_MAX = 300;
	//EyeSight Range
	static const int SPRITE_EYESIGHT_RANGE = 100;
	//Player Size
	static const int PLAYER_SIZE = 64;
	//Player Frames
	static const int PLAYER_NFRAMES = 2;
	//Use item at hp
	static const int AI_HEALTH_LOW = 30;
	//Idle and Wander chance
	static const int AI_IDLE_WANDER_CHANCE = 2;
	//Size of eyeSight
	static const int AI_SIZE_OF_EYESIGHT = 10;
	//Check stuck max
	static const int CHECK_STUCK_MAX = 301;
	//Offset for healthbar
	static const int HEALTH_BAR_OFFSET = 10;

	//==================================================================================
	// Item Settings
	//==================================================================================
	//Number of item lists
	static const int NUMBER_OF_ITEMSETS = 1;
	//Default values
	static const int ITEM_WORTH_DEFAULT = 5;

	//==================================================================================
	// Object Settings
	//==================================================================================
	//Number of objects lists
	static const int NUMBER_OF_OBJECTSETS = 2;
	//Magic damage amount
	static const int MAGIC_DAMAGE_AMOUNT = 40;
	//Number of traps
	static const int NUMBER_OF_TRAPS_OB = 3;

	//==================================================================================
	// Misc Settings
	//==================================================================================
	//Collision Oddities//
	static const int SPRITE_CHIKEN_XACCOUNT_OF_BOUNDS = 5;
	//These two constants take into account the given resolution to the panel and adds the approperate adjustments to the outter form
	static const int FORM_ACCOUNT_SIZEX = 30;
	static const int FORM_ACCOUNT_SIZEY = 90;
	//Player viewpoint is the oddities which are accounted for when poswitioning the player sprite into the center of the screen while accounting collisions
	//DO NOT CHANGE
	static const int PLAYERVIEWPOINT_X_OFFSET = 15;
	static const int PLAYERVIEWPOINT_Y_OFFSET = 3;

	//==================================================================================
	// Magic Settings
	//==================================================================================
	static const int MAGIC_SPEED = 15;
	static const int FIRE_BALL = 137;
};

