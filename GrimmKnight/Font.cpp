#include "Font.h"
#include "Engine.h"
#include <cmath>
#include <fstream>
using namespace std;

Font::Font(const string &filename)
{
    fontTiles = new TileSet(filename, 16, 16);
    ZeroMemory(spacing, sizeof(spacing));
    ZeroMemory(&sprite, sizeof(sprite));

    proportional = false;
    charWidth = fontTiles->TileWidth();
}

Font::~Font()
{
    delete fontTiles;
}

void Font::Spacing(int width)
{
    proportional = false;
    charWidth = width;
}

void Font::Spacing(const string &filename)
{
    ifstream fin;
    fin.open(filename.c_str(), ios_base::in | ios_base::binary);
    fin.read((char *)spacing, 512);
    fin.close();

    proportional = true;
}

void Font::Draw(float x, float y, string text, Color color, float depth, float scale, float rotation)
{
    float posX = x;
    float posY = y;

    rotation = rotation * XM_PI / 180.0f;

    int textLength = int(text.size());

    for (int i = 0; i < textLength; ++i)
    {
        int frame = int(text[i]);

        if (frame < 0)
            frame += 256;

        if (proportional)
            charWidth = spacing[frame];

        sprite.x = posX;
        sprite.y = posY;
        sprite.depth = depth;
        sprite.scale = scale;
        sprite.rotation = rotation;
        sprite.width = fontTiles->TileWidth();
        sprite.height = fontTiles->TileHeight();
        sprite.texSize.x = float(fontTiles->TileWidth()) / fontTiles->Width();
        sprite.texSize.y = float(fontTiles->TileHeight()) / fontTiles->Height();
        sprite.texture = fontTiles->View();
        sprite.texCoord.x = (frame % fontTiles->Columns()) * sprite.texSize.x;
        sprite.texCoord.y = (frame / fontTiles->Columns()) * sprite.texSize.y;
        sprite.color = color;

        Engine::renderer->Draw(sprite);

        posX += charWidth * scale * cos(rotation);
        posY += charWidth * scale * sin(rotation);
    }
}
