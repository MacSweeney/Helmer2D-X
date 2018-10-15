//Samuel MacSweeney
//K00200955

#include "AIDirector.h"

// Health Timer Function
void AIDirector::HealthTimer(float dt)
{
	healthTimer += dt;
};

// Ammo Timer Function
void AIDirector::AmmoTimer(float dt)
{
	ammoTimer += dt;
};

// Game Timer Function
void AIDirector::GameTimer(float dt)
{
	gameTimer += dt;
};

// Function For Setting the Enemy location on Map
void AIDirector::SetEnemyLocation(Enemy *enemySprite, float positionX, float positionY)
{
	//Grab Enemy Sprite use float X and Y to set Sprite Position
	enemySprite->GetEnemySprite()->setPosition(Vec2(positionX, positionY));
};

// Function for respawning Enemies
void AIDirector::EnemyRespawner(Enemy *enemySprite, float positionX, float positionY, bool enemyVisibilityTrigger)
{
	// Check if EnemySprite health is less or equal to 0
	if (enemySprite->GetEnemyHealth() <= 0)
	{
		// Set default health
		int defaultHealth = 40;

		// Set Position
		enemySprite->GetEnemySprite()->setPosition(Vec2(positionX, positionY));

		// Ensure sprite is visible
		enemySprite->GetEnemySprite()->setVisible(true);

		// Set Enemy Health to default health
		enemySprite->SetEnemyHealth(defaultHealth);

		// Reset visibility trigger
		enemyVisibilityTrigger = true;
	}
};

// Function for Respawning health packs
void AIDirector::HealthPickUpRespawn(HealthPickUp *healthPickupSprite, float positionX, float positionY, bool healthTrigger)
{
	// When Health Timer is exceeds 10
	if (healthTimer >= 10)
	{
		// Reset health timer
		healthTimer = 0;

		// Set sprite position
		healthPickupSprite->GetHealthPickUpSprite()->setPosition(Vec2(positionX, positionY));

		// Set visible to true
		healthPickupSprite->GetHealthPickUpSprite()->setVisible(true);

		// Set trigger to true
		healthTrigger = true;
	}

};

// Function for Respawning ammo packs
void AIDirector::AmmoPickUpRespawn(AmmoPickUp *ammoPickupSprite, float positionX, float positionY, bool ammoTrigger)
{
	// When Ammo Timer is exceeds 10 
	if (ammoTimer >= 10)
	{
		// Reset ammo timer
		ammoTimer = 0;

		// Set sprite position
		ammoPickupSprite->GetAmmoPickUpSprite()->setPosition(Vec2(positionX, positionY));

		// Set visible to true
		ammoPickupSprite->GetAmmoPickUpSprite()->setVisible(true);

		// Set trigger to true
		ammoTrigger = true;
	}

};

// Function for spawning Health Pick Ups
void AIDirector::SetHealthPickupLocation(HealthPickUp *healthPickupSprite, float positionX, float positionY)
{
	// Grab Health Pick Up Sprite use float X and Y to set Sprite Position
	healthPickupSprite->GetHealthPickUpSprite()->setPosition(Vec2(positionX, positionY));
};

// Function for Spawning emergency health pick up when player health is too low
void AIDirector::SpawnNewHealthPickup(Player *playerSprite, HealthPickUp *healthPickupSprite)
{
	// If player health is less than 20
	if (playerSprite->GetPlayerHealth() < 20)
	{
		// If A.I.D health pick up trigger is true
		if (aidHealthPU)
		{
			// Grab player position
			float playerX = playerSprite->GetPlayerSprite()->getPositionX();
			float playerY = playerSprite->GetPlayerSprite()->getPositionY();

			// Set pickup relative to the players position - set pick up to true - reset health trigger
			healthPickupSprite->GetHealthPickUpSprite()->setPosition(Vec2(playerX, playerY + 50));
			healthPickupSprite->GetHealthPickUpSprite()->setVisible(true);
			aidHealthPU = false;
		}
	}

	// If player health is above 20
	else if (playerSprite->GetPlayerHealth() > 20)
	{
		// Set trigger to true
		aidHealthPU = true;
	}
};

// Function for Spawning emergency ammo pick up when player ammo is too low
void AIDirector::SpawnNewAmmoPickup(Player *playerSprite, AmmoPickUp *ammoPickupSprite)
{
	// If player ammo is less than 5
	if (playerSprite->GetPlayerAmmo() < 5)
	{
		// If A.I.D ammo pick up trigger is true
		if (aidAmmoPU)
		{
			// Grab player position
			float playerX = playerSprite->GetPlayerSprite()->getPositionX();
			float playerY = playerSprite->GetPlayerSprite()->getPositionY();

			// Set pickup relative to the players position - set pick up to true - reset ammo trigger
			ammoPickupSprite->GetAmmoPickUpSprite()->setPosition(Vec2(playerX, playerY - 50));
			ammoPickupSprite->GetAmmoPickUpSprite()->setVisible(true);
			aidAmmoPU = false;
		}
	}

	// If player ammo is above 5
	else if (playerSprite->GetPlayerAmmo() > 5)
	{
		// Set trigger to true
		aidAmmoPU = true;
	}
};

// Function For Setting the Ammo location on Map
void AIDirector::SetAmmoLocation(AmmoPickUp *ammoSprite, float positionX, float positionY)
{
	// Grab Ammo Sprite use float X and Y to set Sprite Position
	ammoSprite->GetAmmoPickUpSprite()->setPosition(Vec2(positionX, positionY));
};

// Function For setting collision between player and Enemy Sprites using Bounding Boxes
void AIDirector::PlayerEnemyCollision(Player *playerSprite, Enemy *enemySprite)
{
	// Check for intersection between both the player and enemy bounding boxes
	if (playerSprite->GetPlayerSprite()->getBoundingBox().intersectsRect(enemySprite->GetEnemySprite()->getBoundingBox()))
	{
		// Set enemy Damage - is greater or less depending on enemy difficulty mode
		int enemyDamage = enemySprite->GetEnemyDamage();

		// Apply damage to player health
		int health = playerSprite->GetPlayerHealth() - enemyDamage;
		playerSprite->SetPlayerHealth(health);

		// Log out when collision between Player and Enemy Sprites and show player health
		cocos2d::log("Player - Enemy Collision :: Player Health = %d", health);
	}
};

// Function For setting collision between player and Health Pack Sprites using Bounding Boxes
void AIDirector::PlayerHealthCollision(Player *playerSprite, HealthPickUp *healthPickupSprite, bool healthTrigger)
{
	// Check for intersection between both the player and health bounding boxes
	if (playerSprite->GetPlayerSprite()->getBoundingBox().intersectsRect(healthPickupSprite->GetHealthPickUpSprite()->getBoundingBox()))
	{
		// Add 20 to players current health
		int health = playerSprite->GetPlayerHealth() + 20;

		// Set new player health
		playerSprite->SetPlayerHealth(health);

		// Grab health sprite and set visibilty to false
		healthPickupSprite->GetHealthPickUpSprite()->setVisible(false);

		// Reset health trigger
		healthTrigger = false;

		// Log out player collision with A.I.D health pick up - print out player health
		cocos2d::log("Player - A.I.D Health Pickup Collision :: Player A.I.D Health = %d", health);
	}
};

// Function For setting collision between player and Ammo Pack Sprites using Bounding Boxes
void AIDirector::PlayerAmmoCollision(Player *playerSprite, AmmoPickUp *ammoPickupSprite, bool ammoTrigger)
{
	// Check for intersection between both the player and ammo bounding boxes
	if (playerSprite->GetPlayerSprite()->getBoundingBox().intersectsRect(ammoPickupSprite->GetAmmoPickUpSprite()->getBoundingBox()))
	{
		// Add 20 to players current ammo
		int ammo = playerSprite->GetPlayerAmmo() + 20;

		// Set new player ammo
		playerSprite->SetPlayerAmmo(ammo);

		// Grab ammo sprite and set visibilty to false
		ammoPickupSprite->GetAmmoPickUpSprite()->setVisible(false);

		// Reset ammp trigger
		ammoTrigger = false;

		// Log out player collision with A.I.D ammo pick up - print out player ammo
		cocos2d::log("Player - Ammo Pickup Collision :: Player Ammo = %d", ammo);
	}
};

// Function For setting collision between enemy and bullet Sprites using Bounding Boxes
void AIDirector::EnemyBulletCollision(Enemy *enemySprite, Bullets *bulletSprite, bool enemyVisibilityTrigger)
{
	// Check for intersection between both the enemy and bullet bounding boxes
	if (enemySprite->GetEnemySprite()->getBoundingBox().intersectsRect(bulletSprite->GetBulletsSprite()->getBoundingBox()))
	{
		// Take one away from the enemy health
		int health = enemySprite->GetEnemyHealth() - 1;

		// Set new Enemy health
		enemySprite->SetEnemyHealth(health);
		
		// Check to ensure health is less then zero but greater than -1 
		// Correction for analytical data as enemy health is negative 100 - 8000 
		// Until player bullet collides with enemy sprite 
		/* Prevents printing to analytics if enemy health is less then 0*/
		if (health <= 0 && health > -1)
		{
			// Set Enemy sprite to false if health is 0
			enemySprite->GetEnemySprite()->setVisible(false);

			// Add enemy data to analytics
			EnemyDeathAnalytics(enemySprite);

			// Set enemy visibilty trigger to false
			enemyVisibilityTrigger = false;
		}

		// Log out Enemy collision with Bullet Sprite - print out enemy health
		cocos2d::log("Bullet - Collision :: Enemy %d", health);
	}
};

// Function for Setting Enemy PathFinding 
void AIDirector::EnemyPathFinding(PathFinder *enemyPathFinding, TMXTiledMap *mapOne, Enemy *enemySprite, Player *playerSprite, TMXLayer *background, float dt)
{ 
	// Create a new Pathfinding object - pass in current map and path type
	enemyPathFinding = new PathFinder(mapOne, PathFinder::pathType::four);

	// Function that allows for specific layers to be considered ( Empty brackets uses all layers)
	enemyPathFinding->setLayer({});

	// Getting enemy position X based on current tile location in game
	int enemyX = enemySprite->GetEnemySprite()->getPosition().x / mapOne->getTileSize().width;

	// Getting enemy position Y based on current tile location in game
	int enemyY = ((mapOne->getMapSize().height * mapOne->getTileSize().height) - enemySprite->GetEnemySprite()->getPosition().y) / mapOne->getTileSize().height;

	// Getting player position X based on current tile location in game
	int playerX = playerSprite->GetPlayerSprite()->getPosition().x / mapOne->getTileSize().width;

	// Getting player position Y based on current tile location in game
	int playerY = ((mapOne->getMapSize().height * mapOne->getTileSize().height) - playerSprite->GetPlayerSprite()->getPosition().y) / mapOne->getTileSize().height;

	// Creating Vec2 to hold the start position for pathfinding
	Vec2 pathStart(enemyX, enemyY);

	// Creating Vec2 to hold the end position for pathfinding 
	Vec2 pathEnd(playerX, playerY);

	// Push the position for pathStart, pathEnd and all tiles that have the GID of 49 {Meta tiles in tiled}
	std::vector<Vec2> enemyPath = enemyPathFinding->getObstaclePath(pathStart, pathEnd, { 49 });

	// If no nodes in path log no path found error
	if (enemyPath.size() == 0)
	{
		cocos2d::log("ERROR - No Path Found");
	}

	// If enemyPath contains nodes 
	else
	{
		// Each node in enemyPath is a Vec2 Position
		for (Vec2 tilePosition : enemyPath)
		{
			//Debug enemy path
			//log("Enemy X: %f Enemy Y: %f", tilePosition.x, tilePosition.y);

			// Ensure that the path is scaled to the size of the map and is the correct scale for accurate pathfining
			float dx = background->positionAt(tilePosition * Director::getInstance()->getContentScaleFactor()).x - enemySprite->GetEnemySprite()->getPosition().x;
			float dy = background->positionAt(tilePosition).y - enemySprite->GetEnemySprite()->getPosition().y;
			float d = sqrt(dx * dx + dy * dy);
			float v = 100;

			// Add scaled nodes to a Vec2 
			float EEX = enemySprite->GetEnemySprite()->getPosition().x + dx / d * v * dt;
			float EEY = enemySprite->GetEnemySprite()->getPosition().y + dy / d * v * dt;
			Vec2 newPathNode(EEX, EEY);

			//Push new Path into vector and move to each node in vector
			Pathfinding.push_back(newPathNode);

			//Super jittery 
			//enemySprite->GetEnemySprite()->setPosition(Vec2(EEX, EEY))
		}

		// While nodes exist within the vector iterate through each nodes and set the position of the Enemy sprite 
		for (PathNodeIterator = Pathfinding.begin(); PathNodeIterator != Pathfinding.end(); ++PathNodeIterator)
		{
			enemySprite->GetEnemySprite()->setPosition(Vec2(*PathNodeIterator));
		}
	}

	// Destructor for PathFinder - destroy path to prevent build up of usless points - Path optimization
	enemyPathFinding->~PathFinder();
};

// Function for scaling Enemy Difficulty Mode
/*
	-- BULLET DAMAGE IS DEPENDENT ON BOUNDING BOX COLLISION TIME --
*/
void AIDirector::EnemyDifficultyScale(Enemy *enemySprite, float dt, Player *playerSprite)
{
	// Initializing timers - Timers are used to conditions are met at specific times
	easyTimer += dt;
	mediumTimer += dt;
	hardTimer += dt;
	deathTimer += dt;

	// Using triggers locks in Easy mode
	if (easyTrigger == true)
	{
		// If delta time is greater than 221
		if (mediumTimer > 221)
		{
			// Reset medium timer
			mediumTimer = 0;

			// Reset Player Death Counter
			playerSprite->setPlayerDeathCounter(0);
		}

		// Condition to check changing from easy mode to medium mode is possible
		if (mediumTimer > 220 && mediumTimer < 221 && playerSprite->getPlayerDeathCounter() < 3)
		{
			// Unlock Easy Mode
			easyTrigger = false;

			// Set New difficulty
			newDifficulty = difficulty::MEDIUM;

			// Reset Medium timer
			mediumTimer = 0;

			// Reset Player Death Counter
			playerSprite->setPlayerDeathCounter(0);

			// Lock Medium mode
			mediumLock = true;

			// While medium mode is true update enemy stats
			if (mediumLock == true)
			{
				int easyHealth = 20;
				int mediumHealth = 40;
				int hardHealth = 60;
				int deathHealth = 80;

				int easyDamage = 1;
				int mediumDamage = 2;
				int hardDamage = 3;
				int deathDamage = 4;

				// Difficulty switch case 
				// Actually updates the Enemy stats 
				// Adds analytical feed back to .txt file
				switch (newDifficulty)
				{
					// FOUR HITS
					case EASY:

						enemySprite->SetEnemyHealth(easyHealth);
						enemySprite->SetEnemyDamage(easyDamage);

						cocos2d::log("ENEMIES ARE NOW ON EASY MODE");

						break;

					// EIGHT HITS
					case MEDIUM:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						myAnalytics.open("Analytics.txt", ios::out | ios::app);
						myAnalytics << "\nMEDIUM Mode Was Activated At ";
						myAnalytics << gameTimer;
						myAnalytics << " Into Gameplay.\n";
						myAnalytics << "Enemy Health Is Now: ";
						myAnalytics << enemySprite->GetEnemyHealth();
						myAnalytics << "\n";
						myAnalytics << "Enemy Damage Is Now: ";
						myAnalytics << enemySprite->GetEnemyDamage();
						myAnalytics << "\n";
						myAnalytics.close();

						playerSprite->setPlayerDeathCounter(0);

						mediumLock = false;
						mediumTrigger = true;

						break;

					// ELEVEN HITS
					case HARD:

						enemySprite->SetEnemyHealth(hardHealth);
						enemySprite->SetEnemyDamage(hardDamage);

						cocos2d::log("ENEMIES ARE NOW ON HARD MODE");

						break;

					// FOURTEEN HITS
					case DEATH:

						enemySprite->SetEnemyHealth(deathHealth);
						enemySprite->SetEnemyDamage(deathDamage);

						cocos2d::log("ENEMIES ARE NOW ON DEATH MODE");

						break;

					// Reset condition to set all enemies back to original state
					// EIGHT HITS
					case RESET:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					// Default State is state in which all enemies should be at the begining of game 
					// EIGHT HITS
					default:
						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");
				}
			}
		}
	}

	// If medium trigger is activated update to Medium Mode
	if (mediumTrigger == true)
	{
		// INITIALLY SETS BEGINNING DIFFICULTY TO MEDIUM
		// Initial lock is only ever true at the begining of game state after it is locked it cannot be unlocked
		// Needed for the Initial difficulty state as medium lock is only used for updating to medium.
		if (initialLock == true)
		{
			int easyHealth = 20;
			int mediumHealth = 40;
			int hardHealth = 60;
			int deathHealth = 80;

			int easyDamage = 1;
			int mediumDamage = 2;
			int hardDamage = 3;
			int deathDamage = 4;

			switch (newDifficulty)
			{
				// FOUR HITS
				case EASY:
					enemySprite->SetEnemyHealth(easyHealth);
					enemySprite->SetEnemyDamage(easyDamage);

					cocos2d::log("ENEMIES ARE NOW ON EASY MODE");

					break;

				// EIGHT HITS
				case MEDIUM:

					enemySprite->SetEnemyHealth(mediumHealth);
					enemySprite->SetEnemyDamage(mediumDamage);

					cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

					myAnalytics.open("Analytics.txt", ios::out | ios::app);
					myAnalytics << "\nMEDIUM Mode Was Activated At ";
					myAnalytics << gameTimer;
					myAnalytics << " Into Gameplay.\n";
					myAnalytics << "Enemy Health Is Now: ";
					myAnalytics << enemySprite->GetEnemyHealth();
					myAnalytics << "\n";
					myAnalytics << "Enemy Damage Is Now: ";
					myAnalytics << enemySprite->GetEnemyDamage();
					myAnalytics << "\n";
					myAnalytics.close();

					playerSprite->setPlayerDeathCounter(0);

					initialLock = false;
					mediumTrigger = true;

					break;

				// ELEVEN HITS
				case HARD:

					enemySprite->SetEnemyHealth(hardHealth);
					enemySprite->SetEnemyDamage(hardDamage);

					cocos2d::log("ENEMIES ARE NOW ON HARD MODE");

					break;

				// FOURTEEN HITS
				case DEATH:

					enemySprite->SetEnemyHealth(deathHealth);
					enemySprite->SetEnemyDamage(deathDamage);

					cocos2d::log("ENEMIES ARE NOW ON DEATH MODE");

					break;

				// EIGHT HITS
				case RESET:

					enemySprite->SetEnemyHealth(mediumHealth);
					enemySprite->SetEnemyDamage(mediumDamage);

					cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

					break;

				// EIGHT HITS
				default:
					enemySprite->SetEnemyHealth(mediumHealth);
					enemySprite->SetEnemyDamage(mediumDamage);

					cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");
			}
		}

		// Reset statements that clear death counter and reset timers
		// This is neccessary to ensure accurate scaling

		// Easy mode Reset
		if (easyTimer > 121)
		{
			easyTimer = 0;
			playerSprite->setPlayerDeathCounter(0);
		}

		// Hard hard Reset
		if (hardTimer > 521)
		{
			hardTimer = 0;
			playerSprite->setPlayerDeathCounter(0);
		}

		// Condition to check if difficulty needs to be downgraded to easy from Medium mode
		if (easyTimer > 120 && easyTimer < 121 && playerSprite->getPlayerDeathCounter() > 3)
		{
			// Unlock trigger medium mode
			mediumTrigger = false;

			// Set difficulty mode to easy
			newDifficulty = difficulty::EASY;

			// Reset easy timer
			easyTimer = 0;

			// Reset death counter
			playerSprite->setPlayerDeathCounter(0);

			// Lock in Easy mode
			easyLock = true;

			// If Easy mode is true update enemy Stats 
			if (easyLock == true)
			{
				int easyHealth = 20;
				int mediumHealth = 40;
				int hardHealth = 60;
				int deathHealth = 80;

				int easyDamage = 1;
				int mediumDamage = 2;
				int hardDamage = 3;
				int deathDamage = 4;

				switch (newDifficulty)
				{
					// FOUR HITS
					case EASY:

						enemySprite->SetEnemyHealth(easyHealth);
						enemySprite->SetEnemyDamage(easyDamage);

						cocos2d::log("ENEMIES ARE NOW ON EASY MODE");

						myAnalytics.open("Analytics.txt", ios::out | ios::app);
						myAnalytics << "\nEASY Mode Was Activated At ";
						myAnalytics << gameTimer;
						myAnalytics << " Into Gameplay.\n";
						myAnalytics << "Enemy Health Is Now: ";
						myAnalytics << enemySprite->GetEnemyHealth();
						myAnalytics << "\n";
						myAnalytics << "Enemy Damage Is Now: ";
						myAnalytics << enemySprite->GetEnemyDamage();
						myAnalytics << "\n";
						myAnalytics.close();

						playerSprite->setPlayerDeathCounter(0);

						easyLock = false;
						easyTrigger = true;

						break;

					// EIGHT HITS
					case MEDIUM:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					// ELEVEN HITS
					case HARD:

						enemySprite->SetEnemyHealth(hardHealth);
						enemySprite->SetEnemyDamage(hardDamage);

						cocos2d::log("ENEMIES ARE NOW ON HARD MODE");

						break;

					// FOURTEEN HITS
					case DEATH:

						enemySprite->SetEnemyHealth(deathHealth);
						enemySprite->SetEnemyDamage(deathDamage);

						cocos2d::log("ENEMIES ARE NOW ON DEATH MODE");

						break;

					// EIGHT HITS
					case RESET:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					// EIGHT HITS
					default:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");
				}
			}
		}

		// Condition to check if difficulty needs to be upgraded to hard from Medium mode
		if (hardTimer > 520 && hardTimer < 521 && playerSprite->getPlayerDeathCounter() < 3)
		{
			// Untrigger medium mode
			mediumTrigger = false;

			// Set Difficulty Mode to Hard
			newDifficulty = difficulty::HARD;

			// Reset Hard timer
			hardTimer = 0;

			// Reset Player Death count
			playerSprite->setPlayerDeathCounter(0);

			// Lock in Hard mode
			hardLock = true;

			// If Hard mode is activated update enemy stats
			if (hardLock == true)
			{
				int easyHealth = 20;
				int mediumHealth = 40;
				int hardHealth = 60;
				int deathHealth = 80;

				int easyDamage = 1;
				int mediumDamage = 2;
				int hardDamage = 3;
				int deathDamage = 4;

				switch (newDifficulty)
				{
					// FOUR HITS
					case EASY:

						enemySprite->SetEnemyHealth(easyHealth);
						enemySprite->SetEnemyDamage(easyDamage);

						cocos2d::log("ENEMIES ARE NOW ON EASY MODE");

						break;

					// EIGHT HITS
					case MEDIUM:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					// ELEVEN HITS
					case HARD:

						enemySprite->SetEnemyHealth(hardHealth);
						enemySprite->SetEnemyDamage(hardDamage);

						cocos2d::log("ENEMIES ARE NOW ON HARD MODE");

						myAnalytics.open("Analytics.txt", ios::out | ios::app);
						myAnalytics << "\nHARD Mode Was Activated At ";
						myAnalytics << gameTimer;
						myAnalytics << " Into Gameplay.\n";
						myAnalytics << "Enemy Health Is Now: ";
						myAnalytics << enemySprite->GetEnemyHealth();
						myAnalytics << "\n";
						myAnalytics << "Enemy Damage Is Now: ";
						myAnalytics << enemySprite->GetEnemyDamage();
						myAnalytics << "\n";
						myAnalytics.close();

						hardLock = false;
						hardTrigger = true;

						break;
 
					// FOURTEEN HITS
					case DEATH:

						enemySprite->SetEnemyHealth(deathHealth);
						enemySprite->SetEnemyDamage(deathDamage);

						cocos2d::log("ENEMIES ARE NOW ON DEATH MODE");

						break;

					// EIGHT HITS
					case RESET:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					default:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");
				}
			}
		}
	}

	// Reset statements that clear death counter and reset timers
	// This is neccessary to ensure accurate scaling

	// If Hard mode is true
	if (hardTrigger == true)
	{
		// Reset Medium 
		if (mediumTimer > 121)
		{
			// Reset Medium timer
			mediumTimer = 0;

			// Reset player death count
			playerSprite->setPlayerDeathCounter(0);
		}

		// If death timer exceeds 821
		if (deathTimer > 821)
		{
			// Reset death timer
			deathTimer = 0;

			//reset player death count
			playerSprite->setPlayerDeathCounter(0);
		}

		// Condition to check if difficulty needs to be downgraded to medium from Hard mode
		if (mediumTimer > 120 && mediumTimer < 121 && playerSprite->getPlayerDeathCounter() > 3)
		{
			// Untrigger hard trigger
			hardTrigger = false;

			// Set difficulty mode to Medium
			newDifficulty = difficulty::MEDIUM;

			// reset medium timer
			mediumTimer = 0;

			// reset death counter
			playerSprite->setPlayerDeathCounter(0);

			// lock in medium mode
			mediumLock = true;

			// Update enemy stats to medium mode stats update analytical .txt
			if (mediumLock == true)
			{
				int easyHealth = 20;
				int mediumHealth = 40;
				int hardHealth = 60;
				int deathHealth = 80;

				int easyDamage = 1;
				int mediumDamage = 2;
				int hardDamage = 3;
				int deathDamage = 4;

				switch (newDifficulty)
				{
					// FOUR HITS
					case EASY:

						enemySprite->SetEnemyHealth(easyHealth);
						enemySprite->SetEnemyDamage(easyDamage);

						cocos2d::log("ENEMIES ARE NOW ON EASY MODE");

						break;

					// EIGHT HITS
					case MEDIUM:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						myAnalytics.open("Analytics.txt", ios::out | ios::app);
						myAnalytics << "\nMEDIUM Mode Was Activated At ";
						myAnalytics << gameTimer;
						myAnalytics << " Into Gameplay.\n";
						myAnalytics << "Enemy Health Is Now: ";
						myAnalytics << enemySprite->GetEnemyHealth();
						myAnalytics << "\n";
						myAnalytics << "Enemy Damage Is Now: ";
						myAnalytics << enemySprite->GetEnemyDamage();
						myAnalytics << "\n";
						myAnalytics.close();

						mediumLock = false;
						mediumTrigger = true;

						break;

					// ELEVEN HITS
					case HARD:

						enemySprite->SetEnemyHealth(hardHealth);
						enemySprite->SetEnemyDamage(hardDamage);

						cocos2d::log("ENEMIES ARE NOW ON HARD MODE");

						break;

					// FOURTEEN HITS
					case DEATH:

						enemySprite->SetEnemyHealth(deathHealth);
						enemySprite->SetEnemyDamage(deathDamage);

						cocos2d::log("ENEMIES ARE NOW ON DEATH MODE");

						break;

					//EIGHT HITS
					case RESET:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					//EIGHT HITS
					default:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");
				}
			}
		}

		// Condition to check if difficulty needs to be upgraded to Death from Hard mode
		if (deathTimer > 820 && deathTimer < 821 && playerSprite->getPlayerDeathCounter() < 3)
		{
			// Untrigger hard mode
			hardTrigger = false;

			// Set Difficulty to Death Mode
			newDifficulty = difficulty::DEATH;

			// Reset death timer
			deathTimer = 0;

			// Reset Death Counter
			playerSprite->setPlayerDeathCounter(0);

			// Lock in Death mode
			deathLock = true;

			// If death mode is locked in update enemy stats 
			if (deathLock == true)
			{
				int easyHealth = 20;
				int mediumHealth = 40;
				int hardHealth = 60;
				int deathHealth = 80;

				int easyDamage = 1;
				int mediumDamage = 2;
				int hardDamage = 3;
				int deathDamage = 4;

				switch (newDifficulty)
				{
					// FOUR HITS
					case EASY:

						enemySprite->SetEnemyHealth(easyHealth);
						enemySprite->SetEnemyDamage(easyDamage);

						cocos2d::log("ENEMIES ARE NOW ON EASY MODE");

						break;

					// EIGHT HITS
					case MEDIUM:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					// ELEVEN HITS
					case HARD:

						enemySprite->SetEnemyHealth(hardHealth);
						enemySprite->SetEnemyDamage(hardDamage);

						cocos2d::log("ENEMIES ARE NOW ON HARD MODE");

						break;

					// FOURTEEN HITS
					case DEATH:

						enemySprite->SetEnemyHealth(deathHealth);
						enemySprite->SetEnemyDamage(deathDamage);

						cocos2d::log("ENEMIES ARE NOW ON DEATH MODE");

						myAnalytics.open("Analytics.txt", ios::out | ios::app);
						myAnalytics << "\nDEATH Mode Was Activated At ";
						myAnalytics << gameTimer;
						myAnalytics << " Into Gameplay.\n";
						myAnalytics << "Enemy Health Is Now: ";
						myAnalytics << enemySprite->GetEnemyHealth();
						myAnalytics << "\n";
						myAnalytics << "Enemy Damage Is Now: ";
						myAnalytics << enemySprite->GetEnemyDamage();
						myAnalytics << "\n";
						myAnalytics.close();

						deathLock = false;
						deathTrigger = true;

						break;

					// EIGHT HITS
					case RESET:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					//EIGHT HITS
					default:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");
				}
			}
		}
	}

	// Reset statements that clear death counter and reset timers
	// This is neccessary to ensure accurate scaling

	// If Death mode is triggered
	if (deathTrigger == true)
	{
		// Reset hard
		if (hardTimer > 121)
		{
			// Reset Hard Timer
			hardTimer = 0;

			// Reset player death counter
			playerSprite->setPlayerDeathCounter(0);
		}

		// Condition to check if difficulty needs to be downgraded to hard from Death mode
		if (hardTimer > 120 && hardTimer < 121 && playerSprite->getPlayerDeathCounter() > 5)
		{
			// Untrigger Death mode
			deathTrigger = false;

			// Set Difficulty mode to hard 
			newDifficulty = difficulty::HARD;

			// Reset hard timer
			hardTimer = 0;

			// Reset Death Counter
			playerSprite->setPlayerDeathCounter(0);

			// Lock in Hard mode
			hardLock = true;

			// While hard mode is locked in update enemy stats
			if (hardLock == true)
			{
				int easyHealth = 20;
				int mediumHealth = 40;
				int hardHealth = 60;
				int deathHealth = 80;

				int easyDamage = 1;
				int mediumDamage = 2;
				int hardDamage = 3;
				int deathDamage = 4;

				switch (newDifficulty)
				{
					// FOUR HITS
					case EASY:

						enemySprite->SetEnemyHealth(easyHealth);
						enemySprite->SetEnemyDamage(easyDamage);

						cocos2d::log("ENEMIES ARE NOW ON EASY MODE");

						break;

					// EIGHT HITS
					case MEDIUM:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumHealth);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					// ELEVEN HITS
					case HARD:

						enemySprite->SetEnemyHealth(hardHealth);
						enemySprite->SetEnemyDamage(hardDamage);

						cocos2d::log("ENEMIES ARE NOW ON HARD MODE");

						myAnalytics.open("Analytics.txt", ios::out | ios::app);
						myAnalytics << "\nHARD Mode Was Activated At ";
						myAnalytics << gameTimer;
						myAnalytics << " Into Gameplay.\n";
						myAnalytics << "Enemy Health Is Now: ";
						myAnalytics << enemySprite->GetEnemyHealth();
						myAnalytics << "\n";
						myAnalytics << "Enemy Damage Is Now: ";
						myAnalytics << enemySprite->GetEnemyDamage();
						myAnalytics << "\n";
						myAnalytics.close();

						hardLock = false;
						hardTrigger = true;

						break;

					// FOURTEEN HITS
					case DEATH:

						enemySprite->SetEnemyHealth(deathHealth);
						enemySprite->SetEnemyDamage(deathDamage);

						cocos2d::log("ENEMIES ARE NOW ON DEATH MODE");

						break;

					// EIGHT HITS
					case RESET:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");

						break;

					// EIGHT HITS
					default:

						enemySprite->SetEnemyHealth(mediumHealth);
						enemySprite->SetEnemyDamage(mediumDamage);

						cocos2d::log("ENEMIES ARE NOW ON MEDIUM MODE");
				}
			}
		}
	}
};

// Function to add dynamic stats to a boss enemy
void AIDirector::DynamicBoss(Enemy *enemySprite)
{
	// Get enemies current health based on game state
	int eBHealth = enemySprite->GetEnemyHealth();

	// Initialize random number generator between 1 - 20
	int randBooster = rand() % 20;

	// Add a random number of health points to a boss enemy 
	int newEBHealth = eBHealth + randBooster;

	// Set boss sprite scale - make it bigger than regular sprites
	enemySprite->GetEnemySprite()->setScale(3.0);

	// Ensure sprite is visible
	enemySprite->GetEnemySprite()->setVisible(true);

	// Set Boss health
	enemySprite->SetEnemyHealth(newEBHealth);
};

// Add player Specific analytics
void AIDirector::PlayerDeathAnalytics(Player *playerSprite)
{
	// As long as the player is not dead
	if (playerSprite->GetPlayerHealth() <= 0)
	{
		myAnalytics.open("Analytics.txt", ios::out | ios::app);
		myAnalytics << "\nPlayer DIED At ";
		myAnalytics << gameTimer;
		myAnalytics << " Into Gameplay.\n";
		myAnalytics.close();
	}
};

// Add Enemy specific Analytics
void AIDirector::EnemyDeathAnalytics(Enemy *enemySprite)
{
	// As long as the enemy is not dead
	if (enemySprite->GetEnemyHealth() <= 0)
	{
		myAnalytics.open("Analytics.txt", ios::out | ios::app);
		myAnalytics << "\nPlayer KILLED An ENEMY At ";
		myAnalytics << gameTimer;
		myAnalytics << " Into Gameplay.\n";

		myAnalytics.close();
	}
};

// Add Score Specific analytics
void AIDirector::ScoreAnalytics(Enemy *enemySprite, int score)
{
	// As long as the enemy health is above -1 and less then 0
	// Needed sepcifically <0 >-1 to avoid issues with enemy health dropping below 0
	if (enemySprite->GetEnemyHealth() <= 0 && enemySprite->GetEnemyHealth() > -1)
	{
		myAnalytics.open("Analytics.txt", ios::out | ios::app);
		myAnalytics << "\nPlayer SCORE Is Now: ";
		myAnalytics << score;
		myAnalytics << "\n";

		myAnalytics.close();
	}
};