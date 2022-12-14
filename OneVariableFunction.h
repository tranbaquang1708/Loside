//
// OneVariableFunction.h
//

#pragma once

#include <functional>
#include <vector>
#include <DirectXMath.h>

#include <sstream>

class OneVariableFunction
{};



class QuadraticFunction : public OneVariableFunction
{
public:
	// f(x) = a(x + b)^2 + c
	QuadraticFunction(float a, float b, float c)
	{
		f = [a, b, c](float x) { return a * (x + b) * (x + b) + c; };
	}

	std::vector<DirectX::XMFLOAT2> sample(std::vector<float> inputs)
	{
#ifdef _DEBUG
		assert(inputs.size() % 2 == 1 && "Need step to be odd in order to catch maximum value");
#endif

		std::vector<DirectX::XMFLOAT2> results;
	
		for (int i = 0; i < inputs.size(); ++i) {
			results.push_back(DirectX::XMFLOAT2(inputs[i], f(inputs[i])));
		}

		return results;
	}

private:
	std::function<float(float)> f;
};

// f(x) = sprt(r^2 - (x - a)^2) + b
// Y-positive half of circle at center (a, b)
class halfCircleYPositiveFunction : public OneVariableFunction
{
public:
	halfCircleYPositiveFunction(float rSquare, float centerX, float centerY)
	{
		f = [rSquare, centerX, centerY](float x) { return sqrt(rSquare - (x - centerX) * (x - centerX)) + centerY; };
	}

	std::vector<DirectX::XMFLOAT2> sample(std::vector<float> inputs)
	{
#ifdef _DEBUG
		assert(inputs.size() % 2 == 1 && "Need step to be odd in order to catch maximum value");
#endif

		std::vector<DirectX::XMFLOAT2> results;

		for (int i = 0; i < inputs.size(); ++i) {
			results.push_back(DirectX::XMFLOAT2(inputs[i], f(inputs[i])));
		}

//#ifdef _DEBUG
//	    std::wstringstream outSS(L"");
//
//		for (int i = 0; i < inputs.size(); ++i) {
//			outSS << results[i].x << L", " << results[i].y << L"\n";
//		}
//
//		outSS << L"-------------------------------------\n";
//		OutputDebugStringW(outSS.str().c_str());
//
//#endif

		return results;
	}

private:
	std::function<float(float)> f;
};
