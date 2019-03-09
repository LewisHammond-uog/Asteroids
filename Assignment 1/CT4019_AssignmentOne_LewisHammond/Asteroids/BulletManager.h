#ifndef __BULLETMANAGER_H__
#define __BULLETMANAGER_H__

//C includes
#include<vector>

//Pre defines
class Bullet;

//Class to manage bullets 
class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	//Update Function
	void UpdateBullets();
	
	//Bullet List Functions
	void AddNewBullet(Bullet* a_poBulletToAdd);
	static std::vector<Bullet*> GetBulletList();

private:
	//List for Bullets
	static std::vector<Bullet*> s_apoBulletList;

	//Destroy Bullet
	void DestroyBullet(Bullet* a_poBulletToDestroy, int a_iVectorIndex);
};

#endif // !__BULLETMANAGER_H__