//
// Enemy.h
//

#pragma once

#include <pch.h>

//#include <sstream>
#include <chrono>
#include <thread>

class Enemy
{
public:
	void loadTexture(const wchar_t* texturePath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
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
		rotation = 0.f;
		originalRotation = 0.f;
	}

	void update(float elapsedTime)
	{
		if (currentState == IdleState) {
			stay(elapsedTime);
		}
		if (currentState == WalkingLeftState || currentState == WalkingRightState) {
			walk(elapsedTime);
		}
	}

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect)
	{
		DirectX::XMUINT2 textureSize = DirectX::GetTextureSize(this->texture.Get());

		m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
			textureSize,
			DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
			nullptr,
			DirectX::Colors::White, rotation,
			DirectX::XMFLOAT2(textureSize.x / 2.f, textureSize.y / 2.f),
			defaultScaling);
	}

	void reset(std::vector<bool>& m_descriptorStatuses)
	{
		texture.Reset();
		m_descriptorStatuses[descriptorMap] = false;
	}

	void loadWalkAnimation(std::vector<DirectX::XMFLOAT2> sampledTrajectory,
		std::vector<float> sampledAngles,
		float time)
	{
#ifdef _DEBUG
		assert(sampledTrajectory.size() == sampledAngles.size());
#endif
		animationPlayedTime = 0;

		walkTrajectory = sampledTrajectory;
		walkAngles = sampledAngles;
		walkAnimationTime = time;
	}

	void setDefaultScaling(RECT fullscreenRect)
	{
		defaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left) / textureResolution.x;
	}

	void setPosition(DirectX::XMFLOAT2 arg_position)
	{
		position = arg_position;
		originalPosition = arg_position;
	}

	DirectX::XMFLOAT2 getNormalizedTextureSize(RECT fullscreenRect)
	{
		DirectX::XMUINT2 textureSize = DirectX::GetTextureSize(this->texture.Get());
		return DirectX::XMFLOAT2(
			textureSize.x * defaultScaling / fullscreenRect.right,
			textureSize.y * defaultScaling / fullscreenRect.bottom
		);
	}

	void setState(unsigned short state) {
		this->currentState = state;

		animationPlayedTime = 0;
		originalPosition = position;
		originalRotation = rotation;
	}

	void setWalkState()
	{
		if (protagonistBottomRightX > originalPosition.x) {
			setState(WalkingRightState);
		}
		else {
			setState(WalkingLeftState);
		}
	}

	void setState()
	{
		setState(IdleState);
	}

	void setProtagonistBottomRightX(float arg_protagonistBottomRightX)
	{
		this->protagonistBottomRightX = arg_protagonistBottomRightX;
	}

	void stay(float elapsedTime)
	{
		// MYTODO: Need better stay function
		animationPlayedTime += elapsedTime;
		//if (animationPlayedTime > idleAnimationTime) // hard-code idleAnimationTime = 0.3
		if (animationPlayedTime > 0.3f) { // hard-code idleAnimationTime = 0.3
			setWalkState();
		}
	}

	void walk(float elapsedTime) {
		animationPlayedTime += elapsedTime;

		size_t currentPositionIdx = std::min(
			walkTrajectory.size() - 1,
			static_cast<size_t>(animationPlayedTime * (walkTrajectory.size() - 1) / walkAnimationTime)
		);

		if (currentState == WalkingRightState) {
			position.x = originalPosition.x + walkTrajectory[currentPositionIdx].x;
			rotation = originalRotation + walkAngles[currentPositionIdx];
		}
		else {
			position.x = originalPosition.x - walkTrajectory[currentPositionIdx].x;
			rotation = originalRotation - walkAngles[currentPositionIdx];
		}

		position.y = originalPosition.y - walkTrajectory[currentPositionIdx].y;

		if (currentPositionIdx == walkTrajectory.size() - 1) {
			setState(IdleState);
			/*if (currentState == WalkingRightState) {
				currentState = WalkingLeftState;
			}
			else {
				currentState = WalkingRightState;
			}*/
		}

//#ifdef _DEBUG
//		std::wstringstream outSS(L"");
//		outSS << currentState << L"\n";
//		outSS << currentPositionIdx << L": " << walkAngles[currentPositionIdx] << L"\n";
//		outSS << currentPositionIdx  << L": " << walkTrajectory[currentPositionIdx].x << L", " 
//			<< walkTrajectory[currentPositionIdx].y << L"\n";
//		outSS << L"-------------------------------------\n";
//		
//		OutputDebugStringW(outSS.str().c_str());
//#endif
	}

	DirectX::XMFLOAT2 getTextureSize(RECT fullscreenRect)
	{
		DirectX::XMUINT2 textureSize = DirectX::GetTextureSize(this->texture.Get());
		return DirectX::XMFLOAT2(
			textureSize.x * defaultScaling / fullscreenRect.right,
			textureSize.y * defaultScaling / fullscreenRect.bottom
		);
	}

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
	DirectX::XMUINT2						textureResolution;
	float									defaultScaling;

	DirectX::XMFLOAT2						position;
	DirectX::XMFLOAT2						originalPosition;

	float									rotation;
	float									originalRotation;

	int										descriptorMap;

	float									animationPlayedTime;

	float									idleAnimationTime;

	float									walkAnimationTime;
	std::vector<DirectX::XMFLOAT2>			walkTrajectory;
	std::vector<float>						walkAngles;

	unsigned short							currentState;
	enum States : unsigned short
	{
		IdleState,
		WalkingLeftState,
		WalkingRightState
	};

	float									protagonistBottomRightX;
};