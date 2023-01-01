#pragma once

#include "pch.h"
#include "AttackInterface.h"

class SkillUI
{
public:
	void loadTexture(const wchar_t* fireTexturePath, const wchar_t* fireKeyboardTexturePath,
		ID3D12Device* device,
		DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses,
		DirectX::XMUINT2 fireResolution, DirectX::XMUINT2 fireKeyboardResolution);

	void setDefaultScaling(RECT fullscreenRect);
	void setPosition(DirectX::XMFLOAT2 _fireposition);
	
	void setAttackInterface(AttackInterface* _attackInterface);

	void update();
	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect);
	void reset(std::vector<bool>& m_descriptorStatuses);

private:
	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx = 0);

	Microsoft::WRL::ComPtr<ID3D12Resource>	texture;
	DirectX::XMUINT2						textureResolution;
	float									defaultScaling;
	DirectX::XMFLOAT2						size;

	DirectX::XMFLOAT2						position;
	RECT									sourceRect;

	Microsoft::WRL::ComPtr<ID3D12Resource>	keyboardTexture;
	DirectX::XMUINT2						keyboardTextureResolution;
	float									keyboardDefaultScaling;
	DirectX::XMFLOAT2						keyboardSize;

	DirectX::XMFLOAT2						keyboardPosition;

	int										descriptorMap;
	int										keyboardDescriptorMap;

	AttackInterface*						attackInterface;
};