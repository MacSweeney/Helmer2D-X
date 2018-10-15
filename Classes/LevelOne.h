//Samuel MacSweeney
//K00200955

#ifndef LEVELONE_H
#define LEVELONE_H

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include "SightSprite.h"
#include "Bullets.h"
#include "HealthPickUp.h"
#include "AmmoPickUp.h"
#include "PathFinder.h"
#include "AIDirector.h"
#include "SimpleAudioEngine.h"
#include <vector>
#include <math.h>

USING_NS_CC;
using namespace std;

class LevelOne : public Scene
{
public:

	// Functions 

	static Scene* createScene();
	virtual bool init();
	void update(float dt);

	bool CreateMap();
	void CreatePlayer();
	void CreateEnemies();
	void CreateSightSprite();
	void RespawnEnemies();
	void RespawnHealth();
	void RespawnAmmo();
	void CreateHUD();
	void CreateHealth();
	void CreateAmmo();
	void PlayerHealthCollision();
	void PlayerEnemyCollision();
	void PlayerAmmoCollision();
	void BulletCollison();
	void PlayerDamage();
	void PlayerMovement(float dt);
	void PlayerCollision(Vec2 tilePosition);
	void EnemyCollision(Vec2 tilePosition);
	void EnemyPathFinder(float dt);
	void EnemyDifficultyScale(float dt);
	void BulletMove();
	void PlayerDeath();
	void Timer(float dt);
	void MouseListener();
	void KeyBoardListener();
	bool onTouchBegan(Touch *touch, Event *event);
	bool KeyPressed(EventKeyboard::KeyCode);
	int Score();
	void ScoreAnalytics();
	double KeyPressedDuration(EventKeyboard::KeyCode);
	static std::map<EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> keys;

	// Variables

	Vec2 TilePosition(Vec2 tilePosition);

	cocos2d::ProgressTimer *healthBar;
	cocos2d::Sprite* healthBarSprite;
	cocos2d::Label *AmmoLabel;
	cocos2d::Label *ScoreLabel;
	cocos2d::LayerGradient *HUD;
	cocos2d::Size visibleSize;
	cocos2d::Follow* followCamera;

	bool enemyIsVisible = true;
	bool enemy1IsVisible = true;
	bool enemy2IsVisible = true;
	bool enemy3IsVisible = true;
	bool enemy4IsVisible = true;
	bool enemy5IsVisible = true;
	bool enemy6IsVisible = true;
	bool enemy7IsVisible = true;
	bool BossBool = true;
	bool isDead = false;

	CREATE_FUNC(LevelOne);

	TMXTiledMap *mapOne;
	TMXLayer *background;
	TMXLayer *meta;
	TMXObjectGroup *mapObjectGroup;
	ValueMap playerSpawnPoint;
	ValueMap enemySpawnPoint;
	ValueMap enemySpawnPoint1;
	ValueMap enemySpawnPoint2;
	ValueMap enemySpawnPoint3;
	ValueMap enemySpawnPoint4;
	ValueMap enemySpawnPoint5;
	ValueMap enemySpawnPoint6;
	ValueMap enemySpawnPoint7;
	ValueMap healthSpawnPoint;
	ValueMap healthSpawnPoint1;
	ValueMap healthSpawnPoint2;
	ValueMap healthSpawnPoint3;
	ValueMap healthSpawnPoint4;
	ValueMap healthSpawnPoint5;
	ValueMap ammoSpawnPoint;
	ValueMap ammoSpawnPoint1;
	ValueMap ammoSpawnPoint2;
	ValueMap ammoSpawnPoint3;
	ValueMap ammoSpawnPoint4;
	ValueMap ammoSpawnPoint5;
	AIDirector *AID = new AIDirector();
	Player *playerSprite = new Player();
	Enemy *enemySprite = new Enemy();
	Enemy *enemySprite1 = new Enemy();
	Enemy *enemySprite2 = new Enemy();
	Enemy *enemySprite3 = new Enemy();
	Enemy *enemySprite4 = new Enemy();
	Enemy *enemySprite5 = new Enemy();
	Enemy *enemySprite6 = new Enemy();
	Enemy *enemySprite7 = new Enemy();
	Enemy *BossSprite = new Enemy();
	SightSprite *sightSprite = new SightSprite();
	SightSprite *sightSprite1 = new SightSprite();
	SightSprite *sightSprite2 = new SightSprite();
	SightSprite *sightSprite3 = new SightSprite();
	SightSprite *sightSprite4 = new SightSprite();
	SightSprite *sightSprite5 = new SightSprite();
	SightSprite *sightSprite6 = new SightSprite();
	SightSprite *sightSprite7 = new SightSprite();
	SightSprite *BossSight = new SightSprite();
	Bullets *bulletSprite = new Bullets();
	Sprite *thisBullet = new Sprite();
	HealthPickUp *healthPickUpSprite = new HealthPickUp();
	HealthPickUp *healthPickUpSprite1 = new HealthPickUp();
	HealthPickUp *healthPickUpSprite2 = new HealthPickUp();
	HealthPickUp *healthPickUpSprite3 = new HealthPickUp();
	HealthPickUp *healthPickUpSprite4 = new HealthPickUp();
	HealthPickUp *healthPickUpSprite5 = new HealthPickUp();
	HealthPickUp *newHealthPickUpSprite = new HealthPickUp();
	AmmoPickUp *ammoPickUpSprite = new AmmoPickUp();
	AmmoPickUp *ammoPickUpSprite1 = new AmmoPickUp();
	AmmoPickUp *ammoPickUpSprite2 = new AmmoPickUp();
	AmmoPickUp *ammoPickUpSprite3 = new AmmoPickUp();
	AmmoPickUp *ammoPickUpSprite4 = new AmmoPickUp();
	AmmoPickUp *ammoPickUpSprite5 = new AmmoPickUp();
	AmmoPickUp *newAmmoPickUpSprite = new AmmoPickUp();
	PathFinder *enemyPathFinding;
	bool healthUp = true;
	bool healthUp1 = true;
	bool healthUp2 = true;
	bool healthUp3 = true;
	bool healthUp4 = true;
	bool healthUp5 = true;
	bool healthAid;
	bool ammoAid;
	bool ammoUp = true;
	bool ammoUp1 = true;
	bool ammoUp2 = true;
	bool ammoUp3 = true;
	bool ammoUp4 = true;
	bool ammoUp5 = true;

	std::vector<Vec2> enemySpawnLocations;
	std::vector<Vec2>::iterator enemySpawnIterator;

	int score = 0;
	int cTime = 0;
	float previousangle;
	float angle;
	int deathCount = 0;
};

#endif