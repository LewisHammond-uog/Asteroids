////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Brief: File for functions for UFO management within the game
////////////////////////////////////////////////////////////

#include "UFOManager.h"

//C includes
#include <vector>

//UG Includes
#include "UGFW.h"
#include "UG_Defines.h"

//Project Includes
#include "AsteroidManager.h"
#include "PlayerShip.h"
#include "BulletManager.h"
#include "UFO.h"


UFOManager::UFOManager(AsteroidManager* a_poAsteroidManager, PlayerShip* a_poPlayerShip, BulletManager* a_poBulletManager)
{

	m_poAsteroidManager = a_poAsteroidManager;
	m_poPlayerShip = a_poPlayerShip;
	m_poMyBulletManager = a_poBulletManager;
}


UFOManager::~UFOManager()
{

}

///<summary>
/// Updates and Draws UFOs Managed by this manager
///</summary>
void UFOManager::UpdateUFOS() {

	//Check if we should spawn new UFOs
	if (UFOSpawnCheck()) {

		//Increase the UFO wave count
		m_iCurrentUFOWave++;

		//Spawn a UFO
		SpawnUFO();
	}

	for (int i = 0; i < m_apoUFOList.size(); i++) {

		UFO* poUFO = m_apoUFOList[i];

		//Null Check
		if (poUFO != nullptr) {
			//Run Update and Draw
			poUFO->Update();
			poUFO->Draw();
		}
	}

}

///<summary>
/// Checks if we should spawn a UFO
///</summary>
bool UFOManager::UFOSpawnCheck() {

	//Get current asteroid wave and count
	if (m_poAsteroidManager != nullptr) {
		m_iCurrentAsteroidWave = m_poAsteroidManager->GetAsteroidWave();
		m_iCurrentAsteroidCount = m_poAsteroidManager->GetAsteroidCount();
	}

	//If there are less than the number asteroids and the current UFO wave is less than the 
	//current asteroid wave spawn a UFO
	if (m_iCurrentAsteroidCount <= mc_iAsteroidCountToSpawnUFOS && m_iCurrentUFOWave < m_iCurrentAsteroidWave) {

		//Return that we should spawn an UFO
		return true;

	}

	//Return that we shouldn't spawn new UFO
	return false;

}

///<summary>
/// Creates a New UFO and adds to the UFO vector
///</summary>
void UFOManager::SpawnUFO() {

	//Get Screen Size
	int iScreenWidth;
	int iScreenHeight;
	UG::GetScreenSize(iScreenWidth, iScreenHeight);

	//Generate UFO Start Postion, around the edges of the screen
	//Choose Side of the screen to start from
	SCREEN_BOUND eStartingBound = static_cast<SCREEN_BOUND>(rand() % SCREEN_BOUND_COUNT);

	//Setup vars for start location
	int iCreateX = 0;
	int iCreateY = 0;
	
	//Switch for where we are starting
	switch (eStartingBound)
	{
	case UFOManager::SCREEN_BOUND_TOP:
		iCreateX = rand() % iScreenWidth;
		iCreateY = iScreenHeight;
		break;
	case UFOManager::SCREEN_BOUND_RIGHT:
		iCreateX = iScreenWidth;
		iCreateY = rand() % iScreenHeight;
		break;
	case UFOManager::SCREEN_BOUND_BOTTOM:
		iCreateX = rand() % iScreenWidth;
		iCreateY = 0;
		break;
	case UFOManager::SCREEN_BOUND_LEFT:
		iCreateX = 0;
		iCreateY = rand() % iScreenHeight;
		break;
	default:
		iCreateX = 0;
		iCreateY = 0;
		break;
	}

	//Create UFO
 	UFO* poNewUFO = new UFO(iCreateX, iCreateY, this, m_poPlayerShip, m_poMyBulletManager);

	//Check UGO is not a null pointer
	if (poNewUFO != nullptr) {
		//Add to UFO List
		m_apoUFOList.push_back(poNewUFO);
	}

}


///<summary>
/// Destroys a UFO and removes it from a vector
///</summary>
void const UFOManager::DestoryUFO(UFO* a_poUFOToDestroy) {
	

	//Null Check

	if (a_poUFOToDestroy != nullptr) {

		int iUFOToDestoyVectorIndex = NULL;

		//Pointer for the UFO that we are at in the list
		UFO* poUFOToCheck;

		for (int i = 0; i < m_apoUFOList.size(); i++)
		{

			poUFOToCheck = m_apoUFOList[i];

			if (poUFOToCheck != nullptr) {

				poUFOToCheck = m_apoUFOList[i];

				if (poUFOToCheck = a_poUFOToDestroy) {
					//We have found the asteroid, record its postion in the vector
					iUFOToDestoyVectorIndex = i;
					break; //Break out of loop
				}
			}
		}


		//Remove UFO in vector#
		if (iUFOToDestoyVectorIndex != NULL) {
			m_apoUFOList.erase(m_apoUFOList.begin() + iUFOToDestoyVectorIndex);
		}

	}



}