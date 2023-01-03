//
// Enemy.h
//

#pragma once

#include <pch.h>
#include "Ailment.h"
#include "InputSampler.h"

#include <sstream>

class Enemy
{
public:
	enum States : unsigned short
	{
		IdleState,
		WalkingLeftState,
		WalkingRightState,
<<<<<<< HEAD
		StraightFalling
=======
>>>>>>> origin/master
	};

	enum AttackedState : unsigned short
	{
		None,
<<<<<<< HEAD
		//HitStun,
=======
		HitStun,
>>>>>>> origin/master
		PushedBackRight,
		PushedBackLeft
	};

<<<<<<< HEAD
	enum TransformState : unsigned short
	{
		NotTransformed,
		Frog
	};

	enum VisibilityState : unsigned short
	{
		Visible,
		NonExistence
	};

=======
>>>>>>> origin/master
	void loadTexture(const wchar_t* texturePath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		std::vector<bool>& m_descriptorStatuses, DirectX::XMUINT2 resolution);
	void loadWalkAnimation(std::vector<DirectX::XMFLOAT2> sampledTrajectory,
		std::vector<float> sampledAngles,
		float time);

	void setDefaultScaling(RECT fullscreenRect);
	void setPosition(DirectX::XMFLOAT2 arg_position);
	void setAilment(unsigned short _ailment);
	void setState();
	void setState(unsigned short state);
	void setWalkState();
<<<<<<< HEAD
	void setTransformState(unsigned short state);
	void setVisibilityState(unsigned short state);


	DirectX::XMFLOAT2 getPosition();
	DirectX::XMFLOAT2 getTextureSize();
	float getProtagonistBottomRightX();
	unsigned short getAilment();
	unsigned short getState();
	unsigned short getTransformState();
	unsigned short getVisibilityState();
	bool getIsPetrified();
	
=======

	DirectX::XMFLOAT2 getPosition();
	DirectX::XMFLOAT2 getTextureSize();
	unsigned short getAilment();
>>>>>>> origin/master

	void update(float elapsedTime, float arg_protagonistBottomRightX);

	void draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect);
	void drawAilment(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
		std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
		RECT fullscreenRect, Ailment ailmentObject);

	void reset(std::vector<bool>& m_descriptorStatuses);
	
	void playStayAnimation(float elapsedTime);
	void playWalkAnimation(float elapsedTime);
<<<<<<< HEAD
	void playStraightFallAnimation(float elapsedTime);
	void playPushedBackAnimation(float elapsedTime);
	void playPetrifedAnimation(float elapsedTime);

	void straightFall();
	void getAttacked(float _stunTime);
	void getPushedBack(float displacement, float duration);
	void getPetrified(float duration);
=======
	void playPushedBackAnimation(float elapsedTime);

	void getAttacked(float _stunTime);
	void getPushedBack(float displacement, float duration);
>>>>>>> origin/master
	
private:
	int pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx = 0);

	Microsoft::WRL::ComPtr<ID3D12Resource>	texture;
	DirectX::XMUINT2						textureResolution;
	float									defaultScaling;
	DirectX::XMFLOAT2						size;

<<<<<<< HEAD
	DirectX::XMVECTOR						color;
=======
	DirectX::XMVECTORF32					color;
>>>>>>> origin/master

	DirectX::XMFLOAT2						position;
	DirectX::XMFLOAT2						originalPosition;
	DirectX::XMFLOAT2						attackedOriginalPosition;

	float									rotation;
	float									originalRotation;

	int										descriptorMap;

	float									animationPlayedTime;

	float									idleAnimationTime;

	float									walkAnimationTime;
	std::vector<DirectX::XMFLOAT2>			walkTrajectory;
	std::vector<float>						walkAngles;

	float									attackedAnimationPlayedTime;

	float									pushedBackTime;
	std::vector< DirectX::XMFLOAT2>			pushedBackTrajectory;

<<<<<<< HEAD
	bool									isPetrified;
	float									petrifiedTime;
	float									petrifiedPassedTime;
	float									straightFallTime;
	std::vector< DirectX::XMFLOAT2>			straightFallTrajectory;

	float									hitStunTime;
	float									hitStunPassedTime;
	bool									isHitStun;

	unsigned short							currentState;
	unsigned short							currentAttackedState;
	unsigned short							currentTransformState;
	unsigned short							currentVisibilityState;
=======
	float									stunTime;

	unsigned short							currentState;
	unsigned short							currentAttackedState;
>>>>>>> origin/master

	float									protagonistBottomRightX;
	
	unsigned short							ailment; // == Attack::Type
<<<<<<< HEAD
=======

>>>>>>> origin/master
};