#pragma once

#include "pch.h"
#include "AttackInterface.h"

class SkillUI
{
public:
	void loadFireTexture(const wchar_t* fireTexturePath, const wchar_t* fireKeyboardTexturePath,
		ID3D12Device* device,
		DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses,
		DirectX::XMUINT2 fireResolution, DirectX::XMUINT2 fireKeyboardResolution);

	void setFireDefaultScaling(RECT fullscreenRect);
	void setFirePosition(DirectX::XMFLOAT2 _fireposition);
	
	void setAttackInterface(AttackInterface* _attackInterface);

	void update();
	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect);
	void reset(std::vector<bool>& m_descriptorStatuses);

private:
	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx = 0);

	Microsoft::WRL::ComPtr<ID3D12Resource>	fireTexture;
	DirectX::XMUINT2						fireTextureResolution;
	float									fireDefaultScaling;
	DirectX::XMFLOAT2						fireSize;

	DirectX::XMFLOAT2						firePosition;
	RECT									fireSourceRect;

	Microsoft::WRL::ComPtr<ID3D12Resource>	fireKeyboardTexture;
	DirectX::XMUINT2						fireKeyboardTextureResolution;
	float									fireKeyboardDefaultScaling;
	DirectX::XMFLOAT2						fireKeyboardSize;

	DirectX::XMFLOAT2						fireKeyboardPosition;

	int										fireDescriptorMap;
	int										fireKeyboardDescriptorMap;

	AttackInterface*						attackInterface;
};