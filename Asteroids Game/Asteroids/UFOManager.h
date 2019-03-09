#ifndef __UFOMANAGER_H__
#define __UFOMANAGER_H__

//C Includes
#include <vector>


//Predefines
class AsteroidManager;
class PlayerShip;
class BulletManager;
class UFO;

//Class to manage UFOs
class UFOManager
{
public:
	UFOManager(AsteroidManager* a_poAsteroidManager, PlayerShip* a_poPlayerShip, BulletManager* a_poBulletManager);
	~UFOManager();

	void UpdateUFOS();
	void const DestoryUFO(UFO* a_poUFOToDestroy);

private:

	//Enum for side of the screen to start from
	enum SCREEN_BOUND {
		SCREEN_BOUND_TOP,
		SCREEN_BOUND_RIGHT,
		SCREEN_BOUND_BOTTOM,
		SCREEN_BOUND_LEFT,

		SCREEN_BOUND_COUNT
	};

	//Spawning Functions
	void SpawnUFO();
	bool UFOSpawnCheck();

	//Associated Game Objects
	AsteroidManager* m_poAsteroidManager;
	PlayerShip* m_poPlayerShip;
	BulletManager* m_poMyBulletManager;

	int m_iCurrentUFOWave = 0;

	const int mc_iAsteroidCountToSpawnUFOS = 4;

	//Asteroid Vars
	int m_iCurrentAsteroidWave = 0;
	int m_iCurrentAsteroidCount = 0;

	//Current Active UFO
	std::vector<UFO*> m_apoUFOList;


};

#endif