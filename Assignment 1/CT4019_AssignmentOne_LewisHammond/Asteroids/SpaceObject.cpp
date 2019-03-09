////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Brief: File for the parent class of all objects that exists within world space
////////////////////////////////////////////////////////////
#include "SpaceObject.h"

//C Includes
#include <cmath>

//UG Includes
#include "UGFW.h"

//Project Includes
#include "Bullet.h"
#include "BulletManager.h"

//Defines
#define PI 3.14159265

SpaceObject::SpaceObject() {

}

SpaceObject::~SpaceObject()
{
}

/// <summary>
/// Takes in coordinates by refrences and wraps them so that they are always on the screen
/// </summary>
void SpaceObject::WrapCoordinates(float &a_rfXPos, float &a_rfYPos) {

	int iScreenWidth;
	int iScreenHeight;

	UG::GetScreenSize(iScreenWidth, iScreenHeight);

	//Wrap the cordinates around the edges of the screen
	if (a_rfXPos < 0) {
		a_rfXPos += (float)iScreenWidth;
	}

	if (a_rfXPos >= iScreenWidth) {
		a_rfXPos -= (float)iScreenWidth;
	}
	
	if (a_rfYPos < 0) {
		a_rfYPos += (float)iScreenHeight;
	}

	if (a_rfYPos > iScreenHeight) {
		a_rfYPos -= (float)iScreenHeight;
	}

}

/// <summary>
/// Takes in speed and direction then calculates X and Y Components of speed then 
/// modifies the X Speed and Y Speed vars 
/// </summary>
void SpaceObject::CalculateSpeedVectors(float a_fSpeed, float a_fDirection, 
	float &a_rfXSpeedVar, float &a_rfYSpeedVar) {

	//Calculate Speed in x and y directions
	a_rfXSpeedVar = a_fSpeed * (cos(a_fDirection*PI / 180));
	a_rfYSpeedVar = a_fSpeed * (sin(a_fDirection*PI / 180));

}

/// <summary>
/// Update the X and Y Pos of an object based on speed and direction.
/// </summary>
void SpaceObject::DoMovement(float a_fDirection, float a_fSpeed, int a_iSpriteID, 
							Postion& a_sPostion, bool a_bWrapCords) {
	
	//Store vars for X and Y Speed
	float fXSpeed = 0;
	float fYSpeed = 0;

	//Apply Speed in vertial and horizontal direction
	CalculateSpeedVectors(a_fSpeed, a_fDirection, fXSpeed, fYSpeed);

	//Update Postion
	a_sPostion.fXPos += fXSpeed * UG::GetDeltaTime();
	a_sPostion.fYPos += fYSpeed * UG::GetDeltaTime();

	//If wrap cords then wrap cords
	if (a_bWrapCords) {
		WrapCoordinates(a_sPostion.fXPos, a_sPostion.fYPos);
	}

}


/// <summary>
/// Gets the distance between two points
/// </summary>
float SpaceObject::GetDistance(Postion& a_sPostionA, Postion& a_sPositionB) {

	//Get Y and X Distance
	float fYDistance = pow((a_sPositionB.fXPos - a_sPostionA.fXPos),2);
	float fXDistance = pow((a_sPositionB.fYPos - a_sPostionA.fYPos),2);

	float fTotalDistance = sqrt(fXDistance + fYDistance);

	return fTotalDistance;

}

/// <summary>
/// Checks if two objects are inside each other and thus have collided 
/// </summary>
bool SpaceObject::RadiusCollisionCheck(Postion* a_sObjectAPostion, Postion* a_sObjectBPostion, 
									float a_fObjectARadius, float a_fObjectBRadius) {


	float fDistanceBetweenObjects = GetDistance(*a_sObjectAPostion, *a_sObjectBPostion);
	float fSumOfRadius = a_fObjectARadius + a_fObjectBRadius;

	//If the sum of the two radiuses are less than the distance between the objects,
	//then they have collided
	if (fDistanceBetweenObjects < fSumOfRadius) {
  		return true;
	}

	//No Collsion
  	return false;
}

/// <summary>
/// Creates a bullet 
/// </summary>
void const SpaceObject::CreateBullet(const float a_fCreatePosX, const float a_fCreatePosY, const float a_fCreateDirection, 
	BulletManager* a_poBulletManager, bool a_bCreatedByUFO) {

	//Instaniate an instance of the bullet class with our current postion, and direction.
	Bullet* poNewBullet = new Bullet(a_fCreatePosX, a_fCreatePosY, a_fCreateDirection, a_bCreatedByUFO);

	//Check that new bullet is not a null pointer
	if (poNewBullet != nullptr) {
		//Add to bullet manager list of bullets
		a_poBulletManager->AddNewBullet(poNewBullet);
	}

}