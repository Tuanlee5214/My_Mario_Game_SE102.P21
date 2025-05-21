#pragma once
#include "GameObject.h"
#include "PlayScene.h"

#define top_Y 127
#define speed_Y 0.018f

#define GREENPLANT_BBOX_WIDTH 16
#define GREENPLANT_BBOX_HEIGHT 25

#define GREENPLANT_STATE_IDLE 100
#define GREENPLANT_STATE_RISE_LEFT 200
#define GREENPLANT_STATE_RISE_RIGHT 300
#define GREENPLANT_STATE_AIM_LEFTTOP 400
#define GREENPLANT_STATE_AIM_RIGHTTOP 500
#define GREENPLANT_STATE_AIM_LEFTBOTTOM 600
#define GREENPLANT_STATE_AIM_RIGHTBOTTOM 700
#define GREENPLANT_STATE_HIDE_LEFT 800
#define GREENPLANT_STATE_HIDE_RIGHT 900

#define ID_ANI_GREENPLANT_LEFTTOP 5210
#define ID_ANI_GREENPLANT_LEFTBOTTOM 5220
#define ID_ANI_GREENPLANT_RIGHTTOP 5230
#define ID_ANI_GREENPLANT_RIGHTBOTTOM 5240


class CGreenPlant : public CGameObject
{
protected:

	ULONGLONG startTime = 0;
	float start_Y;
	bool hasShoot;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void ShootBullet(CPlayScene* playScene);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CGreenPlant(float x, float y);
	virtual void SetState(int state);
};
