#pragma once
#include "GameObject.h"

#define REDGOOMBA_GRAVITY 0.002f
#define REDGOOMBA_WALKING_SPEED 0.05f


#define REDGOOMBA_BBOX_WIDTH 16
#define REDGOOMBA_BBOX_HEIGHT_WALK 15
#define REDGOOMBA_BBOX_HEIGHT_DIE 8
#define REDGOOMBA_BBOX_HEIGHT_JUMP_WALK 23

#define REDGOOMBA_DIE_TIMEOUT 500

#define REDGOOMBA_STATE_WALKING 100
#define REDGOOMBA_STATE_DIE 200
#define REDGOOMBA_STATE_JUMP_WALKING 300
#define REDGOOMBA_STATE_JUMP 400

#define ID_ANI_REDGOOMBA_WALKING 5010
#define ID_ANI_REDGOOMBA_DIE 5020
#define ID_ANI_REDGOOMBA_JUMP_WALKING 5030
#define ID_ANI_REDGOOMBA_JUMP 5040


class CRedGoomba : public CGameObject
{
protected:
	float ax;
	float ay;
	bool isActive;
	float activationDistance;
	float rightBound;
	float leftBound;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CRedGoomba(float x, float y, float leftBound, float rightBound);
	virtual void SetState(int state);
};