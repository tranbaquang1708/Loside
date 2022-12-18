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

	isAttacked = false;
}

void Enemy::loadGetAttackedAnimation(float length)
{
	getAttackedAnimationTime = length;
	stunTime = 0.f;
}


void Enemy::update(float elapsedTime, float arg_protagonistBottomRightX)
{
	if (isAttacked) {
		stunTime += elapsedTime;
		if (stunTime > getAttackedAnimationTime) {
			isAttacked = false;
		}
		return;
	}

	protagonistBottomRightX = arg_protagonistBottomRightX;

	if (currentState == IdleState) {
		stay(elapsedTime);
	}
	if (currentState == WalkingLeftState || currentState == WalkingRightState) {
		walk(elapsedTime);
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
	 //OutputDebugStringW(L"Draw-----------------\n");
	ailmentObject.position.x = position.x;
	ailmentObject.position.y = position.y - getTextureSize().y;
	ailmentObject.draw(m_spriteBatch, m_resourceDescriptors, fullscreenRect);
}

void Enemy::reset(std::vector<bool>& m_descriptorStatuses)
{
	texture.Reset();
	m_descriptorStatuses[descriptorMap] = false;
}

void Enemy::loadWalkAnimation(std::vector<DirectX::XMFLOAT2> sampledTrajectory,
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

void Enemy::setState()
{
	setState(IdleState);
}

void Enemy::stay(float elapsedTime)
{
	// MYTODO: Need better stay function
	animationPlayedTime += elapsedTime;
	//if (animationPlayedTime > idleAnimationTime) // hard-code idleAnimationTime = 0.3
	if (animationPlayedTime > 0.3f) { // hard-code idleAnimationTime = 0.3
		setWalkState();
	}
}

unsigned short Enemy::getAilment()
{
	return ailment;
}


void Enemy::walk(float elapsedTime)
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

/*void applyAilment(Attack::Attack *attack)
{
	if (ailment != NULL) {
		ailment.apply(attack);
	}
	else {
		ailment = attack;

	}
}*/

DirectX::XMFLOAT2 Enemy::getTextureSize()
{
	return size;
}

DirectX::XMFLOAT2 Enemy::getPosition()
{
	return position;
}

void Enemy::setAilment(unsigned short inAilment)
{
	ailment = inAilment;
}

void Enemy::getAttacked()
{
	isAttacked = true;
	stunTime = 0;
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