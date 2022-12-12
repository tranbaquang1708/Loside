//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "OneVariableFunction.h"
#include "InputSampler.cpp"
#include "Animation2D.h"
#include "Texture2D.h"
#include "Protagonist.h"
#include "Enemy.h"


// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory>    m_graphicsMemory;

    // TODO: Addiional class members
    // Controls
    std::unique_ptr<DirectX::GamePad>           m_gamePad;
    std::unique_ptr<DirectX::Keyboard>          m_keyboard;

    // Texture
     std::unique_ptr<DirectX::DescriptorHeap>   m_resourceDescriptors;
     std::vector<bool>                          m_descriptorStatuses;
     std::unique_ptr<DirectX::SpriteBatch>      m_spriteBatch;
    
    //    Background
    Texture2D                                   m_background;
    Animation2D                                 m_backgroundText;
    RECT                                        m_fullscreenRect;

    //    Ground
    Texture2D                                   m_ground;
    Texture2D                                   m_groundText;

    //    Protagonist
    Protagonist                                 m_protagonist;

    //    Enemy
    Enemy                                       m_enemy;
};
