#ifndef __SPACEOBJECT_H__
#define __SPACEOBJECT_H__

//Predefines
class BulletManager;

//Class for any space object in world space to inherit from
class SpaceObject
{
public:
	SpaceObject();
	~SpaceObject();


protected:
	///<summary>
	///Struct for storing the X and Y Cordinates of any object
	///</summary>
	struct Postion {
		float fXPos = 0.0f;
		float fYPos = 0.0f;
	};

	///<summary>
	///Struct for Speed (Overall, XSpeed, YSpeed)
	///</summary>
	struct Speed {
		float fTotalSpeed = 0.0f;
		float fXSpeed = 0.0f;
		float fYSpeed = 0.0f;
	};

	//Space Object Logic Functions
	void WrapCoordinates(float &a_rfXPos, float &a_rfYPos);
	void CalculateSpeedVectors(float a_fSpeed, float a_fDirection, float &a_rfXSpeedVar, float &a_rfYSpeedVar);
	void DoMovement(float a_fDirection, float a_fSpeed, int a_iSpriteID, Postion& a_sPostion, bool a_bWrapCords);
	bool RadiusCollisionCheck(Postion* a_sObjectAPostion, Postion* a_sObjectBPostion, float a_fObjectARadius, float a_fObjectBRadius);
	void const CreateBullet(const float a_fCreatePosX, const float a_fCreatePosY, const float a_fCreateDirection,BulletManager* a_poBulletManager, bool a_bCreatedByUFO);

	//Postion and Speed Vars/Structs for Space Objects
	Postion m_oCurrentPostion;
	Speed m_oCurrentSpeed;
	float m_fDirection = 0;

private:
	float GetDistance(Postion& a_sPostionA, Postion& a_sPositionB);

};

#endif // !__SPACEOBJECT_H__