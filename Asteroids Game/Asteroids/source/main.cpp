////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Date Created: 16/11/2018
// Brief: Game Main file created for CT 4019 Assignment. This Game loop is used for creating
// and running functions for the asteroids game
////////////////////////////////////////////////////////////

//includes to allow use of the UG Framework
#include "UGFW.h"
#include "UG_Defines.h"
#include "Game.h"
#include <time.h>


int main(int argv, char* argc[])
{

	//Create Game Object
	Game* poRunningGame = new Game();

	//Initalse Game 
	bool bGameInitalised = poRunningGame->Initalise();

	if (bGameInitalised) {
		//Main Game Loop
		do
		{
			//Run Game Update Loop
			poRunningGame->Update();

			// Quit our application when escape is pressed
			if (UG::IsKeyDown(UG::KEY_ESCAPE))
				UG::Close();

		} while (UG::Process());
	}

	//Close down the framework by calling dispose.
	UG::Dispose();

	return 0;
}
