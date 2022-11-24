#include "TileSet.h"
#include "Image.h"

TileSet::TileSet(string filename, uint tileWidth, uint tileHeight, uint numCols, uint numTiles)
    : image(new Image(filename)), width(tileWidth), height(tileHeight), columns(numCols), size(numTiles)
{
}

TileSet::TileSet(string filename, uint numLines, uint numCols)
    : image(new Image(filename)), columns(numCols), size(numLines * numCols)
{
    width = image->Width() / numCols;
    height = image->Height() / numLines;
}

TileSet::~TileSet()
{
    if (image)
    {
        delete image;
    }
}
