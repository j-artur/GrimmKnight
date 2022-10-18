#include "Totem.h"
#include "Util.h"

Totem::Totem(Sprite *sprite, int iX, int iY)
{
    type = TOTEM;

    this->sprite = sprite;

    hp = 6;

    BBox(new Rect(-28.0f, -32.0f, 28.0f, 44.0f));

    MoveTo(iX * 32.0f + 28.0f, iY * 32.0f + 20.0f);
}

bool Totem::TakeDamage(uint damage, Direction dir)
{
    if (hp <= 0)
        return false;

    hurtCd.Restart();
    hp -= 1;
    return true;
}

void Totem::Update()
{
    hurtCd.Add(gameTime);
}

void Totem::Draw()
{
    if (hurtCd.Down())
    {
        float f = 100.0f - 99.0f * hurtCd.Ratio();
        sprite->Draw(x, y, LAYER_TOTEM, 1.0f, 0.0f, {f, f, f, 1.0f});
    }
    else if (Alive())
        sprite->Draw(x, y, LAYER_TOTEM);
    else
        sprite->Draw(x, y, LAYER_TOTEM, 1.0f, 0.0f, {0.5f, 0.5f, 0.5f, 1.0f});
}
