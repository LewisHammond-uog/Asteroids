#ifndef  __BULLET_H__
#define __BULLET_H__


//Project includes
#include "SpaceObject.h"


//Class to represent a bullet
class Bullet : public SpaceObject
{
public:
	Bullet(float a_fstartX, float a_fStartY, float a_fStartDirection, bool a_bCreatedByUFO);
	~Bullet();

	//Draw/Update Functions
	void UpdateBullet();
	void const DrawBullet();

	bool const IsOffScreen();

	//Getter Functions
	int const GetRadius();
	SpaceObject::Postion* const GetPostion();
	bool GetCreatedByUFO();

private:

	//Sprite Variables
	int m_iSpriteID;
	const float m_iSpriteWidth = 10.0f;
	const float m_iSpriteHeight = 4.0f;
	int m_iSize = 0;


	//Speed vars
	const float mc_fBulletSpeed = 500.0f;

	//Created By UFO
	bool m_bCreatedByUFO = false;

	//Collision Check
	void const CollsionCheck();

};

#endif //  __BULLET_H__