//Samuel MacSweeney
//K00200955

#ifndef AMMOPICKUP_H
#define AMMOPICKUP_H

#include "cocos2d.h"

class AmmoPickUp
{
	public:
		AmmoPickUp();
		~AmmoPickUp();

		cocos2d::Sprite* GetAmmoPickUpSprite();
		cocos2d::Sprite* ammoPickUpSprite;
};

#endif 