#include "pch.h"
#include "Ailment.h"

void Ailment::loadTexture(const wchar_t* texturePath, ID3D12Device* device,
	DirectX::ResourceUploadBatch& resourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	std::vector<bool>& m_descriptorStatuses, DirectX::XMUINT2 resolution)
{
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, resourceUpload, texturePath,
			this->texture.ReleaseAndGetAddressOf()));

	int descriptorNo = pushToHeap(m_descriptorStatuses);

	DirectX::CreateShaderResourceView(device, this->texture.Get(),
		m_resourceDescriptors->GetCpuHandle(descriptorNo));

	textureResolution = resolution;
}

void Ailment::setDefaultScaling(RECT fullscreenRect)
{
	defaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left) / textureResolution.x;
}

void Ailment::setPosition(DirectX::XMFLOAT2 _position)
{
	position = _position;
}

void Ailment::draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	RECT fullscreenRect)
{
	DirectX::XMUINT2 textureSize = DirectX::GetTextureSize(this->texture.Get());

	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
		DirectX::GetTextureSize(this->texture.Get()),
		DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
		nullptr, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(textureSize.x / 2.f, textureSize.y / 2.f),
		defaultScaling);
}

void Ailment::reset(std::vector<bool>& m_descriptorStatuses)
{
	texture.Reset();
	m_descriptorStatuses[descriptorMap] = false;
}

int Ailment::pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx)
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
