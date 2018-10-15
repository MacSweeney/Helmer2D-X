//Samuel MacSweeney
//K00200955

#include "SightSprite.h"

// Create a new Sprite with the file EnemySight.png
SightSprite::SightSprite()
{
	sightSprite = new Sprite();
	sightSprite->initWithFile("EnemySight.png");

};

// Create return function to grab specific instance of Sprite
Sprite* SightSprite::GetSightSprite()
{
	return sightSprite;
};