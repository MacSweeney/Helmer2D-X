#include "Bullets.h"

// Bullet Constructor
Bullets::Bullets()
{
	bulletsSprite = new Sprite();
	bulletsSprite->initWithFile("BulletSprite.png");
};

// Bullet Destructor
Bullets::~Bullets()
{};

// Get Bullet Sprite Function
Sprite* Bullets::GetBulletsSprite()
{
	return bulletsSprite;
};

