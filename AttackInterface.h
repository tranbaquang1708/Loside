#pragma once
#include "Enemy.h"
#include "Attack.h"

class AttackInterface {
public:
	void loadAttacks(Attack* inFireAttack, Attack* flameAttack, Attack* stoneAttack);

	void setFireCoolDownTime(float _fireCoolDownTime);
	void setStoneCoolDownTime(float _coolDownTime);

	float getFireCoolDownTime();
	float getFireCoolDownPassedTime();
	bool getIsFireCoolingDown();

	float getStoneCoolDownTime();
	float getStoneCoolDownPassedTime();
	bool getIsStoneCoolingDown();

	void update(float elapsedTime);
	void attackFire(Enemy* enemy);
	void attackStone(Enemy* enemy, float protagonistPositionX);

private:
	Attack*									fireAttack;
	Attack*									flameAttack;
	Attack*									stoneAttack;

	float									fireCoolDownPassedTime;
	float									fireCoolDownTime;
	bool									isFireCoolingDown;

	float									stoneCoolDownPassedTime;
	float									stoneCoolDownTime;
	bool									isStoneCoolingDown;
};
