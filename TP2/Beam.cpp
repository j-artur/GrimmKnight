#include "Beam.h"
#include "TP2.h"

Beam::Beam(Vector direction)
{
	type = ENEMY_ATTACK;

	this->direction = direction;
	bb = new Mixed();

	for (int i = 0; i < 20; i++)
	{
		circles[i] = new Circle(20.0f);

		bb->Insert(circles[i]);
	}

	BBox(bb);
}

Beam::~Beam()
{
	delete animation;
}

void Beam::Update()
{
	if (beamSpawnCd.Up() && spawnCtrl)
	{
		spawnCtrl = false;
		for (int i = 0; i < 20; i++)
		{
			circles[i]->MoveTo(x + direction.XComponent() * i, y - direction.YComponent() * i);
		}
	}

	if (beamDeleteCd.Up())
	{
		TP2::scene->Delete();
	}

	beamSpawnCd.Add(gameTime);
	beamDeleteCd.Add(gameTime);
	//animation->NextFrame();
}

void Beam::Draw()
{
	//animation->Draw(round(x), round(y), LAYER_BOSS_ATTACK, scale, rotation);
}