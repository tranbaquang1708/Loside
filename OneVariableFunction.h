//
// OneVariableFunction.h
//

#pragma once

#include <functional>
#include <vector>
#include <DirectXMath.h>

//#include <sstream>

class OneVariableFunction
{
public:
	void setF(std::function<float(float)> _f);
	std::vector<DirectX::XMFLOAT2> sample(std::vector<float> inputs);

private:
	std::function<float(float)> f;
};

// f(x) = a(x + b)^2 + c
class QuadraticFunction : public OneVariableFunction
{
public:
	QuadraticFunction(float a, float b, float c);
};

// f(x) = sprt(r^2 - (x - a)^2) + b
class halfCircleYPositiveFunction : public OneVariableFunction
{
public:
	halfCircleYPositiveFunction(float r, float centerX, float centerY);

	std::vector<DirectX::XMFLOAT2> sampleByAngle(std::vector<float> inputAngles);

private:
	float r;
	float centerX;
	float centerY;
};



