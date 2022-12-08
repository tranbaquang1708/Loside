#pragma once
#include <pch.h>
#include <DDSTextureLoader.h>
#include <DescriptorHeap.h>
#include <vector>

class Texture2D {
public:
	void loadTexture(const wchar_t* texturePath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses) 
	{
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, texturePath,
				this->texture.ReleaseAndGetAddressOf()));

		int descriptorNo = pushToHeap(m_descriptorStatuses);

		DirectX::CreateShaderResourceView(device, this->texture.Get(),
			m_resourceDescriptors->GetCpuHandle(descriptorNo));
	}

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors)
	{
		m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
			DirectX::GetTextureSize(this->texture.Get()),
			destRect, nullptr, DirectX::Colors::White, 0.f);
	}

	void reset(std::vector<bool>& m_descriptorStatuses)
	{
		texture.Reset();
		m_descriptorStatuses[descriptorMap] = false;
	}

	int getDescriptor()
	{
		return descriptorMap;
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
					(right - left) / static_cast<float>(DirectX::GetTextureSize(this->texture.Get()).x)
				)
				* DirectX::GetTextureSize(this->texture.Get()).y
			);

	}

	void setRect(RECT& rect)
	{
		destRect = rect;
	}

	void updatePosition(LONG x, LONG y)
	{
		this->destRect.left += x;
		this->destRect.top += y;
		this->destRect.right += x;
		this->destRect.bottom += y;
	}

protected:
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

private:
	Microsoft::WRL::ComPtr<ID3D12Resource>	texture;
	RECT									destRect;
	int										descriptorMap;
};
