#include "pch.h"
#include "AttackTransform.h"

void AttackTransform::loadFrog(Enemy* _frog)
{
	frog = _frog;
}

void AttackTransform::loadEnemies(std::list<Enemy>* _enemies)
{
	enemies = _enemies;
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
	Attack::attack(enemy->getPosition());

	//if (enemy->getIsPetrified()) return;

	if (enemy->getTransformState() == Enemy::TransformState::Frog) {
		enemy->getAttacked(getAnimationTime() + 0.1f);
	}
	else {
		enemy->setVisibilityState(Enemy::VisibilityState::NonExistence);
		frog->setPosition(DirectX::XMFLOAT2(enemy->getPosition().x, 0.832f));
		frog->getAttacked(getAnimationTime() + 0.1f);
		enemies->push_front(*frog);
	}
}