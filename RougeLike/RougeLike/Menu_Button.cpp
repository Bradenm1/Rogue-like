#include "Menu_Button.h"



Menu_Button::Menu_Button(Graphics^ startCanvas, Point startPosition, String^ startButtonText, int startFontSize, bool startBackground)
{
	canvas = startCanvas;
	position = startPosition;
	buttonName = startButtonText;
	background = startBackground;

	font = gcnew Font("SDS_8x8", startFontSize);

	//Get text size in pixels
	textSize.X = canvas->MeasureString(buttonName, font).Width;
	textSize.Y = canvas->MeasureString(buttonName, font).Height;

	buttonBackgroundColour = gcnew SolidBrush(Color::FromArgb(BACKGROUND_ALPHA, 0, 0, 0));
	textColour = gcnew SolidBrush(Color::FromArgb(GameSettings::COLOUR_255, GameSettings::COLOUR_255, GameSettings::COLOUR_255));
}

void Menu_Button::draw()
{
	if (background == true)
	{
		canvas->FillRectangle(buttonBackgroundColour, position.X, position.Y, textSize.X, textSize.Y);
	}
	canvas->DrawString(buttonName, font, textColour, Point(position.X, position.Y));
}

bool Menu_Button::checkWithin(Point checkPosition)
{
	//Check if the mouse is within it
	if ((checkPosition.X > position.X) && (checkPosition.Y > position.Y) &&
		(checkPosition.X < position.X + textSize.X) && (checkPosition.Y < position.Y + textSize.Y))
	{
		return true;
	}
	return false;
}