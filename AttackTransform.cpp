#include "pch.h"
#include "AttackTransform.h"

<<<<<<< HEAD
void AttackTransform::loadFrog(Enemy* _frog)
{
	frog = _frog;
}

void AttackTransform::loadEnemies(std::list<Enemy>* _enemies)
{
	enemies = _enemies;
}

=======
>>>>>>> origin/master
unsigned short AttackTransform::getType()
{
	return Attack::Transform;
}

unsigned short AttackTransform::getAilment()
{
	return Ailment::None;
<<<<<<< HEAD
}

void AttackTransform::attack(Enemy* enemy)
{
	Attack::attack(enemy->getPosition());

	if (enemy->getTransformState() == Enemy::TransformState::Frog) {
		enemy->getAttacked(getAnimationTime() + 0.1f);
	}
	else {
		enemy->setVisibilityState(Enemy::VisibilityState::NonExistence);
		frog->setPosition(DirectX::XMFLOAT2(enemy->getPosition().x, 0.832f));
		frog->getAttacked(getAnimationTime() + 0.1f);
		enemies->push_front(*frog);
	}
=======
>>>>>>> origin/master
}