#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.025f
#define KOOPA_WALKING_DIE_SPEED 0.027f

#define KOOPA_DISTANCE_DELETE 280


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 23
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_DIE_TIMEOUT 30000
#define KOOPA_TIME_TO_PICKUP 500
#define KOOPA_DIE_TO_WALK_TIMEOUT 10000

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 200
#define KOOPA_STATE_DIE_RUNL 300
#define KOOPA_STATE_DIE_RUNR 400
#define KOOPA_STATE_OUT_GAME 500


#define ID_ANI_KOOPA_WALKING_R 6000
#define ID_ANI_KOOPA_WALKING_L 6001
#define ID_ANI_KOOPA_DIE_RUN_R	6003 
#define ID_ANI_KOOPA_DIE_RUN_L 6004
#define ID_ANI_KOOPA_DIE 6002
#define ID_ANI_KOOPA_OUT_GAME 6005

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	float rightBound;
	float leftBound;

	bool isKoopaInDieRunState = false;


	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y, float leftBound, float rightBound);
	virtual void SetBound(float leftBound, float rightBound);
	virtual void SetState(int state);
	virtual void Set_Y(float y);
	virtual void Set_ax(float ax);
	virtual void Set_vx(float vx);
	virtual void Set_vy(float vy);
	virtual void Set_vx1();
	virtual void Set_ay(float ay);
	virtual ULONGLONG GetKoopaDieStart();
	virtual bool GetIsKoopaInDieRunState();
};