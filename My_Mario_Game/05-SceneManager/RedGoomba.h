#pragma once
#include "GameObject.h"

#define REDGOOMBA_GRAVITY 0.0009f
#define REDGOOMBA_WALKING_SPEED 0.033f
#define REDGOOMBA_JUMP_WALKING_SPEED 0.04f
#define REDGOOMBA_JUMPLOW_SPEED 0.1f;
#define REDGOOMBA_JUMP_SPEED 0.22f;
#define REDGOOMBA_JUMP_DEFLECT_SPEED 0.15f


#define REDGOOMBA_BBOX_WIDTH 16
#define REDGOOMBA_BBOX_HEIGHT_WALK 15
#define REDGOOMBA_BBOX_HEIGHT_DIE 8
#define REDGOOMBA_BBOX_HEIGHT_JUMP_WALK 19
#define REDGOOMBA_BBOX_HEIGHT_FLY 23

#define REDGOOMBA_DIE_TIMEOUT 500

#define REDGOOMBA_STATE_WALKING 100
#define REDGOOMBA_STATE_DIE 200
#define REDGOOMBA_STATE_JUMP_WALKING 300
#define REDGOOMBA_STATE_JUMP 400
#define REDGOOMBA_STATE_JUMPLOW 500
#define REDGOOMBA_STATE_OUT_GAME 600

#define ID_ANI_REDGOOMBA_WALKING 5010
#define ID_ANI_REDGOOMBA_DIE 5020
#define ID_ANI_REDGOOMBA_JUMP_WALKING 5030
#define ID_ANI_REDGOOMBA_JUMP 5040
#define ID_ANI_REDGOOMBA_JUMPLOW 5050
#define ID_ANI_REDGOOMBA_OUTGAME 5060


class CRedGoomba : public CGameObject
{
protected:
	float ax;
	float ay;
	float rightBound;
	float leftBound;
	bool isOnPlatform;
	int count = 0;
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
	CRedGoomba(float x, float y, float leftBound, float rightBound);
	virtual void SetState(int state);
};