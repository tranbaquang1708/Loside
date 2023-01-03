#include "pch.h"
#include "AttackInterfaceTransform.h"

void AttackInterfaceTransform::loadAttack(AttackTransform* _attackTransform, AttackPetrification* _attackPetrification)
{
	attackTransform = _attackTransform;
	attackPetrification = _attackPetrification;
}

void AttackInterfaceTransform::attack(Enemy* enemy)
{
	setCoolDownPassedTime(0.f);
	setIsCoolingDown(true);

	if (enemy->getAilment() == Ailment::Stone) {
		DirectX::XMFLOAT2 position(
			enemy->getPosition().x + enemy->getTextureSize().x * 0.5f,
			enemy->getPosition().y - enemy->getTextureSize().y * 0.5f + attackPetrification->getSize().y * 0.5f
		);

		attackPetrification->attack(position);

		if (enemy->getTransformState() == Enemy::TransformState::Frog) {
			enemy->straightFall();
		}

		enemy->getPetrified(attackPetrification->getAnimationTime() + 1.2f);
		enemy->setAilment(attackPetrification->getAilment());
	}
	else {
		attackTransform->attack(enemy);

		enemy->setAilment(attackTransform->getAilment());
	}
}

void AttackInterfaceTransform::cancelAllAttacks()
{
	attackTransform->setIsOn(false);
	attackPetrification->setIsOn(false);
}
