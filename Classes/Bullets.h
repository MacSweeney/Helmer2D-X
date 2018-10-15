//Samuel MacSweeney
//K00200955

#ifndef BULLETS_H
#define BULLETS_H

#include "cocos2d.h"

USING_NS_CC;

using namespace std;

class Bullets
{
	public:

		Bullets();
		~Bullets();

		Sprite* GetBulletsSprite();
		Sprite *bulletsSprite;
};

#endif