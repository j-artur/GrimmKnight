#include "Sword.h"
#include "TP2.h"

Sword::Sword(Vector direction, Sprite* sprite)
{
	type = ENEMY_ATTACK;

	this->direction = direction;
	this->sprite = sprite;

	RotateTo(direction.Angle());

	bb = new Mixed();

	for (int i = 0; i < 12; i++)
	{
		circles[i] = new Circle(10.0f);
		bb->Insert(circles[i]);
	}


	BBox(bb);
}

Sword::~Sword()
{
}

void Sword::Update()
{

	if (swordSpawnCd.Up() && !spawned)
	{
		spawned = true;
		for (int i = 0; i < 12; i++)
		{
			circles[i]->MoveTo(x + direction.XComponent() * i, y - direction.YComponent() * i);
		}
	}

	if (spawned)
	{
		this->direction.ScaleTo(500.0f);
		Translate(direction.XComponent() * gameTime, -direction.YComponent() * gameTime);
	}


	if (swordDeleteCd.Up())
	{
		TP2::scene->Delete();
	}


	swordDeleteCd.Add(gameTime);
	swordSpawnCd.Add(gameTime);
}

void Sword::Draw()
{
	sprite->Draw(round(x), round(y), LAYER_BOSS_ATTACK, scale, -rotation);
}