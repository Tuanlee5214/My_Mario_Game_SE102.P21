#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"
#include "NumberText.h"

class CPanelUI : public CGameObject {
protected:
	int width;
	int height;
	int spriteId;
	float posX;
	float posY;
	vector<LPGAMEOBJECT> timer;
	vector<LPGAMEOBJECT> coins;
	vector<LPGAMEOBJECT> points;
	vector<LPGAMEOBJECT> lifes;
	vector<LPGAMEOBJECT> energy;
	bool isInitUI;
public:
	CPanelUI(float x, float y, int widthh, int heightt, int sipriteIdd) : CGameObject(x, y)
	{
		this->width = widthh;
		this->height = heightt;
		this->spriteId = sipriteIdd;
		posX = posY = 0;
		isInitUI = false;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsCollidable() { return 0; }
	int IsBlocking() { return 0; }
	void InitUI();
	void SetPosition(float x, float y);
	void UpdateElements(vector<LPGAMEOBJECT>& elements, DWORD value);
};