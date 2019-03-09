#ifndef __ASTEROID_H__
#define __ASTEROID_H__

//Project Includes (base class)
#include "SpaceObject.h"

//Predefines
class AsteroidManager;

//Class to reperesent asteroids
class Asteroid : public SpaceObject
{
public:
	Asteroid(float a_fStartX, float a_fStartY, int a_iStartSize, AsteroidManager* a_poMyAsteroidManager);
	~Asteroid();

	//Draw/Update Functions
	void const UpdateAsteroid();
	void const DrawAsteroid();

	//Getter Functions
	int const GetRadius();
	Postion* GetPostion();

	//Trigger Asteroid Breakup
	void const TriggerAsteroidBreakup();

private:
	//Random Rotation Direction enum
	enum ROTATION_DIRECTIONS
	{
		ROTATION_DIRECTION_CLOCKWISE,
		ROTATION_DIRECTION_ANTICLOCKWISE,

		ROTATION_DIRECTION_COUNT,
	};


	//Sprite Vars
	int m_iSpriteID;
	int m_iSize = 0;

	//Rotation Functions and Vars
	void const DoRotation(const ROTATION_DIRECTIONS a_direction);
	float m_fSpriteRotation = 0.0f;
	float const mc_fRotationSpeed = 20.0f;
	ROTATION_DIRECTIONS m_eRotationDirection;

	//Speed Functions
	int const mc_fMaxSpeed = 100;
	int const  mc_fMinSpeed = 50;

	//Manager
	AsteroidManager* m_poMyAsteroidManager;


};

#endif // !__ASTEROID_H__
