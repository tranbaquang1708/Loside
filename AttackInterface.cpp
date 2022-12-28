#include "pch.h"
#include "AttackInterface.h"

//void AttackInterface::loadAttacks(Attack* inFireAttack, Attack* inFlameAttack, Attack* inStoneAttack)
//{
//	fireAttack = inFireAttack;
//	flameAttack = inFlameAttack;
//	stoneAttack = inStoneAttack;
//
//	isFireCoolingDown = false;
//	isStoneCoolingDown = false;
//}

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

//void AttackInterface::setStoneCoolDownTime(float _coolDownTime)
//{
//	stoneCoolDownTime = _coolDownTime;
//	isStoneCoolingDown = false;
//}

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

//float AttackInterface::getStoneCoolDownTime()
//{
//	return stoneCoolDownTime;
//}
//
//float AttackInterface::getStoneCoolDownPassedTime()
//{
//	return stoneCoolDownPassedTime;
//}
//
//bool AttackInterface::getIsStoneCoolingDown()
//{
//	return isStoneCoolingDown;
//}

void AttackInterface::update(float elapsedTime)
{
	if (isCoolingDown) {
		coolDownPassedTime += elapsedTime;
		if (coolDownPassedTime >= coolDownTime) {
			isCoolingDown = false;
		}
	}

	/*if (isStoneCoolingDown) {
		stoneCoolDownPassedTime += elapsedTime;
		if (stoneCoolDownPassedTime >= stoneCoolDownTime) {
			isStoneCoolingDown = false;
		}
	}*/
}

//void AttackInterface::attack(Enemy* enemy)
//{
//	if (isFireCoolingDown) return;
//
//	fireCoolDownPassedTime = 0.f;
//	isFireCoolingDown = true;
//
//	DirectX::XMFLOAT2 position(enemy->getPosition().x, enemy->getPosition().y - enemy->getTextureSize().y / 2.f);
//
//	if (enemy->getAilment() == Ailment::Fire) {
//		position.y += flameAttack->getSize().y / 3.8f;
//
//		flameAttack->attack(position);
//
//		enemy->getAttacked(flameAttack->getAnimationTime() + 0.1f);
//		enemy->setAilment(flameAttack->getAilment());
//	}
//	else {
//		position.y += fireAttack->getSize().y / 2.2f;
//
//		fireAttack->attack(position);
//
//		enemy->getAttacked(fireAttack->getAnimationTime() + 0.1f);
//		enemy->setAilment(fireAttack->getAilment());
//	}
//}

//void AttackInterface::attackStone(Enemy* enemy, float protagonistPositionX)
//{
//	if (isStoneCoolingDown) return;
//
//	stoneCoolDownPassedTime = 0.f;
//	isStoneCoolingDown = true;
//
//	DirectX::XMFLOAT2 position(enemy->getPosition().x, enemy->getPosition().y - enemy->getTextureSize().y / 2.f);
//
//	position.y += stoneAttack->getSize().y / 2.2f;
//
//	stoneAttack->attack(position);
//
//	if (protagonistPositionX < enemy->getPosition().x) {
//		enemy->getPushedBack(0.05, 0.35f);
//	}
//	else {
//		enemy->getPushedBack(-0.05, 0.35f);
//	}
//	
//	enemy->setAilment(stoneAttack->getAilment());
//}