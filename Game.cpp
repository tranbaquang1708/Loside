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
    
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 120);

    // Additional initialization

    // Control
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

    // Keyboard controls
    Keyboard::State keyboardInput = m_keyboard->GetState();
#ifdef _DEBUG
    if (keyboardInput.Escape)
    {
        ExitGame();
    }
#endif

    // Protagonist
    m_protagonist.handleInput(keyboardInput, m_keyboardTracker);
    //m_protagonist.update(elapsedTime, &m_enemies[0]/*.data()*/);
    m_protagonist.update(elapsedTime, &m_enemy);


    // Enemy
    /*for (Enemy& enemy : m_enemies) {
        enemy.update(elapsedTime, m_protagonist.getPosition().x);
    }*/
    m_enemy.update(elapsedTime, m_protagonist.getPosition().x);

    // Attack
    m_attackInterfaceFire.update(elapsedTime);
    m_attackInterfaceStone.update(elapsedTime);
    m_attackInterfaceTransform.update(elapsedTime);

    m_attackFire.update(elapsedTime);
    m_attackFlame.update(elapsedTime);
    m_attackStone.update(elapsedTime);
    m_attackTransform.update(elapsedTime);

    // UI
    m_skillUIFire.update();
    m_skillUIStone.update();
    m_skillUITransform.update();

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
    //    MYTODO: Move hard coded resolution to config file
    m_protagonist.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);

    //    Enemy
    /*for (Enemy& enemy : m_enemies) {
        enemy.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);
        if (enemy.getAilment() != Ailment::None) {
            enemy.drawAilment(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect, m_ailments.at(enemy.getAilment()));
        }
    }*/
    m_enemy.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);
    if (m_enemy.getAilment() != Ailment::None) {
        m_enemy.drawAilment(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect, m_ailments.at(m_enemy.getAilment()));
    }

    //    Attack
    m_attackFire.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);
    m_attackFlame.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);
    m_attackStone.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);
    m_attackTransform.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);

    //    UI
    m_skillUIFire.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);
    m_skillUIStone.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);
    m_skillUITransform.draw(m_spriteBatch, m_resourceDescriptors, m_fullscreenRect);

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
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
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
    // FIXME: Y-position got changed on resizing window
    m_protagonist.loadTexture(L"../Assets/Protagonists/protagonist.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160));
    m_protagonist.setState(); // Set to idle state
    m_protagonist.setPosition(XMFLOAT2(0.03f, 0.68f));

    //    Enemy
    //m_enemies.push_back(Enemy());
    m_enemy.loadTexture(L"../Assets/Enemies/enemy.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160));
    m_enemy.setState();
    m_enemy.setPosition(XMFLOAT2(0.9f, 0.835f));
    m_enemy.setAilment(Ailment::None);

    m_frog.loadTexture(L"../Assets/Enemies/enemy.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160));
    m_frog.setState();
    m_frog.setPosition(XMFLOAT2(0.8f, 0.835f));
    m_frog.setAilment(Ailment::None);

    //      Attacks
    m_attackFire.loadAnimation(L"../Assets/Attacks/Fire", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160), 0, .3f);

    m_attackFlame.loadAnimation(L"../Assets/Attacks/Flame", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160), 0.025f);

    m_attackStone.loadAnimation(L"../Assets/Attacks/Stone", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160), 1/18.f);

    m_attackTransform.loadAnimation(L"../Assets/Attacks/Transform", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160), 0, .3f);
    m_attackTransform.loadFrog(&m_frog);

    m_attackInterfaceFire.loadAttack(&m_attackFire, &m_attackFlame);
    m_attackInterfaceFire.setCoolDownTime(.8f);

    m_attackInterfaceStone.loadAttack(&m_attackStone);
    m_attackInterfaceStone.setCoolDownTime(.8f);

    m_attackInterfaceTransform.loadAttack(&m_attackTransform);
    m_attackInterfaceTransform.setCoolDownTime(.8f);

    m_protagonist.loadAttackInterface(&m_attackInterfaceFire, &m_attackInterfaceStone, &m_attackInterfaceTransform);

    //    Ailment
    //      Fire
    Ailment ailmentFire;
    ailmentFire.loadTexture(L"../Assets/Ailments/Fire/fire.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160));
    m_ailments.insert({ Ailment::Fire, ailmentFire });

    Ailment ailmentStone;
    ailmentStone.loadTexture(L"../Assets/Ailments/Stone/stone.dds", device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160));
    m_ailments.insert({ Ailment::Stone, ailmentStone });

    //    UI
    m_skillUIFire.loadTexture(L"../Assets/UI/Skills/Fire/fire.dds", L"../Assets/UI/KeyboardIcons/x.dds",
        device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160), XMUINT2(3840, 2160));
    m_skillUIFire.setAttackInterface(&m_attackInterfaceFire);

    m_skillUIStone.loadTexture(L"../Assets/UI/Skills/Stone/stone.dds", L"../Assets/UI/KeyboardIcons/c.dds",
        device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160), XMUINT2(3840, 2160));
    m_skillUIStone.setAttackInterface(&m_attackInterfaceStone);

    m_skillUITransform.loadTexture(L"../Assets/UI/Skills/Transform/0.dds", L"../Assets/UI/KeyboardIcons/s.dds",
        device, resourceUpload, m_resourceDescriptors,
        m_descriptorStatuses, XMUINT2(3840, 2160), XMUINT2(3840, 2160));
    m_skillUITransform.setAttackInterface(&m_attackInterfaceTransform);

    auto uploadResourcesFinished = resourceUpload.End(
        m_deviceResources->GetCommandQueue());

    uploadResourcesFinished.wait();
    // Upload resources finished
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    // MYTODO: Move all hard-coded values to config file
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

    float walkLength = m_protagonist.getTextureSize().x * 0.8f;
    // Special quadratic function f(x) = -(1/2a) * (x - a)^2 + a/2
    // f(x) = 0 <=> x = 0 || x = 2a
    QuadraticFunction protagWalkTrajectory(-1.f/ (2 * walkLength), -walkLength, walkLength/2);
    m_protagonist.loadWalkAnimation(
        protagWalkTrajectory.sample(InputSampler::sampleInputUniform(0, 2 * walkLength, 20)),
        .15f
    );

    //    Enemy
    m_enemy.setDefaultScaling(m_fullscreenRect);

    walkLength = m_enemy.getTextureSize().x;
    halfCircleYPositiveFunction enemyWalkTrajectory(walkLength * sqrtf(0.5f), walkLength * 0.5f, -walkLength * 0.5f);
    std::vector<float> walkAngles = InputSampler::sampleInputFalling(3 * DirectX::XM_PIDIV4, DirectX::XM_PIDIV4, 10);
    std::vector<float> rollAngles = InputSampler::sampleInputFalling(0, DirectX::XM_PIDIV2, 10);

    m_enemy.loadWalkAnimation(
        enemyWalkTrajectory.sampleByAngle(walkAngles),
        rollAngles,
        0.3f
    );

    m_frog.setDefaultScaling(m_fullscreenRect);

    walkLength = m_frog.getTextureSize().x;
    QuadraticFunction frogWalkTrajectory(-1.f / (2 * walkLength), -walkLength, walkLength / 2);
    std::vector<float> frogWalkAngles = InputSampler::sampleInputUniform(0, 2 * walkLength, 20);
    //std::vector<float> rollAngles = InputSampler::sampleInputFalling(0, DirectX::XM_PIDIV2, 10);
    std::vector<float> frogRollAngles(frogWalkAngles.size(), 0.f);

    m_frog.loadWalkAnimation(
        frogWalkTrajectory.sample(frogWalkAngles),
        frogRollAngles,
        0.3f
    );

    //    Attack
    m_attackFire.setDefaultScaling(m_fullscreenRect);
    m_attackFlame.setDefaultScaling(m_fullscreenRect);
    m_attackStone.setDefaultScaling(m_fullscreenRect);
    m_attackTransform.setDefaultScaling(m_fullscreenRect);

    //    Ailment
    for (auto& a : m_ailments) {
        a.second.setDefaultScaling(m_fullscreenRect);
    }

    //   UI
    m_skillUIFire.setDefaultScaling(m_fullscreenRect);
    m_skillUIFire.setPosition(XMFLOAT2(0.9f, 0.92f));

    m_skillUIStone.setDefaultScaling(m_fullscreenRect);
    m_skillUIStone.setPosition(XMFLOAT2(0.938f, 0.883f));

    m_skillUITransform.setDefaultScaling(m_fullscreenRect);
    m_skillUITransform.setPosition(XMFLOAT2(0.855f, 0.883f));
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
    
    // Attack
    m_attackFire.reset(m_descriptorStatuses);
    m_attackFlame.reset(m_descriptorStatuses);
    m_attackStone.reset(m_descriptorStatuses);
    m_attackTransform.reset(m_descriptorStatuses);
    
    // Enemy
    /*for (Enemy& enemy : m_enemies) {
        enemy.reset(m_descriptorStatuses);
    }*/
    m_enemy.reset(m_descriptorStatuses);
    m_frog.reset(m_descriptorStatuses);

    // Ailment
    for (auto& a : m_ailments) {
        a.second.reset(m_descriptorStatuses);
    }

    // UI
    m_skillUIFire.reset(m_descriptorStatuses);
    m_skillUIStone.reset(m_descriptorStatuses);
    m_skillUITransform.reset(m_descriptorStatuses);

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
