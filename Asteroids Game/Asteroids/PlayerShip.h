#ifndef __PLAYERSHIP_H__
#define __PLAYERSHIP_H__

//UG Includes
#include "UGFW.h"
#include "UG_Defines.h"


//Prject Includes
#include "SpaceObject.h"

//Pre Defines
class BulletManager;
class Game;

//Class for the player controlled ship
class PlayerShip : private SpaceObject
{
public:
	PlayerShip(BulletManager* a_myBulletManager, Game* a_poGameManager);
	~PlayerShip();

	void const SetPostion(const float a_iNewXPostion, const float a_iNewYPostion);
	void const UpdateShip();
	void const DrawShip();
	void GetPostion(Postion& a_oPostion);
	void GetPostion(float &a_fXPos, float &a_fYPos);
	float GetRadius();

private:

	//Object Sprite and sizes
	int m_iSpriteID;
	const float mc_fSpriteWidth = 40.0f;
	const float mc_fSpriteHeight = 40.0f;
	float m_fSize;
				 
	//Force Constatnts
	const float mc_fDirectionForce = 200.0f;
	const float mc_fFrictionForce = 0.05f;
	const float mc_fAccelerationForce = 3.0f;
	const float mc_fRotationCorrectionForce = 20.0f;
	const float mc_fMaxSpeed = 3.0f;

	BulletManager* m_poMyBulletManager;
	Game* m_poGameManager;

	//Internal Logic Functions
	void const DoMovement();
	void const DoShooting();
	void const DoDirection();
	void const DoAcceleration();
	void const DoHyperspace();
	bool const CollisionCheck();

	//Key Binds
	UG::Key m_eAccelerateKey = UG::KEY_W;
	UG::Key m_eRotateLeftKey = UG::KEY_A;
	UG::Key m_eRotateRightKey = UG::KEY_D;
	UG::Key m_eHyperspaceKey = UG::KEY_H;
	bool m_bHyperspaceKeyHeld = false;

	void SetSpriteDirection(float a_iNewDirection);


};

#endif // !__PLAYERSHIP_H__