//Samuel MacSweeney
//K00200955

#include "LevelOne.h"

// Creating the Scene
Scene* LevelOne::createScene()
{
	return LevelOne::create();
};

// Init Function to declare what is intialized at start of game
bool LevelOne::init()
{
	if (!Scene::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Adding keyboard and mouse listeners
	KeyBoardListener();
	MouseListener();

	// Adding Objects For initialization
	CreateMap();
	CreatePlayer();
	CreateEnemies();
	CreateSightSprite();
	CreateHealth();
	CreateAmmo();
	CreateHUD();

	// Schedualed update for effeciency 
	this->scheduleUpdate();

	return true;
};

// Update Function
void LevelOne::update(float dt)
{
	// Set Hud to Position reletive to player position - Ensure it stays in the right place
	HUD->setPosition
	(
		playerSprite->GetPlayerSprite()->getPosition().x - visibleSize.width / 2,
		playerSprite->GetPlayerSprite()->getPosition().y - visibleSize.height / 2
	);

	// Update Ammo and Score Lables each frame
	AmmoLabel->setString("AMMO:" + std::to_string(playerSprite->GetPlayerAmmo()));
	ScoreLabel->setString("Score:" + std::to_string(score));

	// Check if an enemy has died if they have call the respawn function and update score
	if (enemySprite->GetEnemyHealth() <= 0)
	{
		RespawnEnemies();
		Score();
	}

	// If enemy is not dead ensure they are visible
	else if (enemySprite->GetEnemyHealth() > 0)
	{
		enemyIsVisible = true;
	}

	// Initialize timer used by AI Director class
	Timer(dt);

	// Update Enemy Difficulty in real time
	EnemyDifficultyScale(dt);

	// Player Movement functionality 
	PlayerMovement(dt);

	// Update enemy path in real time
	EnemyPathFinder(dt);

	// Check for player damage
	PlayerDamage();

	// Check if player has died
	PlayerDeath();

	// Respawn health packs
	RespawnHealth();

	// Respawn Ammo Packs
	RespawnAmmo();

	// Check if player has collided with a health pick up
	PlayerHealthCollision();

	// Check if player collided with an Enemy
	PlayerEnemyCollision();

	// Check if player collided with an ammo pick up
	PlayerAmmoCollision();

	// Check if bullet collided with the enemy
	BulletCollison();

	// Update the analytics for Player score
	ScoreAnalytics();

	// Initialize AI Director emergency health and ammo pick ups
	AID->SpawnNewHealthPickup(playerSprite, newHealthPickUpSprite);
	AID->SpawnNewAmmoPickup(playerSprite, newAmmoPickUpSprite);

	// Check if player has died
	if (playerSprite->GetPlayerHealth() <= 0)
	{
		// Update Player Analytics
		AID->PlayerDeathAnalytics(playerSprite);

		// Trigger is Dead
		isDead = true;

		// Call player death function
		PlayerDeath();

		// Reset health
		playerSprite->SetPlayerHealth(100);

		// Update death counter
		deathCount = playerSprite->getPlayerDeathCounter() + 1;
		playerSprite->setPlayerDeathCounter(deathCount);
	}

	// Ensure player is visible if not dead
	else if (playerSprite->GetPlayerHealth() > 0)
	{
		isDead = false;
	}

	// Attempting to ensure enemy collision 
	auto enemyPosition = enemySprite->GetEnemySprite()->getPosition();

	if (enemyPosition.x <= (mapOne->getMapSize().width * mapOne->getTileSize().width) 
		&& enemyPosition.y <= (mapOne->getMapSize().height * mapOne->getTileSize().height) 
		&& enemyPosition.y >= 0 && enemyPosition.x >= 0)
	{
		this->EnemyCollision(enemyPosition);
	}

	// Initialze field of view for enemies
	sightSprite->GetSightSprite()->setPosition(enemySprite->GetEnemySprite()->getPosition());
	sightSprite1->GetSightSprite()->setPosition(enemySprite1->GetEnemySprite()->getPosition());
	sightSprite2->GetSightSprite()->setPosition(enemySprite2->GetEnemySprite()->getPosition());
	sightSprite3->GetSightSprite()->setPosition(enemySprite3->GetEnemySprite()->getPosition());
	sightSprite4->GetSightSprite()->setPosition(enemySprite4->GetEnemySprite()->getPosition());
	sightSprite5->GetSightSprite()->setPosition(enemySprite5->GetEnemySprite()->getPosition());
	sightSprite6->GetSightSprite()->setPosition(enemySprite6->GetEnemySprite()->getPosition());
	sightSprite7->GetSightSprite()->setPosition(enemySprite7->GetEnemySprite()->getPosition());
};

// Create playable map
bool LevelOne::CreateMap()
{
	mapOne = TMXTiledMap::create("MapOne.tmx");
	background = mapOne->layerNamed("Background");
	meta = mapOne->layerNamed("Meta");
	meta->setVisible(false);
	mapOne->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(mapOne);

	// Initialize map Objects - created within tiled
	mapObjectGroup = mapOne->getObjectGroup("Objects");

	// If map doesn't load return an error
	if (mapObjectGroup == NULL)
	{
		log("ERROR - Map Has No Object Layer");
		return false;
	}

	return true;
};

// Create the player object
void LevelOne::CreatePlayer()
{
	// Get position of player spawn point
	playerSpawnPoint = mapObjectGroup->getObject("SpawnPoint");

	// Assure currect scale for positions
	float playerX = playerSpawnPoint["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float playerY = playerSpawnPoint["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	playerSprite->GetPlayerSprite()->setScale(Director::getInstance()->getContentScaleFactor());

	// Set player position
	playerSprite->GetPlayerSprite()->setPosition(Vec2(playerX, playerY));

	// Set player health
	playerSprite->SetPlayerHealth(100);

	// Add to scene
	this->addChild(playerSprite->GetPlayerSprite());
};

// Function to set up and create all enemies in game
void LevelOne::CreateEnemies()
{
	enemySpawnPoint = mapObjectGroup->getObject("EnemySpawner");
	float enemyX = enemySpawnPoint["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemyY = enemySpawnPoint["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	enemySprite->GetEnemySprite()->setScale(Director::getInstance()->getContentScaleFactor());

	this->addChild(enemySprite->GetEnemySprite());

	//PUSHING ENEMY SPAWN LOCATION INTO VECTOR
	enemySpawnLocations.push_back(Vec2(enemyX, enemyY));

	enemySpawnPoint1 = mapObjectGroup->getObject("EnemySpawner1");
	float enemy1X = enemySpawnPoint1["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy1Y = enemySpawnPoint1["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	enemySprite1->GetEnemySprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetEnemyLocation(enemySprite1, enemy1X, enemy1Y);

	this->addChild(enemySprite1->GetEnemySprite());

	//PUSHING ENEMY SPAWN LOCATION INTO VECTOR
	enemySpawnLocations.push_back(Vec2(enemy1X, enemy1Y));

	enemySpawnPoint2 = mapObjectGroup->getObject("EnemySpawner2");
	float enemy2X = enemySpawnPoint2["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy2Y = enemySpawnPoint2["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	enemySprite2->GetEnemySprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetEnemyLocation(enemySprite2, enemy2X, enemy2Y);

	this->addChild(enemySprite2->GetEnemySprite());

	//PUSHING ENEMY SPAWN LOCATION INTO VECTOR
	enemySpawnLocations.push_back(Vec2(enemy2X, enemy2Y));

	enemySpawnPoint3 = mapObjectGroup->getObject("EnemySpawner3");
	float enemy3X = enemySpawnPoint3["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy3Y = enemySpawnPoint3["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	enemySprite3->GetEnemySprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetEnemyLocation(enemySprite3, enemy3X, enemy3Y);

	this->addChild(enemySprite3->GetEnemySprite());

	//PUSHING ENEMY SPAWN LOCATION INTO VECTOR
	enemySpawnLocations.push_back(Vec2(enemy3X, enemy3Y));

	enemySpawnPoint4 = mapObjectGroup->getObject("EnemySpawner4");
	float enemy4X = enemySpawnPoint4["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy4Y = enemySpawnPoint4["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	enemySprite4->GetEnemySprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetEnemyLocation(enemySprite4, enemy4X, enemy4Y);

	this->addChild(enemySprite4->GetEnemySprite());

	//PUSHING ENEMY SPAWN LOCATION INTO VECTOR
	enemySpawnLocations.push_back(Vec2(enemy4X, enemy4Y));

	enemySpawnPoint5 = mapObjectGroup->getObject("EnemySpawner5");
	float enemy5X = enemySpawnPoint5["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy5Y = enemySpawnPoint5["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	enemySprite5->GetEnemySprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetEnemyLocation(enemySprite5, enemy5X, enemy5Y);

	this->addChild(enemySprite5->GetEnemySprite());

	//PUSHING ENEMY SPAWN LOCATION INTO VECTOR
	enemySpawnLocations.push_back(Vec2(enemy5X, enemy5Y));

	enemySpawnPoint6 = mapObjectGroup->getObject("EnemySpawner6");
	float enemy6X = enemySpawnPoint6["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy6Y = enemySpawnPoint6["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	enemySprite6->GetEnemySprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetEnemyLocation(enemySprite6, enemy6X, enemy6Y);

	this->addChild(enemySprite6->GetEnemySprite());

	//PUSHING ENEMY SPAWN LOCATION INTO VECTOR
	enemySpawnLocations.push_back(Vec2(enemy6X, enemy6Y));

	enemySpawnPoint7 = mapObjectGroup->getObject("EnemySpawner7");
	float enemy7X = enemySpawnPoint7["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy7Y = enemySpawnPoint7["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	enemySprite7->GetEnemySprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetEnemyLocation(enemySprite7, enemy7X, enemy7Y);

	this->addChild(enemySprite7->GetEnemySprite());

	//PUSHING ENEMY SPAWN LOCATION INTO VECTOR
	enemySpawnLocations.push_back(Vec2(enemy7X, enemy7Y));
};

// Initializing the Field of view for all enemies 
void LevelOne::CreateSightSprite()
{
	sightSprite->GetSightSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	sightSprite->GetSightSprite()->setVisible(false);

	this->addChild(sightSprite->GetSightSprite());

	sightSprite1->GetSightSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	sightSprite1->GetSightSprite()->setVisible(false);

	this->addChild(sightSprite1->GetSightSprite());

	sightSprite2->GetSightSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	sightSprite2->GetSightSprite()->setVisible(false);

	this->addChild(sightSprite2->GetSightSprite());

	sightSprite3->GetSightSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	sightSprite3->GetSightSprite()->setVisible(false);

	this->addChild(sightSprite3->GetSightSprite());

	sightSprite4->GetSightSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	sightSprite4->GetSightSprite()->setVisible(false);

	this->addChild(sightSprite4->GetSightSprite());

	sightSprite5->GetSightSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	sightSprite5->GetSightSprite()->setVisible(false);

	this->addChild(sightSprite5->GetSightSprite());

	sightSprite6->GetSightSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	sightSprite6->GetSightSprite()->setVisible(false);

	this->addChild(sightSprite6->GetSightSprite());

	sightSprite7->GetSightSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	sightSprite7->GetSightSprite()->setVisible(false);

	this->addChild(sightSprite7->GetSightSprite());
};

// Setting up respawners for all enemies
void LevelOne::RespawnEnemies()
{
	// Each time an enemy respawns increase score
	score = score + 5;

	// Enemy 1
	enemySpawnPoint = mapObjectGroup->getObject("EnemySpawner");
	float enemyX = enemySpawnPoint["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemyY = enemySpawnPoint["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	// Using the AI Director to set the repspawn conditions for enemies
	AID->EnemyRespawner(enemySprite, enemyX, enemyY, enemyIsVisible);

	// Enemy 2
	enemySpawnPoint1 = mapObjectGroup->getObject("EnemySpawner1");
	float enemy1X = enemySpawnPoint1["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy1Y = enemySpawnPoint1["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->EnemyRespawner(enemySprite1, enemy1X, enemy1Y, enemy1IsVisible);

	// Enemy 3
	enemySpawnPoint2 = mapObjectGroup->getObject("EnemySpawner2");
	float enemy2X = enemySpawnPoint2["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy2Y = enemySpawnPoint2["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->EnemyRespawner(enemySprite2, enemy2X, enemy2Y, enemy2IsVisible);

	// Enemy 4
	enemySpawnPoint3 = mapObjectGroup->getObject("EnemySpawner3");
	float enemy3X = enemySpawnPoint3["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy3Y = enemySpawnPoint3["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->EnemyRespawner(enemySprite3, enemy3X, enemy3Y, enemy3IsVisible);

	// Enemy 5
	enemySpawnPoint4 = mapObjectGroup->getObject("EnemySpawner4");
	float enemy4X = enemySpawnPoint4["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy4Y = enemySpawnPoint4["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->EnemyRespawner(enemySprite4, enemy4X, enemy4Y, enemy4IsVisible);

	// Enemy 6
	enemySpawnPoint5 = mapObjectGroup->getObject("EnemySpawner5");
	float enemy5X = enemySpawnPoint5["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy5Y = enemySpawnPoint5["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->EnemyRespawner(enemySprite5, enemy5X, enemy5Y, enemy5IsVisible);

	// Enemy 7
	enemySpawnPoint6 = mapObjectGroup->getObject("EnemySpawner6");
	float enemy6X = enemySpawnPoint6["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy6Y = enemySpawnPoint6["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->EnemyRespawner(enemySprite6, enemy6X, enemy6Y, enemy6IsVisible);

	// Enemy 8
	enemySpawnPoint7 = mapObjectGroup->getObject("EnemySpawner7");
	float enemy7X = enemySpawnPoint7["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float enemy7Y = enemySpawnPoint7["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->EnemyRespawner(enemySprite7, enemy7X, enemy7Y, enemy7IsVisible);

	// When player gets a score of 100 Create a boss
	if (score >= 100)
	{
		// Pick a random spawn point based on enemy spawners
		int randomEnemySpawnLocation = rand() % enemySpawnLocations.size();
		Vec2 newEnemySpawnLocation = enemySpawnLocations[randomEnemySpawnLocation];

		// Log out boss Spawn location
		log("BOSS SPAWN LOCATION %f %f", newEnemySpawnLocation.x, newEnemySpawnLocation.y);

		// Update boss with dynamic boss attributes with the AI Director
		AID->DynamicBoss(BossSprite);

		// Set position with the AI Director
		AID->SetEnemyLocation(BossSprite, newEnemySpawnLocation.x, newEnemySpawnLocation.y);

		// Add to the scene
		this->addChild(BossSprite->GetEnemySprite());
	}
};

// Function for repspawning health Packs
void LevelOne::RespawnHealth()
{
	// Get spawner X and Y coords
	healthSpawnPoint = mapObjectGroup->getObject("HealthSpawner");
	float healthX = healthSpawnPoint["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float healthY = healthSpawnPoint["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	// Reset Health Pack With the AI Director
	AID->HealthPickUpRespawn(healthPickUpSprite, healthX, healthY, healthUp);

	healthSpawnPoint1 = mapObjectGroup->getObject("HealthSpawner1");

	float health1X = healthSpawnPoint1["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health1Y = healthSpawnPoint1["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->HealthPickUpRespawn(healthPickUpSprite1, health1X, health1Y, healthUp1);

	healthSpawnPoint2 = mapObjectGroup->getObject("HealthSpawner2");

	float health2X = healthSpawnPoint2["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health2Y = healthSpawnPoint2["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->HealthPickUpRespawn(healthPickUpSprite2, health2X, health2Y, healthUp2);

	healthSpawnPoint3 = mapObjectGroup->getObject("HealthSpawner3");

	float health3X = healthSpawnPoint3["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health3Y = healthSpawnPoint3["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->HealthPickUpRespawn(healthPickUpSprite3, health3X, health3Y, healthUp3);

	healthSpawnPoint4 = mapObjectGroup->getObject("HealthSpawner4");

	float health4X = healthSpawnPoint4["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health4Y = healthSpawnPoint4["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->HealthPickUpRespawn(healthPickUpSprite4, health4X, health4Y, healthUp4);

	healthSpawnPoint5 = mapObjectGroup->getObject("HealthSpawner5");

	float health5X = healthSpawnPoint5["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health5Y = healthSpawnPoint5["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->HealthPickUpRespawn(healthPickUpSprite5, health5X, health5Y, healthUp5);
};

// Function to respawn Ammo Packs
void LevelOne::RespawnAmmo()
{
	// Get ammo Spawners from tiled
	ammoSpawnPoint = mapObjectGroup->getObject("AmmoSpawner");

	float ammoX = ammoSpawnPoint["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammoY = ammoSpawnPoint["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	// Reset ammo pack with AI Director
	AID->AmmoPickUpRespawn(ammoPickUpSprite, ammoX, ammoY, ammoUp);

	ammoSpawnPoint1 = mapObjectGroup->getObject("AmmoSpawner1");

	float ammo1X = ammoSpawnPoint1["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo1Y = ammoSpawnPoint1["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->AmmoPickUpRespawn(ammoPickUpSprite1, ammo1X, ammo1Y, ammoUp1);

	ammoSpawnPoint2 = mapObjectGroup->getObject("AmmoSpawner2");

	float ammo2X = ammoSpawnPoint2["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo2Y = ammoSpawnPoint2["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->AmmoPickUpRespawn(ammoPickUpSprite2, ammo2X, ammo2Y, ammoUp2);

	ammoSpawnPoint3 = mapObjectGroup->getObject("AmmoSpawner3");

	float ammo3X = ammoSpawnPoint3["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo3Y = ammoSpawnPoint3["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->AmmoPickUpRespawn(ammoPickUpSprite3, ammo3X, ammo3Y, ammoUp3);

	ammoSpawnPoint4 = mapObjectGroup->getObject("AmmoSpawner4");

	float ammo4X = ammoSpawnPoint4["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo4Y = ammoSpawnPoint4["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->AmmoPickUpRespawn(ammoPickUpSprite4, ammo4X, ammo4Y, ammoUp4);

	ammoSpawnPoint5 = mapObjectGroup->getObject("AmmoSpawner5");

	float ammo5X = ammoSpawnPoint5["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo5Y = ammoSpawnPoint5["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	AID->AmmoPickUpRespawn(ammoPickUpSprite5, ammo5X, ammo5Y, ammoUp5);
};

// Create Health Packs 
void LevelOne::CreateHealth()
{
	healthSpawnPoint = mapObjectGroup->getObject("HealthSpawner");

	float healthX = healthSpawnPoint["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float healthY = healthSpawnPoint["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	healthPickUpSprite->GetHealthPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetHealthPickupLocation(healthPickUpSprite, healthX, healthY);

	this->addChild(healthPickUpSprite->GetHealthPickUpSprite());

	healthSpawnPoint1 = mapObjectGroup->getObject("HealthSpawner1");

	float health1X = healthSpawnPoint1["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health1Y = healthSpawnPoint1["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	healthPickUpSprite1->GetHealthPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetHealthPickupLocation(healthPickUpSprite1, health1X, health1Y);

	this->addChild(healthPickUpSprite1->GetHealthPickUpSprite());

	healthSpawnPoint2 = mapObjectGroup->getObject("HealthSpawner2");

	float health2X = healthSpawnPoint2["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health2Y = healthSpawnPoint2["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	healthPickUpSprite2->GetHealthPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetHealthPickupLocation(healthPickUpSprite2, health2X, health2Y);

	this->addChild(healthPickUpSprite2->GetHealthPickUpSprite());

	healthSpawnPoint3 = mapObjectGroup->getObject("HealthSpawner3");

	float health3X = healthSpawnPoint3["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health3Y = healthSpawnPoint3["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	healthPickUpSprite3->GetHealthPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetHealthPickupLocation(healthPickUpSprite3, health3X, health3Y);

	this->addChild(healthPickUpSprite3->GetHealthPickUpSprite());

	healthSpawnPoint4 = mapObjectGroup->getObject("HealthSpawner4");

	float health4X = healthSpawnPoint4["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health4Y = healthSpawnPoint4["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	healthPickUpSprite4->GetHealthPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetHealthPickupLocation(healthPickUpSprite4, health4X, health4Y);

	this->addChild(healthPickUpSprite4->GetHealthPickUpSprite());

	healthSpawnPoint5 = mapObjectGroup->getObject("HealthSpawner5");

	float health5X = healthSpawnPoint5["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float health5Y = healthSpawnPoint5["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	healthPickUpSprite5->GetHealthPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetHealthPickupLocation(healthPickUpSprite5, health5X, health5Y);

	this->addChild(healthPickUpSprite5->GetHealthPickUpSprite());

	//THIS IS FOR AI DIRECTOR IN CREATING NEW SPRITE!!!
	newHealthPickUpSprite->GetHealthPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	newHealthPickUpSprite->GetHealthPickUpSprite()->setVisible(false);

	this->addChild(newHealthPickUpSprite->GetHealthPickUpSprite());
};

// Creating Ammo Packs
void LevelOne::CreateAmmo()
{

	ammoSpawnPoint = mapObjectGroup->getObject("AmmoSpawner");

	float ammoX = ammoSpawnPoint["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammoY = ammoSpawnPoint["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	ammoPickUpSprite->GetAmmoPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetAmmoLocation(ammoPickUpSprite, ammoX, ammoY);

	this->addChild(ammoPickUpSprite->GetAmmoPickUpSprite());

	ammoSpawnPoint1 = mapObjectGroup->getObject("AmmoSpawner1");

	float ammo1X = ammoSpawnPoint1["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo1Y = ammoSpawnPoint1["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	ammoPickUpSprite1->GetAmmoPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetAmmoLocation(ammoPickUpSprite1, ammo1X, ammo1Y);

	this->addChild(ammoPickUpSprite1->GetAmmoPickUpSprite());

	ammoSpawnPoint2 = mapObjectGroup->getObject("AmmoSpawner2");

	float ammo2X = ammoSpawnPoint2["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo2Y = ammoSpawnPoint2["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	ammoPickUpSprite2->GetAmmoPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetAmmoLocation(ammoPickUpSprite2, ammo2X, ammo2Y);

	this->addChild(ammoPickUpSprite2->GetAmmoPickUpSprite());

	ammoSpawnPoint3 = mapObjectGroup->getObject("AmmoSpawner3");

	float ammo3X = ammoSpawnPoint3["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo3Y = ammoSpawnPoint3["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	ammoPickUpSprite3->GetAmmoPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetAmmoLocation(ammoPickUpSprite3, ammo3X, ammo3Y);

	this->addChild(ammoPickUpSprite3->GetAmmoPickUpSprite());

	ammoSpawnPoint4 = mapObjectGroup->getObject("AmmoSpawner4");

	float ammo4X = ammoSpawnPoint4["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo4Y = ammoSpawnPoint4["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	ammoPickUpSprite4->GetAmmoPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetAmmoLocation(ammoPickUpSprite4, ammo4X, ammo4Y);

	this->addChild(ammoPickUpSprite4->GetAmmoPickUpSprite());

	ammoSpawnPoint5 = mapObjectGroup->getObject("AmmoSpawner5");

	float ammo5X = ammoSpawnPoint5["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
	float ammo5Y = ammoSpawnPoint5["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

	ammoPickUpSprite5->GetAmmoPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	AID->SetAmmoLocation(ammoPickUpSprite5, ammo5X, ammo5Y);

	this->addChild(ammoPickUpSprite5->GetAmmoPickUpSprite());

	//THIS IS FOR AI DIRECTOR IN CREATING NEW SPRITE!!!
	newAmmoPickUpSprite->GetAmmoPickUpSprite()->setScale(Director::getInstance()->getContentScaleFactor());
	newAmmoPickUpSprite->GetAmmoPickUpSprite()->setVisible(false);

	this->addChild(newAmmoPickUpSprite->GetAmmoPickUpSprite());
};

// Initializing Player Enemy Collision
void LevelOne::PlayerEnemyCollision()
{
	// All Collision between enemy and player objects is passed into the AI Director
	AID->PlayerEnemyCollision(playerSprite, enemySprite);
	AID->PlayerEnemyCollision(playerSprite, enemySprite1);
	AID->PlayerEnemyCollision(playerSprite, enemySprite2);
	AID->PlayerEnemyCollision(playerSprite, enemySprite3);
	AID->PlayerEnemyCollision(playerSprite, enemySprite4);
	AID->PlayerEnemyCollision(playerSprite, enemySprite5);
	AID->PlayerEnemyCollision(playerSprite, enemySprite6);
	AID->PlayerEnemyCollision(playerSprite, enemySprite7);
	AID->PlayerEnemyCollision(playerSprite, BossSprite);
};

// Function to replenish player health if they collide with a health pack
void LevelOne::PlayerHealthCollision()
{
	// Ensure player health can't be over 100
	if (playerSprite->GetPlayerHealth() > 100)
	{
		playerSprite->SetPlayerHealth(100);
	}

	//AID Health Pick Up Collision
	AID->PlayerHealthCollision(playerSprite, newHealthPickUpSprite, healthAid);

	// If health Pick up is triggered
	if (healthUp)
	{
		// Use the AI Director to handle collision of player sprite and the health pack that was collided with
		AID->PlayerHealthCollision(playerSprite, healthPickUpSprite, healthUp);
	}

	if (healthUp1)
	{
		AID->PlayerHealthCollision(playerSprite, healthPickUpSprite1, healthUp1);
	}

	if (healthUp2)
	{
		AID->PlayerHealthCollision(playerSprite, healthPickUpSprite2, healthUp2);
	}

	if (healthUp3)
	{
		AID->PlayerHealthCollision(playerSprite, healthPickUpSprite3, healthUp3);
	}

	if (healthUp4)
	{
		AID->PlayerHealthCollision(playerSprite, healthPickUpSprite4, healthUp4);
	}

	if (healthUp5)
	{
		AID->PlayerHealthCollision(playerSprite, healthPickUpSprite5, healthUp5);
	}
};

// Function to replenish player ammo if they collide with a ammo pack
void LevelOne::PlayerAmmoCollision()
{
	// Ensure player ammo can't be over 100
	if (playerSprite->GetPlayerAmmo() > 100)
	{
		playerSprite->SetPlayerAmmo(100);
	}

	// Check For collision with emergency ammo pack through AI Director
	AID->PlayerAmmoCollision(playerSprite, newAmmoPickUpSprite, ammoAid);

	// If ammo Pick up is triggered
	if (ammoUp)
	{
		// Use the AI Director to handle collision of player sprite and the ammo pack that was collided with
		AID->PlayerAmmoCollision(playerSprite, ammoPickUpSprite, ammoUp);
	}

	if (ammoUp1)
	{
		AID->PlayerAmmoCollision(playerSprite, ammoPickUpSprite1, ammoUp1);
	}

	if (ammoUp2)
	{
		AID->PlayerAmmoCollision(playerSprite, ammoPickUpSprite2, ammoUp2);
	}

	if (ammoUp3)
	{
		AID->PlayerAmmoCollision(playerSprite, ammoPickUpSprite3, ammoUp3);
	}

	if (ammoUp4)
	{
		AID->PlayerAmmoCollision(playerSprite, ammoPickUpSprite4, ammoUp4);
	}

	if (ammoUp5)
	{
		AID->PlayerAmmoCollision(playerSprite, ammoPickUpSprite5, ammoUp5);
	}
};

// Bullet Collision with enemy object function
void LevelOne::BulletCollison()
{
	// Use the Ai Director to check for collision between Bullet and Enemy as long as Enemy Sprite is visible
	if (enemyIsVisible)
	{
		AID->EnemyBulletCollision(enemySprite, bulletSprite, enemyIsVisible);
	}

	if (enemy1IsVisible)
	{
		AID->EnemyBulletCollision(enemySprite1, bulletSprite, enemy1IsVisible);
	}

	if (enemy2IsVisible)
	{
		AID->EnemyBulletCollision(enemySprite2, bulletSprite, enemy2IsVisible);
	}

	if (enemy3IsVisible)
	{
		AID->EnemyBulletCollision(enemySprite3, bulletSprite, enemy3IsVisible);
	}

	if (enemy4IsVisible)
	{
		AID->EnemyBulletCollision(enemySprite4, bulletSprite, enemy4IsVisible);
	}

	if (enemy5IsVisible)
	{
		AID->EnemyBulletCollision(enemySprite5, bulletSprite, enemy5IsVisible);
	}

	if (enemy6IsVisible)
	{
		AID->EnemyBulletCollision(enemySprite6, bulletSprite, enemy6IsVisible);
	}

	if (enemy7IsVisible)
	{
		AID->EnemyBulletCollision(enemySprite7, bulletSprite, enemy7IsVisible);
	}

	if (BossBool)
	{
		AID->EnemyBulletCollision(BossSprite, bulletSprite, BossBool);
	}
	
};

// Function for handeling player movement each frame
void LevelOne::PlayerMovement(float dt)
{
	// Follow Camera function to ensure player is centered in screen at all times
	followCamera = Follow::create(playerSprite->GetPlayerSprite(), Rect::ZERO);
	this->runAction(followCamera);

	Node::update(dt);
	auto playerPosition = playerSprite->GetPlayerSprite()->getPosition();


	// Keyboard detection
	if (KeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) || KeyPressed(EventKeyboard::KeyCode::KEY_A))
	{
		playerPosition.x -= 200 * dt;
		playerSprite->GetPlayerSprite()->setRotation(270);
	}

	if (KeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) || KeyPressed(EventKeyboard::KeyCode::KEY_D))
	{
		playerPosition.x += 200 * dt;
		playerSprite->GetPlayerSprite()->setRotation(90);
	}

	if (KeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) || KeyPressed(EventKeyboard::KeyCode::KEY_W))
	{
		playerPosition.y += 200 * dt;
		playerSprite->GetPlayerSprite()->setRotation(0);
	}

	if (KeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) || KeyPressed(EventKeyboard::KeyCode::KEY_S))
	{
		playerPosition.y -= 200 * dt;
		playerSprite->GetPlayerSprite()->setRotation(180);
	}

	if ((KeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) || KeyPressed(EventKeyboard::KeyCode::KEY_A)) && (KeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) || (KeyPressed(EventKeyboard::KeyCode::KEY_W))))
	{
		playerSprite->GetPlayerSprite()->setRotation(300);
	}

	if ((KeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) || KeyPressed(EventKeyboard::KeyCode::KEY_D)) && (KeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) || (KeyPressed(EventKeyboard::KeyCode::KEY_W))))
	{
		playerSprite->GetPlayerSprite()->setRotation(30);
	}

	if ((KeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) || KeyPressed(EventKeyboard::KeyCode::KEY_A)) && (KeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) || (KeyPressed(EventKeyboard::KeyCode::KEY_S))))
	{
		playerSprite->GetPlayerSprite()->setRotation(210);
	}

	if ((KeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) || KeyPressed(EventKeyboard::KeyCode::KEY_D)) && (KeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) || (KeyPressed(EventKeyboard::KeyCode::KEY_S))))
	{
		playerSprite->GetPlayerSprite()->setRotation(150);
	}

	if (playerPosition.x <= (mapOne->getMapSize().width * mapOne->getTileSize().width) && playerPosition.y <= (mapOne->getMapSize().height * mapOne->getTileSize().height) && playerPosition.y >= 0 && playerPosition.x >= 0)
	{
		this->PlayerCollision(playerPosition);
	}

	// Log out player movements
	//log("Player X Position: %f", playerSprite->GetPlayerSprite()->getPosition().x / mapOne->getTileSize().width);
	//log("Player Y Position: %f", ((mapOne->getMapSize().height * mapOne->getTileSize().height) - playerSprite->GetPlayerSprite()->getPosition().y) / mapOne->getTileSize().height);
};

// Initializing the enemy pathfinding function each frame
void LevelOne::EnemyPathFinder(float dt)
{
	// Only pathfind if Enemy is visible
	if (enemyIsVisible)
	{
		// Only PathFind if Enemy can see the player
		if (sightSprite->GetSightSprite()->getBoundingBox().intersectsRect(playerSprite->GetPlayerSprite()->getBoundingBox()))
		{
			// Pass neccessary information to the AI Director and the enemy will be assigned a path and moved along it
			AID->EnemyPathFinding(enemyPathFinding, mapOne, enemySprite, playerSprite, background, dt);
		}	
	}

	if (enemy1IsVisible)
	{
		if (sightSprite1->GetSightSprite()->getBoundingBox().intersectsRect(playerSprite->GetPlayerSprite()->getBoundingBox()))
		{
			AID->EnemyPathFinding(enemyPathFinding, mapOne, enemySprite1, playerSprite, background, dt);
		}
	}

	if (enemy2IsVisible)
	{
		if (sightSprite2->GetSightSprite()->getBoundingBox().intersectsRect(playerSprite->GetPlayerSprite()->getBoundingBox()))
		{
			AID->EnemyPathFinding(enemyPathFinding, mapOne, enemySprite2, playerSprite, background, dt);
		}
	}

	if (enemy3IsVisible)
	{
		if (sightSprite3->GetSightSprite()->getBoundingBox().intersectsRect(playerSprite->GetPlayerSprite()->getBoundingBox()))
		{
			AID->EnemyPathFinding(enemyPathFinding, mapOne, enemySprite3, playerSprite, background, dt);
		}	
	}

	if (enemy4IsVisible)
	{
		if (sightSprite4->GetSightSprite()->getBoundingBox().intersectsRect(playerSprite->GetPlayerSprite()->getBoundingBox()))
		{
			AID->EnemyPathFinding(enemyPathFinding, mapOne, enemySprite4, playerSprite, background, dt);
		}	
	}

	if (enemy5IsVisible)
	{

		if (sightSprite5->GetSightSprite()->getBoundingBox().intersectsRect(playerSprite->GetPlayerSprite()->getBoundingBox()))
		{
			AID->EnemyPathFinding(enemyPathFinding, mapOne, enemySprite5, playerSprite, background, dt);
		}	
	}

	if (enemy6IsVisible)
	{
		if (sightSprite6->GetSightSprite()->getBoundingBox().intersectsRect(playerSprite->GetPlayerSprite()->getBoundingBox()))
		{
			AID->EnemyPathFinding(enemyPathFinding, mapOne, enemySprite6, playerSprite, background, dt);
		}
	}

	if (enemy7IsVisible)
	{
		if (sightSprite7->GetSightSprite()->getBoundingBox().intersectsRect(playerSprite->GetPlayerSprite()->getBoundingBox()))
		{
			AID->EnemyPathFinding(enemyPathFinding, mapOne, enemySprite7, playerSprite, background, dt);
		}
	}

	if (BossBool)
	{
		AID->EnemyPathFinding(enemyPathFinding, mapOne, BossSprite, playerSprite, background, dt);
	}
	
};

// Function to check for the edge of the whole map - used to prevent the player and enemies from moving off map
Vec2 LevelOne::TilePosition(Vec2 tilePosition)
{
	int x = tilePosition.x / mapOne->getTileSize().width;
	int y = ((mapOne->getMapSize().height * mapOne->getTileSize().height) - tilePosition.y) / mapOne->getTileSize().height;

	return Vec2(x, y);
};

// Update each enemy so they scale difficulty dynamically
void LevelOne::EnemyDifficultyScale(float dt)
{
	// Pass in delta time, enemy objects, and the player object
	AID->EnemyDifficultyScale(enemySprite, dt, playerSprite);
	AID->EnemyDifficultyScale(enemySprite1, dt, playerSprite);
	AID->EnemyDifficultyScale(enemySprite2, dt, playerSprite);
	AID->EnemyDifficultyScale(enemySprite3, dt, playerSprite);
	AID->EnemyDifficultyScale(enemySprite4, dt, playerSprite);
	AID->EnemyDifficultyScale(enemySprite5, dt, playerSprite);
	AID->EnemyDifficultyScale(enemySprite6, dt, playerSprite);
	AID->EnemyDifficultyScale(enemySprite7, dt, playerSprite);
};

// Ensure that player collides with Meta Tiles
void LevelOne::PlayerCollision(Vec2 tilePosition)
{
	// Get the position of all tiles that are collidable
	Vec2 collisionTiles = this->TilePosition(tilePosition);
	int tileGID = meta->tileGIDAt(collisionTiles);

	// If the tile is collidable
	if (tileGID)
	{
		// Check properites for tile at this position
		ValueMap collisionTilesProperties = mapOne->propertiesForGID(tileGID).asValueMap();

		// If the tile has property attacthed to it
		if (collisionTilesProperties.size() > 0)
		{
			// Check for the "Collidable" property
			Value collision = collisionTilesProperties["Collidable"];

			// If player collides with the meta tile acknowledge the collision
			if (!collision.isNull() && collision.asString() == "true")
			{
				return;
			}
		}
	}

	// Set Player to a position where they are not colliding
	playerSprite->GetPlayerSprite()->setPosition(tilePosition);
};

// Function to handle player health bar reduction and refil
void LevelOne::PlayerDamage()
{
	// Player health bar is a progress bar within cocos and is 
	// Set to percentages that correspond with players in game health
	if (playerSprite->GetPlayerHealth() == 100)
	{
		this->healthBar->setPercentage(100);
	}
	if (playerSprite->GetPlayerHealth() == 90)
	{
		this->healthBar->setPercentage(90);
	}
	if (playerSprite->GetPlayerHealth() == 80)
	{
		this->healthBar->setPercentage(80);
	}
	if (playerSprite->GetPlayerHealth() == 70)
	{
		this->healthBar->setPercentage(70);
	}
	if (playerSprite->GetPlayerHealth() == 60)
	{
		this->healthBar->setPercentage(60);
	}
	if (playerSprite->GetPlayerHealth() == 50)
	{
		this->healthBar->setPercentage(50);
	}
	if (playerSprite->GetPlayerHealth() == 40)
	{
		this->healthBar->setPercentage(40);
	}
	if (playerSprite->GetPlayerHealth() == 30)
	{
		this->healthBar->setPercentage(30);
	}
	if (playerSprite->GetPlayerHealth() == 20)
	{
		this->healthBar->setPercentage(20);
	}
	if (playerSprite->GetPlayerHealth() == 10)
	{
		this->healthBar->setPercentage(10);
	}
	if (playerSprite->GetPlayerHealth() == 0)
	{
		this->healthBar->setPercentage(0);
	}
	if (playerSprite->GetPlayerHealth() > 100)
	{
		playerSprite->SetPlayerHealth(100);
		this->healthBar->setPercentage(100);
	}
};

// Create the HUD Layer
void LevelOne::CreateHUD()
{
	// Set HUD colour and ensure its added to scene
	HUD = LayerGradient::create(Color4B(0, 0, 0, 0), Color4B(0, 0, 0, 0));
	HUD->setContentSize(visibleSize);
	this->addChild(HUD);

	// Set Hud Image
	cocos2d::Sprite *HUDImage = Sprite::create("HudImage.png");

	// Set Hud Position
	HUDImage->setPosition(visibleSize.width / 2, visibleSize.height - 65);

	HUD->addChild(HUDImage, 2);

	// Add health bar as the progress Timer
	this->healthBar = cocos2d::ProgressTimer::create(healthBarSprite = Sprite::create("HealthBar.png"));

	// Type is BAR
	this->healthBar->setType(ProgressTimerType::BAR);

	// Set which side it subtracts the size from when being reduced
	this->healthBar->setBarChangeRate(Vec2(1, 0));

	// Set the position
	this->healthBar->setPosition(visibleSize.width / 2, visibleSize.height - 50);

	// Ensuring it is visible
	this->healthBar->setVisible(true);

	// Initialize to full at start
	this->healthBar->setPercentage(100);

	// Setting it's midpoint
	this->healthBar->setMidpoint(Vec2(1, 0));
	HUD->addChild(this->healthBar, 2);

	// Adding position and contents for the ammo label
	AmmoLabel = Label::createWithTTF("AMMO: $f", "fonts/Marker Felt.ttf", 30);
	AmmoLabel->setPosition(visibleSize.width / 2 + 400, visibleSize.height - 50);
	HUD->addChild(AmmoLabel, 2);

	// Adding the position and contents for the score label
	ScoreLabel = Label::createWithTTF("Score: $f", "fonts/Marker Felt.ttf", 30);
	ScoreLabel->setPosition(visibleSize.width / 2 - 400, visibleSize.height - 50);
	HUD->addChild(ScoreLabel, 2);
};

/***  Enemy Collision not registering  ***/
void LevelOne::EnemyCollision(Vec2 tilePosition)
{
	Vec2 collisionTiles = this->TilePosition(tilePosition);
	int tileGID = meta->tileGIDAt(collisionTiles);

	if (tileGID)
	{
		ValueMap collisionTilesProperties = mapOne->propertiesForGID(tileGID).asValueMap();

		if (collisionTilesProperties.size() > 0)
		{
			Value collision = collisionTilesProperties["Collidable"];

			if (!collision.isNull() && collision.asString() == "true")
			{
				return;
			}
		}
	}

	enemySprite->GetEnemySprite()->setPosition(tilePosition);
};

// Adding mouse functionality
void LevelOne::MouseListener()
{
	// When the mouse is clicked call the onTouchBegin function that contains all mouse fucntionality
	auto mouseEventListener = EventListenerTouchOneByOne::create();
	mouseEventListener->onTouchBegan = CC_CALLBACK_2(LevelOne::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseEventListener, playerSprite->GetPlayerSprite());
};

// When a mouse click is detected fire a bullet
bool LevelOne::onTouchBegan(Touch *touch, Event *event)
{
	// Only fire if ammo is greater than zero
	if (playerSprite->GetPlayerAmmo() > 0)
	{
		// Create a new bullet sprite
		bulletSprite = new Bullets();

		// Set bullets position
		bulletSprite->GetBulletsSprite()->setPosition(playerSprite->GetPlayerSprite()->getPosition());

		// Add to the scene
		this->addChild(bulletSprite->GetBulletsSprite());

		// Decrease player ammo
		int playerAmmo = playerSprite->GetPlayerAmmo() - 1;
		playerSprite->SetPlayerAmmo(playerAmmo);

		// Call the bullet move function
		BulletMove();

		// Call bullet collision function to detect if bullet has made contact
		BulletCollison();
	}

	return true;
};

// Player Death function to handle player respawn
void LevelOne::PlayerDeath()
{
	// when player is triggered isDead
	if (isDead)
	{
		// Grab spawn point location
		playerSpawnPoint = mapObjectGroup->getObject("SpawnPoint");
		float playerX = playerSpawnPoint["x"].asFloat() * Director::getInstance()->getContentScaleFactor();
		float playerY = playerSpawnPoint["y"].asFloat() * Director::getInstance()->getContentScaleFactor();

		// Decrease Score
		score = score - 10;

		// Ensure score can't be less than zero
		if (score <= 0)
		{
			score = 0;
		}

		// Reset Player Position
		playerSprite->GetPlayerSprite()->setPosition(playerX, playerY);
	}

	// If player is not dead don't trigger isDead
	else 
	{ 
		isDead = false; 
	}
};


// Function used to increase score when needed
int LevelOne::Score()
{
	score = score + 5;

	return score;
};

// Initializing Analytical data 
void LevelOne::ScoreAnalytics()
{
	// Using the AI Director pass in the Enemy Objects and the Current Score to print relevent data to a .TXT file
	AID->ScoreAnalytics(enemySprite, score);
	AID->ScoreAnalytics(enemySprite1, score);
	AID->ScoreAnalytics(enemySprite2, score);
	AID->ScoreAnalytics(enemySprite3, score);
	AID->ScoreAnalytics(enemySprite4, score);
	AID->ScoreAnalytics(enemySprite5, score);
	AID->ScoreAnalytics(enemySprite6, score);
	AID->ScoreAnalytics(enemySprite7, score);
};

// Initializing the Timers 
void LevelOne::Timer(float dt)
{
	// Timers are passing the game Scenes current delta time so that it can accurately make decisions in real time
	AID->HealthTimer(dt);
	AID->AmmoTimer(dt);
	AID->GameTimer(dt);
};

// Bullet Move function determines how the bullet can move based on the direction the player is facing 
void LevelOne::BulletMove()
{
	if (playerSprite->GetPlayerSprite()->getRotation() == 0)
	{
		// Shoot bullet up
		auto bulletMove = MoveBy::create(2.0f, Vec2(0, 1000));
		auto bulletRemove = RemoveSelf::create();
		bulletSprite->GetBulletsSprite()->runAction(Sequence::create(bulletMove, bulletRemove, nullptr));
		BulletCollison();
	}

	if (playerSprite->GetPlayerSprite()->getRotation() == 180)
	{
		// Shoot bullet down
		auto bulletMove = MoveBy::create(2.0f, Vec2(0, -1000));
		auto bulletRemove = RemoveSelf::create();
		bulletSprite->GetBulletsSprite()->runAction(Sequence::create(bulletMove, bulletRemove, nullptr));
		BulletCollison();
	}

	if (playerSprite->GetPlayerSprite()->getRotation() == 270)
	{
		// Shoot bullet left
		auto bulletMove = MoveBy::create(2.0f, Vec2(-1000, 0));
		auto bulletRemove = RemoveSelf::create();
		bulletSprite->GetBulletsSprite()->runAction(Sequence::create(bulletMove, bulletRemove, nullptr));
		BulletCollison();
	}

	if (playerSprite->GetPlayerSprite()->getRotation() == 90)
	{
		// Shoot Bullet right
		auto bulletMove = MoveBy::create(2.0f, Vec2(1000, 0));
		auto bulletRemove = RemoveSelf::create();
		bulletSprite->GetBulletsSprite()->runAction(Sequence::create(bulletMove, bulletRemove, nullptr));
		BulletCollison();
	}

	if (playerSprite->GetPlayerSprite()->getRotation() == 300)
	{
		// Shoot Bullet Left & Up
		auto bulletMove = MoveBy::create(2.0f, Vec2(-1000, 1000));
		auto bulletRemove = RemoveSelf::create();
		bulletSprite->GetBulletsSprite()->runAction(Sequence::create(bulletMove, bulletRemove, nullptr));
		BulletCollison();
	}

	if (playerSprite->GetPlayerSprite()->getRotation() == 30)
	{
		// Shoot buller Right & Up
		auto bulletMove = MoveBy::create(2.0f, Vec2(1000, 1000));
		auto bulletRemove = RemoveSelf::create();
		bulletSprite->GetBulletsSprite()->runAction(Sequence::create(bulletMove, bulletRemove, nullptr));
		BulletCollison();
	}

	if (playerSprite->GetPlayerSprite()->getRotation() == 210)
	{
		// Shoot bullet left & down
		auto bulletMove = MoveBy::create(2.0f, Vec2(-1000, -1000));
		auto bulletRemove = RemoveSelf::create();
		bulletSprite->GetBulletsSprite()->runAction(Sequence::create(bulletMove, bulletRemove, nullptr));
		BulletCollison();
	}

	if (playerSprite->GetPlayerSprite()->getRotation() == 150)
	{
		// Shoot Bullet right & Down
		auto bulletMove = MoveBy::create(2.0f, Vec2(1000, -1000));
		auto bulletRemove = RemoveSelf::create();
		bulletSprite->GetBulletsSprite()->runAction(Sequence::create(bulletMove, bulletRemove, nullptr));
		BulletCollison();
	}
}

// Keyboard Listener - used for player movement
void LevelOne::KeyBoardListener()
{
	auto keyboardEventListener = EventListenerKeyboard::create();
	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);

	// Detect when a key has been pressed
	keyboardEventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (keys.find(keyCode) == keys.end())
		{
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}
	};

	// Detect when a key has been released
	keyboardEventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		keys.erase(keyCode);
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);
};

// If key is pressed determin which one
bool LevelOne::KeyPressed(EventKeyboard::KeyCode code)
{
	if (keys.find(code) != keys.end())
	{
		return true;
	}

	return false;
};

// Determine how long key was pressed for
double LevelOne::KeyPressedDuration(EventKeyboard::KeyCode code)
{
	// Use W, A, S, D keys to move up, down, left, right respectfully
	if (!KeyPressed(EventKeyboard::KeyCode::KEY_W))
	{
		return 0;
	}

	if (!KeyPressed(EventKeyboard::KeyCode::KEY_A))
	{
		return 0;
	}

	if (!KeyPressed(EventKeyboard::KeyCode::KEY_S))
	{
		return 0;
	}

	if (!KeyPressed(EventKeyboard::KeyCode::KEY_D))
	{
		return 0;
	}

	// Use Up, Down, Left, Right arrow keys to move up, down, left, right respectfully
	if (!KeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW))
	{
		return 0;
	}

	if (!KeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW))
	{
		return 0;
	}

	if (!KeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW))
	{
		return 0;
	}

	if (!KeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW))
	{
		return 0;
	}

	if (!KeyPressed(EventKeyboard::KeyCode::KEY_SPACE))
	{
		return 0;
	}

	// Get key press duration in milliseconds based on clock time
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - keys[code]).count();
};

// Get Key Codes
std::map<EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> LevelOne::keys;