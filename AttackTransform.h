#pragma once

#include "Attack.h"
#include "Enemy.h"

class AttackTransform : public Attack
{
public:
	void loadFrog(Enemy* _frog);

	unsigned short getType();
	unsigned short getAilment();

	void attack(Enemy* enemy);

private:
	//std::list<Frog> frogs;
	Enemy* frog;
};