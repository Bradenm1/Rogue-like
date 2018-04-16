#pragma once

#include "GameSettings.h"

//Namespaces
using namespace System;
using namespace System::Drawing;

//==================================================================================
// This class contains the text which is logged from any class which uses it
// This text is displayed in the UI
//==================================================================================
ref class Logger
{
private:
	array<String^>^ loggingText;
public:
	Logger();
	void addText(String ^ newText);
	void sortText();

	void removeAllText();

	int getArraySize() { return loggingText->Length; }
	String^ getString(int index) { return loggingText[index]; }
};

