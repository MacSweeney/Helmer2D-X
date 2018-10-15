//Samuel MacSweeney
//K00200955

#include "AmmoPickUp.h"

USING_NS_CC;

// AmmoPickUp Constructor
AmmoPickUp::AmmoPickUp()
{
	ammoPickUpSprite = Sprite::create("AmmoPickUp.png");
};

// AmmoPickUp Destructor
AmmoPickUp::~AmmoPickUp()
{};

// Get the AmmoPickUp Sprite
Sprite* AmmoPickUp::GetAmmoPickUpSprite()
{
	return ammoPickUpSprite;
};