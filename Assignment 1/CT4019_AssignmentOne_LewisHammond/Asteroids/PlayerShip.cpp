////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Brief: File for functions for the playership within the game
////////////////////////////////////////////////////////////
#include "PlayerShip.h"

//UG Includes
#include "UGFW.h"
#include "UG_Defines.h"

//Project Includes
#include "Bullet.h"
#include "BulletManager.h"
#include "Asteroid.h"
#include "AsteroidManager.h"
#include "SpaceObject.h"
#include "Game.h"


//Constructor
PlayerShip::PlayerShip(BulletManager* a_poMyBulletManager, Game* a_poGameManager)
{

	//Set our sprite
	m_iSpriteID = UG::CreateSprite("./images/game_sprites/playerShip.png", mc_fSpriteWidth, mc_fSpriteHeight, true);

	//Set Size
	m_fSize = fmax(mc_fSpriteWidth, mc_fSpriteHeight);

	//Set Starting Position
	m_oCurrentPostion.fXPos = 1024/2;
	m_oCurrentPostion.fYPos = 768/2;

	//Link our Bullet BulletManager
	m_poMyBulletManager = a_poMyBulletManager;
	m_poGameManager = a_poGameManager;

}


//Detructor
PlayerShip::~PlayerShip()
{
	UG::DestroySprite(m_iSpriteID);
}

/// <summary>
/// Runs functions to Update Player Ship Postion and Status
/// </summary>
void const PlayerShip::UpdateShip() {

	DoMovement();
	DoShooting();

	//If we have collided with something then decrease the number of lives and reset playership
	if (CollisionCheck() == true) {
		//Null Check
		if (m_poGameManager != nullptr) {
			m_poGameManager->RemoveLife();
			m_poGameManager->ResetGame();
		}
	}

}

/// <summary>
/// Draw PlayerShip Sprite
/// </summary>
void const PlayerShip::DrawShip() {

	UG::DrawSprite(m_iSpriteID);

}

#pragma region Ship Movement

/// <summary>
/// Sets the position of the playership. Wraps them around the string confines
/// </summary>
/// <param name="a_fNewXPostion">X Postion to Set</param>
/// <param name="a_fNewXPostion">Y Postion to Set</param>
void const PlayerShip::SetPostion(float const a_fNewXPostion, float const a_fNewYPostion) {

	//Update postion
	m_oCurrentPostion.fXPos = a_fNewXPostion;
	m_oCurrentPostion.fYPos = a_fNewYPostion;

	//Wrap Cordiates so that the player is always on screen
	WrapCoordinates(m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos);

	//Set Sprite Postion
	UG::SetSpritePosition(m_iSpriteID, m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos);

}


/// <summary>
/// Sets the Direction of the Sprite and then updates the velocity Direction
/// </summary>
/// <param name="a_iSpriteID">Sprite ID to set</param>
void PlayerShip::SetSpriteDirection(float a_fNewDirection) {

	//Always use Direction between 0 and 360
	if (a_fNewDirection > 360 || a_fNewDirection < 0) {
		a_fNewDirection = (float)fmod(a_fNewDirection,360);
	}

	//Update Direction Var
	m_fDirection = a_fNewDirection;

	//Set Sprite Direction
	UG::SetSpriteRotation(m_iSpriteID, a_fNewDirection);

}


/// <summary>
/// Triggers Functions to do Direction and when acceleration of the player ship
/// </summary>
void const PlayerShip::DoMovement() {

	DoDirection();
	DoAcceleration();

	//Check to trigger "hyperspace", check that the key was not held down last frame
	//So that we don't keep hyperpspacing
	if (UG::WasKeyPressed(m_eHyperspaceKey)) {
		DoHyperspace();
	}

}


/// <summary>
/// Checks for player input and then calculates the acceleration to apply to the x and y directions of the ship
/// </summary>
void const PlayerShip::DoAcceleration() {

	if (UG::IsKeyDown(m_eAccelerateKey) && m_oCurrentSpeed.fTotalSpeed < mc_fMaxSpeed) {
		//Apply Acceleration
		m_oCurrentSpeed.fTotalSpeed += (mc_fAccelerationForce * UG::GetDeltaTime());

		CalculateSpeedVectors(m_oCurrentSpeed.fTotalSpeed, m_fDirection, m_oCurrentSpeed.fXSpeed, m_oCurrentSpeed.fYSpeed);


	}
	else if(m_oCurrentSpeed.fTotalSpeed > 0){
		//Apply Friction
		m_oCurrentSpeed.fTotalSpeed -= (mc_fFrictionForce * UG::GetDeltaTime());
	}

	//Call Function to update position
	SetPostion(m_oCurrentPostion.fXPos + m_oCurrentSpeed.fXSpeed, m_oCurrentPostion.fYPos + m_oCurrentSpeed.fYSpeed);

}

/// <summary>
/// Checks for player input and then calculates the direction change to apply to the ship
/// </summary>
void const PlayerShip::DoDirection() {

	if (UG::IsKeyDown(m_eRotateLeftKey)) {
		SetSpriteDirection(m_fDirection + (mc_fDirectionForce * UG::GetDeltaTime()));
	}

	if (UG::IsKeyDown(m_eRotateRightKey)) {
		SetSpriteDirection(m_fDirection - (mc_fDirectionForce * UG::GetDeltaTime()));
	}

}

/// <summary>
/// Picks a random location on the screen and teleports the ship there.
/// </summary>
void const PlayerShip::DoHyperspace() {

	//Get Screen Confines
	int iScreenWidth = 0;
	int iScreenHeight = 0;
	UG::GetScreenSize(iScreenWidth, iScreenHeight);

	//Pick a random place on the screen
	int iHyperspaceLocationX = rand() % iScreenWidth;
	int iHyperspaceLocationY = rand() % iScreenHeight;

	//Set the postion
	SetPostion((const float)iHyperspaceLocationX, (const float)iHyperspaceLocationY);

}

#pragma endregion

/// <summary>
/// Checks if correct key has been pressed if it has then shoot a bullet 
/// </summary>
void const PlayerShip::DoShooting() {
	//Has Key been pressed?
	if (UG::WasKeyPressed(UG::KEY_SPACE)) {
		//Create a new bullet
		CreateBullet(m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos, m_fDirection , m_poMyBulletManager, false);
	}
}

/// <summary>
/// Does collision checks to see if the ship has collided with anything and then acts on that 
/// </summary>
bool const PlayerShip::CollisionCheck() {

	//Check for collsion with Asteroid
	for each (Asteroid* poAsteroid in AsteroidManager::GetAsteroidList())
	{
		//Null Check
		if (poAsteroid != nullptr) {
			if (RadiusCollisionCheck(poAsteroid->GetPostion(), &m_oCurrentPostion, (float)poAsteroid->GetRadius(), mc_fSpriteWidth)) {
				return true;
			}
		}
	}

	//Check for Collision with Bullet
	for each (Bullet* poBullet in BulletManager::GetBulletList())
	{
		//Null Check
		if (poBullet != nullptr) {
			//Check bullet was not created by us
			if (poBullet->GetCreatedByUFO() == true) {
				if (RadiusCollisionCheck(poBullet->GetPostion(), &m_oCurrentPostion, (float)poBullet->GetRadius(), mc_fSpriteWidth)) {
					return true;
				}
			}
		}
	}

	return false;

}

/// <summary>
/// Returns the current postion of the playership
/// </summary>
void PlayerShip::GetPostion(Postion& a_oPostion) {

	//Assign Postion
	a_oPostion = m_oCurrentPostion;

}

/// <summary>
/// Returns the current postion of the playership
/// </summary>
void PlayerShip::GetPostion(float &a_fXPos, float &a_fYPos) {

	//Set postion vars
	a_fXPos = m_oCurrentPostion.fXPos;
	a_fYPos = m_oCurrentPostion.fYPos;

}

/// <summary>
/// Returns the radius of the playership
/// </summary>
float PlayerShip::GetRadius() {

	return m_fSize / 2;

}

