#include "Orb.h"
#include "TP2.h"

Orb::Orb(Sprite* sprite)
{
	type = ENEMY_ATTACK;

	this->sprite = sprite;

	maxSpeed = 640.0f;

	BBox(new Circle(16.0f));
}

Orb::~Orb()
{
}

void Orb::OnCollision(Object* other)
{
	if (other->Type() == WALL_TOP)
		TP2::scene->Delete(this, MOVING);
	
}

void Orb::Update()
{
	Rotate(360.0f * gameTime);

	acceleration = Vector(atan2((y - TP2::player->Y()), x - TP2::player->X()) * 180 / M_PI, 640.0f * gameTime);
	speed.Add(acceleration);
	
	if (orbSpawnCd.Up() && !spawned)
	{
		spawned = true;
		speed = Vector(atan2(y - TP2::player->Y(), x - TP2::player->X()) * 180 / M_PI, 320.0f * gameTime);
	}

	if (orbDeleteCd.Up())
		TP2::scene->Delete();

	if (speed.Magnitude() > maxSpeed)
		speed.ScaleTo(maxSpeed);

	if (spawned)
		Translate(-speed.XComponent() * gameTime, -speed.YComponent() * gameTime);

	orbSpawnCd.Add(gameTime);
	orbDeleteCd.Add(gameTime);
}

void Orb::Draw()
{
	sprite->Draw(round(x), round(y), LAYER_BOSS_ATTACK, scale, -rotation);
}