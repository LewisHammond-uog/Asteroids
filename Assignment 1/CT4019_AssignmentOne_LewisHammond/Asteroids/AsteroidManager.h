#ifndef __ASTEROIDMANAGER_H__
#define __ASTEROIDMANAGER_H__

//C includes
#include<vector>

//Predefines
class Game;
class Asteroid;
class PlayerShip;

//Class for managing asteroid
class AsteroidManager
{
public:
	AsteroidManager(Game* a_poGameManager, PlayerShip* a_poPlayerShip);
	~AsteroidManager();

	//Update and Draw Postions of asteroids
	void UpdateAsteroids();

	//Funct to get all asteroids
	static std::vector<Asteroid*> GetAsteroidList();
	int GetAsteroidWave();
	int GetAsteroidCount();

	void Reset();

	void const BreakupAsteroid(Asteroid* a_poAsteroidToBreakUp);

private:
	
	//Game Manager and Playership
	Game* m_poMyGameManager;
	PlayerShip* m_poPlayerShip;

	//Asteroid Spawning and Deletion
	void const CreateAsteroid();
	void const CreateAsteroid(int a_iCreateX, int a_iCreateY, int a_iCreateSize);
	void const NewAsteroidWave();
	void const SpawnAsteroids(int a_iNumberToCreate);
	void const DestroyAsteroid(Asteroid* a_poAsteroidToDestroy, int a_iVectorIndex);
	bool SpawningWithinPlayer(PlayerShip* a_poPlayerShip, int a_iAsteroidCreateX, int a_iAsteroidCreateY, int a_iAsteroidCreateSize);

	//Consts for asteroid counts and size
	const int mc_iMaxAsteroidSize = 128;
	const int mc_iMinAsteroidSize = 16;
	const int mc_iBaseSpawnAsteroidCount = 10;

	//Current Asteroid Wave
	int m_iCurrentAsteroidWave = 0;
	float m_fAsteroidWaveSpawnModifier = 0.2f;

	//Consts for creating new asteroids after break up
	const int mc_iAsteroidBreakUpCreateAmount = 2;
	const float mc_iAsteroidBreakUpSizeMultiplyer = 0.5; //Sets the % size of asteroids created after break up

	//Current asteroid count
	int m_iAsteroidCount = 0;

	//List for Asteroids
	static std::vector<Asteroid*> s_apoAsteroidList;
};

#endif // !__ASTEROIDMANAGER_H__