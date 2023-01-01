#include "pch.h"
#include "Texture2D.h"

void Texture2D::loadTexture(const wchar_t* texturePath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	std::vector<bool>& m_descriptorStatuses)
{
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, resourceUpload, texturePath,
			texture.ReleaseAndGetAddressOf()));

	int descriptorNo = pushToHeap(m_descriptorStatuses);

	DirectX::CreateShaderResourceView(device, texture.Get(),
		m_resourceDescriptors->GetCpuHandle(descriptorNo));
}

void Texture2D::setRect(RECT& rect)
{
	destRect = rect;
}

// Set rectangle from top-left, down-right
// top coordinate will be scaled automatically from texture size
void Texture2D::setRect(LONG left, LONG right, LONG bottom)
{
	destRect.left = left;
	destRect.right = right;
	destRect.bottom = bottom;
	destRect.top = bottom
		- static_cast<LONG>(
			(
				(right - left) / static_cast<float>(DirectX::GetTextureSize(texture.Get()).x)
				)
			* DirectX::GetTextureSize(texture.Get()).y
			);

}

int Texture2D::getDescriptor()
{
	return descriptorMap;
}

void Texture2D::draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors)
{
	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
		DirectX::GetTextureSize(texture.Get()),
		destRect, nullptr, DirectX::Colors::White, 0.f);
}

void Texture2D::reset(std::vector<bool>& m_descriptorStatuses)
{
	texture.Reset();
	m_descriptorStatuses[descriptorMap] = false;
}

void Texture2D::updatePosition(LONG x, LONG y)
{
	destRect.left += x;
	destRect.top += y;
	destRect.right += x;
	destRect.bottom += y;
}

int Texture2D::pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx)
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