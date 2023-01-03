#include "pch.h"
#include "AttackInterfaceTransform.h"

<<<<<<< HEAD
void AttackInterfaceTransform::loadAttack(AttackTransform* _attackTransform, AttackPetrification* _attackPetrification)
{
	attackTransform = _attackTransform;
	attackPetrification = _attackPetrification;
=======
void AttackInterfaceTransform::loadAttack(AttackTransform* _attackTransform)
{
	attackTransform = _attackTransform;
>>>>>>> origin/master
}

void AttackInterfaceTransform::attack(Enemy* enemy)
{
<<<<<<< HEAD
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
=======
	if (getIsCoolingDown()) return;

	setCoolDownPassedTime(0.f);
	setIsCoolingDown(true);

	DirectX::XMFLOAT2 position(enemy->getPosition().x, enemy->getPosition().y - enemy->getTextureSize().y / 2.f);

	/*if (enemy->getAilment() == Ailment::Fire) {
		position.y += attackFlame->getSize().y / 3.8f;

		attackFlame->attack(position);

		enemy->getAttacked(attackFlame->getAnimationTime() + 0.1f);
		enemy->setAilment(attackFlame->getAilment());
	}*/
	//else {
		position.y += attackTransform->getSize().y / 2.2f;

		attackTransform->attack(position);

		enemy->getAttacked(attackTransform->getAnimationTime() + 0.1f);
		enemy->setAilment(attackTransform->getAilment());
	//}
>>>>>>> origin/master
}
