//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <filesystem>

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    /*m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 120);*/

    // Additional initialization

    // Control
    m_gamePad = std::make_unique<GamePad>();
    m_keyboard = std::make_unique<Keyboard>();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    // Independent animations
    m_backgroundText.update(elapsedTime);
    // MYTODO: Set animations based on elapsedTime
    //auto time = static_cast<float>(timer.GetTotalSeconds());

    // Pad conntrols
    //auto pad = m_gamePad->GetState(0);

    //if (pad.IsConnected())
    //{
    //    if (pad.IsViewPressed())
    //    {
    //        ExitGame();
    //    }

    //    if (pad.IsLeftStickPressed())
    //    {
    //        m_yaw = m_pitch = 0.f;
    //    }
    //    else
    //    {
    //        constexpr float ROTATION_GAIN = 0.1f;
    //        m_yaw += -pad.thumbSticks.leftX * ROTATION_GAIN;
    //        m_pitch += pad.thumbSticks.leftY * ROTATION_GAIN;
    //    }
    //}

    // Keyboard controls
    Keyboard::State keyboardInput = m_keyboard->GetState();
#ifdef _DEBUG
    if (keyboardInput.Escape)
    {
        ExitGame();
    }
#endif

    // Protagonist
    m_protagonist.handleInput(keyboardInput);
    m_protagonist.update(elapsedTime);

    // Enemy

    // limit pitch to straight up or straight down
    /*constexpr float limit = XM_PIDIV2 - 0.01f;
    m_pitch = std::max(-limit, m_pitch);
    m_pitch = std::min(+limit, m_pitch);*/

    // keep longitude in sane range by wrapping
    /*if (m_yaw > XM_PI)
    {
        m_yaw -= XM_2PI;
    }
    else if (m_yaw < -XM_PI)
    {
        m_yaw += XM_2PI;
    }*/

    /*float y = sinf(m_pitch);
    float r = cosf(m_pitch);
    float z = r * cosf(m_yaw);
    float x = r * sinf(m_yaw);*/

    //m_cameraPos += move;
    //XMVECTOR lookAt = m_cameraPos +  Vector3(x, y, z);
    //XMVECTOR lookAt = Vector3(x, y, z);
    //m_view = XMMatrixLookAtRH(m_cameraPos, lookAt, Vector3::Up);
    //m_view = Matrix::CreateLookAt(
    //    //Vector3(2.f, 2.f, 2.f), Vector3(1.f, 0.f, 0.f), Vector3::UnitY);
    //    m_cameraPos, lookAt, Vector3::UnitY);

//#ifdef _DEBUG
//    std::wstringstream outSS(L"");
//    /*outSS << L"Camera: ";
//    outSS << m_cameraPos.x << L", ";
//    outSS << m_cameraPos.y << L", ";
//    outSS << m_cameraPos.z << L"\n";*/
//
//    /*outSS << L"Look at: ";
//    outSS << XMVectorGetX(lookAt) << L", ";
//    outSS << XMVectorGetY(lookAt) << L", ";
//    outSS << XMVectorGetZ(lookAt) << L"\n";*/
//
//    /*outSS << L"Fullscreen rect: ";
//    outSS << m_fullscreenRect.top << L", ";
//    outSS << m_fullscreenRect.left << L", ";
//    outSS << m_fullscreenRect.right << L", ";
//    outSS << m_fullscreenRect.bottom << L"\n";*/
//
//    outSS << L"FrameTime: " << elapsedTime << L"(" << 1/elapsedTime << L"FPS)" << L"\n";
//
//    outSS << L"-------------------------------------\n";
//
//    OutputDebugStringW(outSS.str().c_str());
//#endif
    /*auto time = static_cast<float>(timer.GetTotalSeconds());

    m_world = Matrix::CreateRotationZ(cosf(time) * 2.f);*/
    // END TODO
    elapsedTime;

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
    Clear();

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // TODO: Add your rendering code here.
    // Texture
    ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
    commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    m_spriteBatch->Begin(commandList);

    //    Background
    m_background.draw(m_spriteBatch, m_resourceDescriptors);
    m_backgroundText.draw(m_spriteBatch, m_resourceDescriptors);

    //    Ground
    m_ground.draw(m_spriteBatch, m_resourceDescriptors);
    m_groundText.draw(m_spriteBatch, m_resourceDescriptors);

    //    Protagonist
    //    Move hard coded resolution to config file
    m_protagonist.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);

    //    Enemy
    m_enemy.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);

    m_spriteBatch->End();
    
    
    // END TODO
    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());

    PIXEndEvent();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto const rtvDescriptor = m_deviceResources->GetRenderTargetView();
    auto const dsvDescriptor = m_deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto const viewport = m_deviceResources->GetScreenViewport();
    auto const scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
    m_gamePad->Resume();
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
    m_gamePad->Suspend();
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
    m_gamePad->Suspend();
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
    m_gamePad->Resume();
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1280;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // Check Shader Model 6 support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: Shader Model 6.0 is not supported!\n");
#endif
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    // TODO: Initialize device dependent objects here (independent of window size).
    // MYTODO: Move hard coded descriptor heap size to config file
    m_resourceDescriptors = std::make_unique<DescriptorHeap>(device, 20000);
    m_descriptorStatuses.resize(20000);

    // MYTODO: Move hard coded resolution to config file
    Vector2 resolution = Vector2(3840, 2160);

    ResourceUploadBatch resourceUpload(device);

    // Upload resources
    resourceUpload.Begin();

    //    Sprite batch
    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
        m_deviceResources->GetDepthBufferFormat());

    SpriteBatchPipelineStateDescription pd(rtState);
    m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

    //    Background
    m_background.loadTexture(L"../Assets/Backgrounds/white.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses);
    m_backgroundText.loadAnimation(L"../Assets/Backgrounds/Animations/BackgroundText", device, resourceUpload,
        m_resourceDescriptors, m_descriptorStatuses, 1., true);

    //    Ground
    m_ground.loadTexture(L"../Assets/Grounds/ground.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses);
    m_groundText.loadTexture(L"../Assets/Grounds/groundText.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses);

    //    Protagonist
    m_protagonist.loadTexture(L"../Assets/Protagonists/protagonist.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160));
    m_protagonist.setState(); // Set to idle state

    //    Enemy
    m_enemy.loadTexture(L"../Assets/Enemies/enemy.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160));

    auto uploadResourcesFinished = resourceUpload.End(
        m_deviceResources->GetCommandQueue());

    uploadResourcesFinished.wait();
    // Upload resources finished
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    auto viewport = m_deviceResources->GetScreenViewport();
    m_spriteBatch->SetViewport(viewport);

    m_fullscreenRect = m_deviceResources->GetOutputSize();



    // Set texture render rectangle
    //    Background
    m_background.setRect(m_fullscreenRect);
    m_backgroundText.setRect(m_fullscreenRect);

    //    Ground
    m_ground.setRect(m_fullscreenRect);
    m_groundText.setRect(m_fullscreenRect);

    //    Protagonist
    m_protagonist.setDefaultScaling(m_fullscreenRect);
    /*m_protagonist.setRect(static_cast<LONG>(m_fullscreenRect.right * 0.1),
        static_cast<LONG>(m_fullscreenRect.bottom * 0.85),
        m_fullscreenRect);*/
    m_protagonist.setPosition(XMFLOAT2(0.03f, 0.68f));

    float walkLength = m_protagonist.getNormalizedTextureSize(m_fullscreenRect).x * 0.5f;
    // Special quadratic function f(x) = -(1/2a) * (x - a)^2 + a/2
    // f(x) = 0 <=> x = 0 || x = 2a
    QuadraticFunction trajectory(-1.f/ (2 * walkLength), -walkLength, walkLength/2);
    m_protagonist.loadWalkAnimation(trajectory.sample(InputSampler::sampleInputUniform(0, 2 * walkLength, 10)), .15f);

    //   Enemy
    m_enemy.setDefaultScaling(m_fullscreenRect);
    m_enemy.setPosition(XMFLOAT2(0.83f, 0.798f));

//#ifdef _DEBUG
//    //auto sample = trajection.sample(0, m_fullscreenRect.right * (float)(0.025), 10);
//    //auto sample = trajection.sample(0, 200, 10);
//    auto sample = trajection.sample(0, 2 * walkLength, 10);
//    std::wstringstream outSS(L"");
//    for (size_t i = 0; i != sample.size(); ++i) {
//        outSS << sample[i].x << L", " << sample[i].y << L"\n";
//    }
//
//    outSS << L"-------------------------------------\n";
//
//    OutputDebugStringW(outSS.str().c_str());
//#endif
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    // Sprite batch
    m_spriteBatch.reset();
    
    // Background
    m_background.reset(m_descriptorStatuses);
    m_backgroundText.reset(m_descriptorStatuses);

    // Ground
    m_ground.reset(m_descriptorStatuses);
    m_groundText.reset(m_descriptorStatuses);

    // Protagonist
    m_protagonist.reset(m_descriptorStatuses);
    
    // Enemy
    m_enemy.reset(m_descriptorStatuses);

    // END TODO
    // If using the DirectX Tool Kit for DX12, uncomment this line:
     m_graphicsMemory.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
