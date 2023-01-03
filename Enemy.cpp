#include "pch.h"
#include "Enemy.h"

void Enemy::loadTexture(const wchar_t* texturePath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
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
	color = DirectX::Colors::White;
	isPetrified = false;

	currentAttackedState = None;
}

void Enemy::loadWalkAnimation(
	std::vector<DirectX::XMFLOAT2> sampledTrajectory,
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

void Enemy::setDefaultScaling(RECT fullscreenRect)
{
	defaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left) / textureResolution.x;

	DirectX::XMUINT2 originalSize = DirectX::GetTextureSize(this->texture.Get());
	size.x = originalSize.x * defaultScaling / (fullscreenRect.right);
	size.y = originalSize.y * defaultScaling / (fullscreenRect.bottom);
}

void Enemy::setPosition(DirectX::XMFLOAT2 arg_position)
{
	position = arg_position;
	originalPosition = arg_position;
}

void Enemy::setAilment(unsigned short _ailment)
{
	ailment = _ailment;
}

void Enemy::setState()
{
	setState(IdleState);
}

void Enemy::setState(unsigned short state)
{
	this->currentState = state;

	animationPlayedTime = 0;
	originalPosition = position;
	originalRotation = rotation;
}

void Enemy::setWalkState()
{
	if (protagonistBottomRightX > originalPosition.x) {
		setState(WalkingRightState);
	}
	else {
		setState(WalkingLeftState);
	}
}

void Enemy::setTransformState(unsigned short state)
{
	currentTransformState = state;
}

void Enemy::setVisibilityState(unsigned short state)
{
	currentVisibilityState = state;
}

DirectX::XMFLOAT2 Enemy::getPosition()
{
	return position;
}

DirectX::XMFLOAT2 Enemy::getTextureSize()
{
	return size;
}

float Enemy::getProtagonistBottomRightX()
{
	return protagonistBottomRightX;
}

unsigned short Enemy::getState()
{
	return currentState;
}

unsigned short Enemy::getAilment()
{
	return ailment;
}

unsigned short Enemy::getTransformState()
{
	return currentTransformState;
}

unsigned short Enemy::getVisibilityState()
{
	return currentVisibilityState;
}

bool Enemy::getIsPetrified()
{
	return isPetrified;
}

void Enemy::update(float elapsedTime, float arg_protagonistBottomRightX)
{
	unsigned short priority = 0;


	if (currentVisibilityState == VisibilityState::NonExistence)
		priority = 1;

	if (priority == 1)
		return;

	if (isHitStun) {
		priority = 2;

		hitStunPassedTime += elapsedTime;
		if (hitStunPassedTime > hitStunTime) {
			//currentAttackedState = None;
			isHitStun = false;
		}
	}

	if (priority == 2)
		return;

	if (currentState == StraightFalling) {
		playStraightFallAnimation(elapsedTime);
		priority = 3;
	}

	/*if (currentAttackedState == Petrified) {
		playPetrifedAnimation(elapsedTime);
		return;
	}*/
	if (isPetrified) {
		playPetrifedAnimation(elapsedTime);
		priority = 3;
	}

	if (currentAttackedState == PushedBackRight || currentAttackedState == PushedBackLeft) {
		playPushedBackAnimation(elapsedTime);
		priority = 3;
	}

	if (priority == 3)
		return;

	protagonistBottomRightX = arg_protagonistBottomRightX;

	if (currentState == IdleState) {
		playStayAnimation(elapsedTime);
		return;
	}

	if (currentState == WalkingLeftState || currentState == WalkingRightState) {
		playWalkAnimation(elapsedTime);
		return;
	}
}

void Enemy::draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	RECT fullscreenRect)
{
	if (currentVisibilityState == VisibilityState::NonExistence)
		return;

	DirectX::XMUINT2 textureSize = DirectX::GetTextureSize(this->texture.Get());

	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
		textureSize,
		DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
		nullptr,
		color, rotation,
		DirectX::XMFLOAT2(textureSize.x / 2.f, textureSize.y / 2.f),
		defaultScaling);
}

void Enemy::drawAilment(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	RECT fullscreenRect, Ailment ailmentObject)
{
	ailmentObject.setPosition(DirectX::XMFLOAT2(position.x, position.y - getTextureSize().y));
	ailmentObject.draw(m_spriteBatch, m_resourceDescriptors, fullscreenRect);
}

void Enemy::reset(std::vector<bool>& m_descriptorStatuses)
{
	texture.Reset();
	m_descriptorStatuses[descriptorMap] = false;
}

void Enemy::playStayAnimation(float elapsedTime)
{
	// MYTODO: Need better stay function
	animationPlayedTime += elapsedTime;
	//if (animationPlayedTime > idleAnimationTime) // hard-code idleAnimationTime = 0.3
	if (animationPlayedTime > 0.3f) { // hard-code idleAnimationTime = 0.3
		setWalkState();
	}
}

void Enemy::playWalkAnimation(float elapsedTime)
{
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
	}
}

void Enemy::playStraightFallAnimation(float elapsedTime)
{
	animationPlayedTime += elapsedTime;

	size_t currentPositionIdx = std::min(
		straightFallTrajectory.size() - 1,
		static_cast<size_t>(animationPlayedTime * (straightFallTrajectory.size() - 1) / straightFallTime)
	);

	position.y = originalPosition.y + straightFallTrajectory[currentPositionIdx].y;

	if (currentPositionIdx == straightFallTrajectory.size() - 1) {
		currentState = IdleState;
		getPetrified(petrifiedTime);
	}
}

void Enemy::playPushedBackAnimation(float elapsedTime)
{
	attackedAnimationPlayedTime += elapsedTime;

	size_t currentPositionIdx = std::min(
		pushedBackTrajectory.size() - 1,
		static_cast<size_t>(attackedAnimationPlayedTime * (pushedBackTrajectory.size() - 1) / pushedBackTime)
	);

	if (currentAttackedState == PushedBackRight) {
		position.x = attackedOriginalPosition.x + pushedBackTrajectory[currentPositionIdx].x;
	}
	else {
		position.x = attackedOriginalPosition.x - pushedBackTrajectory[currentPositionIdx].x;
	}

	if (currentPositionIdx == pushedBackTrajectory.size() - 1) {
		if (currentAttackedState == PushedBackRight) {
			originalPosition.x += pushedBackTrajectory[currentPositionIdx].x;
		}
		else {
			originalPosition.x -= pushedBackTrajectory[currentPositionIdx].x;
		}
		currentAttackedState = None;
		getAttacked(0.1f);
	}
}

void Enemy::playPetrifedAnimation(float elapsedTime)
{
	petrifiedPassedTime += elapsedTime;

	if (petrifiedPassedTime > petrifiedTime) {
		color = DirectX::Colors::White;
		isPetrified = false;
		getAttacked(0.05f);
	}
}

void Enemy::straightFall()
{
	getAttacked(0.7f);

	animationPlayedTime = 0.f;

	originalPosition = position;
	straightFallTime = 0.03f;

	std::vector<float> straightFallYTrajectory = InputSampler::sampleInputFriction(0, 0.832f - position.y, 8);
	straightFallTrajectory.clear();
	std::transform(straightFallYTrajectory.begin(), straightFallYTrajectory.end(),
		back_inserter(straightFallTrajectory),
		[this](float const& y) { return DirectX::XMFLOAT2(position.x, y); });

	currentState = StraightFalling;
}

void Enemy::getAttacked(float _stunTime)
{
	hitStunTime = _stunTime;
	isHitStun = true;
	hitStunPassedTime = 0;
}

void Enemy::getPushedBack(float displacement, float duration)
{
	// MYTODO: Find better way to implement push back direction
	// If currentAttackedState is not None then update original position (not implemented)

	// Is enemy is petrified -> half the displacement
	//if (currentAttackedState == Petrified)
	if (isPetrified)
		displacement *= 0.5f;

	isHitStun = false; // If enemy is being hit stunned -> cancel hit stun
	pushedBackTime = duration;
	attackedOriginalPosition = position;
	attackedAnimationPlayedTime = 0;

	if (displacement > 0) {
		currentAttackedState = PushedBackRight;
	}
	else {
		currentAttackedState = PushedBackLeft;
		displacement = -displacement;
	}

	std::vector<float> pushedBackXTrajectory = InputSampler::sampleInputFriction(0, displacement, 10);
	pushedBackTrajectory.clear();
	std::transform(pushedBackXTrajectory.begin(), pushedBackXTrajectory.end(), back_inserter(pushedBackTrajectory),
		[this](float const& x) { return DirectX::XMFLOAT2(x, position.y); });
}

void Enemy::getPetrified(float duration)
{
	if (currentAttackedState != None 
		&& currentAttackedState != PushedBackRight 
		&& currentAttackedState != PushedBackLeft) 
	{
		originalPosition.x += position.x - attackedOriginalPosition.x;
	}
	petrifiedTime = duration;
	petrifiedPassedTime = 0.f;
	color = DirectX::Colors::White * 0.7f;
	//currentAttackedState = Petrified;
	isPetrified = true;
}

int Enemy::pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx)
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