#include "pch.h"
#include "AttackInterfaceTransform.h"

void AttackInterfaceTransform::loadAttack(AttackTransform* _attackTransform)
{
	attackTransform = _attackTransform;
}

void AttackInterfaceTransform::attack(Enemy* enemy)
{
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
}
