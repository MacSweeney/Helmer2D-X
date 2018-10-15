//Samuel MacSweeney
//K00200955

#include "Player.h"

// Player Constructer to set Sprite
Player::Player()
{
	playerSprite = new Sprite();
	playerSprite->initWithFile("PlayerSprite.png");
};

// Player Destructor - not really needed yet
Player::~Player()
{};

// Create return function to grab specific instance of Sprite
Sprite* Player::GetPlayerSprite()
{
	return playerSprite;
};

// Create return function to set specific instance of object Health
void Player::SetPlayerHealth(int playerHealth)
{
	health = playerHealth;
};

// Create return function to grab specific instance of object Health
int Player::GetPlayerHealth()
{
	return health;
};

// Create return function to set specific instance of Object Ammo
void Player::SetPlayerAmmo(int playerAmmo)
{
	ammo = playerAmmo;
};

// Create return function to grab specific instance of Object Ammo
int Player::GetPlayerAmmo()
{
	return ammo;
};

// Create return function to set specific instance of Player Death Count
void Player::setPlayerDeathCounter(int death)
{
	deathCount = death;
};

// Create return function to grab specific instance of Player Death Count
int Player::getPlayerDeathCounter()
{
	return deathCount;
};