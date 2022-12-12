//
// InputSampler.h
//

#pragma once
#include <vector>

namespace InputSampler {
	inline std::vector<float> sampleInputUniform(float minX, float maxX, int stepNum);
	inline std::vector<float> sampleInputBouncingBall(float minX, float maxX, int stepNum);
};

