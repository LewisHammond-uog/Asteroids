////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Brief: File for functions for the UFO within the game
////////////////////////////////////////////////////////////

#include "UFO.h"

//Project Includes
#include "PlayerShip.h"
#include "UFOManager.h"
#include "BulletManager.h"
#include "Bullet.h"

#define PI 3.1415927;

UFO::UFO(int a_iCreateX, int a_iCreateY, UFOManager* a_poMyUFOManager, PlayerShip* a_poPlayerShip, BulletManager* a_poBulletManager)
{

	//Set sprite
	m_iSpriteID = UG::CreateSprite("./images/game_sprites/UFO.png", mc_fSpriteWidth, mc_fSpriteHeight, true);

	//Set Postion
	m_oCurrentPostion.fXPos = (float)a_iCreateX;
	m_oCurrentPostion.fYPos = (float)a_iCreateY;

	//Set Sprite Postion
	UG::SetSpritePosition(m_iSpriteID, m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos);

	//Set our PlayerShip and BulletManager
	m_poPlayerShip = a_poPlayerShip;
	m_poMyBulletManager = a_poBulletManager;
	m_poMyUFOManager = a_poMyUFOManager;

}


UFO::~UFO()
{

	//Destroy Sprite
	UG::DestroySprite(m_iSpriteID);

}

/// <summary>
/// Updates the postion and runs collision check on UFO
/// </summary>
void const UFO::Update() {
	
	MoveTowardsPlayer();
	DoShooting();

	//If our collision check returns true then destroy the UFO
	if (CollisionCheck()) {
		TriggerUFODestruction();
	}

}

/// <summary>
/// Draws the UFO
/// </summary>
void const UFO::Draw() {

	//Draw Sprite
	UG::DrawSprite(m_iSpriteID);

}


/// <summary>
/// Moves the UFO towards the players location
/// </summary>
void const UFO::MoveTowardsPlayer() {

	//Get Player Postion
	Postion oPlayerPostion;
	if (m_poPlayerShip != nullptr) {
		m_poPlayerShip->GetPostion(oPlayerPostion);
	}


	//Calculate Angle Difference between me and the player
	float fAngleDifference = atan2(m_oCurrentPostion.fYPos - oPlayerPostion.fYPos, m_oCurrentPostion.fXPos - oPlayerPostion.fXPos);
	m_fDirection = fAngleDifference * 180 / PI ; 
	m_fDirection -= 180;


	DoMovement(m_fDirection, mc_UFOSpeed, m_iSpriteID, m_oCurrentPostion, false);
	UG::SetSpritePosition(m_iSpriteID,m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos);
    

}


/// <summary>
/// Works out wheter we should shoot and then shoots a bullet towards the player if applicable
/// </summary>
void const UFO::DoShooting() {

	//Null Check Bullet Manager
	if (m_poMyBulletManager != nullptr) {

		//Work Out if we should shoot
		if (m_fShotTimer < 0) {

			//Create a bullet
			CreateBullet(m_oCurrentPostion.fXPos, m_oCurrentPostion.fYPos, m_fDirection, m_poMyBulletManager, true);

			//Reset Timer
			m_fShotTimer = mc_fShotSpacing;

		}
	}

	//Reduce the shot timer by delta time
	m_fShotTimer -= UG::GetDeltaTime();

}


/// <summary>
/// Checks if we are colliding with any object
/// </summary>
bool const UFO::CollisionCheck() {


	//Check for Collision with Bullet
	for each (Bullet* poBullet in BulletManager::GetBulletList())
	{
		//Null Check
		if (poBullet != nullptr) {
			//Check bullet was not created by us
			if (poBullet->GetCreatedByUFO() != true) {
				if (RadiusCollisionCheck(poBullet->GetPostion(), &m_oCurrentPostion, (float)poBullet->GetRadius(), mc_fSpriteWidth)) {
					return true;
				}
			}
		}
	}

	return false;

}

void const UFO::TriggerUFODestruction() {

	if (m_poMyUFOManager != nullptr) {
		//Runs function on UFO Manager
		m_poMyUFOManager->DestoryUFO(this);
	}

}