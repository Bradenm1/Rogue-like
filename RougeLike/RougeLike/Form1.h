/*  Porgram name:      RougeLike
*  Project file name:  RougeLike
*  Author:             Braden Mckewen
*  Date:               16/10/2017
*  Language:           C++ CLI
*  Platform:           Microsoft Visual Studio 2015
*  Purpose:            The aim of the game is to nagivate through dungeons and try collect as much money as possible, without dying.
*  Description:        In this game there's a player which will nagivate through dungeons avoiding traps, enemys, etc. While trying to collect money.
*  Additional Features:   Projectiles, FSM.
*/

/*
*  References:
*
*  Art:
*  DawnBringer. (2014). DawnLike - 16x16 Universal Rogue-like tileset v1.81. Retrieved from https://opengameart.org/content/dawnlike-16x16-universal-rogue-like-tileset-v181
*
*  Double-Buffering:
*  Microsoft. (2017). BufferedGraphics Class. Retrieved from https://msdn.microsoft.com/en-us/library/system.drawing.bufferedgraphics(v=vs.110).aspx
*
*  Image Cloning
*  Microsoft. (2017). Bitmap::Clone Method (Rectangle, PixelFormat). Retrieved from https://msdn.microsoft.com/en-us/library/ms141944(v=vs.110).aspx
*/

#pragma once

//Included Classes
#include "GameManager.h"

//Setting used to work out frame-rate
#define ONEMSC 1000

namespace RougeLike {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Panel^  panel1;
	protected:
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		//==================================================================================
		// User Defined Varaiables
		//==================================================================================
		GameManager^ gamemanager;
		Point resolution;
		Graphics^ canvas;
		Random^ rGen;
		//End of User Defined Varaiables


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(12, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(200, 100);
			this->panel1->TabIndex = 0;
			this->panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel1_MouseDown);
			this->panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel1_MouseMove);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->panel1);
			this->KeyPreview = true;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyUp);
			this->ResumeLayout(false);

		}
#pragma endregion

	//==================================================================================
	// Form1_Load Event
	//==================================================================================
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
	{
		//Set the resolution for the game
		resolution.X = GameSettings::RESOLUTION_X;
		resolution.Y = GameSettings::RESOLUTION_Y;

		//Maximize the game
		//WindowState = FormWindowState::Maximized;
		FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		WindowState = FormWindowState::Maximized;

		//Positions the form at the top left hand of the screen
		Top = 0;
		Left = 0;

		//Alow keydown, keyup to be used while panel is in-focus
		KeyPreview = false;

		//The given amount which should be a second (1000) is divded by 60 to give 60 frames a second
		int frameRate = ONEMSC / GameSettings::FRAME_RATE;
		//Sets the framerate for the game
		timer1->Interval = frameRate;

		//Sets the panel to be the width of the custom set row and width in clientsize
		Width = resolution.X;
		Height = resolution.Y;
		//Set the panel to the resolution
		panel1->Top = 0;
		panel1->Left = 0;
		panel1->Width = resolution.X;
		panel1->Height = resolution.Y;
		//Create graphics on the panel
		canvas = panel1->CreateGraphics();
		//Creates the random
		rGen = gcnew Random();
		//Create the Game Manager for the game
		gamemanager = gcnew GameManager(canvas, rGen, Point(GameSettings::MAP_ROW, GameSettings::MAP_COL), resolution);
		//Enable the timer
		timer1->Enabled = true;
	}//End Form1_Load Event

	 //==================================================================================
	 // timer1_Tick Event
	 //==================================================================================
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		//Runs the game cycle
		gamemanager->gameCycle();
	}//End timer1_Tick Event

	//==================================================================================
	// Keydown Event
	//==================================================================================
	private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		//Passes in the key pressed
		keyPress(e->KeyData, true);
	}//End Form1_KeyDown

	//==================================================================================
	// Keyup Event
	//==================================================================================
	private: System::Void Form1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		//Passed in the key released
		keyPress(e->KeyData, false);
	}//End Form1_KeyUp

	//==================================================================================
	// Allows a key to be passed in and if it has been pressed or realsed.
	//==================================================================================
	private: void keyPress(Keys key, bool isPressed)
	{
		//Checks when a key is not being pressed anymore
		switch (key)
		{
		case Keys::Left:
			gamemanager->spriteDirection(EDirection::WEST, isPressed);
			break;
		case Keys::Right:
			gamemanager->spriteDirection(EDirection::EAST, isPressed);
			break;
		case Keys::Up:
			gamemanager->spriteDirection(EDirection::NORTH, isPressed);
			break;
		case Keys::Down: 
			gamemanager->spriteDirection(EDirection::SOUTH, isPressed);
			break;
		case Keys::F:
			gamemanager->actorAttack();
			break;
		case Keys::E:
			gamemanager->actorInteract();
			break;
		case Keys::Escape:
			//Create the Game Manager for the game
			gamemanager = gcnew GameManager(canvas, rGen, Point(GameSettings::MAP_ROW, GameSettings::MAP_COL), resolution);
			break;
		default:
			break;
		}
		//Updates the viewport position
		gamemanager->updateViewPort();
	}//End keyPress

	//==================================================================================
	// Allows a key to be passed in and if it has been pressed or realsed.
	//==================================================================================
	private: System::Void panel1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		gamemanager->setMousePosition(e->Location);
	}//End panel1_MouseMove

	//==================================================================================
	// Allow the mouse down to be passed in
	//==================================================================================
	private: System::Void panel1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		switch (e->Button)
		{
		case ::MouseButtons::Left:
			gamemanager->setMouseDown();
			break;
		case ::MouseButtons::Right:
			gamemanager->setMouseDownRight();
			break;
		default:
			break;
		}
	}//End panel1_MouseDown
};
}
