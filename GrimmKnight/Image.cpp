#include "Image.h"
#include "Graphics.h"

Image::Image(string filename) : textureView(nullptr), width(0), height(0)
{
    D3D11CreateTextureFromFile(Graphics::device, Graphics::context, filename.c_str(), nullptr, &textureView, width,
                               height);
}

Image::~Image()
{
    if (textureView)
    {
        ID3D11Resource *resource = nullptr;
        textureView->GetResource(&resource);

        if (resource)
        {
            resource->Release();
            resource = nullptr;
        }

        textureView->Release();
        textureView = nullptr;
    }
}
