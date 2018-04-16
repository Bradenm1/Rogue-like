#include "StoreImages.h"
#include "GameSettings.h"

//==================================================================================
// Constructor
//==================================================================================
StoreImages::StoreImages(int startArraySize)
{
	spriteImage = gcnew array<Bitmap^, GameSettings::NUMBER_OF_FRAMES_AI>(startArraySize, GameSettings::NUMBER_OF_FRAMES_AI);
}//End Constructor

//==================================================================================
// Adds a given image, index and number of the frame to the images array
//==================================================================================
void StoreImages::addImage(Bitmap ^ newImage, int index, int frame)
{
	spriteImage[index, frame] = newImage;
}//End addImage

//==================================================================================
// Gets the spriteSheet at a given index
//==================================================================================
array<Bitmap^>^ StoreImages::getSprite(int sprite)
{
	array<Bitmap^>^ sprites = gcnew array<Bitmap^>(GameSettings::NUMBER_OF_FRAMES_AI);

	for (int i = 0; i < sprites->Length; i++)
	{
		sprites[i] = gcnew Bitmap(spriteImage[sprite, i]);
	}

	return sprites;
}//End getSprite
