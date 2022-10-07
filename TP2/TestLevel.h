#ifndef _TESTLEVEL_H_
#define _TESTLEVEL_H_

#include "Scene.h"
#include "TP2.h"
#include "FalseKnight.h"

class TestLevel : public Game
{
private:
	Sprite* background = nullptr;
	Sprite* foreground = nullptr;

	FalseKnight* boss;

	static Scene* scene;

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();
};

#endif
