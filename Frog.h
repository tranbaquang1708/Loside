#pragma once
#include "Enemy.h"

class Frog : public Enemy
{
public:
	enum FrogAttackedState : unsigned short
	{
		None,
		Flipped
	};

	void loadFlippedAnimation(std::vector<DirectX::XMFLOAT2> sampledTrajectory,
		std::vector<float> sampledAngles,
		float duration = 0.f, float frameTime = 0.f);

	void playFlippedAnimation(float elapsedTime);

	void getFlipped(float duration = 0, float frameTime = 0);

private:
	float									flippedAnimationTime;
	std::vector<DirectX::XMFLOAT2>			flippedTrajectory;
	std::vector<float>						flippedAngles;
};