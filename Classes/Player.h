//Samuel MacSweeney
//K00200955

#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"

USING_NS_CC;

class Player
{
	public:
		Player();
		~Player();

		// Functions 
		Sprite* GetPlayerSprite();

		int GetPlayerHealth();
		void SetPlayerHealth(int playerHealth);
		int GetPlayerAmmo();
		void SetPlayerAmmo(int playerAmmo);
		int getPlayerDeathCounter();
		void setPlayerDeathCounter(int death);

		//Variables
		Sprite *playerSprite;

		int health = 100;
		int ammo = 10;
		int deathCount = 0;
};

#endif
