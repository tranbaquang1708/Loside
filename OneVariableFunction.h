#pragma once

#include <functional>
#include <vector>
#include <DirectXMath.h>

//#include <sstream>
class QuadraticFunction
{
public:
	// f(x) = a(x + b)^2 + c
	QuadraticFunction(float a, float b, float c)
	{
		f = [a, b, c](float x) { return a * (x + b) * (x + b) + c; };
	}

	std::vector<DirectX::XMFLOAT2> sample(float minX, float maxX, int stepNum)
	{
#ifdef _DEBUG
		assert(minX <= maxX);
		assert(stepNum % 2 == 0 && "Need step to be odd in order to catch maximum value");
#endif

		std::vector<DirectX::XMFLOAT2> results;

		std::vector<float> input = sampleInputUniform(minX, maxX, stepNum);
	
		for (int i = 0; i < input.size(); ++i) {
			results.push_back(DirectX::XMFLOAT2(input[i], f(input[i])));
		}

		return results;
	}

	std::vector<float> sampleInputUniform(float minX, float maxX, int stepNum)
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
	std::vector<float> sampleInputBouncingBall(float minX, float maxX, int stepNum)
	{
		std::vector<float> results;

		float length = maxX - minX;
		float left = minX;

		results.push_back(left);

		for (int i = 1; i < stepNum/2; ++i) {
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

#ifdef _DEBUG
		std::wstringstream outSS(L"");

		for (int i = 0; i < results.size(); ++i) {
			outSS << results[i] << L", ";
		}
		//outSS << L"OGPos: " << originalPosition.x << L", " << originalPosition.y << L"\n";
		outSS << L"\n";
		outSS << L"-------------------------------------\n";

		OutputDebugStringW(outSS.str().c_str());
#endif

		return results;
	}


private:
	std::function<float(float)> f;
};
