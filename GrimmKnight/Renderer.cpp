#include "Renderer.h"
#include <algorithm>
#include <d3dcompiler.h>

Renderer::Renderer()
{
    window = nullptr;
    graphics = nullptr;
    inputLayout = nullptr;
    vertexShader = nullptr;
    pixelShader = nullptr;
    rasterState = nullptr;
    sampler = nullptr;
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    constantBuffer = nullptr;

    vertexBufferPosition = 0;

    pixelPlotTexture = nullptr;
    pixelPlotView = nullptr;
    ZeroMemory(&pixelPlotSprite, sizeof(pixelPlotSprite));
    videoMemoryPitch = 0;
    videoMemory = nullptr;

    storage = new SpriteData[MaxBatchSize];
    storageIndex = 0;
}

Renderer::~Renderer()
{
    delete[] storage;

    if (pixelPlotView)
    {
        pixelPlotView->Release();
        pixelPlotView = nullptr;
    }

    if (pixelPlotTexture)
    {
        pixelPlotTexture->Release();
        pixelPlotTexture = nullptr;
    }

    if (constantBuffer)
    {
        constantBuffer->Release();
        constantBuffer = nullptr;
    }

    if (indexBuffer)
    {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }

    if (vertexBuffer)
    {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }

    if (sampler)
    {
        sampler->Release();
        sampler = nullptr;
    }

    if (rasterState)
    {
        rasterState->Release();
        rasterState = nullptr;
    }

    if (pixelShader)
    {
        pixelShader->Release();
        pixelShader = nullptr;
    }

    if (vertexShader)
    {
        vertexShader->Release();
        vertexShader = nullptr;
    }

    if (inputLayout)
    {
        inputLayout->Release();
        inputLayout = nullptr;
    }
}

void Renderer::BeginPixels()
{
    D3D11_MAPPED_SUBRESOURCE mappedTex;
    graphics->context->Map(pixelPlotTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

    videoMemoryPitch = mappedTex.RowPitch >> 2;
    videoMemory = (ulong *)mappedTex.pData;

    memset(videoMemory, 0xff000000, size_t(mappedTex.RowPitch) * window->Height());
}

void Renderer::Draw(Geometry *shape, ulong color)
{
    switch (shape->Type())
    {
    case POINT_T:
        Draw((Point *)shape, color);
        break;
    case LINE_T:
        Draw((Line *)shape, color);
        break;
    case RECTANGLE_T:
        Draw((Rect *)shape, color);
        break;
    case CIRCLE_T:
        Draw((Circle *)shape, color);
        break;
    case POLYGON_T:
        Draw((Poly *)shape, color);
        break;
    case MIXED_T:
        Draw((Mixed *)shape, color);
        break;
    }
}

void Renderer::Draw(Point *point, ulong color)
{
    if (point->X() >= 0 && point->X() < window->Width())
        if (point->Y() >= 0 && point->Y() < window->Height())
            PlotPixel(int(point->X()), int(point->Y()), color);
}

void Renderer::Draw(Line *line, ulong color)
{
    int x1 = int(line->Ax());
    int y1 = int(line->Ay());
    int x2 = int(line->Bx());
    int y2 = int(line->By());

    if (ClipLine(x1, y1, x2, y2))
        DrawLine(x1, y1, x2, y2, color);
}

int Renderer::ClipLine(int &x1, int &y1, int &x2, int &y2)
{
    int min_clip_x = 0;
    int min_clip_y = 0;
    int max_clip_x = window->Width() - 1;
    int max_clip_y = window->Height() - 1;

#define CLIP_CODE_C 0x0000
#define CLIP_CODE_N 0x0008
#define CLIP_CODE_S 0x0004
#define CLIP_CODE_E 0x0002
#define CLIP_CODE_W 0x0001

#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009
#define CLIP_CODE_SW 0x0005

    int xc1 = x1, yc1 = y1, xc2 = x2, yc2 = y2;

    int p1_code = 0, p2_code = 0;

    if (y1 < min_clip_y)
        p1_code |= CLIP_CODE_N;
    else if (y1 > max_clip_y)
        p1_code |= CLIP_CODE_S;

    if (x1 < min_clip_x)
        p1_code |= CLIP_CODE_W;
    else if (x1 > max_clip_x)
        p1_code |= CLIP_CODE_E;

    if (y2 < min_clip_y)
        p2_code |= CLIP_CODE_N;
    else if (y2 > max_clip_y)
        p2_code |= CLIP_CODE_S;

    if (x2 < min_clip_x)
        p2_code |= CLIP_CODE_W;
    else if (x2 > max_clip_x)
        p2_code |= CLIP_CODE_E;

    if ((p1_code & p2_code))
        return 0;

    if (p1_code == 0 && p2_code == 0)
        return 1;

    switch (p1_code)
    {
    case CLIP_CODE_C:
        break;
    case CLIP_CODE_N: {
        yc1 = min_clip_y;
        xc1 = int(x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1));
    }
    break;
    case CLIP_CODE_S: {
        yc1 = max_clip_y;
        xc1 = int(x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1));
    }
    break;
    case CLIP_CODE_W: {
        xc1 = min_clip_x;
        yc1 = int(y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1));
    }
    break;
    case CLIP_CODE_E: {
        xc1 = max_clip_x;
        yc1 = int(y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1));
    }
    break;
    case CLIP_CODE_NE: {
        yc1 = min_clip_y;
        xc1 = int(x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1));

        if (xc1 < min_clip_x || xc1 > max_clip_x)
        {
            xc1 = max_clip_x;
            yc1 = int(y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1));
        }
    }
    break;
    case CLIP_CODE_SE: {
        yc1 = max_clip_y;
        xc1 = int(x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1));

        if (xc1 < min_clip_x || xc1 > max_clip_x)
        {
            xc1 = max_clip_x;
            yc1 = int(y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1));
        }
    }
    break;
    case CLIP_CODE_NW: {
        yc1 = min_clip_y;
        xc1 = int(x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1));

        if (xc1 < min_clip_x || xc1 > max_clip_x)
        {
            xc1 = min_clip_x;
            yc1 = int(y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1));
        }
    }
    break;
    case CLIP_CODE_SW: {
        yc1 = max_clip_y;
        xc1 = int(x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1));

        if (xc1 < min_clip_x || xc1 > max_clip_x)
        {
            xc1 = min_clip_x;
            yc1 = int(y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1));
        }
    }
    break;
    default:
        break;
    }

    switch (p2_code)
    {
    case CLIP_CODE_C:
        break;
    case CLIP_CODE_N: {
        yc2 = min_clip_y;
        xc2 = x2 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);
    }
    break;
    case CLIP_CODE_S: {
        yc2 = max_clip_y;
        xc2 = x2 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);
    }
    break;
    case CLIP_CODE_W: {
        xc2 = min_clip_x;
        yc2 = y2 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
    }
    break;
    case CLIP_CODE_E: {
        xc2 = max_clip_x;
        yc2 = y2 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
    }
    break;
    case CLIP_CODE_NE: {
        yc2 = min_clip_y;
        xc2 = int(x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2));

        if (xc2 < min_clip_x || xc2 > max_clip_x)
        {
            xc2 = max_clip_x;
            yc2 = int(y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2));
        }
    }
    break;
    case CLIP_CODE_SE: {
        yc2 = max_clip_y;
        xc2 = int(x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2));

        if (xc2 < min_clip_x || xc2 > max_clip_x)
        {
            xc2 = max_clip_x;
            yc2 = int(y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2));
        }
    }
    break;
    case CLIP_CODE_NW: {
        yc2 = min_clip_y;
        xc2 = int(x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2));
        if (xc2 < min_clip_x || xc2 > max_clip_x)
        {
            xc2 = min_clip_x;
            yc2 = int(y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2));
        }
    }
    break;
    case CLIP_CODE_SW: {
        yc2 = max_clip_y;
        xc2 = int(x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2));

        if (xc2 < min_clip_x || xc2 > max_clip_x)
        {
            xc2 = min_clip_x;
            yc2 = int(y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2));
        }
    }
    break;
    default:
        break;
    }

    if ((xc1 < min_clip_x) || (xc1 > max_clip_x) || (yc1 < min_clip_y) || (yc1 > max_clip_y) || (xc2 < min_clip_x) ||
        (xc2 > max_clip_x) || (yc2 < min_clip_y) || (yc2 > max_clip_y))
    {
        return 0;
    }

    x1 = xc1;
    y1 = yc1;
    x2 = xc2;
    y2 = yc2;

    return 1;
}

void Renderer::DrawLine(int a1, int b1, int a2, int b2, ulong color)
{
    int dx, dy, incr1, incr2, D, x, y, xend, c, pixels_left;
    int x1, y1;
    int sign_x, sign_y, step, reverse, i;

    dx = (a2 - a1) * (sign_x = ((a2 - a1) < 0 ? -1 : 1));
    dy = (b2 - b1) * (sign_y = ((b2 - b1) < 0 ? -1 : 1));

    if (sign_x == sign_y)
        step = 1;
    else
        step = -1;

    if (dy > dx)
    {
        a1 ^= b1;
        b1 ^= a1;
        a1 ^= b1;
        a2 ^= b2;
        b2 ^= a2;
        a2 ^= b2;
        dx ^= dy;
        dy ^= dx;
        dx ^= dy;
        reverse = 1;
    }
    else
        reverse = 0;

    if (a1 > a2)
    {
        x = a2;
        y = b2;
        x1 = a1;
        y1 = b1;
    }
    else
    {
        x = a1;
        y = b1;
        x1 = a2;
        y1 = b2;
    }

    xend = (dx - 1) / 4;
    pixels_left = (dx - 1) % 4;
    PlotLine(x, y, reverse, color);
    if (pixels_left < 0)
        return;

    PlotLine(x1, y1, reverse, color);
    incr2 = 4 * dy - 2 * dx;
    if (incr2 < 0)
    {
        c = 2 * dy;
        incr1 = 2 * c;
        D = incr1 - dx;

        for (i = 0; i < xend; i++)
        {
            ++x;
            --x1;
            if (D < 0)
            {
                PlotLine(x, y, reverse, color);
                PlotLine(++x, y, reverse, color);
                PlotLine(x1, y1, reverse, color);
                PlotLine(--x1, y1, reverse, color);
                D += incr1;
            }
            else
            {
                if (D < c)
                {
                    PlotLine(x, y, reverse, color);
                    PlotLine(++x, y += step, reverse, color);
                    PlotLine(x1, y1, reverse, color);
                    PlotLine(--x1, y1 -= step, reverse, color);
                }
                else
                {
                    PlotLine(x, y += step, reverse, color);
                    PlotLine(++x, y, reverse, color);
                    PlotLine(x1, y1 -= step, reverse, color);
                    PlotLine(--x1, y1, reverse, color);
                }
                D += incr2;
            }
        }
        if (pixels_left)
        {
            if (D < 0)
            {
                PlotLine(++x, y, reverse, color);
                if (pixels_left > 1)
                    PlotLine(++x, y, reverse, color);
                if (pixels_left > 2)
                    PlotLine(--x1, y1, reverse, color);
            }
            else
            {
                if (D < c)
                {
                    PlotLine(++x, y, reverse, color);
                    if (pixels_left > 1)
                        PlotLine(++x, y += step, reverse, color);
                    if (pixels_left > 2)
                        PlotLine(--x1, y1, reverse, color);
                }
                else
                {
                    PlotLine(++x, y += step, reverse, color);
                    if (pixels_left > 1)
                        PlotLine(++x, y, reverse, color);
                    if (pixels_left > 2)
                        PlotLine(--x1, y1 -= step, reverse, color);
                }
            }
        }
    }
    else
    {
        c = 2 * (dy - dx);
        incr1 = 2 * c;
        D = incr1 + dx;
        for (i = 0; i < xend; i++)
        {
            ++x;
            --x1;
            if (D > 0)
            {
                PlotLine(x, y += step, reverse, color);
                PlotLine(++x, y += step, reverse, color);
                PlotLine(x1, y1 -= step, reverse, color);
                PlotLine(--x1, y1 -= step, reverse, color);
                D += incr1;
            }
            else
            {
                if (D < c)
                {
                    PlotLine(x, y, reverse, color);
                    PlotLine(++x, y += step, reverse, color);
                    PlotLine(x1, y1, reverse, color);
                    PlotLine(--x1, y1 -= step, reverse, color);
                }
                else
                {
                    PlotLine(x, y += step, reverse, color);
                    PlotLine(++x, y, reverse, color);
                    PlotLine(x1, y1 -= step, reverse, color);
                    PlotLine(--x1, y1, reverse, color);
                }
                D += incr2;
            }
        }
        if (pixels_left)
        {
            if (D > 0)
            {
                PlotLine(++x, y += step, reverse, color);
                if (pixels_left > 1)
                    PlotLine(++x, y += step, reverse, color);
                if (pixels_left > 2)
                    PlotLine(--x1, y1 -= step, reverse, color);
            }
            else
            {
                if (D < c)
                {
                    PlotLine(++x, y, reverse, color);
                    if (pixels_left > 1)
                        PlotLine(++x, y += step, reverse, color);
                    if (pixels_left > 2)
                        PlotLine(--x1, y1, reverse, color);
                }
                else
                {
                    PlotLine(++x, y += step, reverse, color);
                    if (pixels_left > 1)
                        PlotLine(++x, y, reverse, color);
                    if (pixels_left > 2)
                    {
                        if (D > c)
                            PlotLine(--x1, y1 -= step, reverse, color);
                        else
                            PlotLine(--x1, y1, reverse, color);
                    }
                }
            }
        }
    }
}

void Renderer::Draw(Rect *rect, ulong color)
{
    Line top(rect->Left(), rect->Top(), rect->Right(), rect->Top());
    Line left(rect->Left(), rect->Top() + 1, rect->Left(), rect->Bottom());
    Line right(rect->Right(), rect->Top() + 1, rect->Right(), rect->Bottom());
    Line bottom(rect->Left() + 1, rect->Bottom(), rect->Right() - 1, rect->Bottom());

    Draw(&top, color);
    Draw(&left, color);
    Draw(&right, color);
    Draw(&bottom, color);
}

void Renderer::Draw(Circle *circ, ulong color)
{
    int xpos = int(circ->CenterX());
    int ypos = int(circ->CenterY());

    int r = int(circ->Radius());

    int p = 3 - (2 * r);
    int x = 0;
    int y = r;

    Point a(xpos, ypos + r);
    Draw(&a, color);
    Point b(xpos, ypos - r);
    Draw(&b, color);
    Point c(xpos, ypos + r);
    Draw(&c, color);
    Point d(xpos, ypos - r);
    Draw(&d, color);
    Point e(xpos + r, ypos);
    Draw(&e, color);
    Point f(xpos + r, ypos);
    Draw(&f, color);
    Point g(xpos - r, ypos);
    Draw(&g, color);
    Point h(xpos - r, ypos);
    Draw(&h, color);

    while (++x < y)
    {
        if (p < 0)
            p = p + (4 * x) + 6;
        else
        {
            p = p + 4 * (x - y) + 10;
            y = y - 1;
        }

        a.MoveTo(float(xpos + x), float(ypos + y));
        Draw(&a, color);
        b.MoveTo(float(xpos + x), float(ypos - y));
        Draw(&b, color);
        c.MoveTo(float(xpos - x), float(ypos + y));
        Draw(&c, color);
        d.MoveTo(float(xpos - x), float(ypos - y));
        Draw(&d, color);
        e.MoveTo(float(xpos + y), float(ypos + x));
        Draw(&e, color);
        f.MoveTo(float(xpos + y), float(ypos - x));
        Draw(&f, color);
        g.MoveTo(float(xpos - y), float(ypos + x));
        Draw(&g, color);
        h.MoveTo(float(xpos - y), float(ypos - x));
        Draw(&h, color);
    }
}

void Renderer::Draw(Poly *pol, ulong color)
{
    float x1, y1, x2, y2;
    uint i;

    for (i = 0; i < pol->vertexCount - 1; ++i)
    {
        x1 = pol->vertexList[i].X() * pol->Scale() + pol->X();
        y1 = pol->vertexList[i].Y() * pol->Scale() + pol->Y();
        x2 = pol->vertexList[i + 1].X() * pol->Scale() + pol->X();
        y2 = pol->vertexList[i + 1].Y() * pol->Scale() + pol->Y();

        Line line(x1, y1, x2, y2);
        Draw(&line, color);
    }

    x1 = pol->vertexList[0].X() * pol->Scale() + pol->X();
    y1 = pol->vertexList[0].Y() * pol->Scale() + pol->Y();
    x2 = pol->vertexList[i].X() * pol->Scale() + pol->X();
    y2 = pol->vertexList[i].Y() * pol->Scale() + pol->Y();

    Line line(x1, y1, x2, y2);
    Draw(&line, color);
}

void Renderer::Draw(Mixed *mul, ulong color)
{
    for (auto i : mul->shapes)
        Draw(i, color);
}

void Renderer::EndPixels()
{
    graphics->context->Unmap(pixelPlotTexture, 0);

    Draw(pixelPlotSprite);
}

bool Renderer::Initialize(Window *window, Graphics *graphics)
{
    this->window = window;
    this->graphics = graphics;

    ID3DBlob *vShader = nullptr;
    D3DReadFileToBlob(L"Shaders/Vertex.cso", &vShader);

    graphics->device->CreateVertexShader(vShader->GetBufferPointer(), vShader->GetBufferSize(), NULL, &vertexShader);

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

    graphics->device->CreateInputLayout(layoutDesc, 3, vShader->GetBufferPointer(), vShader->GetBufferSize(),
                                        &inputLayout);

    vShader->Release();

    ID3DBlob *pShader = nullptr;
    D3DReadFileToBlob(L"Shaders/Pixel.cso", &pShader);

    graphics->device->CreatePixelShader(pShader->GetBufferPointer(), pShader->GetBufferSize(), NULL, &pixelShader);

    pShader->Release();

    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(rasterDesc));
    rasterDesc.FillMode = D3D11_FILL_SOLID;

    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthClipEnable = true;

    graphics->device->CreateRasterizerState(&rasterDesc, &rasterState);

    D3D11_BUFFER_DESC vertexBufferDesc = {0};
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * MaxBatchSize * VerticesPerSprite;
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    graphics->device->CreateBuffer(&vertexBufferDesc, nullptr, &vertexBuffer);

    D3D11_BUFFER_DESC indexBufferDesc = {0};
    indexBufferDesc.ByteWidth = sizeof(short) * MaxBatchSize * IndicesPerSprite;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    vector<short> indices;
    indices.reserve(MaxBatchSize * IndicesPerSprite);
    for (short i = 0; i < MaxBatchSize * VerticesPerSprite; i += VerticesPerSprite)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(i + 1);
        indices.push_back(i + 3);
        indices.push_back(i + 2);
    }

    D3D11_SUBRESOURCE_DATA indexData = {0};
    indexData.pSysMem = &indices.front();

    if FAILED (graphics->device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer))
        return false;

    D3D11_BUFFER_DESC constBufferDesc = {0};
    constBufferDesc.ByteWidth = sizeof(XMMATRIX);
    constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    float xScale = (graphics->viewport.Width > 0) ? 2.0f / graphics->viewport.Width : 0.0f;
    float yScale = (graphics->viewport.Height > 0) ? 2.0f / graphics->viewport.Height : 0.0f;

    XMMATRIX transformMatrix(xScale, 0, 0, 0, 0, -yScale, 0, 0, 0, 0, 1, 0, -1, 1, 0, 1);

    D3D11_SUBRESOURCE_DATA constantData = {0};
    XMMATRIX worldViewProj = XMMatrixTranspose(transformMatrix);
    constantData.pSysMem = &worldViewProj;

    graphics->device->CreateBuffer(&constBufferDesc, &constantData, &constantBuffer);

    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = (graphics->device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.BorderColor[0] = 0.0f;
    samplerDesc.BorderColor[1] = 0.0f;
    samplerDesc.BorderColor[2] = 0.0f;
    samplerDesc.BorderColor[3] = 0.0f;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    graphics->device->CreateSamplerState(&samplerDesc, &sampler);

    uint vertexStride = sizeof(Vertex);
    uint vertexOffset = 0;
    graphics->context->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);
    graphics->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    graphics->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    graphics->context->IASetInputLayout(inputLayout);
    graphics->context->VSSetShader(vertexShader, NULL, 0);
    graphics->context->VSSetConstantBuffers(0, 1, &constantBuffer);
    graphics->context->PSSetShader(pixelShader, NULL, 0);
    graphics->context->PSSetSamplers(0, 1, &sampler);
    graphics->context->RSSetState(rasterState);

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Width = int(window->Width());
    desc.Height = int(window->Height());
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if FAILED (graphics->device->CreateTexture2D(&desc, nullptr, &pixelPlotTexture))
        return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC pixelPlotDesc;
    ZeroMemory(&pixelPlotDesc, sizeof(pixelPlotDesc));

    pixelPlotDesc.Format = desc.Format;
    pixelPlotDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    pixelPlotDesc.Texture2D.MipLevels = desc.MipLevels;
    pixelPlotDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;

    if FAILED (graphics->device->CreateShaderResourceView((ID3D11Resource *)pixelPlotTexture, &pixelPlotDesc,
                                                          &pixelPlotView))
        return false;

    pixelPlotSprite.x = window->CenterX();
    pixelPlotSprite.y = window->CenterY();
    pixelPlotSprite.scale = 1.0f;
    pixelPlotSprite.depth = 0.0f;
    pixelPlotSprite.rotation = 0.0f;
    pixelPlotSprite.width = window->Width();
    pixelPlotSprite.height = window->Height();
    pixelPlotSprite.texture = pixelPlotView;
    pixelPlotSprite.texCoord.x = 0.0f;
    pixelPlotSprite.texCoord.y = 0.0f;
    pixelPlotSprite.texSize.x = 1.0f;
    pixelPlotSprite.texSize.y = 1.0f;
    pixelPlotSprite.color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    return true;
}

void Renderer::RenderBatch(ID3D11ShaderResourceView *texture, SpriteData **sprites, uint count)
{
    graphics->context->PSSetShaderResources(0, 1, &texture);

    while (count > 0)
    {
        uint batchSize = count;

        uint remainingSpace = MaxBatchSize - vertexBufferPosition;

        if (batchSize > remainingSpace)
        {
            if (remainingSpace < MinBatchSize)
            {
                vertexBufferPosition = 0;
                batchSize = (count < MaxBatchSize) ? count : MaxBatchSize;
            }
            else
            {
                batchSize = remainingSpace;
            }
        }

        D3D11_MAP mapType = (vertexBufferPosition == 0) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;
        D3D11_MAPPED_SUBRESOURCE mappedBuffer;
        graphics->context->Map(vertexBuffer, 0, mapType, 0, &mappedBuffer);

        Vertex *vertices = (Vertex *)mappedBuffer.pData + size_t(vertexBufferPosition) * VerticesPerSprite;

        for (uint i = 0; i < batchSize; ++i)
        {
            XMVECTOR size = XMVectorMergeXY(XMLoadInt(&sprites[i]->width), XMLoadInt(&sprites[i]->height));
            XMVECTOR textureSize = XMConvertVectorUIntToFloat(size, 0);
            XMVECTOR inverseTextureSize = XMVectorReciprocal(textureSize);

            XMFLOAT2 positionxy(sprites[i]->x, sprites[i]->y);
            float scale = sprites[i]->scale;
            XMFLOAT2 center(sprites[i]->width * sprites[i]->texSize.x / 2.0f,
                            sprites[i]->height * sprites[i]->texSize.y / 2.0f);
            float rotation = sprites[i]->rotation;
            float layerDepth = sprites[i]->depth;

            XMVECTOR source = XMVectorSet(sprites[i]->texCoord.x, sprites[i]->texCoord.y, sprites[i]->texSize.x,
                                          sprites[i]->texSize.y);
            XMVECTOR destination = XMVectorPermute<0, 1, 4, 4>(XMLoadFloat2(&positionxy), XMLoadFloat(&scale));
            XMVECTOR color =
                XMVectorSet(sprites[i]->color.r, sprites[i]->color.g, sprites[i]->color.b, sprites[i]->color.a);
            XMVECTOR originRotationDepth = XMVectorSet(center.x, center.y, rotation, layerDepth);

            XMVECTOR sourceSize = XMVectorSwizzle<2, 3, 2, 3>(source);
            XMVECTOR destinationSize = XMVectorSwizzle<2, 3, 2, 3>(destination);

            XMVECTOR isZeroMask = XMVectorEqual(sourceSize, XMVectorZero());
            XMVECTOR nonZeroSourceSize = XMVectorSelect(sourceSize, g_XMEpsilon, isZeroMask);

            XMVECTOR origin = XMVectorDivide(originRotationDepth, nonZeroSourceSize);

            origin *= inverseTextureSize;

            destinationSize *= textureSize;

            XMVECTOR rotationMatrix1;
            XMVECTOR rotationMatrix2;

            if (rotation != 0)
            {
                float sin, cos;

                XMScalarSinCos(&sin, &cos, rotation);

                XMVECTOR sinV = XMLoadFloat(&sin);
                XMVECTOR cosV = XMLoadFloat(&cos);

                rotationMatrix1 = XMVectorMergeXY(cosV, sinV);
                rotationMatrix2 = XMVectorMergeXY(-sinV, cosV);
            }
            else
            {
                rotationMatrix1 = g_XMIdentityR0;
                rotationMatrix2 = g_XMIdentityR1;
            }

            static XMVECTORF32 cornerOffsets[VerticesPerSprite] = {
                {0, 0},
                {1, 0},
                {0, 1},
                {1, 1},
            };

            int mirrorBits = 0;

            for (int i = 0; i < VerticesPerSprite; ++i)
            {
                XMVECTOR cornerOffset = (cornerOffsets[i] - origin) * destinationSize;

                XMVECTOR position1 = XMVectorMultiplyAdd(XMVectorSplatX(cornerOffset), rotationMatrix1, destination);
                XMVECTOR position2 = XMVectorMultiplyAdd(XMVectorSplatY(cornerOffset), rotationMatrix2, position1);

                XMVECTOR position = XMVectorPermute<0, 1, 7, 6>(position2, originRotationDepth);

                XMStoreFloat4(reinterpret_cast<XMFLOAT4 *>(&vertices[i].pos), position);

                XMStoreFloat4(&vertices[i].color, color);

                XMVECTOR textureCoordinate = XMVectorMultiplyAdd(cornerOffsets[i ^ mirrorBits], sourceSize, source);

                XMStoreFloat2(&vertices[i].tex, textureCoordinate);
            }

            vertices += VerticesPerSprite;
        }

        graphics->context->Unmap(vertexBuffer, 0);

        uint startIndex = (uint)vertexBufferPosition * IndicesPerSprite;
        uint indexCount = (uint)batchSize * IndicesPerSprite;
        graphics->context->DrawIndexed(indexCount, startIndex, 0);

        vertexBufferPosition += batchSize;

        sprites += batchSize;

        count -= batchSize;
    }
}

void Renderer::Render()
{
    sort(spriteVector.begin(), spriteVector.end(),
         [](SpriteData *a, SpriteData *b) -> bool { return a->depth > b->depth; });

    uint spriteVectorSize = uint(spriteVector.size());

    if (spriteVectorSize == 0)
        return;

    ID3D11ShaderResourceView *batchTexture = nullptr;
    uint batchStart = 0;

    for (uint pos = 0; pos < spriteVectorSize; ++pos)
    {
        ID3D11ShaderResourceView *texture = spriteVector[pos]->texture;

        if (texture != batchTexture)
        {
            if (pos > batchStart)
            {
                RenderBatch(batchTexture, &spriteVector[batchStart], pos - batchStart);
            }

            batchTexture = texture;
            batchStart = pos;
        }
    }

    RenderBatch(batchTexture, &spriteVector[batchStart], spriteVectorSize - batchStart);

    spriteVector.clear();
    storageIndex = 0;
}

void Renderer::Draw(SpriteData &sprite)
{
    if (storageIndex < MaxBatchSize)
    {
        storage[storageIndex] = sprite;
        spriteVector.push_back(&storage[storageIndex]);
        ++storageIndex;
    }
}
