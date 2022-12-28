#pragma once

#include "AttackInterface.h"
#include "AttackFire.h"
#include "AttackFlame.h"

class AttackInterfaceFire : public AttackInterface
{
public:
	void loadAttack(AttackFire* _attackFire, AttackFlame* _attackFlame);
	void attack(Enemy* enemy);

private:
	AttackFire*		attackFire;
	AttackFlame*	attackFlame;
};