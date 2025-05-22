#include "Goomba.h"
#include "Troopa.h"
#include "GameObject.h"
#include "Mario.h"
#include "Game.h"
#include "Textures.h"

CTroopa::CTroopa(float x, float y, float leftBound, float rightBound) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = TROOPA_GRAVITY;
	die_start = -1;
	isOnPlatform = true;
	this->leftBound = leftBound;
	this->rightBound = rightBound;
	SetState(TROOPA_STATE_WALKING);
}

void CTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == TROOPA_STATE_DIE || state == TROOPA_STATE_DIE_RUNR || 
		state == TROOPA_STATE_DIE_RUNL)
	{
		left = x - TROOPA_BBOX_WIDTH / 2;
		top = y - TROOPA_BBOX_HEIGHT_DIE / 2;
		right = left + TROOPA_BBOX_WIDTH;
		bottom = top + TROOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - TROOPA_BBOX_WIDTH / 2;
		top = y - TROOPA_BBOX_HEIGHT / 2;
		right = left + TROOPA_BBOX_WIDTH;
		bottom = top + TROOPA_BBOX_HEIGHT;
	}
}

void CTroopa::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - (r - l) / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CTroopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CTroopa*>(e->obj)) vx = -vx;

	if (e->ny != 0)
	{
		vy = 0;
	}
	//else if (e->ny == 0 && nx == 0) vy = TROOPA_GRAVITY * 50;
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == TROOPA_STATE_DIE) && (GetTickCount64() - die_start > TROOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if ((state == TROOPA_STATE_DIE_RUNL || state == TROOPA_STATE_DIE_RUNR) && (GetTickCount64() - die_start > TROOPA_DIE_TIMEOUT))
	{
		//isDeleted = true;
		//return;
	}

	if (x < leftBound) {
		x = leftBound;
		vx = TROOPA_WALKING_SPEED;
	}
	else if (x > rightBound) {
		x = rightBound;
		vx = -TROOPA_WALKING_SPEED;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTroopa::Render()
{
	int aniId;

	if (state == TROOPA_STATE_DIE) {
		aniId = ID_ANI_TROOPA_DIE;
	}
	else if (state == TROOPA_STATE_DIE_RUNL)
	{
		aniId = ID_ANI_TROOPA_DIE_RUN_L;
	}
	else if (state == TROOPA_STATE_DIE_RUNR)
	{
		aniId = ID_ANI_TROOPA_DIE_RUN_R;
	}
	else {
		aniId = (vx > 0) ? ID_ANI_TROOPA_WALKING_R : ID_ANI_TROOPA_WALKING_L;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CTroopa::SetY(float y)
{
	this->y = (this->y - y);
}

void CTroopa::Set_ay(float ay)
{
	this->ay = ay;
}

void CTroopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TROOPA_STATE_DIE:
		die_start = GetTickCount64();
		y += (TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = TROOPA_GRAVITY;
		ay = 0;
		break;
	case TROOPA_STATE_WALKING:
		vx = -TROOPA_WALKING_SPEED;
		ay = TROOPA_GRAVITY;
		break;	
	case TROOPA_STATE_DIE_RUNL:
		vx = -TROOPA_WALKING_DIE_SPEED * 10;
		ay = TROOPA_GRAVITY * 2;
		//y += (TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE) / 2;
		break;
	case TROOPA_STATE_DIE_RUNR:
		vx = TROOPA_WALKING_DIE_SPEED * 10;
		ay = TROOPA_GRAVITY * 2;
		//y += (TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE) / 2;
		break;
	}
}
