#include "pch.h"
#include "AttackInterfaceStone.h"

void AttackInterfaceStone::loadAttack(AttackStone* _attackStone)
{
	attackStone = _attackStone;
}

void AttackInterfaceStone::attack(Enemy* enemy, float protagonistPositionX)
{
<<<<<<< HEAD
	setCoolDownPassedTime(0.f);
	setIsCoolingDown(true);
	
	DirectX::XMFLOAT2 position(enemy->getPosition().x, 0.835f);

=======
	if (getIsCoolingDown()) return;

	setCoolDownPassedTime(0.f);
	setIsCoolingDown(true);
	
	DirectX::XMFLOAT2 position(enemy->getPosition().x, enemy->getPosition().y - enemy->getTextureSize().y / 2.f);
	
	position.y += attackStone->getSize().y / 2.2f;
	
>>>>>>> origin/master
	attackStone->attack(position);
	
	if (protagonistPositionX < enemy->getPosition().x) {
		enemy->getPushedBack(0.05, 0.35f);
	}
	else {
		enemy->getPushedBack(-0.05, 0.35f);
	}
		
	enemy->setAilment(attackStone->getAilment());
}
<<<<<<< HEAD

void AttackInterfaceStone::cancelAllAttacks()
{
	attackStone->setIsOn(false);
}
=======
>>>>>>> origin/master
