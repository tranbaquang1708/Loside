#include "pch.h"
#include "Attack.h"

void Attack::loadAnimation(const wchar_t* texturesPath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors, std::vector<bool>& m_descriptorStatuses,
	DirectX::XMUINT2 resolution, float arg_frameTime, float animationLength)
{
#ifdef _DEBUG
	assert(((arg_frameTime != 0 && animationLength == 0) || (arg_frameTime == 0 && animationLength != 0))
		&&
		"Attack:: Please provide either frame time or animation length.");
#endif

	int numOfFrames = 0;

	for (const auto& entry : std::filesystem::directory_iterator(texturesPath)) {
		numOfFrames++;
	}

	frames.resize(numOfFrames);

	for (const auto& entry : std::filesystem::directory_iterator(texturesPath)) {
		int frameNo = std::stoi(entry.path().stem());
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, entry.path().c_str(),
				this->frames[frameNo].ReleaseAndGetAddressOf()));

		// TODO: implement startIdx to decrease the number of loops
		int descriptorNo = pushToHeap(m_descriptorStatuses, frameNo);

		DirectX::CreateShaderResourceView(device, this->frames[frameNo].Get(),
			m_resourceDescriptors->GetCpuHandle(descriptorNo));
	}

	if (arg_frameTime != 0) {
		this->frameTime = arg_frameTime;
	}
	else {
		this->frameTime = animationLength / frames.size();
	}

	textureResolution = resolution;
	currentFrameIdx = 0;
	animationPlayedTime = 0;
	position = DirectX::XMFLOAT2(0.f, 0.f);
	origin.x = DirectX::GetTextureSize(frames[0].Get()).x / 2.f;
	origin.y = DirectX::GetTextureSize(frames[0].Get()).y / 2.f;

	isOn = false;
}

void Attack::setDefaultScaling(RECT fullscreenRect)
{
	defaultScaling = static_cast<float>(fullscreenRect.right - fullscreenRect.left) / textureResolution.x;

	DirectX::XMUINT2 originalSize = DirectX::GetTextureSize(this->frames[0].Get());
	size.x = originalSize.x * defaultScaling / (fullscreenRect.right);
	size.y = originalSize.y * defaultScaling / (fullscreenRect.bottom);
}

void Attack::setIsOn(bool _isOn)
{
	isOn = _isOn;
}

bool Attack::getIsOn()
{
	return isOn;
}

float Attack::getAnimationTime()
{
	return frameTime * frames.size();
}

DirectX::XMFLOAT2 Attack::getSize()
{
	return size;
}


void Attack::draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors,
	RECT fullscreenRect)
{
	if (!isOn) return;

	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap.find(currentFrameIdx)->second),
		DirectX::GetTextureSize(this->frames[currentFrameIdx].Get()),
		DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
		nullptr, DirectX::Colors::White, 0.f, origin, defaultScaling);

	if (currentFrameIdx < frames.size() - 1) {
		m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap.find(currentFrameIdx + 1)->second),
			DirectX::GetTextureSize(this->frames[currentFrameIdx + 1].Get()),
			DirectX::XMFLOAT2(position.x * fullscreenRect.right, position.y * fullscreenRect.bottom),
			nullptr, DirectX::Colors::White * 0.05f, 0.f, origin, defaultScaling);
	}
}

void Attack::update(float elapsedTime)
{
	if (!isOn) return;

	this->animationPlayedTime += elapsedTime;

	this->currentFrameIdx = static_cast<int>(this->animationPlayedTime / this->frameTime);

	if (this->currentFrameIdx > this->frames.size() - 1) {
		isOn = false;
	}
}

void Attack::reset(std::vector<bool>& m_descriptorStatuses)
{
	for (Microsoft::WRL::ComPtr<ID3D12Resource>& f : frames) {
		f.Reset();
	}

	for (std::map<int, int>::iterator it = descriptorMap.begin(); it != descriptorMap.end(); ++it) {
		m_descriptorStatuses[it->second] = false;
	}
}

void Attack::attack(DirectX::XMFLOAT2 inPosition)
{
	isOn = true;
	position = inPosition;
	animationPlayedTime = 0;
}

int Attack::pushToHeap(std::vector<bool>& m_descriptorStatuses, int frameNo, int startIdx)
{
	for (int i = startIdx; i < m_descriptorStatuses.size(); ++i) {
		if (!m_descriptorStatuses[i]) {
			descriptorMap.insert({ frameNo, i });
			m_descriptorStatuses[i] = true;
			return i;
		}
	}

	assert(false && "Heap is full");
	return -1;
}