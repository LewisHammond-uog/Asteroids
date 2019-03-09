////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Brief: File for the manager of asteroids within the game
////////////////////////////////////////////////////////////

#include "AsteroidManager.h"

//C Includes
#include <vector>

//UG Includes
#include "UGFW.h"
#include "UG_Defines.h"

//Project Includes
#include "Asteroid.h"
#include "BulletManager.h"
#include "PlayerShip.h"
#include "SpaceObject.h"
#include "Game.h"

std::vector<Asteroid*> AsteroidManager::s_apoAsteroidList;


//Constructor
AsteroidManager::AsteroidManager(Game* a_poGameManager, PlayerShip* a_poPlayerShip)
{

	//Set our game manager
	if (a_poGameManager != nullptr) {
		m_poMyGameManager = a_poGameManager;
	}

	//Set out PlayerShip
	if (a_poPlayerShip != nullptr) {
		m_poPlayerShip = a_poPlayerShip;
	}

}

//Destructor
AsteroidManager::~AsteroidManager()
{

	//Destroy all our asteroids that are managed by us
	for each(Asteroid* poAsteroid in s_apoAsteroidList) {
		if (poAsteroid != nullptr) {
			//Destroy Asteroid
			delete poAsteroid;
		}
	}
	
	//Clear Vector
	s_apoAsteroidList.clear();

}

/// <summary>
/// Updates the current asteroids and spawns more if neccesary  
/// </summary>
void AsteroidManager::UpdateAsteroids() {

	//Check to see if there are enough asteroids
	if (m_iAsteroidCount <= 0) {
		//Trigger New Asteroid Wave
		NewAsteroidWave();
	}

	//Go through the vector of asteroids
	for each(Asteroid* poAsteroid in s_apoAsteroidList) {
		//Null Check
		if (poAsteroid != nullptr) {
			//Run Update Function
			poAsteroid->UpdateAsteroid();
			poAsteroid->DrawAsteroid();
		}
	}

}

/// <summary>
/// Creates an asteroid on screen and adds it to the asteroid vector
/// </summary>
void const AsteroidManager::CreateAsteroid() {

	//Random Generate Start Postion
	int iScreenWidth;
	int iScreenHeight;
	UG::GetScreenSize(iScreenWidth, iScreenHeight);
	
	//Make sure we don't spawn an asteroid within the player
	int iCreateX;
	int iCreateY;
	int iCreateSize;

	do {
		iCreateX = rand() % iScreenWidth;
		iCreateY = rand() % iScreenHeight;
		iCreateSize = mc_iMinAsteroidSize + rand() % (mc_iMaxAsteroidSize - mc_iMinAsteroidSize);
	} while (!SpawningWithinPlayer(m_poPlayerShip, iCreateX, iCreateY, iCreateSize));

	

	//Call "proper" create function
	CreateAsteroid(iCreateX, iCreateY, iCreateSize);

}

/// <summary>
/// Checks if we are about to spawn an asteroid inside the player
/// </summary>
bool AsteroidManager::SpawningWithinPlayer(PlayerShip* a_poPlayerShip, int a_iAsteroidCreateX, int a_iAsteroidCreateY,
						int a_iAsteroidCreateSize) {

	//Get Playership Postion and Radius
	float fPlayerXPos = 0.0f;
	float fPlayerYPos = 0.0f;
	float fPlayerRadius = 0.0f;

	if (a_poPlayerShip != nullptr) {
		a_poPlayerShip->GetPostion(fPlayerXPos, fPlayerYPos);
		fPlayerRadius - a_poPlayerShip->GetRadius();
	}

	//Check Distance between objects
	float fYDistance = pow((fPlayerXPos - a_iAsteroidCreateX), 2);
	float fXDistance = pow((fPlayerYPos - a_iAsteroidCreateY), 2);

	float fTotalDistance = sqrt(fXDistance + fYDistance);

	//Check if the sum of the radiuses are less than the distance
	if ((fPlayerRadius + a_iAsteroidCreateSize) < fTotalDistance) {
		//We will collide and thus will spawn within player
		return true;
	}
	else {
		//We will not spawn inside player
		return false;
	}

}

/// <summary>
/// Creates an asteroid on screen and adds it to the asteroid vector
/// </summary>
void const AsteroidManager::CreateAsteroid(int a_iCreateX, int a_iCreateY, int a_iCreateSize) {

	//Create Asteroid
	Asteroid* poNewAsteroid = new Asteroid((float)a_iCreateX, (float)a_iCreateY, a_iCreateSize, this);

	//Check that Asteroid is not null pointer
	if (poNewAsteroid != nullptr) {
		//Add to Asteroid List
		s_apoAsteroidList.push_back(poNewAsteroid);
	}

	//Add to asteroid count
	m_iAsteroidCount++;

}

/// <summary>
/// Breaks up an Asteroid in to multiple other asteroids
/// </summary>
void const AsteroidManager::BreakupAsteroid(Asteroid* a_poAsteroidToBreakUp) {


	//Null Check
	if (a_poAsteroidToBreakUp != nullptr) {
		//Find this asteroid in the asteroid vector

		int iAsteroidToDestoyVectorIndex = NULL;

		//Pointer for the asteroid that we are at in the list
		Asteroid* poAsteroidToCheck;

		for (int i = 0; i < s_apoAsteroidList.size(); i++)
		{

			poAsteroidToCheck = s_apoAsteroidList[i];

			if (poAsteroidToCheck == a_poAsteroidToBreakUp) {
				//We have found the asteroid, record its postion in the vector
				iAsteroidToDestoyVectorIndex = i;
				break; //Break out of loop
			}
		}

		//Check if the asteroid is too big enough to break up
		if ((a_poAsteroidToBreakUp->GetRadius() * mc_iAsteroidBreakUpSizeMultiplyer) > mc_iMinAsteroidSize) {

			//Determine Create Size
			int iCreateSize = (int)round(a_poAsteroidToBreakUp->GetRadius() * mc_iAsteroidBreakUpSizeMultiplyer);

			//Create 2 New Asteroids with half the radius of the one we just broke up
			for (int i = 0; i < mc_iAsteroidBreakUpCreateAmount; i++) {
				
				//Create Asteroid
				CreateAsteroid((int)a_poAsteroidToBreakUp->GetPostion()->fXPos,
					(int)a_poAsteroidToBreakUp->GetPostion()->fYPos, (int)iCreateSize);

   			}
		}

		//Increase Score
		if (m_poMyGameManager != nullptr) {
			m_poMyGameManager->AddScore(a_poAsteroidToBreakUp->GetRadius() * m_iCurrentAsteroidWave);
		}


		DestroyAsteroid(a_poAsteroidToBreakUp, iAsteroidToDestoyVectorIndex);


	}

}

/// <summary>
/// Destroys an asteroid and removes it from the asteroid vector
/// </summary>
void const AsteroidManager::DestroyAsteroid(Asteroid* a_poAsteroidToDestroy, int a_iVectorIndex) {
	
	//Remove Asteroid from vector
	s_apoAsteroidList.erase(s_apoAsteroidList.begin() + a_iVectorIndex);

	//Decrease Asteroid Count
	m_iAsteroidCount--;

	//Destroy Bullet itself
	delete a_poAsteroidToDestroy;
}

/// <summary>
/// Creates a new wave of asteroids, increases the asteroid wave
/// </summary>
void const AsteroidManager::NewAsteroidWave() {

	//Increase wave count
	m_iCurrentAsteroidWave++;

	//Create Number of Asteroids based on the current wave
	//This is the base asteroid count + the base count * (the current wave * the spawn amount modifier)
	int iNumberAsteroidsToCreate = (int)(mc_iBaseSpawnAsteroidCount + round(mc_iBaseSpawnAsteroidCount * (m_iCurrentAsteroidWave * m_fAsteroidWaveSpawnModifier)));

  	SpawnAsteroids(iNumberAsteroidsToCreate);
}

/// <summary>
/// Spawns a given number of asteroids
/// </summary>
void const AsteroidManager::SpawnAsteroids(int a_iNumberToCreate) {

	//Loop through as many times as required to spawn new asteroids
	for (int i = 0; i < a_iNumberToCreate; i++) {
		CreateAsteroid();
	}

}

///<summary>
/// Resets the asteroid manager
///</summary>
void AsteroidManager::Reset() {

	//Delete all current asteroids
	for each(Asteroid* poAsteroid in s_apoAsteroidList) {
		if (poAsteroid != nullptr) {
			//Destroy Asteroid
			delete poAsteroid;
		}
	}

	//Clear Vector
	s_apoAsteroidList.clear();

	//Reset Asteroid Wave Count
	m_iCurrentAsteroidWave = 0;

	//Set Asteroid Count to 0
	m_iAsteroidCount = 0;

}

///<summary>
///Gets the asteroid list and returns it
///</summary>
std::vector<Asteroid*> AsteroidManager::GetAsteroidList() {

	return s_apoAsteroidList;

}

///<summary>
/// Gets the current asteroid wave
///</summary>
int AsteroidManager::GetAsteroidWave() {

	return m_iCurrentAsteroidWave;

}

///<summary>
/// Gets the current asteroid count
///</summary>
int AsteroidManager::GetAsteroidCount() {

	return m_iAsteroidCount;

}
