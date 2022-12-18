#pragma once
#include "Enemy.h"
#include "Attack.h"

struct AttackInterface {
	void loadAttacks(Attack* inFireAttack, Attack* flameAttack);
	void update(float elapsedTime);
	void attackFire(Enemy* enemy);

	float									fireCoolDownPassedTime;
	float									fireCoolDownTime;

	bool									isFireCoolingDown;

	Attack*									fireAttack;
	Attack*									flameAttack;
};
