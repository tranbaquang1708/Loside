#pragma once

#include "Enemy.h"
#include "AttackInterface.h"

class Protagonist
{
public:
	void loadTexture(const wchar_t* texturePath, ID3D12Device* device,
		DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses, DirectX::XMUINT2 resolution);

	void loadWalkAnimation(std::vector<DirectX::XMFLOAT2> sampledTrajectory, float time);
	void loadAttackInterface(AttackInterface* inAttackInterface);

	void setDefaultScaling(RECT fullscreenRect);
	void setPosition(DirectX::XMFLOAT2 arg_position);
	void setState();
	void setState(unsigned short state);

	DirectX::XMFLOAT2 getPosition();
	DirectX::XMFLOAT2 getTextureSize();

	void update(float elapsedTime, Enemy* inEnemy);
	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect);
	void reset(std::vector<bool>& m_descriptorStatuses);

	void walk(float elapsedTime);
	void cancelWalk();

	void handleInput(DirectX::Keyboard::State keyboardInput, DirectX::Keyboard::KeyboardStateTracker& keyboardTracker);

private:
	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx = 0);

	Microsoft::WRL::ComPtr<ID3D12Resource>	texture;
	DirectX::XMUINT2						textureResolution;
	float									defaultScaling;
	DirectX::XMFLOAT2						size;

	DirectX::XMFLOAT2						position;
	DirectX::XMFLOAT2						originalPosition;

	int										descriptorMap;

	float									animationPlayedTime;

	float									walkAnimationTime;
	std::vector<DirectX::XMFLOAT2>			walkTrajectory;

	unsigned short							currentState;
	enum States : unsigned short
	{
		IdleState,
		WalkingLeftState,
		WalkingRightState
	};

	Enemy*									targetingEnemy;

	AttackInterface*						attackInterface;
};