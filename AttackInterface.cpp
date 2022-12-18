#include "pch.h"
#include "AttackInterface.h"

void AttackInterface::loadAttacks(Attack* inFireAttack, Attack* inFlameAttack)
{
	fireAttack = inFireAttack;
	flameAttack = inFlameAttack;

	isFireCoolingDown = false;
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

void AttackInterface::attackFire(Enemy* enemy)
{
	if (isFireCoolingDown) return;

	fireCoolDownPassedTime = 0.f;
	isFireCoolingDown = true;

	DirectX::XMFLOAT2 position(enemy->getPosition().x, enemy->getPosition().y - enemy->getTextureSize().y / 2.f);

	if (enemy->getAilment() == Attack::Fire) {
		position.y += flameAttack->size.y / 3.f;

		flameAttack->attack(position);

		enemy->getAttacked(flameAttack->getAnimationTime() + 1.f/60);
		enemy->setAilment(Attack::None);
	}
	else {
		position.y += fireAttack->size.y / 2.2f;

		fireAttack->attack(position);

		enemy->getAttacked(fireAttack->getAnimationTime() + 1.f/60);
		enemy->setAilment(Attack::Fire);
	}

}
