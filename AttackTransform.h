#pragma once

#include "Attack.h"
<<<<<<< HEAD
#include "Enemy.h"
=======
>>>>>>> origin/master

class AttackTransform : public Attack
{
public:
<<<<<<< HEAD
	void loadFrog(Enemy* _frog);
	void loadEnemies(std::list<Enemy>* _enemies);

	unsigned short getType();
	unsigned short getAilment();

	void attack(Enemy* enemy);

private:
	std::list<Enemy> *enemies;
	Enemy* frog;
=======
	unsigned short getType();
	unsigned short getAilment();
>>>>>>> origin/master
};