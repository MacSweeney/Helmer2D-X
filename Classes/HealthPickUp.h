//Samuel MacSweeney
//K00200955

#ifndef HEALTHPICKUP_H
#define HEALTHPICKUP_H

#include "cocos2d.h"

class HealthPickUp
{
	public:
		HealthPickUp();
		~HealthPickUp();

		cocos2d::Sprite* GetHealthPickUpSprite();
		cocos2d::Sprite* healthPickUpSprite;
};

#endif 