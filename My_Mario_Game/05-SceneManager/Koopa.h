#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.035f
#define KOOPA_WALKING_DIE_SPEED 0.03f

#define KOOPA_DISTANCE_DELETE 280


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 23
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_DIE_TIMEOUT 7000

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 200
#define KOOPA_STATE_DIE_RUNL 300
#define KOOPA_STATE_DIE_RUNR 400


#define ID_ANI_KOOPA_WALKING_R 6000
#define ID_ANI_KOOPA_WALKING_L 6001
#define ID_ANI_KOOPA_DIE_RUN_R	6003 
#define ID_ANI_KOOPA_DIE_RUN_L 6004
#define ID_ANI_KOOPA_DIE 6002

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
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
	CKoopa(float x, float y, float leftBound, float rightBound);
	virtual void SetBound(float leftBound, float rightBound);
	virtual void SetState(int state);
	virtual void Set_Y(float y);
};