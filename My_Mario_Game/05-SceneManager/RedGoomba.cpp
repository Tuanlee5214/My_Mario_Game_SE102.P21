#include "RedGoomba.h"
#include "GameObject.h"

CRedGoomba::CRedGoomba(float x, float y, float leftBound, float rightBound) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = REDGOOMBA_GRAVITY;
	die_start = -1;
	this->leftBound = leftBound;
	this->rightBound = rightBound;
	SetState(REDGOOMBA_STATE_WALKING);

}

void CRedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == REDGOOMBA_STATE_DIE)
	{
		left = x - REDGOOMBA_BBOX_WIDTH / 2;
		top = y - REDGOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + REDGOOMBA_BBOX_WIDTH;
		bottom = top + REDGOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - REDGOOMBA_BBOX_WIDTH / 2;
		top = y - REDGOOMBA_BBOX_HEIGHT_WALK / 2;
		right = left + REDGOOMBA_BBOX_WIDTH;
		bottom = top + REDGOOMBA_BBOX_HEIGHT_WALK;
	}
}

void CRedGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRedGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CRedGoomba*>(e->obj))
		return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CRedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == REDGOOMBA_STATE_DIE) && (GetTickCount64() - die_start > REDGOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if (x < leftBound)
	{
		x = leftBound;
		vx = REDGOOMBA_WALKING_SPEED;
	}
	else if (x > rightBound)
	{
		x = rightBound;
		vx = -REDGOOMBA_WALKING_SPEED;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CRedGoomba::Render()
{
	int aniId = ID_ANI_REDGOOMBA_WALKING;
	if (state == REDGOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_REDGOOMBA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CRedGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case REDGOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (REDGOOMBA_BBOX_HEIGHT_WALK - REDGOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case REDGOOMBA_STATE_WALKING:
		vx = -REDGOOMBA_WALKING_SPEED;
		break;
	}
}
