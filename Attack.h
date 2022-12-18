#pragma once

#include <map>
#include <filesystem>
#include "pch.h"
#include "Enemy.h"

struct Attack
{
	enum Type : unsigned short
	{
		None,
		Fire,
		Flame
	};

	unsigned short getType();

	void loadAnimation(const wchar_t* texturesPath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors, std::vector<bool>& m_descriptorStatuses,
		DirectX::XMUINT2 resolution, float arg_frameTime = 1 / 12., float animationLength = 0);
	void setDefaultScaling(RECT fullscreenRect);
	void setType(unsigned short inType);
	bool getIsOn();

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect);
	void update(float elapsedTime);
	void attack(DirectX::XMFLOAT2 inPosition);
	void reset(std::vector<bool>& m_descriptorStatuses);

	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int frameNo, int startIdx = 0);


	unsigned short										type;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> frames;
	int													currentFrameIdx;

	DirectX::XMUINT2									textureResolution;
	float												defaultScaling;
	DirectX::XMFLOAT2									origin;
	DirectX::XMFLOAT2									position;
	DirectX::XMFLOAT2									size;

	std::map<int, int>									descriptorMap;

	float												animationPlayedTime;
	float												frameTime;


	bool												isOn;
};