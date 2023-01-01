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

DirectX::XMFLOAT2 Enemy::getPosition()
{
	return position;
}

DirectX::XMFLOAT2 Enemy::getTextureSize()
{
	return size;
}

unsigned short Enemy::getAilment()
{
	return ailment;
}

void Enemy::update(float elapsedTime, float arg_protagonistBottomRightX)
{
	if (isHitStun) {
		hitStunPassedTime += elapsedTime;
		if (hitStunPassedTime > hitStunTime) {
			//currentAttackedState = None;
			isHitStun = false;
		}
		return;
	}

	if (currentAttackedState == PushedBackRight || currentAttackedState == PushedBackLeft) {
		playPushedBackAnimation(elapsedTime);
		return;
	}

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
		currentAttackedState = None;
		getAttacked(0.1f);
	}
}

void Enemy::getAttacked(float _stunTime)
{
	hitStunTime = _stunTime;
	//currentAttackedState = HitStun;
	isHitStun = true;
	hitStunPassedTime = 0;
}

void Enemy::getPushedBack(float displacement, float duration)
{
	// MYTODO: Find better way to implement push back direction
	// If currentAttackedState is not None then update original position (not implemented)
	originalPosition.x += displacement;
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