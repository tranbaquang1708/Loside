//
// Enemy.h
//

#pragma once

#include <pch.h>
#include "Ailment.h"

//#include <sstream>

class Enemy
{
public:
	enum States : unsigned short
	{
		IdleState,
		WalkingLeftState,
		WalkingRightState,
	};

	void loadTexture(const wchar_t* texturePath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses, DirectX::XMUINT2 resolution);
	void loadWalkAnimation(std::vector<DirectX::XMFLOAT2> sampledTrajectory,
		std::vector<float> sampledAngles,
		float time);
	void loadGetAttackedAnimation(float length);

	void setDefaultScaling(RECT fullscreenRect);
	void setPosition(DirectX::XMFLOAT2 arg_position);
	DirectX::XMFLOAT2 getNormalizedTextureSize(RECT fullscreenRect);

	void setAilment(unsigned short ailmentEnum);

	void setState(unsigned short state);
	void setState();
	void setWalkState();

	DirectX::XMFLOAT2 getPosition();
	DirectX::XMFLOAT2 getTextureSize();
	unsigned short getAilment();

	void update(float elapsedTime, float arg_protagonistBottomRightX);

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect);
	void drawAilment(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect, Ailment ailmentObject);

	void reset(std::vector<bool>& m_descriptorStatuses);
	
	void stay(float elapsedTime);
	void walk(float elapsedTime);

	void getAttacked();
	
	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx = 0);

private:
	Microsoft::WRL::ComPtr<ID3D12Resource>	texture;
	DirectX::XMUINT2						textureResolution;
	float									defaultScaling;
	DirectX::XMFLOAT2						size;

	DirectX::XMVECTORF32					color;

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

	bool									isAttacked;
	float									getAttackedAnimationTime;
	float									stunTime;

	unsigned short							currentState;

	float									protagonistBottomRightX;
	
	unsigned short							ailment; // == Attack::Type

};