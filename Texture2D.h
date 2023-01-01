#pragma once
#include <pch.h>
#include <vector>

class Texture2D {
public:
	void loadTexture(const wchar_t* texturePath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses);

	void setRect(RECT& rect);
	void setRect(LONG left, LONG right, LONG bottom);

	int getDescriptor();

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors);

	void reset(std::vector<bool>& m_descriptorStatuses);

	void updatePosition(LONG x, LONG y);

private:
	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx = 0);
	
	Microsoft::WRL::ComPtr<ID3D12Resource>	texture;
	RECT									destRect;
	int										descriptorMap;
};