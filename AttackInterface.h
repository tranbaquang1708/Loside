#pragma once
#include "Enemy.h"
#include "Attack.h"

class AttackInterface {
public:
	void setCoolDownTime(float _coolDownTime);

	float getCoolDownTime();
	float getCoolDownPassedTime();
	bool getIsCoolingDown();

	void update(float elapsedTime);

	virtual void cancelAllAttacks() = 0;

protected:
	void setCoolDownPassedTime(float _coolDownPassedTime);
	void setIsCoolingDown(bool _isCoolingDown);

private:
	float									coolDownPassedTime;
	float									coolDownTime;
	bool									isCoolingDown;
};
