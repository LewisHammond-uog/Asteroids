////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Lewis Hammond
// Brief: File for functions for bullet management within the game
////////////////////////////////////////////////////////////

#include "BulletManager.h"

//C includes
#include<list>

//Project Includes
#include "Bullet.h"

//Define of static vector
std::vector<Bullet*> BulletManager::s_apoBulletList;

BulletManager::BulletManager()
{

}


BulletManager::~BulletManager()
{
	//Delete all bullets that are managed by this manager
	for each(Bullet* poBullet in s_apoBulletList) {
		if (poBullet != nullptr) {
			//Delete bullet
			delete poBullet;
		}
	}

	//Clear Vector
	s_apoBulletList.clear();
}

///<summary>
///Runs update function on all bullets in the bullet list
///</summary>
void BulletManager::UpdateBullets() {

	//Pointer for current bullet
	Bullet* poBullet;

	//Go through all the bullets in the vector
	for(int i = 0; i < s_apoBulletList.size(); i++)
	{
		poBullet = s_apoBulletList[i];

		//Check that bullet is not a null pointer
		if (poBullet != nullptr) {

			//Run the bullet update and Draw functions
			poBullet->UpdateBullet();
			poBullet->DrawBullet();

			//Check to see if bullet is offscreen
			if (poBullet->IsOffScreen()) {
				//Destroy this bullet
				DestroyBullet(poBullet, i);
			}
		}

	}


}

///<summary>
///Adds a bullet to the bullet list
///</summary>
void BulletManager::AddNewBullet(Bullet* a_poBulletToAdd) {
	//Check that we are not trying to add a null pointer
	if (a_poBulletToAdd != nullptr) {
		//Add to list
		s_apoBulletList.push_back(a_poBulletToAdd);
	}

}

///<summary>
///Destroys a given bullet and removes it from the bullet list
///</summary>
void BulletManager::DestroyBullet(Bullet* a_poBulletToDestroy, int a_iVectorIndex) {

	//Remove Bullet from vector
	s_apoBulletList.erase(s_apoBulletList.begin() + a_iVectorIndex);

	//Destroy Bullet itself
	delete a_poBulletToDestroy;

}

///<summary>
///Gets the bullet list and returns it
///</summary>
std::vector<Bullet*> BulletManager::GetBulletList() {

	return s_apoBulletList;

}