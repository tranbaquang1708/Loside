//
// InputSampler.h
//

#pragma once
#include "InputSampler.h"

std::vector<float> InputSampler::sampleInputUniform(float minX, float maxX, int stepNum)
{
	std::vector<float> results;
	float step = (maxX - minX) / stepNum;

	for (int i = 0; i < stepNum; ++i)
	{
		results.push_back(minX + i * step);
	}

	results.push_back(maxX);

	return results;
}

// Assume function reaches max at (minX + maxX)/2
// MYTODO: Find a better way to divide the domain [minX, maxX]
std::vector<float> InputSampler::sampleInputBouncingBall(float minX, float maxX, int stepNum)
{
	std::vector<float> results;

	float length = maxX - minX;
	float left = minX;

	results.push_back(left);

	for (int i = 1; i < stepNum / 2; ++i) {
		left += length / powf(2, i + 1.f);
		results.push_back(left);
	}

	left = (minX + maxX) / 2;
	results.push_back(left);

	for (int i = stepNum / 2 - 1; i > 0; --i) {
		left += length / powf(2, i + 1.f);
		results.push_back(left);
	}

	results.push_back(maxX);

	//#ifdef _DEBUG
	//		std::wstringstream outSS(L"");
	//
	//		for (int i = 0; i < results.size(); ++i) {
	//			outSS << results[i] << L", ";
	//		}
	//		//outSS << L"OGPos: " << originalPosition.x << L", " << originalPosition.y << L"\n";
	//		outSS << L"\n";
	//		outSS << L"-------------------------------------\n";
	//
	//		OutputDebugStringW(outSS.str().c_str());
	//#endif

	return results;
}
