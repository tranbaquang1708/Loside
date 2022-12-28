#pragma once

#include "AttackInterface.h"
#include "AttackTransform.h"

class AttackInterfaceTransform : public AttackInterface
{
public:
	void loadAttack(AttackTransform* _attackTransform);
	void attack(Enemy* enemy);

private:
	AttackTransform* attackTransform;
};