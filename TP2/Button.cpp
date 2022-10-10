#include "Button.h"
#include "Util.h"

Button::Button(Sprite *sprite, std::function<void(void)> action)
{
    type = BUTTON;

    this->sprite = sprite;
    this->action = action;

    Mixed *bbox = new Mixed();
    Circle *left = new Circle(32.0f);
    left->Translate(-85.0f, 0.0f);
    Circle *right = new Circle(32.0f);
    right->Translate(85.0f, 0.0f);
    bbox->Insert(new Rect(-85.0f, -32.0f, 85.0f, 32.0f));
    bbox->Insert(left);
    bbox->Insert(right);
    BBox(bbox);
}

void Button::Update()
{
    if (hover && window->KeyDown(VK_LBUTTON) && !pressed)
    {
        pressed = true;
        action();
    }
    else
    {
        if (window->KeyUp(VK_LBUTTON))
            pressed = false;

        if (hover)
        {
            hoverTime += gameTime;
            if (hoverTime >= hoverTimeMax)
                hoverTime = hoverTimeMax;
        }
        else
        {
            hoverTime -= gameTime;
            if (hoverTime <= 0.0f)
                hoverTime = 0.0f;
        }

        hover = false;
    }
}

void Button::Draw()
{
    sprite->Draw(x, y, LAYER_BUTTON, 1.0f + hoverTime);
}

void Button::OnCollision(Object *other)
{
    if (other->Type() == CURSOR)
    {
        hover = true;
    }
}
