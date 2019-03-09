////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Brief: File for functions of bullets within the game 
////////////////////////////////////////////////////////////

#include "Bullet.h"

//UG Includes
#include "UGFW.h"
#include "UG_Defines.h"

//Project Includes
#include "Asteroid.h"
#include "AsteroidManager.h"
#include "SpaceObject.h"

Bullet::Bullet(float a_fstartX, float a_fStartY, float a_fStartDirection, bool a_bCreatedByUFO)
{
	//Create Sprite
	m_iSpriteID = UG::CreateSprite("./images/game_sprites/bullet.png", m_iSpriteWidth, m_iSpriteHeight, true);

	//Set Direction
	m_fDirection = a_fStartDirection;
	UG::SetSpriteRotation(m_iSpriteID,m_fDirection);

	//Set start postion
	m_oCurrentPostion.fXPos = a_fstartX;
	m_oCurrentPostion.fYPos = a_fStartY;
	UG::SetSpritePosition(m_iSpriteID ,m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos);

	//Set Radius
	m_iSize = (int)(fmax(m_iSpriteWidth, m_iSpriteHeight));

	//Set Creation Instance
	m_bCreatedByUFO = a_bCreatedByUFO;


}


Bullet::~Bullet()
{
	//Destroy our sprite
	UG::DestroySprite(m_iSpriteID);
}

/// <summary>
/// Update the postion and status of the bullet
/// </summary>
void Bullet::UpdateBullet() {

	DoMovement(m_fDirection, mc_fBulletSpeed, m_iSpriteID, m_oCurrentPostion, false);
	UG::SetSpritePosition(m_iSpriteID, m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos);
	CollsionCheck();

}

/// <summary>
/// Draw the current bullet sprite
/// </summary>
void const Bullet::DrawBullet() {

	UG::DrawSprite(m_iSpriteID);
	
}

/// <summary>
/// Performs collision check to see if this bullet has collided with anything, then acts on that 
/// </summary>
void const Bullet::CollsionCheck() {

	//Check if we have collided with any asteroids
	for each (Asteroid* poAsteroid in AsteroidManager::GetAsteroidList())
	{
		//Null Check
		if (poAsteroid != nullptr) {

			//Get Postion and Radius of Asteroid
			Postion* sAsteroidPostion = poAsteroid->GetPostion();
			float sAsteroidRadius = (float)poAsteroid->GetRadius();

			if (RadiusCollisionCheck(sAsteroidPostion, &m_oCurrentPostion, sAsteroidRadius, (float)GetRadius())) {

				//Trigger Asteroid to Breakup
				poAsteroid->TriggerAsteroidBreakup();

				//Destroy Bullet by moving it off screen so bullet manager cleans it up
				m_oCurrentPostion = { 1000,1000 };

			}
		}
	}

}

/// <summary>
/// Returns the a pointer to the postion of this object
/// </summary>
SpaceObject::Postion* const Bullet::GetPostion() {

	return &m_oCurrentPostion;

}

/// <summary>
/// Returns the radius of this object for collsions
/// </summary>
int const Bullet::GetRadius() {

	return m_iSize / 2;

}

/// <summary>
/// Returns the creation instance of this bullet
/// </summary>
bool Bullet::GetCreatedByUFO() {

	return m_bCreatedByUFO;

}




/// <summary>
/// Checks if the bullet is off screen, returns bool
/// </summary>
bool const Bullet::IsOffScreen() {
	int iScreenWidth;
	int iScreenHeight;

	UG::GetScreenSize(iScreenWidth, iScreenHeight);

	if (m_oCurrentPostion.fXPos < 0 || m_oCurrentPostion.fXPos > iScreenWidth) {
		return true;
	}
	else if (m_oCurrentPostion.fYPos < 0 || m_oCurrentPostion.fYPos > iScreenHeight) {
		return true;
	}
	else {
		return false;
	}
}

