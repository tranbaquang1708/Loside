#include "pch.h"
#include "SkillUI.h"

void SkillUI::loadTexture(const wchar_t* fireTexturePath, const wchar_t* fireKeyboardTexturePath,
	ID3D12Device* device,
	DirectX::ResourceUploadBatch& resourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	std::vector<bool>& m_descriptorStatuses,
	DirectX::XMUINT2 fireResolution, DirectX::XMUINT2 fireKeyboardResolution)
{
	// Skill UI
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, resourceUpload, fireTexturePath, texture.ReleaseAndGetAddressOf()));

	descriptorMap = pushToHeap(m_descriptorStatuses);

	DirectX::CreateShaderResourceView(device, texture.Get(),
		m_resourceDescriptors->GetCpuHandle(descriptorMap));

	textureResolution = fireResolution;

	// Keyboard UI
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, resourceUpload, fireKeyboardTexturePath,
			keyboardTexture.ReleaseAndGetAddressOf())
	);

	keyboardDescriptorMap = pushToHeap(m_descriptorStatuses);

	DirectX::CreateShaderResourceView(device, keyboardTexture.Get(),
		m_resourceDescriptors->GetCpuHandle(keyboardDescriptorMap));

	keyboardTextureResolution = fireKeyboardResolution;
}

void SkillUI::setDefaultScaling(RECT fullscreenRect)
{
	// Skill UI
	defaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left) / textureResolution.x;
	DirectX::XMUINT2 fireOriginalSize = DirectX::GetTextureSize(texture.Get());
	size.x = fireOriginalSize.x * defaultScaling / (fullscreenRect.right);
	size.y = fireOriginalSize.y * defaultScaling / (fullscreenRect.bottom);

	// Keyboard UI
	keyboardDefaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left)
		/ 
		keyboardTextureResolution.x;
	DirectX::XMUINT2 fireKeyboardOriginalSize = DirectX::GetTextureSize(keyboardTexture.Get());
	keyboardSize.x = fireKeyboardOriginalSize.x * keyboardDefaultScaling / (fullscreenRect.right);
	keyboardSize.y = fireKeyboardOriginalSize.y * keyboardDefaultScaling / (fullscreenRect.bottom);
}

void SkillUI::setPosition(DirectX::XMFLOAT2 _firePosition)
{
	position = _firePosition;

	keyboardPosition.x = _firePosition.x + size.x - 0.55f * keyboardSize.x;
	keyboardPosition.y = _firePosition.y + 0.75f * size.y;
}

void SkillUI::setAttackInterface(AttackInterface* _attackInterface)
{
	attackInterface = _attackInterface;
}


void SkillUI::update()
{
	if (attackInterface->getIsCoolingDown()) {
		float fireCooledDownPercentage = attackInterface->getCoolDownPassedTime() / attackInterface->getCoolDownTime();
		sourceRect.top = 0;
		sourceRect.left = 0;
		sourceRect.bottom = static_cast<unsigned int>(
				DirectX::GetTextureSize(texture.Get()).y * (1 - fireCooledDownPercentage)
			);
		sourceRect.right = DirectX::GetTextureSize(texture.Get()).x;
	}
}

void SkillUI::draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	RECT fullscreenRect)
{
	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
		DirectX::GetTextureSize(texture.Get()),
		DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
		nullptr, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0, 0),
		defaultScaling);

	if (attackInterface->getIsCoolingDown()) {
		m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
			DirectX::GetTextureSize(texture.Get()),
			DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
			&sourceRect, DirectX::Colors::LightGray, 0.f, DirectX::XMFLOAT2(0, 0),
			defaultScaling);
	}

	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(keyboardDescriptorMap),
		DirectX::GetTextureSize(keyboardTexture.Get()),
		DirectX::XMFLOAT2(keyboardPosition.x * fullscreenRect.right, keyboardPosition.y * fullscreenRect.bottom),
		nullptr, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0, 0),
		keyboardDefaultScaling);
}

void SkillUI::reset(std::vector<bool>& m_descriptorStatuses)
{
	texture.Reset();
	m_descriptorStatuses[descriptorMap] = false;
	keyboardTexture.Reset();
	m_descriptorStatuses[keyboardDescriptorMap] = false;
}

int SkillUI::pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx)
{
	for (int i = startIdx; i < m_descriptorStatuses.size(); ++i) {
		if (!m_descriptorStatuses[i]) {
			m_descriptorStatuses[i] = true;
			return i;
		}
	}

	return -1;
}