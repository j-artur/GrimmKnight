#include "Animation.h"
#include "Engine.h"
#include "Renderer.h"

Animation::Animation(TileSet *tiles, float delay, bool repeat) : tileSet(tiles), animDelay(delay), animLoop(repeat)
{
    frame = iniFrame = 0;

    endFrame = tileSet->Size() - 1;

    sprite.width = tileSet->TileWidth();
    sprite.height = tileSet->TileHeight();
    sprite.texSize.x = float(tileSet->TileWidth()) / tileSet->Width();
    sprite.texSize.y = float(tileSet->TileHeight()) / tileSet->Height();
    sprite.texture = tileSet->View();

    timer.Start();

    sequence = nullptr;
}

Animation::~Animation()
{
    if (!table.empty())
    {
        for (const auto &[id, seq] : table)
            delete seq.first;
    }
}

void Animation::Add(uint id, uint *seq, uint seqSize)
{
    AnimSeq newSeq(new uint[seqSize], seqSize);

    memcpy(newSeq.first, seq, sizeof(uint) * seqSize);

    table[id] = newSeq;

    sequence = newSeq.first;

    iniFrame = frame = 0;

    endFrame = seqSize - 1;
}

void Animation::Select(uint id)
{
    const auto &[seq, size] = table[id];

    if (sequence != seq)
    {
        sequence = seq;

        iniFrame = 0;
        endFrame = size - 1;

        if (frame > endFrame)
            frame = 0;
    }
}

void Animation::NextFrame()
{
    if (timer.Elapsed(animDelay))
    {
        frame++;

        if (frame > endFrame)
        {
            if (animLoop)
            {
                frame = 0;
                timer.Start();
            }
            else
            {
                frame = endFrame;
            }
        }
        else
        {
            timer.Start();
        }
    }
}

void Animation::Draw(uint aFrame, float x, float y, float z, float scale, float rotation, Color color)
{
    sprite.x = x;
    sprite.y = y;
    sprite.depth = z;
    sprite.scale = scale;
    sprite.rotation = rotation * XM_PI / 180.0f;
    sprite.color = color;

    sprite.texCoord.x = (aFrame % tileSet->Columns()) * sprite.texSize.x;
    sprite.texCoord.y = (aFrame / tileSet->Columns()) * sprite.texSize.y;

    Engine::renderer->Draw(sprite);
}
