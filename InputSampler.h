//
// InputSampler.h
//

#pragma once
#include <vector>
#include <algorithm>
#include <pch.h>

//#include <sstream>


namespace InputSampler {
	inline std::vector<float> sampleInputUniform(float minX, float maxX, int stepNum);
	inline std::vector<float> sampleInputFalling(float minX, float maxX, int stepNum);
	inline std::vector<float> sampleInputBouncingBall(float minX, float maxX, int stepNum);
};

