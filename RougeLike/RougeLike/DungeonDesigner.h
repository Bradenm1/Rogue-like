#pragma once

//Included Classes
#include "GameSettings.h"
#include "TileMap.h"

public enum ERooms
{
	VERY_LIGHT_BLUE_BRICK_ROOM, LIGHT_BLUE_BRICK_ROOM, BLUE_BRICK_ROOM, DARK_BLUE_BRICK_ROOM,
	VERY_LIGHT_GRAVEL_ORANGE_ROOM, LIGHT_GRAVEL_ORANGE_ROOM, GRAVEL_ORANGE_ROOM, DARK_GRAVEL_ORANGE_ROOM,
	ENUM_ROOMS_LENGTH
};

public enum ERoomTile
{
	TOP_RIGHT_CORNER_WALL, TOP_MIDDLE_WALL, TOP_LEFT_CORNER_WALL,
	MIDDLE_RIGHT_WALL, MIDDLE_LEFT_WALL,
	BOTTOM_RIGHT_CORNER_WALL, BOTTOM_LEFT_CORNER_WALL,
	FLOOR_TILE, CORRIDOR_TILE,
	ROOM_TILES_LENGTH
};

public enum EVoid
{
	RED_LAVA, BLUE_LAVA, GREEN_LAVA,
	LIGHT_BLUE_WATER, BLUE_WATER,
	ENUM_VOID_LENGTH
};

public enum EVoidTile
{
	FILLED,
	ENUM_VOID_TILE_LENGTH
};

//==================================================================================
// This class is used to generate the dungeon for the game
// This does not include the sprites
//==================================================================================
ref class DungeonDesigner
{
private:
	array<Rectangle>^ rooms;
	array<Point>^ roomTiles;
	array<Point>^ voidTiles;
	TileMap^ tilemap; //Containts the map
	ERooms roomType;
	Random^ rGen;
	int corridorSize;

public:
	DungeonDesigner(TileMap^ startTilemap, Random^ startRGen);

	void makeNewDungeon(int nRooms);
	void makeRoom(int nRoom);
	void placeRoom(Rectangle room);
	void makeCorridor(int room);
	void placeCorridor(int smallerPos, int higherPos, int staticPos, bool isYPos, int room);
	void placeWallAroundCorridor(int linePos, int staticPos, bool isYPos);
	void buildWalls();
	void clearDungeon();
	array<Point>^ getRoomTiles(ERooms room);
	array<Point>^ getVoidTiles(EVoid voidFill);
};

