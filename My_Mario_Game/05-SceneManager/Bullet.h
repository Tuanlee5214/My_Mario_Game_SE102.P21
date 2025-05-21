#pragma once
#include "GameObject.h"

#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8
#define BULLET_SPEEDX 0.06f
#define BULLET_SPEEDY 0.04f
#define ID_ANI_BULLET 5111
class CBullet : public CGameObject
{
protected:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public: 
	CBullet(float x, float y, float vx, float vy);
};

