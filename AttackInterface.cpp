#include "pch.h"
#include "AttackInterface.h"

void AttackInterface::setCoolDownTime(float _coolDownTime)
{
	coolDownTime = _coolDownTime;
	isCoolingDown = false;
}

void AttackInterface::setCoolDownPassedTime(float _coolDownPassedTime)
{
	coolDownPassedTime = _coolDownPassedTime;
}

void AttackInterface::setIsCoolingDown(bool _isCoolingDown)
{
	isCoolingDown = _isCoolingDown;
}

float AttackInterface::getCoolDownTime()
{
	return coolDownTime;
}

float AttackInterface::getCoolDownPassedTime()
{
	return coolDownPassedTime;
}

bool AttackInterface::getIsCoolingDown()
{
	return isCoolingDown;
}

void AttackInterface::update(float elapsedTime)
{
	if (isCoolingDown) {
		coolDownPassedTime += elapsedTime;
		if (coolDownPassedTime >= coolDownTime) {
			isCoolingDown = false;
		}
	}
}