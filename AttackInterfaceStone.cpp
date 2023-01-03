#include "pch.h"
#include "AttackInterfaceStone.h"

void AttackInterfaceStone::loadAttack(AttackStone* _attackStone)
{
	attackStone = _attackStone;
}

void AttackInterfaceStone::attack(Enemy* enemy, float protagonistPositionX)
{
	setCoolDownPassedTime(0.f);
	setIsCoolingDown(true);

	DirectX::XMFLOAT2 position(enemy->getPosition().x, 0.835f);

	attackStone->attack(position);

	if (protagonistPositionX < enemy->getPosition().x) {
		enemy->getPushedBack(0.05, 0.35f);
	}
	else {
		enemy->getPushedBack(-0.05, 0.35f);
	}

	enemy->setAilment(attackStone->getAilment());
}

void AttackInterfaceStone::cancelAllAttacks()
{
	attackStone->setIsOn(false);
}