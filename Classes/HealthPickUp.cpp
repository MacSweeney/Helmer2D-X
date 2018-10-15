//Samuel MacSweeney
//K00200955

#include "HealthPickUp.h"

USING_NS_CC;

HealthPickUp::HealthPickUp()
{
	healthPickUpSprite = Sprite::create("HealthPickUpSprite.png");
};

HealthPickUp::~HealthPickUp()
{};

Sprite* HealthPickUp::GetHealthPickUpSprite()
{
	return healthPickUpSprite;
};