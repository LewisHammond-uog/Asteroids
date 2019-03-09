////////////////////////////////////////////////////////////
// File: Asteroid.cpp
// Author: Lewis Hammond
// Brief: File for doing the functions of asteroids within the game
////////////////////////////////////////////////////////////

#include "Asteroid.h"

//UG Includes
#include "UGFW.h"
#include "UG_Defines.h"

//C Includes
#include <cmath>

//Project Includes
#include "SpaceObject.h"
#include "AsteroidManager.h"


Asteroid::Asteroid(float a_fStartX, float a_fStartY, int a_iStartSize, 
					AsteroidManager* a_poAsteroidManager)
{
	//Create Sprite at Start Size
	m_iSize = a_iStartSize;
	m_iSpriteID = UG::CreateSprite("./images/game_sprites/asteroid_1.png", (float)m_iSize, (float)m_iSize, true);

	//Set Sprite position
	m_oCurrentPostion.fXPos = a_fStartX;
	m_oCurrentPostion.fYPos = a_fStartY;
	UG::SetSpritePosition(m_iSpriteID, m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos);

	//Set Asteroid Manager
	m_poMyAsteroidManager = a_poAsteroidManager;

	//Pick a random direction
	m_fDirection = (float)(rand() % 360);

	//Pick a random sprite rotation direction
	float fRotationDirectionPick = (float)(rand() % 2);
	if (fRotationDirectionPick == 0) {
		m_eRotationDirection = ROTATION_DIRECTION_CLOCKWISE;
	}
	else {
		m_eRotationDirection = ROTATION_DIRECTION_ANTICLOCKWISE;
	}

	//Random Speed
	m_oCurrentSpeed.fTotalSpeed = (float)(mc_fMinSpeed + (rand() % (mc_fMaxSpeed - mc_fMinSpeed)));

}

Asteroid::~Asteroid()
{
}

/// <summary>
/// Updates the postion of the asteroid and renders it
/// </summary>
void const Asteroid::UpdateAsteroid() {

	//Update Postion based on speed and direction
	DoMovement(m_fDirection, m_oCurrentSpeed.fTotalSpeed, m_iSpriteID, m_oCurrentPostion, true);
	UG::SetSpritePosition(m_iSpriteID, m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos);
	DoRotation(m_eRotationDirection);

}

/// <summary>
/// Updates the rotation of the asteroid based on its rotation direction
/// </summary>
void const Asteroid::DoRotation(const ROTATION_DIRECTIONS a_direction) {

	if (a_direction == ROTATION_DIRECTION_CLOCKWISE) {
		m_fSpriteRotation += (mc_fRotationSpeed * UG::GetDeltaTime());
	}
	else {
		m_fSpriteRotation -= (mc_fRotationSpeed * UG::GetDeltaTime());
	}

	UG::SetSpriteRotation(m_iSpriteID, m_fSpriteRotation);

}

/// <summary>
/// Returns the radius of this object for collsions
/// </summary>
int const Asteroid::GetRadius() {

	return m_iSize / 2;

}

/// <summary>
/// Returns the a pointer to the postion of this object
/// </summary>
SpaceObject::Postion* Asteroid::GetPostion() {

	return &m_oCurrentPostion;

}

/// <summary>
/// Renders the asteroid
/// </summary>
void const Asteroid::DrawAsteroid() {

	UG::DrawSprite(m_iSpriteID);

}

/// <summary>
/// Lets the asteroid manager know that we want to break up this asteroid
/// </summary>
void const Asteroid::TriggerAsteroidBreakup() {

	m_poMyAsteroidManager->BreakupAsteroid(this);

}