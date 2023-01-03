#include "pch.h"
#include "AttackInterfaceFire.h"

void AttackInterfaceFire::loadAttack(AttackFire* _attackFire, AttackFlame* _attackFlame)
{
	attackFire = _attackFire;
	attackFlame = _attackFlame;
}

void AttackInterfaceFire::attack(Enemy* enemy)
{
<<<<<<< HEAD
=======
	if (getIsCoolingDown()) return;

>>>>>>> origin/master
	setCoolDownPassedTime(0.f);
	setIsCoolingDown(true);

	DirectX::XMFLOAT2 position(enemy->getPosition().x, enemy->getPosition().y - enemy->getTextureSize().y / 2.f);

	if (enemy->getAilment() == Ailment::Fire) {
		position.y += attackFlame->getSize().y / 3.8f;

		attackFlame->attack(position);

		enemy->getAttacked(attackFlame->getAnimationTime() + 0.1f);
		enemy->setAilment(attackFlame->getAilment());
	}
	else {
		position.y += attackFire->getSize().y / 2.2f;

		attackFire->attack(position);

		enemy->getAttacked(attackFire->getAnimationTime() + 0.1f);
		enemy->setAilment(attackFire->getAilment());
	}
<<<<<<< HEAD
}

void AttackInterfaceFire::cancelAllAttacks()
{
	attackFire->setIsOn(false);
	attackFlame->setIsOn(false);
=======
>>>>>>> origin/master
}