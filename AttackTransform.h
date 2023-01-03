#pragma once

#include "Attack.h"
#include "Enemy.h"

class AttackTransform : public Attack
{
public:
	void loadFrog(Enemy* _frog);
	void loadEnemies(std::list<Enemy>* _enemies);

	unsigned short getType();
	unsigned short getAilment();

	void attack(Enemy* enemy);

private:
	std::list<Enemy>* enemies;
	Enemy* frog;
};