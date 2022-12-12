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
		float arg_frameTime = 1/12., bool arg_isLoop=false) {
		int numOfFrames = 0;

		for (const auto& entry : std::filesystem::directory_iterator(texturesPath)) {
			numOfFrames++;
		}

		this->frames.resize(numOfFrames);

		for (const auto& entry : std::filesystem::directory_iterator(texturesPath)) {
			int frameNo = std::stoi(entry.path().stem());
			DX::ThrowIfFailed(
				CreateDDSTextureFromFile(device, resourceUpload, entry.path().c_str(),
					this->frames[frameNo].ReleaseAndGetAddressOf()));

			// TODO: implement startIdx to decrease the number of loops
			int descriptorNo = pushToHeap(m_descriptorStatuses, frameNo);

			DirectX::CreateShaderResourceView(device, this->frames[frameNo].Get(),
				m_resourceDescriptors->GetCpuHandle(descriptorNo));
		}

		this->isLoop = arg_isLoop;
		this->frameTime = arg_frameTime;
		this->currentFrameIdx = 0;
		this->playedTime = 0;
#ifdef _DEBUG
		assert(frames.size() == 2);
#endif
	}

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors) 
	{
		m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap.find(currentFrameIdx)->second),
			DirectX::GetTextureSize(this->frames[currentFrameIdx].Get()),
			destRect, nullptr, DirectX::Colors::White, 0.f);
	}

	void update(float elapsedTime)
	{
		this->playedTime += elapsedTime;
		if (isLoop) {
			this->currentFrameIdx = static_cast<int>(this->playedTime / this->frameTime) % this->frames.size();
		}
		else {
			this->currentFrameIdx = std::min(static_cast<int>(this->playedTime / (this->frameTime * this->frames.size())),
				static_cast<int>(this->frames.size() - 1));
		}

		//OutputDebugStringA(std::to_string(this->currentFrameIdx).c_str());
	}

	void reset(std::vector<bool>& m_descriptorStatuses)
	{
		// MYTODO: Parallelize this
		for (Microsoft::WRL::ComPtr<ID3D12Resource> f : frames) {
			f.Reset();
		}

		// MYTODO: Parallelize this
		for (std::map<int, int>::iterator it = descriptorMap.begin(); it != descriptorMap.end(); ++it) {
			m_descriptorStatuses[it->second] = false;
		}
	}

	// Set rectangle from top-left, down-right
	// top coordinate will be scaled automatically from texture size
	void setRect(LONG left, LONG right, LONG bottom)
	{
		//XMUINT2 textureSize = ;
		destRect.left = left;
		destRect.right = right;
		destRect.bottom = bottom;
		destRect.top = bottom
			- static_cast<LONG>(
				(
					(right - left) / static_cast<float>(DirectX::GetTextureSize(this->frames[0].Get()).x)
					)
				* DirectX::GetTextureSize(this->frames[0].Get()).y
				);

	}

	void setRect(RECT& rect)
	{
		destRect = rect;
	}

private:
	int pushToHeap(std::vector<bool>& m_descriptorStatuses,
		int frameNo,
		int startIdx = 0)
	{
		for (int i = startIdx; i < m_descriptorStatuses.size(); ++i) {
			if (!m_descriptorStatuses[i]) {
				descriptorMap.insert({ frameNo, i });
				m_descriptorStatuses[i] = true;
				return i;
			}
		}

		assert(false && "Heap is full");
		return -1;
	}

	float												playedTime;
	float												frameTime;
	int													currentFrameIdx;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> frames;
	std::vector<DirectX::XMUINT2>						bottomRightCoords;
	bool												isLoop;
	RECT												destRect;		// Assume all frames have the same size

	std::map<int, int>									descriptorMap;	// Map frame with descriptors
};
