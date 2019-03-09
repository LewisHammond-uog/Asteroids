////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Brief: Main game loop and menu loop for the game
////////////////////////////////////////////////////////////
#include "Game.h"

//C Includes
#include <string>
#include <cstring>
#include <time.h>
#include <chrono>
#include <iostream>
#include <fstream>

//UG Includes
#include "UGFW.h"
#include "UG_Defines.h"

//Project Includes
#include "AsteroidManager.h"
#include "BulletManager.h"
#include "UFOManager.h"
#include "PlayerShip.h"



Game::Game()
{
	//Initalise enum
	m_eCurrentGameState = GAME_STATE_MENU;

}


Game::~Game()
{
}

/// <summary>
/// Initalises Game Variables and Creates Screen
/// </summary>
bool Game::Initalise() {

	//Attempt to create a window, get if it was successful or not
	bool bWindowCreateSuccess = UG::Create(1024, 768, false, "Sample", 100, 100);

	if (bWindowCreateSuccess) {
		//Set window background colour
		UG::SetBackgroundColor(UG::SColour(0x00, 0x00, 0x00, 0xFF));

		//Clear Screen
		UG::ClearScreen();

		//Initalise Menu
		InitaliseMenu();

	}
	
	//Return if we have created the window or not
	return bWindowCreateSuccess;

}

void Game::DeInitalise() {

	//Clear the Screen
	UG::ClearScreen();

	//De Initliase Menu
	DeInitaliseMenu();

}

/// <summary>
/// Main Game Loop
/// </summary>
void Game::Update() {

	switch (m_eCurrentGameState)
	{
	case Game::GAME_STATE_MENU:
		DoMenu();
		DrawMenu();
		break;
	case Game::GAME_STATE_PLAYING:
		DoPlaying();
		DrawPlaying();
		DrawGUI();
		break;
	case Game::GAME_STATE_GAMEOVER:
		DoGameOver();
		DrawGameOver();
		break;
	case Game::GAME_STATE_EXIT:
		//Close Game
		UG::Close();
		break;
	default:
		break;
	}


}

#pragma region Main Menu

/// <summary>
/// Initalises Game Main Menu
/// </summary>
void Game::InitaliseMenu() {

	//Initalise Selected Option
	m_eCurrentMenuSelection = MENU_OPTION_PLAY;

	//Set Menu Title Sprite
	m_iMenuTitleSprite = UG::CreateSprite("./images/game_menu/asteroids_logo.png", (float)1927/4, (float)741/4, true);
	UG::SetSpritePosition(m_iMenuTitleSprite, 1024 / 2, 768 / 2 + 270);

	//Initalise Selection Arrow
	m_oMenuSelectionArrow = new SelectionArrow;

	//Null Check Menu Selection Arrow
	if (m_oMenuSelectionArrow != nullptr) {
		m_oMenuSelectionArrow->m_iSpriteID = UG::CreateSprite("./images/game_menu/selectionArrow.png", 40, 40, true);
	}

	//Create and Set Fonts
	m_iMenuFont = UG::AddFont("./fonts/menufont.ttf", 30);
	UG::SetFont(m_iMenuFont);

}

/// <summary>
/// Runs Menu Logic
/// </summary>
void Game::DoMenu(){

	//Moving Menu Up and Down
	//Only allow menu selection to be changed if it was not changed last frame
	//Other wise we skip through the options too fast
	if (UG::WasKeyPressed(UG::KEY_DOWN)) {

		//See what option we should now select
		switch (m_eCurrentMenuSelection)
		{
		case Game::MENU_OPTION_PLAY:
			m_eCurrentMenuSelection = MENU_OPTION_EXIT;
			break;
		case Game::MENU_OPTION_EXIT:
		default:
			m_eCurrentMenuSelection = MENU_OPTION_PLAY;
			break;
		}

	}
	else if (UG::WasKeyPressed(UG::KEY_UP)) {

		//See what option we should now select
		switch (m_eCurrentMenuSelection)
		{
		case Game::MENU_OPTION_PLAY:
			m_eCurrentMenuSelection = MENU_OPTION_EXIT;
			break;
		case Game::MENU_OPTION_EXIT:
			m_eCurrentMenuSelection = MENU_OPTION_PLAY;
			break;
		default:
			m_eCurrentMenuSelection = MENU_OPTION_PLAY;
			break;
		}
	}


	//Update Selection Arrow Postion
	//Null Check Selection Arrow
	if (m_oMenuSelectionArrow != nullptr) {

		//Calculate Selection Arrow Postion
		m_oMenuSelectionArrow->iXPos = mc_iMenuTextLeft + m_oMenuSelectionArrow->c_iArrowHorizontalOffset;
		m_oMenuSelectionArrow->iYPos = mc_iMenuTextTop - (mc_iMenuTextSpacing * m_eCurrentMenuSelection) + m_oMenuSelectionArrow->c_iArrowVerticalOffset;

		//Apply New Arrow Postion
		UG::SetSpritePosition(m_oMenuSelectionArrow->m_iSpriteID, (float)m_oMenuSelectionArrow->iXPos, (float)m_oMenuSelectionArrow->iYPos);
	}

	//Apply Option if enter is pressed
	if (UG::WasKeyPressed(UG::KEY_ENTER)) {
		
		//Check what our current menu selection is and then change our state based on that
		switch (m_eCurrentMenuSelection)
		{
		case Game::MENU_OPTION_PLAY:
			//Initalise Game Playing Mode then switch state
			InitalisePlaying();
			m_eCurrentGameState = GAME_STATE_PLAYING;
			break;
		case Game::MENU_OPTION_EXIT:
			DeInitalise();
			m_eCurrentGameState = GAME_STATE_EXIT;
			break;
		default:
			break;
		}

		//De Initalise Menu
		DeInitaliseMenu();

	}


}

/// <summary>
/// Draws Menu Buttons and Title
/// </summary>
void Game::DrawMenu() {

	//Clear Screen
	UG::ClearScreen();

	//Draw Menu Text (as Image)
	UG::DrawSprite(m_iMenuTitleSprite);

	//Draw Text Options
	UG::DrawString("PLAY", mc_iMenuTextLeft, (mc_iMenuTextTop), 0);
	UG::DrawString("QUIT", mc_iMenuTextLeft, (mc_iMenuTextTop - mc_iMenuTextSpacing), 0);

	//Draw Arrow
	if (m_oMenuSelectionArrow != nullptr) {
		UG::DrawSprite(m_oMenuSelectionArrow->m_iSpriteID);
	}

}

/// <summary>
/// DeInitalises Game Main Menu, destorying created assets
/// </summary>
void Game::DeInitaliseMenu() {

	//Destory Menu Text Sprite
	UG::DestroySprite(m_iMenuTitleSprite);

	//Remove Font
	UG::RemoveFont(m_iMenuFont);

	//Destroy Selection Arrow Struct, if not nullptr
	if (m_oMenuSelectionArrow != nullptr) {

		//Delete sprite
		UG::DestroySprite(m_oMenuSelectionArrow->m_iSpriteID);

		//Delete Object
		delete m_oMenuSelectionArrow;
		m_oMenuSelectionArrow = nullptr;
	}


}

#pragma endregion

#pragma region Playing

/// <summary>
/// Initalises the Game Playing Space
/// </summary>
void Game::InitalisePlaying() {

	//Seed Random
	srand(time(NULL));

	//Create Managers
	m_poGameBulletManager = new BulletManager();
	m_poGamePlayerShip = new PlayerShip(m_poGameBulletManager, this);
	m_poGameAsteroidManager = new AsteroidManager(this, m_poGamePlayerShip);
	m_poGameUFOManager = new UFOManager(m_poGameAsteroidManager, m_poGamePlayerShip, m_poGameBulletManager);

	//Set Score
	m_iGameScore = 0;

	//Set Lives
	m_iGameLives = mc_iStartingLives;

	//Initalise GUI
	//Create and Set Fonts
	m_iPlayingGUIFont = UG::AddFont("./fonts/scorefont.ttf", 20);
	UG::SetFont(m_iPlayingGUIFont);

}

/// <summary>
/// De initalises the playing mode
/// </summary>
void Game::DeInitalisePlaying(){

	//Delete all our game managers
	delete m_poGameAsteroidManager;
	delete m_poGameBulletManager;
	delete m_poGamePlayerShip;
	delete m_poGameUFOManager;

	//Null all our pointers
	m_poGameAsteroidManager = nullptr;
	m_poGameBulletManager = nullptr;
	m_poGamePlayerShip = nullptr;
	m_poGameUFOManager = nullptr;

}

/// <summary>
/// Runs Loop for main gameplay
/// </summary>
void Game::DoPlaying() {

	//Null Check our pointers
	if (PlayingNullCheck()) {

		//Run Update Functions on Managers
		m_poGameAsteroidManager->UpdateAsteroids();
		m_poGameBulletManager->UpdateBullets();
		m_poGameUFOManager->UpdateUFOS();
		m_poGamePlayerShip->UpdateShip();

	}

}

/// <summary>
/// Draws main game objects
/// </summary>
void Game::DrawPlaying() {

	//Clear the screen
	UG::ClearScreen();

	//Null Check our pointers
	if (m_poGamePlayerShip != nullptr) {

		//All other objects are drawn via their managers update functions
		m_poGamePlayerShip->DrawShip();

	}

}

/// <summary>
/// Draws the GUI display
/// </summary>
void Game::DrawGUI() {
	UG::SetFont(m_iPlayingGUIFont);

	//Convert Score and Lives ints to string
	std::string sScoreDisplay = "SCORE: " + std::to_string(m_iGameScore);
	std::string sLivesDisplay = "LIVES: " + std::to_string(m_iGameLives);


	UG::DrawString(sScoreDisplay.c_str(), 10, 735, 0);
	UG::DrawString(sLivesDisplay.c_str(), 10, 710, 0);
}

/// <summary>
/// Adds to the game score
/// </summary>
void Game::AddScore(int a_iScoreToAdd) {

	//Increase Score var
	m_iGameScore += a_iScoreToAdd;

}

/// <summary>
/// Decreases Lives
/// </summary>
void Game::RemoveLife() {

	//Remove 1 Life
	m_iGameLives -= mc_iLivesToRemove;

}

/// <summary>
/// Resets the player ship if they have they have more than 0 lives
/// </summary>
void Game::ResetGame() {

	//Check if we have more than 0 lives
	if (m_iGameLives > 0) {
		//Get Screen bounds
		int iScreenWidth;
		int iScreenHeight;
		UG::GetScreenSize(iScreenWidth, iScreenHeight);

		//Reset Playership to the center of the screen
		if (m_poGamePlayerShip != nullptr) {
			m_poGamePlayerShip->SetPostion((const float)iScreenWidth / 2, (const float)iScreenHeight / 2);
		}

		//Reset Asteroids
		if (m_poGameAsteroidManager != nullptr) {
			m_poGameAsteroidManager->Reset();
		}
	}
	else {

		//We have lost all our lives, go to game end screen
		DeInitalisePlaying();
		m_eCurrentGameState = GAME_STATE_GAMEOVER;
		InitaliseGameOver();

	}

}

/// <summary>
/// Gets the current game score
/// </summary>
int Game::GetScore() {

	return m_iGameScore;

}

/// <summary>
/// Resets the player ship if they have they have more than 0 lives
/// </summary>
bool Game::PlayingNullCheck() {
	if(m_poGameAsteroidManager != nullptr && m_poGameBulletManager != nullptr && 
		m_poGamePlayerShip != nullptr && m_poGameUFOManager != nullptr){

		//Not Null
		return true;

	}
	else {

		//Something is null
		return false;

	}
}

#pragma endregion


#pragma region Game Over

/// <summary>
/// Initalises the game over screen
/// </summary>
void Game::InitaliseGameOver() {

	m_iGameOverTitleFont = UG::AddFont("./fonts/gameovertitlefont.ttf", 75);
	m_iGameOverScoreFont = UG::AddFont("./fonts/gameoverscorefont.ttf", 30);

}

/// <summary>
/// Does the logic for the game over section
/// </summary>
void Game::DoGameOver() {

	//If Enter is pressed then return to menu
	if (UG::WasKeyPressed(UG::KEY_ENTER)) {
		m_eCurrentGameState = GAME_STATE_MENU;
		InitaliseMenu();
	}

}


/// <summary>
/// Draws the game over screen
/// </summary>
void Game::DrawGameOver() {

	//Clear the screen
	UG::ClearScreen();
	
	//Show Game Over Title
	UG::SetFont(m_iGameOverTitleFont);
	UG::DrawString("GAME OVER", mc_iGameOverTitleX, mc_iGameOverTitleY, 0);

	//Show Score
	UG::SetFont(m_iGameOverScoreFont);
	std::string sScoreDisplay = "SCORE: " + std::to_string(m_iGameScore);
	UG::DrawString(sScoreDisplay.c_str(), mc_iGameOverScoreX, mc_iGameOverScoreY, 0);

	UG::DrawString("PRESS ENTER TO RETURN TO MENU", mc_iHighScoreExitTextX, mc_iHighScoreExitTextY, 0);
}

/// <summary>
/// Deinitalises game over screen
/// </summary>
void Game::DeInitaliseGameOver() {

	//Delete Fonts
	UG::RemoveFont(m_iGameOverTitleFont);
	UG::RemoveFont(m_iGameOverScoreFont);

}

#pragma endregion
