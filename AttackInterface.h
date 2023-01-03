#pragma once
#include "Enemy.h"
#include "Attack.h"

class AttackInterface {
public:
<<<<<<< HEAD
	void setCoolDownTime(float _coolDownTime);
=======
	//void loadAttacks(Attack* inFireAttack, Attack* flameAttack, Attack* stoneAttack);

	void setCoolDownTime(float _coolDownTime);
	//void setStoneCoolDownTime(float _coolDownTime);
>>>>>>> origin/master

	float getCoolDownTime();
	float getCoolDownPassedTime();
	bool getIsCoolingDown();

<<<<<<< HEAD
	void update(float elapsedTime);

	virtual void cancelAllAttacks() = 0;
=======
	/*float getStoneCoolDownTime();
	float getStoneCoolDownPassedTime();
	bool getIsStoneCoolingDown();*/

	void update(float elapsedTime);

	//virtual void attack(Enemy* enemy) = 0;
	//void attackStone(Enemy* enemy, float protagonistPositionX);
>>>>>>> origin/master

protected:
	void setCoolDownPassedTime(float _coolDownPassedTime);
	void setIsCoolingDown(bool _isCoolingDown);

private:
<<<<<<< HEAD
	float									coolDownPassedTime;
	float									coolDownTime;
	bool									isCoolingDown;
=======
	//Attack*									fireAttack;
	//Attack*									flameAttack;
	//Attack*									stoneAttack;

	float									coolDownPassedTime;
	float									coolDownTime;
	bool									isCoolingDown;

	//float									stoneCoolDownPassedTime;
	//float									stoneCoolDownTime;
	//bool									isStoneCoolingDown;
>>>>>>> origin/master
};
