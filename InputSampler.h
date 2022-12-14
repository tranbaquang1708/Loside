//
// InputSampler.h
//

#pragma once
#include <vector>
#include <pch.h>

namespace InputSampler {
	inline std::vector<float> sampleInputUniform(float minX, float maxX, int stepNum);
	// sample x on a circle with uniform angle
	inline std::vector<float> sampleInputUniformByAngle(float minX, float maxX, int stepNum, float r);

	inline std::vector<float> sampleInputBouncingBall(float minX, float maxX, int stepNum);
};

