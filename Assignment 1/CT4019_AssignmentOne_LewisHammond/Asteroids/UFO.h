#ifndef __UFO_H__
#define __UFO_H_

//Project Includes
#include "SpaceObject.h"

//Predefines
class PlayerShip;
class BulletManager;
class UFOManager;

//Class to represent a UFO
class UFO : public SpaceObject
{
public:
	UFO(int a_iCreateX, int a_iCreateY, UFOManager* a_poMyUFOManager, PlayerShip* a_poPlayerShip, BulletManager* a_poBulletManager);
	~UFO();

	//Update and Draw Function
	void const Update();
	void const Draw();

private:

	//Internal Logic Functions
	void const DoShooting();
	void const MoveTowardsPlayer();
	bool const CollisionCheck();
	void const TriggerUFODestruction();

	//Associated Game Objects
	UFOManager* m_poMyUFOManager;
	PlayerShip* m_poPlayerShip;
	BulletManager* m_poMyBulletManager;

	//Sprite Variables
	int m_iSpriteID;
	const float mc_fSpriteWidth = 46.0f;
	const float mc_fSpriteHeight = 31.0f;

	//Speed of UFO
	float const mc_UFOSpeed = 20.0f;

	//Shooting Variable
	float const mc_fShotSpacing = 2.0f;
	float m_fShotTimer = 0.0f;

};

#endif // !__UFO_H__
