#pragma once

#include "AttackInterface.h"
#include "AttackTransform.h"
<<<<<<< HEAD
#include "AttackPetrification.h"
=======
>>>>>>> origin/master

class AttackInterfaceTransform : public AttackInterface
{
public:
<<<<<<< HEAD
	void loadAttack(AttackTransform* _attackTransform, AttackPetrification* _attackPetrfication);
	void attack(Enemy* enemy);

	void cancelAllAttacks() override;

private:
	AttackTransform*		attackTransform;
	AttackPetrification*	attackPetrification;
=======
	void loadAttack(AttackTransform* _attackTransform);
	void attack(Enemy* enemy);

private:
	AttackTransform* attackTransform;
>>>>>>> origin/master
};