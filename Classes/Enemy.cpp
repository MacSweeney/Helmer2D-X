//Samuel MacSweeney
//K00200955

#include "Enemy.h"

// Enemy Constructer to set Sprite
Enemy::Enemy()
{
	enemySprite = new Sprite();
	enemySprite->initWithFile("EnemySprite.png");

};

// Enemy Destructor - not really needed yet
Enemy::~Enemy()
{};

// Create return function to grab specific instance of Sprite
Sprite* Enemy::GetEnemySprite()
{
	return enemySprite;
};

// Create Function to grab sight sprite to set pathfind radius
Sprite* Enemy::GetSightSprite()
{
	return sightSprite;
};

// Create return function to set specific instance of object Health
void Enemy::SetEnemyHealth(int enemyHealth)
{
	health = enemyHealth;
};

// Create return function to grab specific instance of object Health
int Enemy::GetEnemyHealth()
{
	return health;
};

// Set damage output for Enemies
void Enemy::SetEnemyDamage(int enemyDamage)
{
	damage = enemyDamage;
};

// Get the Damage output for Enemies
int Enemy::GetEnemyDamage()
{
	return damage;
};