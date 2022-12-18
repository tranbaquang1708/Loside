#pragma once

struct Ailment {
	void loadTexture(const wchar_t* texturePath, ID3D12Device* device,
		DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses, DirectX::XMUINT2 resolution);

	void setDefaultScaling(RECT fullscreenRect);
	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect);
	void reset(std::vector<bool>& m_descriptorStatuses);
	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx = 0);

	Microsoft::WRL::ComPtr<ID3D12Resource>	texture;
	DirectX::XMUINT2						textureResolution;
	float									defaultScaling;

	int										descriptorMap;
	
	DirectX::XMFLOAT2						position;

	unsigned short							type;
};
