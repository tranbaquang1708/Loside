//
// InputSampler.h
//

#pragma once
#include "InputSampler.h"

std::vector<float> InputSampler::sampleInputUniform(float min, float max, int stepNum)
{
	std::vector<float> results;
	float step = (max - min) / stepNum;

	for (int i = 0; i < stepNum; ++i)
	{
		results.push_back(min + i * step);
	}

	results.push_back(max);

	return results;
}

// Sampling function: (n: stepNum, i: stepNo, a: acceleration)
//   f(i) = f(i-1) + i * a 
//        = i * (i + 1) * a / 2
//   f(0) = 0
//   f(n) = 1
// => a = 2 / (n * (n + 1))
//            i * (i + 1)
// => f(i) = -------------
//            n * (n + 1)
// Set half_a = 1 / (n * (n + 1)) => f(i) = i * (i + 1) / half_a
std::vector<float> InputSampler::sampleInputFalling(float min, float max, int stepNum)
{
	float delta = max - min;
	float half_a = 1.f / (stepNum * (stepNum + 1));

	std::vector<float> results;
	results.reserve(stepNum + 1);

	results.emplace_back(min);

	for (int i = 1; i < stepNum; ++i) {
		results.emplace_back(min + i * (i + 1) * half_a * delta);
	}

	// Emplace back max (not f(n)) to avoid floating point error
	results.emplace_back(max);

	return results;
}

// Use sampleInputFalling 2 times
// Assume function reaches max at (min + max)/2
std::vector<float> InputSampler::sampleInputBouncingBall(float min, float max, int stepNum)
{
#ifdef _DEBUG
	assert(stepNum % 2 == 0 && "Number of step must be even in order to catch maximum y value");
#endif

	float delta = (max - min) * 0.5f;
	float mid = (max + min) * 0.5f;
	float half_a = 1.f / (stepNum * 0.5f * (stepNum * 0.5f + 1.f));

	std::vector<float> results;
	results.reserve(stepNum + 1);

	for (int i = 0; i < stepNum / 2; ++i) {
		results.emplace_back(mid - i * (i + 1) * half_a * delta);
	}
	results.emplace_back(min);

	std::reverse(results.begin(), results.begin() + stepNum / 2 + 1);

	for (int i = 1; i < stepNum / 2; ++i) {
		results.emplace_back(mid + i * (i + 1) * half_a * delta);
	}

	results.push_back(max);

	//
//#ifdef _DEBUG
//	std::wstringstream outSS(L"");
//
//	for (int i = 1; i < results.size(); ++i) {
//		outSS << results[i] - results[i-1] << L", ";
//	}
//
//	outSS << L"\n";
//
//	outSS << L"-------------------------------------\n";
//	OutputDebugStringW(outSS.str().c_str());
//
//#endif

	return results;
}
