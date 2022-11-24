#pragma once
#include <pch.h>
#include <DDSTextureLoader.h>
#include <DescriptorHeap.h>
#include <vector>

class Texture2D {
public:
	void loadTexture(const wchar_t* path, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses) 
	{
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, path,
				this->texture.ReleaseAndGetAddressOf()));

		int descriptorNo = pushToHeap(m_descriptorStatuses);

		DirectX::CreateShaderResourceView(device, this->texture.Get(),
			m_resourceDescriptors->GetCpuHandle(descriptorNo));
	}

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT& m_fullscreenRect)
	{
		m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
			DirectX::GetTextureSize(this->texture.Get()),
			m_fullscreenRect, nullptr, DirectX::Colors::White, 0.f);
	}

	void reset() {
		texture.Reset();
	}

private:
	int pushToHeap(std::vector<bool>& m_descriptorStatuses,
		int startIdx = 0)
	{
		for (int i = startIdx; i < m_descriptorStatuses.size(); ++i) {
			if (!m_descriptorStatuses[i]) {
				descriptorMap = i;
				m_descriptorStatuses[i] = true;
				return i;
			}
		}

		return -1;
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> texture;
	int descriptorMap;
};
