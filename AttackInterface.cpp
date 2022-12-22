#include "pch.h"
#include "AttackInterface.h"

void AttackInterface::loadAttacks(Attack* inFireAttack, Attack* inFlameAttack)
{
	fireAttack = inFireAttack;
	flameAttack = inFlameAttack;

	isFireCoolingDown = false;
}

void AttackInterface::setFireCoolDownTime(float _fireCoolDownTime)
{
	fireCoolDownTime = _fireCoolDownTime;
}

void AttackInterface::update(float elapsedTime)
{
	if (isFireCoolingDown) {
		fireCoolDownPassedTime += elapsedTime;
		if (fireCoolDownPassedTime >= fireCoolDownTime) {
			isFireCoolingDown = false;
		}
	}
}

float AttackInterface::getFireCoolDownTime()
{
	return fireCoolDownTime;
}

float AttackInterface::getFireCoolDownPassedTime()
{
	return fireCoolDownPassedTime;
}

bool AttackInterface::getIsFireCoolingDown()
{
	return isFireCoolingDown;
}

void AttackInterface::attackFire(Enemy* enemy)
{
	if (isFireCoolingDown) return;

	fireCoolDownPassedTime = 0.f;
	isFireCoolingDown = true;

	DirectX::XMFLOAT2 position(enemy->getPosition().x, enemy->getPosition().y - enemy->getTextureSize().y / 2.f);

	if (enemy->getAilment() == Ailment::Fire) {
		position.y += flameAttack->getSize().y / 3.8f;

		flameAttack->attack(position);

		enemy->getAttacked(flameAttack->getAnimationTime() + 0.1f);
		enemy->setAilment(flameAttack->getAilment());
	}
	else {
		position.y += fireAttack->getSize().y / 2.2f;

		fireAttack->attack(position);

		enemy->getAttacked(fireAttack->getAnimationTime() + 0.1f);
		enemy->setAilment(fireAttack->getAilment());
	}
}