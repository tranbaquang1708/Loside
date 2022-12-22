#include "pch.h"
#include "SkillUI.h"

void SkillUI::loadFireTexture(const wchar_t* fireTexturePath, const wchar_t* fireKeyboardTexturePath,
	ID3D12Device* device,
	DirectX::ResourceUploadBatch& resourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	std::vector<bool>& m_descriptorStatuses,
	DirectX::XMUINT2 fireResolution, DirectX::XMUINT2 fireKeyboardResolution)
{
	// Skill UI
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, resourceUpload, fireTexturePath, fireTexture.ReleaseAndGetAddressOf()));

	fireDescriptorMap = pushToHeap(m_descriptorStatuses);

	DirectX::CreateShaderResourceView(device, fireTexture.Get(),
		m_resourceDescriptors->GetCpuHandle(fireDescriptorMap));

	fireTextureResolution = fireResolution;

	// Keyboard UI
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, resourceUpload, fireKeyboardTexturePath,
			fireKeyboardTexture.ReleaseAndGetAddressOf())
	);

	fireKeyboardDescriptorMap = pushToHeap(m_descriptorStatuses);

	DirectX::CreateShaderResourceView(device, fireKeyboardTexture.Get(),
		m_resourceDescriptors->GetCpuHandle(fireKeyboardDescriptorMap));

	fireKeyboardTextureResolution = fireKeyboardResolution;
}

void SkillUI::setFireDefaultScaling(RECT fullscreenRect)
{
	// Skill UI
	fireDefaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left) / fireTextureResolution.x;
	DirectX::XMUINT2 fireOriginalSize = DirectX::GetTextureSize(fireTexture.Get());
	fireSize.x = fireOriginalSize.x * fireDefaultScaling / (fullscreenRect.right);
	fireSize.y = fireOriginalSize.y * fireDefaultScaling / (fullscreenRect.bottom);

	// Keyboard UI
	fireKeyboardDefaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left)
		/ 
		fireKeyboardTextureResolution.x;
	DirectX::XMUINT2 fireKeyboardOriginalSize = DirectX::GetTextureSize(fireKeyboardTexture.Get());
	fireKeyboardSize.x = fireKeyboardOriginalSize.x * fireKeyboardDefaultScaling / (fullscreenRect.right);
	fireKeyboardSize.y = fireKeyboardOriginalSize.y * fireKeyboardDefaultScaling / (fullscreenRect.bottom);
}

void SkillUI::setFirePosition(DirectX::XMFLOAT2 _firePosition)
{
	firePosition = _firePosition;

	fireKeyboardPosition.x = _firePosition.x + fireSize.x - 0.5f * fireKeyboardSize.x;
	fireKeyboardPosition.y = _firePosition.y + 0.8f * fireSize.y;
}

void SkillUI::setAttackInterface(AttackInterface* _attackInterface)
{
	attackInterface = _attackInterface;
}


void SkillUI::update()
{
	if (attackInterface->getIsFireCoolingDown()) {
		float fireCooledDownPercentage = attackInterface->getFireCoolDownPassedTime() / attackInterface->getFireCoolDownTime();
		fireSourceRect.top = 0;
		fireSourceRect.left = 0;
		fireSourceRect.bottom = static_cast<unsigned int>(
				DirectX::GetTextureSize(fireTexture.Get()).y * (1 - fireCooledDownPercentage)
			);
		fireSourceRect.right = DirectX::GetTextureSize(fireTexture.Get()).x;
	}
}

void SkillUI::draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	RECT fullscreenRect)
{
	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(fireDescriptorMap),
		DirectX::GetTextureSize(fireTexture.Get()),
		DirectX::XMFLOAT2(firePosition.x * fullscreenRect.right, firePosition.y * fullscreenRect.bottom),
		nullptr, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0, 0),
		fireDefaultScaling);

	if (attackInterface->getIsFireCoolingDown()) {
		m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(fireDescriptorMap),
			DirectX::GetTextureSize(fireTexture.Get()),
			DirectX::XMFLOAT2(firePosition.x * fullscreenRect.right, firePosition.y * fullscreenRect.bottom),
			&fireSourceRect, DirectX::Colors::LightGray, 0.f, DirectX::XMFLOAT2(0, 0),
			fireDefaultScaling);
	}

	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(fireKeyboardDescriptorMap),
		DirectX::GetTextureSize(fireKeyboardTexture.Get()),
		DirectX::XMFLOAT2(fireKeyboardPosition.x * fullscreenRect.right, fireKeyboardPosition.y * fullscreenRect.bottom),
		nullptr, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0, 0),
		fireKeyboardDefaultScaling);
}

void SkillUI::reset(std::vector<bool>& m_descriptorStatuses)
{
	fireTexture.Reset();
	m_descriptorStatuses[fireDescriptorMap] = false;
	fireKeyboardTexture.Reset();
	m_descriptorStatuses[fireKeyboardDescriptorMap] = false;
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