//Samuel MacSweeney
//K00200955

#ifndef AIDIRECTOR_H
#define AIDIRECTOR_H

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullets.h"
#include "HealthPickUp.h"
#include "AmmoPickUp.h"
#include "PathFinder.h"
#include "SightSprite.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <math.h>

USING_NS_CC;
using namespace std;

class AIDirector
{
	public:

		// Functions
		void HealthTimer(float dt);
		void AmmoTimer(float dt);
		void GameTimer(float dt);

		// Object Setters
		void SetEnemyLocation(Enemy *enemySprite, float positionX, float positionY);
		void SetHealthPickupLocation(HealthPickUp *healthPickupSprite, float positionX, float positionY);
		void SetAmmoLocation(AmmoPickUp *ammoSprite, float positionX, float positionY);

		// Object Spawners
		void SpawnNewAmmoPickup(Player *playerSprite, AmmoPickUp *ammoPickupSprite);
		void SpawnNewHealthPickup(Player *playerSprite, HealthPickUp *healthPickupSprite);
		
		// Object Collision Detection
		void PlayerEnemyCollision(Player *playerSprite, Enemy *enemySprite);
		void PlayerHealthCollision(Player *playerSprite, HealthPickUp *healthPickupSprite, bool healthTrigger);
		void PlayerAmmoCollision(Player *playerSprite, AmmoPickUp *ammoPickupSprite, bool ammoTrigger);
		void EnemyBulletCollision(Enemy *enemySprite, Bullets *bulletSprite, bool enemyVisibilityTrigger);

		// PathFinder
		void EnemyPathFinding(PathFinder *enemyPathFinding, TMXTiledMap *mapOne, Enemy *enemySprite, Player *playerSprite, TMXLayer *background, float dt);
		
		// Dynamic Scaling
		void EnemyDifficultyScale(Enemy *enemySprite, float dt, Player *playerSprite);
		void DynamicBoss(Enemy *enemySprite);
		
		// Object Re-Spawning 
		void EnemyRespawner(Enemy *enemySprite, float positionX, float positionY, bool enemyVisibilityTrigger);
		void HealthPickUpRespawn(HealthPickUp *healthPickupSprite, float positionX, float positionY, bool healthTrigger);
		void AmmoPickUpRespawn(AmmoPickUp *ammoPickupSprite, float positionX, float positionY, bool ammoTrigger);
		
		// Analyics 
		void PlayerDeathAnalytics(Player *playerSprite);
		void EnemyDeathAnalytics(Enemy *enemySprite);
		void ScoreAnalytics(Enemy *enemySprite, int score);

		// Variables
		enum difficulty { EASY, MEDIUM, HARD, DEATH, RESET };
		difficulty newDifficulty = difficulty::MEDIUM;

		ofstream myAnalytics;

		std::vector<Vec2>::iterator PathNodeIterator;
		std::vector<Vec2>Pathfinding;

		bool easyTrigger = false;
		bool mediumTrigger = true;
		bool hardTrigger = false;
		bool deathTrigger = false;
		bool aidHealthPU;
		bool aidAmmoPU;
		bool easyLock = false;
		bool mediumLock = false;
		bool hardLock = false;
		bool deathLock = false;
		bool initialLock = true;

		float healthTimer = 0;
		float ammoTimer = 0;
		float gameTimer = 0;
		float easy = 30;
		float easyTimer;
		float medium = 30;
		float mediumTimer;
		float hard = 30;
		float hardTimer;
		float death = 30;
		float deathTimer;
};

#endif