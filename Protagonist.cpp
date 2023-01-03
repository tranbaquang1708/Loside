//
// Protagonist.cpp
//

#include "pch.h"
#include "Protagonist.h"

void Protagonist::loadTexture(const wchar_t* texturePath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
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

void Protagonist::loadWalkAnimation(std::vector<DirectX::XMFLOAT2> sampledTrajectory, float time)
{
	animationPlayedTime = 0;

	walkTrajectory = sampledTrajectory;
	walkAnimationTime = time;
}

void Protagonist::loadAttackInterface(AttackInterfaceFire* _attackInterfaceFire,
	AttackInterfaceStone* _attackInterfaceStone,
	AttackInterfaceTransform* _attackInterfaceTransform)
{
	attackInterfaceFire = _attackInterfaceFire;
	attackInterfaceStone = _attackInterfaceStone;
	attackInterfaceTransform = _attackInterfaceTransform;
}

void Protagonist::setDefaultScaling(RECT fullscreenRect)
{
	defaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left) / textureResolution.x;
	DirectX::XMUINT2 originalSize = DirectX::GetTextureSize(this->texture.Get());
	size.x = originalSize.x * defaultScaling / (fullscreenRect.right);
	size.y = originalSize.y * defaultScaling / (fullscreenRect.bottom);
}

void Protagonist::setPosition(DirectX::XMFLOAT2 arg_position)
{
	position = arg_position;
	originalPosition = arg_position;
}

void Protagonist::setState()
{
	setState(IdleState);
}

void Protagonist::setState(unsigned short state)
{
	this->currentState = state;

	animationPlayedTime = 0;
	originalPosition = position;
}

DirectX::XMFLOAT2 Protagonist::getPosition()
{
	return position;
}

DirectX::XMFLOAT2 Protagonist::getTextureSize()
{
	return size;
}

void Protagonist::update(float elapsedTime, Enemy* inEnemy)
{
	if (currentState == WalkingLeftState || currentState == WalkingRightState) {
		walk(elapsedTime);
	}

	targetingEnemy = inEnemy;
}

void Protagonist::draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	RECT fullscreenRect)
{
	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap),
		DirectX::GetTextureSize(this->texture.Get()),
		DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
		nullptr, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0, 0),
		defaultScaling);
}

void Protagonist::reset(std::vector<bool>& m_descriptorStatuses)
{
	texture.Reset();
	m_descriptorStatuses[descriptorMap] = false;
}

void Protagonist::walk(float elapsedTime)
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

void Protagonist::cancelWalk()
{
	position.y = originalPosition.y;
	setState(IdleState);
}

void Protagonist::cancelAllAttacks()
{
	attackInterfaceFire->cancelAllAttacks();
	attackInterfaceStone->cancelAllAttacks();
	attackInterfaceTransform->cancelAllAttacks();
}

void Protagonist::handleInput(DirectX::Keyboard::State keyboardInput, DirectX::Keyboard::KeyboardStateTracker& keyboardTracker)
{
	keyboardTracker.Update(keyboardInput);

	if (keyboardInput.Left && keyboardInput.Right) {
		cancelWalk();
	}
	else if (keyboardInput.Left) {
		if (currentState != WalkingLeftState && currentState != WalkingRightState) {
			setState(WalkingLeftState);
		}

		if (currentState == WalkingRightState) cancelWalk();
	}
	else if (keyboardInput.Right) {
		if (currentState != WalkingLeftState && currentState != WalkingRightState) {
			setState(WalkingRightState);
		}

		if (currentState == WalkingLeftState) cancelWalk();
	}

	if (keyboardTracker.pressed.X) { // X for fire
		if (targetingEnemy != nullptr && !(attackInterfaceFire->getIsCoolingDown())) {
			cancelAllAttacks();
			attackInterfaceFire->attack(targetingEnemy);
		}
	}

	if (keyboardTracker.pressed.C) { // C for stone
		if (targetingEnemy != nullptr && !(attackInterfaceStone->getIsCoolingDown())) {
			cancelAllAttacks();
			attackInterfaceStone->attack(targetingEnemy, position.x);
		}
	}

	if (keyboardTracker.pressed.S) { // S for transform
		if (targetingEnemy != nullptr && !(attackInterfaceTransform->getIsCoolingDown())) {
			cancelAllAttacks();
			attackInterfaceTransform->attack(targetingEnemy);
		}
	}
}

int Protagonist::pushToHeap(std::vector<bool>& m_descriptorStatuses, int startIdx)
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