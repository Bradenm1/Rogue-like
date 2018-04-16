#include "Logger.h"


//==================================================================================
// Constructor
//==================================================================================
Logger::Logger()
{
	loggingText = gcnew array<String^>(GameSettings::LOGGING_ARRAY_SIZE);

	//Set default string as ""
	removeAllText();
}//End Contructor

//==================================================================================
// Adds a new string to the array
//==================================================================================
void Logger::addText(String^ newText)
{
	//Check if the first slot is empty
	if (loggingText[0]->Equals(""))
	{
		loggingText[0] = newText;
	}
	//If not move all strings up in the array then put the new text in the first slot
	else
	{
		sortText();
		loggingText[0] = newText;
	}
}//End addText

//==================================================================================
// Sorts the array if all slots are full of text and allows the new text to be added
//==================================================================================
void Logger::sortText()
{
	String^ temp01 = loggingText[1];
	String^ temp02 = loggingText[2];
	loggingText[1] = loggingText[0];

	loggingText[2] = temp01;
	temp01 = loggingText[3];
	loggingText[3] = temp02;
	temp02 = loggingText[4];
	loggingText[4] = temp01;

	loggingText[5] = temp02;
}//End sortText

//==================================================================================
// Removes all strings from the array
//==================================================================================
void Logger::removeAllText()
{
	for (int i = 0; i < GameSettings::LOGGING_ARRAY_SIZE; i++)
	{
		loggingText[i] = "";
	}
}//End removeAllText