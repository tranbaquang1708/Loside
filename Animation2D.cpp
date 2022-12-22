#include "pch.h"
#include "Animation2D.h"

void Animation2D::loadAnimation(const wchar_t* texturesPath, ID3D12Device* device, DirectX::ResourceUploadBatch& resourceUpload,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors, std::vector<bool>& m_descriptorStatuses,
	float arg_frameTime, bool arg_isLoop)
{
	size_t numOfFrames = 0;

	for (const auto& entry : std::filesystem::directory_iterator(texturesPath)) {
		numOfFrames++;
	}

	frames.resize(numOfFrames);

	for (const auto& entry : std::filesystem::directory_iterator(texturesPath)) {
		int frameNo = std::stoi(entry.path().stem());
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, entry.path().c_str(),
				frames[frameNo].ReleaseAndGetAddressOf()));

		// TODO: implement startIdx to decrease the number of loops
		int descriptorNo = pushToHeap(m_descriptorStatuses, frameNo);

		DirectX::CreateShaderResourceView(device, frames[frameNo].Get(),
			m_resourceDescriptors->GetCpuHandle(descriptorNo));
	}

	isLoop = arg_isLoop;
	frameTime = arg_frameTime;
	currentFrameIdx = 0;
	playedTime = 0;
}

void Animation2D::setRect(RECT& rect)
{
	destRect = rect;
}

// Set rectangle from top-left, down-right
// top coordinate will be scaled automatically from texture size
void Animation2D::setRect(LONG left, LONG right, LONG bottom)
{
	destRect.left = left;
	destRect.right = right;
	destRect.bottom = bottom;
	destRect.top = bottom
		- static_cast<LONG>(
			(
				(right - left) / static_cast<float>(DirectX::GetTextureSize(frames[0].Get()).x)
				)
			* DirectX::GetTextureSize(frames[0].Get()).y
			);

}

void Animation2D::draw(std::unique_ptr<DirectX::SpriteBatch>& m_spriteBatch,
	std::unique_ptr<DirectX::DescriptorHeap>& m_resourceDescriptors)
{
	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(descriptorMap.find(currentFrameIdx)->second),
		DirectX::GetTextureSize(frames[currentFrameIdx].Get()),
		destRect, nullptr, DirectX::Colors::White, 0.f);
}

void Animation2D::update(float elapsedTime)
{
	playedTime += elapsedTime;
	if (isLoop) {
		currentFrameIdx = static_cast<int>(playedTime / frameTime) % frames.size();
	}
	else {
		currentFrameIdx = std::min(static_cast<int>(playedTime / (frameTime * frames.size())),
			static_cast<int>(frames.size() - 1));
	}
}

void Animation2D::reset(std::vector<bool>& m_descriptorStatuses)
{
	for (Microsoft::WRL::ComPtr<ID3D12Resource>& f : frames) {
		f.Reset();
	}

	for (std::map<int, int>::iterator it = descriptorMap.begin(); it != descriptorMap.end(); ++it) {
		m_descriptorStatuses[it->second] = false;
	}
}

int Animation2D::pushToHeap(std::vector<bool>& m_descriptorStatuses, int frameNo, int startIdx)
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