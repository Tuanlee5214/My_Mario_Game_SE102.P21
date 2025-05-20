#pragma once
#include "GameObject.h"

#define REDPLANT_GRAVITY 0.0009f



#define REDPLANT_BBOX_WIDTH 16
#define REDPLANT_BBOX_HEIGHT 32

#define REDPLANT_STATE_LEFTTOP 100
#define REDPLANT_STATE_LEFTBOTTOM 200
#define REDPLANT_STATE_RIGHTTOP 300
#define REDPLANT_STATE_RIGHTBOTTOM 400

#define ID_ANI_REDPLANT_LEFTTOP 5110
#define ID_ANI_REDPLANT_LEFTBOTTOM 5120
#define ID_ANI_REDPLANT_RIGHTTOP 5130
#define ID_ANI_REDPLANT_RIGHTBOTTOM 5140


class CRedPlant : public CGameObject
{
protected:
	float ax;
	float ay;
	float rightBound;
	float leftBound;
	bool isOnPlatform;
	ULONGLONG startTime = 0;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CRedPlant(float x, float y, float leftBound, float rightBound);
	virtual void SetState(int state);
};