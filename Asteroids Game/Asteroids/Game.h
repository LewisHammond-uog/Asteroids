#ifndef __GAME_H__
#define __GAME_H__

//C Includes
#include <chrono>
#include <string>

//Predefines
class AsteroidManager;
class BulletManager;
class UFOManager;
class PlayerShip;

//Class to run the main game loop and menu
class Game
{

public:
	Game();
	~Game();

	//Game Initalisation and Update
	bool Initalise();
	void Update();

	//Game Manipulation Functions
	void AddScore(int a_iScoreToAdd);
	void RemoveLife();
	void ResetGame();
	int GetScore();

private:

	//Enum for Game State
	enum GAME_STATE {
		GAME_STATE_MENU,
		GAME_STATE_PLAYING,
		GAME_STATE_GAMEOVER,
		GAME_STATE_EXIT,

		GAME_STATE_COUNT,
	};

	//Game Control Variables
	int m_iGameScore = 0;
	int m_iGameLives = 0;
	const int mc_iStartingLives = 3;
	const int mc_iLivesToRemove = 1;

	//Deinitise Function
	void DeInitalise();


	#pragma region Menu 

	//Enum for Menu Options
	enum MENU_OPTION {
		MENU_OPTION_PLAY,
		MENU_OPTION_EXIT,

		MENU_OPTION_COUNT,
	};

	//Struct for Selection Arrow
	struct SelectionArrow
	{
		//Sprite
		int m_iSpriteID;

		//Offsets
		int const c_iArrowVerticalOffset = 15;
		int const c_iArrowHorizontalOffset = -40;

		//Postion
		int iXPos;
		int iYPos;

	};

	//Menu Functions
	void InitaliseMenu();
	void DeInitaliseMenu();
	void DoMenu();
	void DrawMenu();

	//Menu Vars
	int m_iMenuTitleSprite;
	int m_iMenuFont;
	int const mc_iMenuTextTop = 375;
	int const mc_iMenuTextLeft = 425;
	int const mc_iMenuTextSpacing = 75;
	int const mc_iMenuOptionsFontSize = 30;
	bool m_bMenuSelectionChanged;

	//Selection Arrow Pointer
	SelectionArrow* m_oMenuSelectionArrow;

	//Menu State
	MENU_OPTION m_eCurrentMenuSelection;

	#pragma endregion

	#pragma region Playing

	//Game Running Functions
	void InitalisePlaying();
	void DeInitalisePlaying();
	bool PlayingNullCheck();
	void DoPlaying();
	void DrawPlaying();
	void DrawGUI();


	int m_iPlayingGUIFont;

	//Pointers for Managers and PlayerShip
	AsteroidManager* m_poGameAsteroidManager;
	BulletManager* m_poGameBulletManager;
	UFOManager* m_poGameUFOManager;
	PlayerShip* m_poGamePlayerShip;


	#pragma endregion

	
	#pragma region Game Over

	//Array for all characters in the high score menu
	int const mc_iHighScoreCharacterCount = 25;
	std::string const mc_asHighScoreCharacters[26] = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
	int m_aHighScoreCharacterSelection[3] = { 0,0,0 }; //Stores the currently selected character numbers

	void InitaliseGameOver();
	void DeInitaliseGameOver();
	void DoGameOver();
	void DrawGameOver();


	int m_iGameOverTitleFont;
	int const mc_iGameOverTitleY = 600;
	int const mc_iGameOverTitleX = 250;

	int m_iGameOverScoreFont;
	int const mc_iGameOverScoreX = 430;
	int const mc_iGameOverScoreY = 450;

	int const mc_iHighScoreLabelX = 280;
	int const mc_iHighScoreLabelY = 280;

	int const mc_iHighScoreExitTextX = 200;
	int const mc_iHighScoreExitTextY = 200;


	#pragma endregion

	//void (*m_pCurrentGameFunct)(void);
	GAME_STATE m_eCurrentGameState;

};

#endif // !__GAME_H__

