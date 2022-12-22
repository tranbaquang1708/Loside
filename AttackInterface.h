#pragma once
#include "Enemy.h"
#include "Attack.h"

class AttackInterface {
public:
	void loadAttacks(Attack* inFireAttack, Attack* flameAttack);

	void setFireCoolDownTime(float _fireCoolDownTime);

	float getFireCoolDownTime();
	float getFireCoolDownPassedTime();
	bool getIsFireCoolingDown();

	void update(float elapsedTime);
	void attackFire(Enemy* enemy);

private:
	float									fireCoolDownPassedTime;
	float									fireCoolDownTime;
	bool									isFireCoolingDown;

	Attack*									fireAttack;
	Attack*									flameAttack;
};
