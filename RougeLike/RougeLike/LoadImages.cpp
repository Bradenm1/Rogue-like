#include "LoadImages.h"


//=================================================================================
// Constructor
//=================================================================================
LoadImages::LoadImages()
{
	//All these images could be loaded as intances of this class instead of code duplication
	sprites = gcnew array<StoreImages^>(ESpriteType::ENUM_LENGTH_SPRITETYPE);
	//Actors
	undead = gcnew array<TileList^>(GameSettings::NUMBER_OF_FRAMES_AI);
	//Objects
	doors = gcnew array<TileList^>(GameSettings::NUMBER_OF_FRAMES_AI);
	traps = gcnew array<TileList^>(GameSettings::NUMBER_OF_FRAMES_AI);
	effects = gcnew array<TileList^>(GameSettings::NUMBER_OF_FRAMES_AI);
	//Items
	money = gcnew TileList("images/Items/Money.bmp", GameSettings::TILE_SIZE, true);
	potion = gcnew TileList("images/Items/Potion.bmp", GameSettings::TILE_SIZE, true);
	//GUI
	gui = gcnew array<TileList^>(GameSettings::NUMBER_OF_FRAMES_AI);

	//Store the actor images into the game
	//Two sets are needed for the current animations
	for (int i = 0; i < GameSettings::NUMBER_OF_FRAMES_AI; i++)
	{
		undead[i] = gcnew TileList("images/Characters/Undead" + i.ToString() + ".bmp", GameSettings::TILE_SIZE, true);
		doors[i] = gcnew TileList("images/Objects/Door" + i.ToString() + ".bmp", GameSettings::TILE_SIZE, true);
		traps[i] = gcnew TileList("images/Objects/Trap" + i.ToString() + ".bmp", GameSettings::TILE_SIZE, true);
		effects[i] = gcnew TileList("images/Objects/Effect" + i.ToString() + ".bmp", GameSettings::TILE_SIZE, true);
		gui[i] = gcnew TileList("images/GUI/GUI" + i.ToString() + ".bmp", GameSettings::TILE_SIZE, true);
	}

	//Calulate the size of the arrays for the images given the height and width of the spriteSheets
	int actorSize = (undead[0]->getTileLstSizeY() * undead[0]->getTileLstSizeX());

	int objectSize = (doors[0]->getTileLstSizeX() * doors[0]->getTileLstSizeY() +
		traps[0]->getTileLstSizeX() * traps[0]->getTileLstSizeY() +
		effects[0]->getTileLstSizeX() * effects[0]->getTileLstSizeY());

	int itemsSize = (money->getTileLstSizeX() * money->getTileLstSizeY() +
		potion->getTileLstSizeX() * potion->getTileLstSizeY());

	int guiSize = (gui[0]->getTileLstSizeX() * gui[0]->getTileLstSizeY());

	sprites[ESpriteType::ACTOR] = gcnew StoreImages(actorSize);
	sprites[ESpriteType::OBJECT] = gcnew StoreImages(objectSize);
	sprites[ESpriteType::ITEM] = gcnew StoreImages(itemsSize);
	sprites[ESpriteType::GUI] = gcnew StoreImages(guiSize);

	//Creates the a bitmap used for the player sprite
	playerSpriteSheet = gcnew Bitmap("images/Commissions/Warrior.bmp", true);
	//ConvertImage(playerSpriteSheet);

	//Load the images
	//Actors
	index = 0;
	loadUndeadImages();
	//loadHumanoidImages();
	//loadPestImages();
	//Objects
	index = 0;
	loadDoorImages();
	loadTrapImages();
	loadEffectsImages();
	//Items
	index = 0;
	loadMoneyImages();
	loadPotionImages();
	//GUI
	index = 0;
	loadguimages();
}//End LoadImages

//=================================================================================
// Section for loading undead Actors
//=================================================================================
void LoadImages::loadUndeadImages()
{
		for (int i = 0; i  < undead[0]->getTileLstSizeX(); i++)
		{
			for (int o = 0; o < undead[0]->getTileLstSizeY(); o++)
			{
				for (int f = 0; f < GameSettings::NUMBER_OF_FRAMES_AI; f++)
				{
					Bitmap^ spriteImage = undead[f]->GetSpriteBitmap(Point(i, o));
					sprites[ESpriteType::ACTOR]->addImage(spriteImage, index, f);
				}
				index++;
			}
		}
}//End loadUndeadImages

//=================================================================================
// Section for loading Door images
//=================================================================================
void LoadImages::loadDoorImages()
{
		for (int i = 0; i < doors[0]->getTileLstSizeX(); i++)
		{
			for (int o = 0; o < doors[0]->getTileLstSizeY(); o++)
			{
				for (int f = 0; f < GameSettings::NUMBER_OF_FRAMES_AI; f++)
				{
					Bitmap^ spriteImage = doors[f]->GetSpriteBitmap(Point(i, o));
					sprites[ESpriteType::OBJECT]->addImage(spriteImage, index, f);
				}
				index++;
			}
		}
}//End loadPestImages

//=================================================================================
// Section for loading Trap images
//=================================================================================
void LoadImages::loadTrapImages()
{
	for (int i = 0; i < traps[0]->getTileLstSizeX(); i++)
	{
		for (int o = 0; o < traps[0]->getTileLstSizeY(); o++)
		{
			for (int f = 0; f < GameSettings::NUMBER_OF_FRAMES_AI; f++)
			{
				Bitmap^ spriteImage = traps[f]->GetSpriteBitmap(Point(i, o));
				sprites[ESpriteType::OBJECT]->addImage(spriteImage, index, f);
			}
			index++;
		}
	}
}//End loadImagesImages

//=================================================================================
// Section for loading Trap images
//=================================================================================
void LoadImages::loadEffectsImages()
{
	for (int i = 0; i < effects[0]->getTileLstSizeX(); i++)
	{
		for (int o = 0; o < effects[0]->getTileLstSizeY(); o++)
		{
			for (int f = 0; f < GameSettings::NUMBER_OF_FRAMES_AI; f++)
			{
				Bitmap^ spriteImage = effects[f]->GetSpriteBitmap(Point(i, o));
				sprites[ESpriteType::OBJECT]->addImage(spriteImage, index, f);
			}
			index++;
		}
	}
}//End loadEffectsImages

//=================================================================================
// Section for loading Money Items
//=================================================================================
void LoadImages::loadMoneyImages()
{
	for (int i = 0; i < money->getTileLstSizeX(); i++)
	{
		for (int o = 0; o < money->getTileLstSizeY(); o++)
		{
			for (int f = 0; f < GameSettings::NUMBER_OF_FRAMES_AI; f++)
			{
				Bitmap^ spriteImage = money->GetSpriteBitmap(Point(i, o));
				sprites[ESpriteType::ITEM]->addImage(spriteImage, index, f);
			}
			index++;
		}
	}
}//End loadMoneyImages

//=================================================================================
// Section for loading Potion Items
//=================================================================================
void LoadImages::loadPotionImages()
{
	for (int i = 0; i < potion->getTileLstSizeX(); i++)
	{
		for (int o = 0; o < potion->getTileLstSizeY(); o++)
		{
			for (int f = 0; f < GameSettings::NUMBER_OF_FRAMES_AI; f++)
			{
				Bitmap^ spriteImage = potion->GetSpriteBitmap(Point(i, o));
				sprites[ESpriteType::ITEM]->addImage(spriteImage, index, f);
			}
			index++;
		}
	}
}//End loadMoneyImages

//=================================================================================
// Section for loading gui Elements
//=================================================================================
void LoadImages::loadguimages()
{
	for (int i = 0; i < gui[0]->getTileLstSizeX(); i++)
	{
		for (int o = 0; o < gui[0]->getTileLstSizeY(); o++)
		{
			for (int f = 0; f < GameSettings::NUMBER_OF_FRAMES_AI; f++)
			{
				Bitmap^ spriteImage = gui[f]->GetSpriteBitmap(Point(i, o));
				sprites[ESpriteType::GUI]->addImage(spriteImage, index, f);
			}
			index++;
		}
	}
}//End loadguimages

array<Bitmap^>^ LoadImages::getImages(ESpriteType sprite, int actor)
{
	return sprites[sprite]->getSprite(actor);
}

//=================================================================================
// Crops part of a given image used for the Player
//=================================================================================
Bitmap^ LoadImages::LoadActor(Bitmap^ spriteSheet, Rectangle croppedArea)
{
	Bitmap^ actorImage;

	actorImage = (Bitmap^)spriteSheet->Clone(croppedArea, spriteSheet->PixelFormat);

	return actorImage;
}//End LoadActor
//
 //==================================================================================
 // Creates the player actor
 //==================================================================================
array<Bitmap^>^ LoadImages::LoadPlayerImage()
{
	int directions = GameSettings::NUMBER_OF_DRECTIONS;
	Rectangle croppedtile;
	Bitmap^ east;
	Bitmap^ west;
	Bitmap^ north;
	Bitmap^ south;

	///////////////PLAYER//////////////////
	//Set spritesheets in the bitmaps
	//Crops a selected position which contains the character from the characters bitmap
	croppedtile = CropSection(0, 0, 3, 1);
	west = LoadActor(playerSpriteSheet, croppedtile);
	croppedtile = CropSection(0, 1, 3, 1);
	north = LoadActor(playerSpriteSheet, croppedtile);
	croppedtile = CropSection(0, 2, 3, 1);
	east = LoadActor(playerSpriteSheet, croppedtile);
	croppedtile = CropSection(0, 3, 3, 1);
	south = LoadActor(playerSpriteSheet, croppedtile);

	//Convert the bitmaps above into an array of bitmaps
	return CreateBitMapSpriteArray(east, west, north, south, directions);
} //End LoadSpriteImages
//
//==================================================================================
// Used to create the array of images for the sprites
//==================================================================================
array<Bitmap^>^ LoadImages::CreateBitMapSpriteArray(Bitmap^ east, Bitmap^ south, Bitmap^ west, Bitmap^ north, int directions)
{
	//Create array for the directions of the chicken
	array<Bitmap^>^ imagesArray = gcnew array<Bitmap^>(directions);

	//Declare the directions the chickenIamges uses using the above bitmaps
	imagesArray[0] = east;
	imagesArray[1] = south;
	imagesArray[2] = west;
	imagesArray[3] = north;

	//Removes the background for
	for (int i = 0; i < directions; i++)
	{
		Color transparentColor = imagesArray[i]->GetPixel(0, 0);
		imagesArray[i]->MakeTransparent(transparentColor);
	}

	return imagesArray;
}//End CreateBitMapSpriteArray
//

 //=================================================================================
 // Crops part of the image using tilesize
 //=================================================================================
Rectangle LoadImages::CropSection(int xPos, int yPos, int Width, int Height)
{
	Rectangle croppedArea = Rectangle(xPos * GameSettings::TILE_SIZE, yPos * GameSettings::TILE_SIZE, Width * GameSettings::TILE_SIZE, Height * GameSettings::TILE_SIZE);

	return croppedArea;
}//End CropSection
