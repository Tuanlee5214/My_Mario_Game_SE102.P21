﻿#include "Goomba.h"
#include "GameObject.h"
#include "RedGoomba.h"

CGoomba::CGoomba(float x, float y, float leftBound, float rightBound): CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	this->leftBound = leftBound;
	this->rightBound = rightBound;
	SetState(GOOMBA_STATE_WALKING);

}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else if (state == GOOMBA_STATE_OUT_GAME)
	{
			left = top = right = bottom = 0;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 

	CRedGoomba* redGoomba = dynamic_cast<CRedGoomba*>(e->obj);
	if (redGoomba && (e->nx != 0)) return;

	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba && (e->nx != 0)) vx = -vx;

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}
	if (x < leftBound)
	{
		x = leftBound;
		vx = GOOMBA_WALKING_SPEED;
	}
	else if (x > rightBound)
	{
		x = rightBound;
		vx = -GOOMBA_WALKING_SPEED;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	else if (state == GOOMBA_STATE_OUT_GAME)
	{
		aniId = ID_ANI_GOOMBA_OUT_GAME;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_OUT_GAME:
			vx = 0;
			ax = 0;
			vy = -GOOMBA_JUMP_DEFLECT_SPEED;
			break;
	}
}
