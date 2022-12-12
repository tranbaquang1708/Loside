#pragma once

#include "Texture2D.h"

class Protagonist
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
	}

	void loadWalkAnimation(std::vector<DirectX::XMFLOAT2> trajectory, float time)
	{
		animationPlayedTime = 0;

		walkTrajectory = trajectory;
		walkAnimationTime = time;
	}

	void handleInput(DirectX::Keyboard::State keyboardInput)
	{
		if (keyboardInput.Left) {
			if (currentState != WalkingLeftState && currentState != WalkingRightState) {
				setState(WalkingLeftState);
			}

			if (currentState == WalkingRightState) cancelWalk();
		}

		if (keyboardInput.Right) {
			if (currentState != WalkingLeftState && currentState != WalkingRightState) {
				setState(WalkingRightState);
			}

			if (currentState == WalkingLeftState) cancelWalk();
		}
	}

	void update(float elapsedTime)
	{
		if (currentState == WalkingLeftState || currentState == WalkingRightState) {
			walk(elapsedTime);
		}
	}

	void walk(float elapsedTime)
	{
		animationPlayedTime += elapsedTime;
		size_t currentPositionIdx = std::min(
				walkTrajectory.size() - 1,
				static_cast<size_t>(animationPlayedTime * (walkTrajectory.size() - 1) / walkAnimationTime)
			);

		if (currentState == WalkingRightState) {
			position.x = originalPosition.x + walkTrajectory[currentPositionIdx].x;
		}
		else {
			position.x = originalPosition.x - walkTrajectory[currentPositionIdx].x;
		}

		position.y = originalPosition.y - walkTrajectory[currentPositionIdx].y;

		if (currentPositionIdx == walkTrajectory.size() - 1) {
			setState(IdleState);
		}
	}

	void cancelWalk() {
		position.y = originalPosition.y;
		setState(IdleState);
	}

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect)
	{

		m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
			DirectX::GetTextureSize(this->texture.Get()),
			DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
			nullptr, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0, 0),
			defaultScaling);
	}

	void reset(std::vector<bool>& m_descriptorStatuses)
	{
		texture.Reset();
		m_descriptorStatuses[descriptorMap] = false;
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
	}

	void setState() {
		setState(IdleState);
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

	/*RECT									destRect;
	RECT									originalDestRect;*/
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



};

// Set rectangle from top-left, down-right
// top coordinate will be scaled automatically from texture size
//void setRect(float right, float bottom, RECT fullscreenRect)
//{
//	originalDestRect.right = static_cast<LONG>(right * fullscreenRect.right);
//	originalDestRect.bottom = static_cast<LONG>(bottom * fullscreenRect.bottom);

//	originalDestRect.top = originalDestRect.bottom - defaultScaling * DirectX::GetTextureSize(this->texture.Get()).y;
//	originalDestRect.left = originalDestRect.right - defaultScaling * DirectX::GetTextureSize(this->texture.Get()).x;

//	/*destRect.left = originalDestRect.left;
//	destRect.right = originalDestRect.right;
//	destRect.top = originalDestRect.top;
//	destRect.bottom = originalDestRect.bottom;*/
//	destRect = originalDestRect;
//}