#pragma once

#include "AttackInterface.h"
#include "AttackStone.h"

class AttackInterfaceStone : public AttackInterface
{
public:
	void loadAttack(AttackStone* _attackStone);
	void attack(Enemy* enemy, float protagonistPositionX);

<<<<<<< HEAD
	void cancelAllAttacks() override;

=======
>>>>>>> origin/master
private:
	AttackStone* attackStone;
};