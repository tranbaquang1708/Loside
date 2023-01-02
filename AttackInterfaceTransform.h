#pragma once

#include "AttackInterface.h"
#include "AttackTransform.h"
#include "AttackPetrification.h"

class AttackInterfaceTransform : public AttackInterface
{
public:
	void loadAttack(AttackTransform* _attackTransform, AttackPetrification* _attackPetrfication);
	void attack(Enemy* enemy);

	void cancelAllAttacks() override;

private:
	AttackTransform*		attackTransform;
	AttackPetrification*	attackPetrification;
};