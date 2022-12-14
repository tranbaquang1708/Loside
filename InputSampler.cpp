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

// MYTODO: Too many assumption
// Assume the angle is 90 degree
// Assume the minX == 0 && f(minX) == 0 (focus on displacement)
// Step 1: Assume the circle center is (0, 0)
// Step 2: Tranlate the center to (centerX, centerY) ( => minX == 0 && f(minX) == 0)
std::vector<float> InputSampler::sampleInputUniformByAngle(
	float minX, float maxX, int stepNum,
	float r
)
{
	std::vector<float> results;
	//float angle = DirectX::XM_PIDIV2;
	float deltaAngle = DirectX::XM_PIDIV2 / stepNum;	// Assume the angle is 90 degree
	float startAngle = 3 * DirectX::XM_PIDIV4;			// Assume the arc is rotated 45 degree

	results.push_back(minX);
	float startX = cosf(startAngle) * r;

	for (int i = 1; i < stepNum; ++i) {
		results.push_back(cosf(startAngle - i * deltaAngle) * r - startX);
	}

	results.push_back(maxX);
//#ifdef _DEBUG
//	std::wstringstream outSS(L"");
//
//	for (int i = 0; i < results.size(); ++i) {
//		outSS << results[i] << L", ";
//	}
//	//outSS << L"OGPos: " << originalPosition.x << L", " << originalPosition.y << L"\n";
//	outSS << L"\n";
//	outSS << L"-------------------------------------\n";
//
//	OutputDebugStringW(outSS.str().c_str());
//#endif

	return results;
}

// Assume function reaches max at (min + max)/2
// MYTODO: Find a better way to divide the domain [min, max]
std::vector<float> InputSampler::sampleInputBouncingBall(float min, float max, int stepNum)
{
	std::vector<float> results;

	float length = max - min;
	float left = min;

	results.push_back(left);

	for (int i = 1; i < stepNum / 2; ++i) {
		left += length / powf(2, i + 1.f);
		results.push_back(left);
	}

	left = (min + max) / 2;
	results.push_back(left);

	for (int i = stepNum / 2 - 1; i > 0; --i) {
		left += length / powf(2, i + 1.f);
		results.push_back(left);
	}

	results.push_back(max);

	return results;
}
