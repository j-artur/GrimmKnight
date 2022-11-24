#ifndef _PROGJOGOS_GRAPHICS_H_
#define _PROGJOGOS_GRAPHICS_H_

#include "Types.h"
#include "Window.h"
#include <d3d11.h>
#include <dxgi.h>

class Graphics
{
  private:
    IDXGISwapChain *swapChain;
    ID3D11RenderTargetView *renderTargetView;
    ID3D11BlendState *blendState;
    D3D_FEATURE_LEVEL featureLevel;
    float bgColor[4];
    bool vSync;

  public:
    Graphics();
    ~Graphics();

    static ID3D11Device *device;
    static ID3D11DeviceContext *context;
    static D3D11_VIEWPORT viewport;

    void VSync(bool state);
    void Clear();
    void Present();
    bool Initialize(Window *window);
};

inline void Graphics::VSync(bool state)
{
    vSync = state;
}

inline void Graphics::Clear()
{
    context->ClearRenderTargetView(renderTargetView, bgColor);
}

inline void Graphics::Present()
{
    swapChain->Present(vSync, NULL);
    context->OMSetRenderTargets(1, &renderTargetView, nullptr);
}

#endif
