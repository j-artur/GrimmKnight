#ifndef _PROGJOGOS_FONT_H_
#define _PROGJOGOS_FONT_H_

#include "Sprite.h"
#include "TileSet.h"
#include "Types.h"
#include <string>
using std::string;

class Font
{
  private:
    TileSet *fontTiles;
    SpriteData sprite;
    short spacing[256];
    bool proportional;
    int charWidth;

  public:
    Font(const string &filename);
    ~Font();

    void Spacing(int width);
    void Spacing(const string &filename);

    void Draw(float x, float y, string text, Color color = {1, 1, 1, 1}, float depth = Layer::FRONT, float scale = 1.0f,
              float rotation = 0.0f);
};

#endif
