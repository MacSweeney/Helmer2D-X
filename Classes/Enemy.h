//Samuel MacSweeney
//K00200955

#ifndef  ENEMY_H
#define ENEMY_H

#include "cocos2d.h"

USING_NS_CC;

class Enemy
{
	public:
		Enemy();
		~Enemy();

		// Functions 
		Sprite* GetEnemySprite();
		Sprite* GetSightSprite();

		int GetEnemyHealth();
		void SetEnemyHealth(int enemyHealth);
		int GetEnemyDamage();
		void SetEnemyDamage(int enemyDamage);

		// Variables
		Sprite *enemySprite;
		Sprite *sightSprite;

		int health;
		int damage;
};

#endif