#ifndef _TESTLEVEL_H_
#define _TESTLEVEL_H_

#include "Scene.h"
#include "TP2.h"
#include "FalseKnight.h"
#include "Mace.h"

class TestLevel : public Game
{
private:
	Sprite* background = nullptr;
	Sprite* foreground = nullptr;

	FalseKnight* boss;
	Mace* mace;

	static Scene* scene;

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

#endif
