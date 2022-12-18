#include "pch.h"
#include "OneVariableFunction.h"

std::vector<DirectX::XMFLOAT2> OneVariableFunction::sample(std::vector<float> inputs)
{
#ifdef _DEBUG
	assert(inputs.size() % 2 == 1 && "Need step to be odd in order to catch maximum value");
#endif

	std::vector<DirectX::XMFLOAT2> results;
	results.reserve(inputs.size());

	for (int i = 0; i < inputs.size(); ++i) {
		results.emplace_back(DirectX::XMFLOAT2(inputs[i], f(inputs[i])));
	}

	return results;
}

QuadraticFunction::QuadraticFunction(float a, float b, float c)
{
	f = [a, b, c](float x) { return a * (x + b) * (x + b) + c; };
}

halfCircleYPositiveFunction::halfCircleYPositiveFunction(float r, float centerX, float centerY) :
	r(r),
	centerX(centerX),
	centerY(centerY)
{
	f = [r, centerX, centerY](float x) { return sqrt(r * r - (x - centerX) * (x - centerX)) + centerY; };
}

// Angle always has Ox as one of its arm
std::vector<DirectX::XMFLOAT2> halfCircleYPositiveFunction::sampleByAngle(std::vector<float> inputAngles)
{
	std::vector<DirectX::XMFLOAT2> results;
	results.reserve(inputAngles.size());

	for (size_t i = 0; i < inputAngles.size(); ++i) {
		results.emplace_back(r * cosf(inputAngles[i]) + centerX, r * sinf(inputAngles[i]) + centerY);
	}

	return results;
}