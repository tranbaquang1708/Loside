#include "pch.h"
#include "AttackTransform.h"

void AttackTransform::loadFrog(Enemy* _frog)
{
	frog = _frog;
}

unsigned short AttackTransform::getType()
{
	return Attack::Transform;
}

unsigned short AttackTransform::getAilment()
{
	return Ailment::None;
}

void AttackTransform::attack(Enemy* enemy)
{
	*enemy = *frog;
	Attack::attack(enemy->getPosition());
}