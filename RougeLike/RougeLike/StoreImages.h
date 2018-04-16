#pragma once

using namespace System;
using namespace System::Drawing;

//==================================================================================
// This class is used to store each image of sprite 
// it stores two for each frame of the image
//==================================================================================
ref class StoreImages
{
private:
	array<Bitmap^, 2>^ spriteImage;
public:
	StoreImages(int startArraySize);

	void addImage(Bitmap^ newImage, int index, int frame);
	array<Bitmap^>^ getSprite(int sprite);
};

