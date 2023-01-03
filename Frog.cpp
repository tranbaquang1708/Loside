#include "pch.h"
#include "Frog.h"

void Frog::loadFlippedAnimation(std::vector<DirectX::XMFLOAT2> sampledTrajectory,
	std::vector<float> sampledAngles,
	float duration, float frameTime)
{
#ifdef _DEBUG
	assert(duration != frameTime && duration * frameTime == 0);
#endif

	flippedTrajectory = sampledTrajectory;
	flippedAngles = sampledAngles;
	if (duration != 0) {
		flippedAnimationTime = duration;
	}
	else {
		flippedAnimationTime = frameTime * sampledAngles.size();
	}
}