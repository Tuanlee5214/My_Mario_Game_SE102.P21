#pragma once
#include "GameObject.h"

#define TROOPA_GRAVITY 0.002f
#define TROOPA_WALKING_SPEED 0.03f
#define TROOPA_WALKING_DIE_SPEED 0.03f

#define TROOPA_BBOX_WIDTH 16
#define TROOPA_BBOX_HEIGHT 27
#define TROOPA_BBOX_HEIGHT_DIE 16

#define TROOPA_DIE_TIMEOUT 30000
#define TROOPA_DIE_TO_WALK_TIMEOUT 10000

#define TROOPA_STATE_WALKING 100
#define TROOPA_STATE_DIE 200
#define TROOPA_STATE_DIE_RUNL 300
#define TROOPA_STATE_DIE_RUNR 400
#define TROOPA_STATE_OUT_GAME 500

#define ID_ANI_TROOPA_WALKING_R 6011
#define ID_ANI_TROOPA_WALKING_L 6012
#define ID_ANI_TROOPA_DIE_RUN_R 6014
#define ID_ANI_TROOPA_DIE_RUN_L 6015
#define ID_ANI_TROOPA_DIE 6013
#define ID_ANI_TROOPA_OUT_GAME 6016

class CTroopa : public CGameObject {
protected:
	float ax;
	float ay;
	float rightBound;
	float leftBound;
	bool isOnPlatform;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void RenderBoundingBox();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);


	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CTroopa(float x, float y, float leftBound, float rightBound);
	virtual void SetBound(float leftBound, float rightBound);
	virtual void SetState(int state);
	virtual void SetY(float y);
	virtual void Set_ay(float ay);
	virtual ULONGLONG GetTroopaDieStart();
	virtual float GetY();
};
