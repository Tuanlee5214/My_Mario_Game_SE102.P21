#include "Goomba.h"
#include "Troopa.h"
#include "GameObject.h"
#include "Mario.h"

CTroopa::CTroopa(float x, float y, float leftBound, float rightBound) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	die_start = -1;
	this->leftBound = leftBound;
	this->rightBound = rightBound;
	SetState(TROOPA_STATE_WALKING);
}

void CTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == TROOPA_STATE_DIE)
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

void CTroopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj) && e->ny < 0)
	{
		SetState(TROOPA_STATE_DIE);
		return;
	}
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CTroopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
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
	else {
		aniId = (vx > 0) ? ID_ANI_TROOPA_WALKING_R : ID_ANI_TROOPA_WALKING_L;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
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
		break;
	}
}
