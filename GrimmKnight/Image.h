#ifndef _PROGJOGOS_IMAGE_H_
#define _PROGJOGOS_IMAGE_H_

#include "Texture.h"
#include "Types.h"
#include <string>
using std::string;

class Image
{
  private:
    ID3D11ShaderResourceView *textureView;
    uint width;
    uint height;

  public:
    Image(string filename);
    ~Image();

    uint Width() const;
    uint Height() const;
    ID3D11ShaderResourceView *View() const;
};

inline uint Image::Width() const
{
    return width;
}

inline uint Image::Height() const
{
    return height;
}

inline ID3D11ShaderResourceView *Image::View() const
{
    return textureView;
}

#endif
