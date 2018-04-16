#include "DungeonDesigner.h"


//==================================================================================
// Constructor
//==================================================================================
DungeonDesigner::DungeonDesigner(TileMap^ startTilemap, Random^ startRGen)
{
	tilemap = startTilemap;
	rGen = startRGen;
}//End constructor

//==================================================================================
// Creates a new dungeon
//==================================================================================
void DungeonDesigner::makeNewDungeon(int nRooms)
{
	rooms = gcnew array<Rectangle>(nRooms);

	ERooms roomSelection = (ERooms)rGen->Next(ERooms::ENUM_ROOMS_LENGTH);
	roomTiles = getRoomTiles(roomSelection);

	EVoid voidSelection = (EVoid)rGen->Next(EVoid::ENUM_VOID_LENGTH);
	voidTiles = getVoidTiles(voidSelection);
	//Sets corridor size
	corridorSize = rGen->Next(GameSettings::CORRIDOR_MIN_SIZE, GameSettings::CORRIDOR_MAX_SIZE);

	roomType = roomSelection;

	clearDungeon();

	// Create the rooms
	for (int i = 0; i < nRooms; i++)
	{
		makeRoom(i);
	}

	// Create the line corridors
	for (int r = 0; r < nRooms - 1; r++)
	{
		makeCorridor(r);
	}

	//buildWalls();
}//End makeNewDungeon


//==================================================================================
// Creates a new room
//==================================================================================
void DungeonDesigner::makeRoom(int nRoom)
{
	ETileType emptySlot = ETileType::WALL;
	Rectangle room;

	bool isFloor = false;
	bool isEmpty = false;

	do
	{
		//Get a size for the room
		do
		{
			room.Width = rGen->Next(GameSettings::ROOM_MIN_SIZE, GameSettings::ROOM_MAX_SIZE);
			room.Height = rGen->Next(GameSettings::ROOM_MIN_SIZE, GameSettings::ROOM_MAX_SIZE);
			room.X = rGen->Next(tilemap->nRows - 1);
			room.Y = rGen->Next(tilemap->nCols - 1);
			isFloor = false;

		} while ((room.X + room.Width >= tilemap->nRows) || (room.Y + room.Height >= tilemap->nCols));

		//Check if it's on another room
		for (int i = room.X; i < room.Width + room.X; i++)
		{
			for (int o = room.Y; o < room.Height + room.Y; o++)
			{
				emptySlot = tilemap->GetTileType(i, o);
				if (emptySlot == ETileType::FLOOR)
				{
					isFloor = true;
				}
			}
		}

		if (isFloor == false)
		{
			isEmpty = true;
		}
	} while (isEmpty == false);

	//Place the room
	placeRoom(room);
	//Put room in array
	rooms[nRoom] = room;
}//End makeRoom


 //==================================================================================
 // Places the room
 //==================================================================================
void DungeonDesigner::placeRoom(Rectangle room)
{
	//Place the room
	for (int i = room.X + 1; i < room.Width + room.X - 1; i++)
	{
		for (int o = room.Y + 1; o < room.Height + room.Y - 1; o++)
		{
			int heightoffset = (room.Y + room.Height) - 2;
			int widthofset = (room.X + room.Width) - 2;
			//Checks if the loops are at the edge of the map, and places a selected tile
			if ((o == heightoffset) || (i == widthofset) || (o == room.Y + 1) || (i == room.X + 1))
			{
				if (i == room.X + 1)
				{
					//Top lefthand corner of the room
					if (o == room.Y + 1)
					{
						tilemap->SetMapEntry(i, o, roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL], ETileType::WALL);
					}
					else
					{
						//Top righthand corner of the room
						if (o == room.Height + room.Y - 2)
						{
							tilemap->SetMapEntry(i, o, roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL], ETileType::WALL);
						}
						//Lefthand wall
						else
						{
							tilemap->SetMapEntry(i, o, roomTiles[ERoomTile::TOP_MIDDLE_WALL], ETileType::WALL);
						}
					}
				}
				else
				{
					if (i == room.Width + room.X - 2)
					{
						//bottom lefthand corner of the room
						if (o == room.Y + 1)
						{
							tilemap->SetMapEntry(i, o, roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL], ETileType::WALL);
						}
						else
						{
							//bottom righthand corner of the room
							if (o == room.Height + room.Y - 2)
							{
								tilemap->SetMapEntry(i, o, roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL], ETileType::WALL);
							}
							//Righthand wall
							else
							{
								tilemap->SetMapEntry(i, o, roomTiles[ERoomTile::TOP_MIDDLE_WALL], ETileType::WALL);
							}
						}
					}
					else
					{
						//Right hand wall and left hand wall
						tilemap->SetMapEntry(i, o, roomTiles[ERoomTile::MIDDLE_RIGHT_WALL], ETileType::WALL);
					}
				}
			}
			else // Set floor tile
			{
				tilemap->SetMapEntry(i, o, roomTiles[ERoomTile::FLOOR_TILE], ETileType::FLOOR);
			}
		}
	}
}//End placeRoom

//==================================================================================
// Creates a new corridor
//==================================================================================
void DungeonDesigner::makeCorridor(int room)
{
	int middleOfRoomX = (rooms[room].X + (rooms[room].Width / 2));
	int middleOfRoomY = (rooms[room].Y + (rooms[room].Height / 2));
	int nextMiddleOfRoomX = (rooms[room + 1].X + (rooms[room + 1].Width / 2));
	int nextMiddleOfRoomY = (rooms[room + 1].Y + (rooms[room + 1].Height / 2));

	if (middleOfRoomX < nextMiddleOfRoomX)
	{
		placeCorridor(middleOfRoomX, nextMiddleOfRoomX, middleOfRoomY, false, room);
	}
	else
	{
		placeCorridor(nextMiddleOfRoomX, middleOfRoomX, middleOfRoomY, false, room);
	}

	if (middleOfRoomY < nextMiddleOfRoomY)
	{
		placeCorridor(middleOfRoomY, nextMiddleOfRoomY, nextMiddleOfRoomX, true, room);
	}
	else
	{
		placeCorridor(nextMiddleOfRoomY, middleOfRoomY, nextMiddleOfRoomX, true, room);
	}
}//End makeCorridor

//==================================================================================
// Places a Corridor
//==================================================================================
void DungeonDesigner::placeCorridor(int smallerPos, int higherPos, int staticPos, bool isYPos, int room)
{
	Point tile = (getRoomTiles(roomType))[ERoomTile::CORRIDOR_TILE];

	for (int i = smallerPos; i < higherPos + 1; i++)
	{
		for (int size = 0; size < corridorSize; size++)
		{
			//Checks if tiles are being places on Y or X axis as line
			if (isYPos)
			{
				ETileType tileType = tilemap->GetTileType(staticPos + size,i);
				if (tileType != ETileType::FLOOR)
				{
					tilemap->SetMapEntry(staticPos + size, i, tile, ETileType::FLOOR);
					tilemap->SetMapEntry(staticPos - size, i, tile, ETileType::FLOOR);
					//placeWallAroundCorridor(i, staticPos + size + 1, isYPos);
					//placeWallAroundCorridor(i, staticPos - size - 1, isYPos);
				}
			}
			else
			{
				ETileType tileType = tilemap->GetTileType(i, staticPos + size);
				if (tileType != ETileType::FLOOR)
				{
					tilemap->SetMapEntry(i, staticPos + size, tile, ETileType::FLOOR);
					tilemap->SetMapEntry(i, staticPos - size, tile, ETileType::FLOOR);
					//placeWallAroundCorridor(i, staticPos + size + 1, isYPos);
					//placeWallAroundCorridor(i, staticPos - size - 1, isYPos);
				}
			}
		}
	}
}//End placeRoom

 //==================================================================================
 // Places a Wall in Corridor
 //==================================================================================
void DungeonDesigner::placeWallAroundCorridor(int linePos, int staticPos, bool isYPos)
{
	ETileType tileType;
	if (isYPos)
	{
		tileType = tilemap->GetTileType(staticPos, linePos);
		if (tileType == ETileType::VOID01)
		{
			//tilemap->SetMapEntry(staticPos, linePos, wallTile, ETileType::WALL);
		}
	}
	else
	{
		tileType = tilemap->GetTileType(linePos, staticPos);
		if (tileType == ETileType::VOID01)
		{
			//tilemap->SetMapEntry(linePos, staticPos, wallTile, ETileType::WALL);
		}
	}
}//End placeWallAroundCorridor

//==================================================================================
// Places Walls around all Floor tiles in touching distance of a VOID type
//==================================================================================
void DungeonDesigner::buildWalls()
{
	ETileType tileType;
	ETileType checkIfVoid;
	//Col
	for (int i = 0; i < tilemap->nCols; i++)
	{
		//Row
		for (int o = 0; o < tilemap->nRows; o++)
		{
			tileType = tilemap->GetTileType(i, o);
			if (tileType == ETileType::FLOOR)
			{
				//Place void
				//Col
				//for (int j = i - 1; j < i + 2; j++)
				//{
				//	//Row
				//	for (int k = o - 1; k < o + 2; k++)
				//	{
				//		checkIfVoid = tilemap->GetTileType(j, k);
				//		if (checkIfVoid == ETileType::VOID01)
				//		{
				//			tilemap->SetMapEntry(j, k, wallTile01, ETileType::WALL);
				//		}
				///*	}
				//}*/
			}
		}
	}
}//End clearDungeon

//==================================================================================
// Clears the dungeon
//==================================================================================
void DungeonDesigner::clearDungeon()
{
	//Col
	for (int i = 0; i < tilemap->nCols; i++)
	{
		//Row
		for (int o = 0; o < tilemap->nRows; o++)
		{
			tilemap->SetMapEntry(i, o, voidTiles[EVoidTile::FILLED], ETileType::VOID01);
		}
	}
}//End clearDungeon

//==================================================================================
// 
//==================================================================================
array<Point>^ DungeonDesigner::getRoomTiles(ERooms room)
{
	array<Point>^ roomTiles = gcnew array<Point>(ERoomTile::ROOM_TILES_LENGTH);

	switch (room)
	{
	case VERY_LIGHT_BLUE_BRICK_ROOM:
		roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL] = Point(3, 2);
		roomTiles[ERoomTile::TOP_MIDDLE_WALL] = Point(3, 1);
		roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL] = Point(3, 0);
		roomTiles[ERoomTile::MIDDLE_RIGHT_WALL] = Point(4, 0);
		roomTiles[ERoomTile::MIDDLE_LEFT_WALL] = Point(4, 0);
		roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL] = Point(5, 2);
		roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL] = Point(5, 0);
		roomTiles[ERoomTile::FLOOR_TILE] = Point(4,1);
		roomTiles[ERoomTile::CORRIDOR_TILE] = Point(3, 3);
		break;
	case LIGHT_BLUE_BRICK_ROOM:
		roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL] = Point(6, 2);
		roomTiles[ERoomTile::TOP_MIDDLE_WALL] = Point(6, 1);
		roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL] = Point(6, 0);
		roomTiles[ERoomTile::MIDDLE_RIGHT_WALL] = Point(7, 0);
		roomTiles[ERoomTile::MIDDLE_LEFT_WALL] = Point(7, 0);
		roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL] = Point(8, 2);
		roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL] = Point(8, 0);
		roomTiles[ERoomTile::FLOOR_TILE] = Point(7, 1);
		roomTiles[ERoomTile::CORRIDOR_TILE] = Point(6, 3);
		break;
	case BLUE_BRICK_ROOM:
		roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL] = Point(9, 2);
		roomTiles[ERoomTile::TOP_MIDDLE_WALL] = Point(9, 1);
		roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL] = Point(9, 0);
		roomTiles[ERoomTile::MIDDLE_RIGHT_WALL] = Point(10, 0);
		roomTiles[ERoomTile::MIDDLE_LEFT_WALL] = Point(10, 0);
		roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL] = Point(11, 2);
		roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL] = Point(11, 0);
		roomTiles[ERoomTile::FLOOR_TILE] = Point(10, 1);
		roomTiles[ERoomTile::CORRIDOR_TILE] = Point(9, 3);
		break;
	case DARK_BLUE_BRICK_ROOM:
		roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL] = Point(12, 2);
		roomTiles[ERoomTile::TOP_MIDDLE_WALL] = Point(12, 1);
		roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL] = Point(12, 0);
		roomTiles[ERoomTile::MIDDLE_RIGHT_WALL] = Point(13, 0);
		roomTiles[ERoomTile::MIDDLE_LEFT_WALL] = Point(13, 0);
		roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL] = Point(14, 2);
		roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL] = Point(14, 0);
		roomTiles[ERoomTile::FLOOR_TILE] = Point(13, 1);
		roomTiles[ERoomTile::CORRIDOR_TILE] = Point(12, 3);
		break;
	case VERY_LIGHT_GRAVEL_ORANGE_ROOM:
		roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL] = Point(15, 2);
		roomTiles[ERoomTile::TOP_MIDDLE_WALL] = Point(15, 1);
		roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL] = Point(15, 0);
		roomTiles[ERoomTile::MIDDLE_RIGHT_WALL] = Point(16, 0);
		roomTiles[ERoomTile::MIDDLE_LEFT_WALL] = Point(16, 0);
		roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL] = Point(17, 2);
		roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL] = Point(17, 0);
		roomTiles[ERoomTile::FLOOR_TILE] = Point(16, 1);
		roomTiles[ERoomTile::CORRIDOR_TILE] = Point(15, 3);
		break;
	case LIGHT_GRAVEL_ORANGE_ROOM:
		roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL] = Point(18, 2);
		roomTiles[ERoomTile::TOP_MIDDLE_WALL] = Point(18, 1);
		roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL] = Point(18, 0);
		roomTiles[ERoomTile::MIDDLE_RIGHT_WALL] = Point(19, 0);
		roomTiles[ERoomTile::MIDDLE_LEFT_WALL] = Point(19, 0);
		roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL] = Point(20, 2);
		roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL] = Point(20, 0);
		roomTiles[ERoomTile::FLOOR_TILE] = Point(19, 1);
		roomTiles[ERoomTile::CORRIDOR_TILE] = Point(18, 3);
		break;
	case GRAVEL_ORANGE_ROOM:
		roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL] = Point(21, 2);
		roomTiles[ERoomTile::TOP_MIDDLE_WALL] = Point(21, 1);
		roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL] = Point(21, 0);
		roomTiles[ERoomTile::MIDDLE_RIGHT_WALL] = Point(22, 0);
		roomTiles[ERoomTile::MIDDLE_LEFT_WALL] = Point(22, 0);
		roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL] = Point(23, 2);
		roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL] = Point(23, 0);
		roomTiles[ERoomTile::FLOOR_TILE] = Point(22, 1);
		roomTiles[ERoomTile::CORRIDOR_TILE] = Point(21, 3);
		break;
	case DARK_GRAVEL_ORANGE_ROOM:
		roomTiles[ERoomTile::TOP_RIGHT_CORNER_WALL] = Point(24, 2);
		roomTiles[ERoomTile::TOP_MIDDLE_WALL] = Point(24, 1);
		roomTiles[ERoomTile::TOP_LEFT_CORNER_WALL] = Point(24, 0);
		roomTiles[ERoomTile::MIDDLE_RIGHT_WALL] = Point(25, 0);
		roomTiles[ERoomTile::MIDDLE_LEFT_WALL] = Point(25, 0);
		roomTiles[ERoomTile::BOTTOM_RIGHT_CORNER_WALL] = Point(26, 2);
		roomTiles[ERoomTile::BOTTOM_LEFT_CORNER_WALL] = Point(26, 0);
		roomTiles[ERoomTile::FLOOR_TILE] = Point(25, 1);
		roomTiles[ERoomTile::CORRIDOR_TILE] = Point(24, 3);
		break;
	default:
		break;
	}

	for (int i = 0; i < roomTiles->Length; i++)
	{
		Point tile = roomTiles[i];

		tile.X += tile.X * GameSettings::TILE_SIZE;
		tile.Y += tile.Y * GameSettings::TILE_SIZE;

		roomTiles[i] = tile;
	}

	return roomTiles;
}//End getRoomTiles

 //==================================================================================
 // 
 //==================================================================================
array<Point>^ DungeonDesigner::getVoidTiles(EVoid voidFill)
{
	array<Point>^ voidTiles = gcnew array<Point>(EVoidTile::ENUM_VOID_TILE_LENGTH);

	switch (voidFill)
	{
	case RED_LAVA:
		voidTiles[EVoidTile::FILLED] = Point(27, 1);
		break;
	case BLUE_LAVA:
		voidTiles[EVoidTile::FILLED] = Point(29, 1);
		break;
	case GREEN_LAVA:
		voidTiles[EVoidTile::FILLED] = Point(31, 1);
		break;
	case LIGHT_BLUE_WATER:
		voidTiles[EVoidTile::FILLED] = Point(9, 1);
		break;
	case BLUE_WATER:
		voidTiles[EVoidTile::FILLED] = Point(15, 1);
		break;
	default:
		break;
	}

	for (int i = 0; i < voidTiles->Length; i++)
	{
		Point tile = voidTiles[i];

		tile.X += tile.X * GameSettings::TILE_SIZE;
		tile.Y += tile.Y * GameSettings::TILE_SIZE;

		voidTiles[i] = tile;
	}

	return voidTiles;
}//End getVoidTiles