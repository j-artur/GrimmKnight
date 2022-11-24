#include "Graphics.h"

ID3D11Device *Graphics::device = nullptr;
ID3D11DeviceContext *Graphics::context = nullptr;
D3D11_VIEWPORT Graphics::viewport = {};

Graphics::Graphics()
{
    swapChain = nullptr;
    renderTargetView = nullptr;
    blendState = nullptr;
    featureLevel = D3D_FEATURE_LEVEL_11_0;

    bgColor[0] = 0.0f;
    bgColor[1] = 0.0f;
    bgColor[2] = 0.0f;
    bgColor[3] = 0.0f;

    vSync = false;
}

Graphics::~Graphics()
{
    if (blendState)
    {
        blendState->Release();
        blendState = nullptr;
    }

    if (renderTargetView)
    {
        renderTargetView->Release();
        renderTargetView = nullptr;
    }

    if (swapChain)
    {
        swapChain->SetFullscreenState(false, NULL);
        swapChain->Release();
        swapChain = nullptr;
    }

    if (context)
    {
        context->ClearState();
        context->Release();
        context = nullptr;
    }

    if (device)
    {
        device->Release();
        device = nullptr;
    }
}

bool Graphics::Initialize(Window *window)
{
    uint createDeviceFlags = 0;

#ifdef _DEBUG

    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    if FAILED (D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, NULL, 0, D3D11_SDK_VERSION,
                                 &device, &featureLevel, &context))
        return false;

    COLORREF color = window->Color();

    bgColor[0] = GetRValue(color) / 255.0f;
    bgColor[1] = GetGValue(color) / 255.0f;
    bgColor[2] = GetBValue(color) / 255.0f;
    bgColor[3] = 1.0f;

    IDXGIDevice *dxgiDevice = nullptr;
    if FAILED (device->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice))
        return false;

    IDXGIAdapter *dxgiAdapter = nullptr;
    if FAILED (dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgiAdapter))
        return false;

    IDXGIFactory *dxgiFactory = nullptr;
    if FAILED (dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory))
        return false;

    DXGI_SWAP_CHAIN_DESC swapDesc = {0};
    swapDesc.BufferDesc.Width = uint(window->Width());
    swapDesc.BufferDesc.Height = uint(window->Height());
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount = 2;
    swapDesc.OutputWindow = window->Id();
    swapDesc.Windowed = (window->Mode() != FULLSCREEN);
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    if FAILED (dxgiFactory->CreateSwapChain(device, &swapDesc, &swapChain))
        return false;

    if FAILED (dxgiFactory->MakeWindowAssociation(window->Id(), DXGI_MWA_NO_ALT_ENTER))
        return false;

    ID3D11Texture2D *backBuffer = nullptr;
    if FAILED (swapChain->GetBuffer(0, __uuidof(backBuffer), (void **)(&backBuffer)))
        return false;

    if FAILED (device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView))
        return false;

    context->OMSetRenderTargets(1, &renderTargetView, nullptr);

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = float(window->Width());
    viewport.Height = float(window->Height());
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

    if FAILED (device->CreateBlendState(&blendDesc, &blendState))
        return false;

    context->OMSetBlendState(blendState, nullptr, 0xffffffff);

    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();
    backBuffer->Release();

    return true;
}
