#pragma once
#include "GameObject.h"

#define PARATROOPA_GRAVITY 0.00045f
#define PARATROOPA_WALKING_SPEED 0.042f
#define PARATROOPA_JUMP_SPEED 0.17f

#define PARATROOPA_BBOX_WIDTH 16
#define PARATROOPA_BBOX_HEIGHT 27
#define PARATROOPA_BBOX_HEIGHT_DIE 16

#define PARATROOPA_DIE_TIMEOUT 30000

#define PARATROOPA_STATE_WALKING 100
#define PARATROOPA_STATE_DIE 200
#define PARATROOPA_STATE_DIE_RUNL 300
#define PARATROOPA_STATE_DIE_RUNR 400
#define PARATROOPA_STATE_WALKING_FLY 500
#define PARATROOPA_STATE_OUT_GAME 600


#define ID_ANI_PARATROOPA_WALKING_FLY_R 6111
#define ID_ANI_PARATROOPA_WALKING_FLY_L 6112

class CParaTroopa : public CGameObject {
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
	CParaTroopa(float x, float y, float leftBound, float rightBound);
	virtual void SetState(int state);
};
