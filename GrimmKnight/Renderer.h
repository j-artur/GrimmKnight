#ifndef _PROGJOGOS_RENDERER_H_
#define _PROGJOGOS_RENDERER_H_

#include "Geometry.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Types.h"
#include "Window.h"
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;
using std::vector;

struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT4 color;
    XMFLOAT2 tex;
};

class Renderer
{
  private:
    Window *window;
    Graphics *graphics;

    ID3D11InputLayout *inputLayout;
    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    ID3D11RasterizerState *rasterState;
    ID3D11SamplerState *sampler;
    ID3D11Buffer *vertexBuffer;
    ID3D11Buffer *indexBuffer;
    ID3D11Buffer *constantBuffer;
    uint vertexBufferPosition;

    static const uint MinBatchSize = 128;
    static const uint MaxBatchSize = 4096;
    static const uint VerticesPerSprite = 4;
    static const uint IndicesPerSprite = 6;

    ID3D11Texture2D *pixelPlotTexture;
    ID3D11ShaderResourceView *pixelPlotView;
    SpriteData pixelPlotSprite;
    long videoMemoryPitch;
    ulong *videoMemory;

    void PlotPixel(int x, int y, ulong color);
    void PlotLine(int x, int y, int flag, ulong color);
    void DrawLine(int a1, int b1, int a2, int b2, ulong color);
    int ClipLine(int &x1, int &y1, int &x2, int &y2);

    SpriteData *storage;
    uint storageIndex;
    vector<SpriteData *> spriteVector;

    void RenderBatch(ID3D11ShaderResourceView *texture, SpriteData **sprites, uint cont);

  public:
    Renderer();
    ~Renderer();

    void BeginPixels();
    void Draw(Geometry *shape, ulong color);
    void Draw(Point *point, ulong color);
    void Draw(Line *line, ulong color);
    void Draw(Rect *rect, ulong color);
    void Draw(Circle *circ, ulong color);
    void Draw(Poly *pol, ulong color);
    void Draw(Mixed *mul, ulong color);
    void EndPixels();

    bool Initialize(Window *window, Graphics *graphics);
    void Draw(SpriteData &sprite);
    void Render();
};

inline void Renderer::PlotPixel(int x, int y, ulong color)
{
    videoMemory[x + y * videoMemoryPitch] = color;
}

inline void Renderer::PlotLine(int x, int y, int flag, ulong color)
{
    flag ? PlotPixel(y, x, color) : PlotPixel(x, y, color);
}

#endif
