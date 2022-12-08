#pragma once

#include "Texture2D.h"

class Protagonist : public Texture2D
{
public:
	void moveHorizontal()
	{
		BlockInput(true);

	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource>	texture;
	RECT									destRect;
	int										descriptorMap;
};