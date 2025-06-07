#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Brick.h"
#include "PlayScene.h"
#include "debug.h"


class CUIManager : public CGameObject {
protected:
	static CUIManager* __instance;
public:
	int points;
	int timer;
	int lifes;
	int coins;
	float elapsedTime;
	static CUIManager* GetInstance();
	CUIManager(float x = 0, float y = 0) : CGameObject(x, y)
	{
		points = coins = 0;
		timer = 300;
		elapsedTime = 0;
		lifes = 4;
	}
	void Render() {}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetId() == 3)
		{
			elapsedTime += dt;

			if (elapsedTime >= 1000 && timer >= 0)
			{
				//DebugOut(L"++ Timer\n");
				timer--;
				elapsedTime -= 1000;
			}
		}
		else {
			timer = 300;
			elapsedTime = 0;
			coins = 0;
			points = 0;
			if (lifes == 0)
				lifes = 4;
		}
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	int IsBlocking() { return 0; }
};
