#pragma once
#include "GameObject.h"

#define top_Y 123
#define speed_Y 0.018f

#define REDPLANT_BBOX_WIDTH 16
#define REDPLANT_BBOX_HEIGHT 32

#define REDPLANT_STATE_IDLE 100
#define REDPLANT_STATE_RISE_LEFT 200
#define REDPLANT_STATE_RISE_RIGHT 300
#define REDPLANT_STATE_AIM_LEFTTOP 400
#define REDPLANT_STATE_AIM_RIGHTTOP 500
#define REDPLANT_STATE_AIM_LEFTBOTTOM 600
#define REDPLANT_STATE_AIM_RIGHTBOTTOM 700
#define REDPLANT_STATE_HIDE_LEFT 800
#define REDPLANT_STATE_HIDE_RIGHT 900

#define ID_ANI_REDPLANT_LEFTTOP 5110
#define ID_ANI_REDPLANT_LEFTBOTTOM 5120
#define ID_ANI_REDPLANT_RIGHTTOP 5130
#define ID_ANI_REDPLANT_RIGHTBOTTOM 5140


class CRedPlant : public CGameObject
{
protected:

	ULONGLONG startTime = 0;
	float start_Y;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CRedPlant(float x, float y);
	virtual void SetState(int state);
};