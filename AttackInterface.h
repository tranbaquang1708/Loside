#pragma once
#include "Enemy.h"
#include "Attack.h"

class AttackInterface {
public:
	//void loadAttacks(Attack* inFireAttack, Attack* flameAttack, Attack* stoneAttack);

	void setCoolDownTime(float _coolDownTime);
	//void setStoneCoolDownTime(float _coolDownTime);

	float getCoolDownTime();
	float getCoolDownPassedTime();
	bool getIsCoolingDown();

	/*float getStoneCoolDownTime();
	float getStoneCoolDownPassedTime();
	bool getIsStoneCoolingDown();*/

	void update(float elapsedTime);

	//virtual void attack(Enemy* enemy) = 0;
	//void attackStone(Enemy* enemy, float protagonistPositionX);

protected:
	void setCoolDownPassedTime(float _coolDownPassedTime);
	void setIsCoolingDown(bool _isCoolingDown);

private:
	//Attack*									fireAttack;
	//Attack*									flameAttack;
	//Attack*									stoneAttack;

	float									coolDownPassedTime;
	float									coolDownTime;
	bool									isCoolingDown;

	//float									stoneCoolDownPassedTime;
	//float									stoneCoolDownTime;
	//bool									isStoneCoolingDown;
};
