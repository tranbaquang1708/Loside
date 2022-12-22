//
// Animation2D.h
//

#pragma once

#include <pch.h>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <map>

// Create 2D animation from multipple images stored in a folder
class Animation2D {
public:
	void loadAnimation(const wchar_t* texturesPath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors, std::vector<bool>& m_descriptorStatuses,
		float arg_frameTime = 1 / 12., bool arg_isLoop = false);

	void setRect(RECT& rect);
	void setRect(LONG left, LONG right, LONG bottom);

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors);

	void update(float elapsedTime);

	void reset(std::vector<bool>& m_descriptorStatuses);

private:
	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int frameNo, int startIdx = 0);

	float												playedTime;
	float												frameTime;
	int													currentFrameIdx;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> frames;
	std::vector<DirectX::XMUINT2>						bottomRightCoords;
	bool												isLoop;
	RECT												destRect;		// Assume all frames have the same size

	std::map<int, int>									descriptorMap;	// Map frame with descriptors
};
