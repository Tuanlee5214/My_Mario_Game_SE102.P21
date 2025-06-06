#include "RedGoomba.h"
#include "GameObject.h"
#include "debug.h"

CRedGoomba::CRedGoomba(float x, float y, float leftBound, float rightBound) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = REDGOOMBA_GRAVITY;
	die_start = -1;
	this->leftBound = leftBound;
	this->rightBound = rightBound;
	SetState(REDGOOMBA_STATE_JUMP_WALKING);

}

void CRedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	int bboxHeight = REDGOOMBA_BBOX_HEIGHT_WALK - 1;
	if (state == REDGOOMBA_STATE_DIE)
	{
		bboxHeight = REDGOOMBA_BBOX_HEIGHT_DIE - 1;
	}
	else if (state == REDGOOMBA_STATE_JUMP_WALKING)
	{
		bboxHeight = REDGOOMBA_BBOX_HEIGHT_JUMP_WALK - 1;
	}
	else if (state == REDGOOMBA_STATE_JUMP || state == REDGOOMBA_STATE_JUMPLOW)
	{
		bboxHeight = REDGOOMBA_BBOX_HEIGHT_FLY - 4;
	}
	else if(state == REDGOOMBA_STATE_OUT_GAME)
	{
		left = top = right = bottom = 0;
		return;
	}

	left = x - REDGOOMBA_BBOX_WIDTH / 2;
	top = y - bboxHeight / 2;
	right = left + REDGOOMBA_BBOX_WIDTH;
	bottom = top + bboxHeight;
}

void CRedGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
};

void CRedGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CRedGoomba*>(e->obj))
		return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
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
	switch (state)
	{
	case REDGOOMBA_STATE_JUMP_WALKING:
		if (GetTickCount64() - startTime > 1150)
		{
			SetState(REDGOOMBA_STATE_JUMPLOW);
		}
		break;
	case REDGOOMBA_STATE_JUMPLOW:
		if (isOnPlatform && count < 3)
		{
			SetState(REDGOOMBA_STATE_JUMPLOW);
		}
		else if (isOnPlatform && count >= 3)
			SetState(REDGOOMBA_STATE_JUMP);
		break;
	case REDGOOMBA_STATE_JUMP:
		if (isOnPlatform)
		{
			SetState(REDGOOMBA_STATE_JUMP_WALKING);
		}
		break;
	}
	
	if (x < leftBound)
	{
		x = leftBound;
		if (state == REDGOOMBA_STATE_JUMP_WALKING)
		{
			vx = REDGOOMBA_JUMP_WALKING_SPEED;
		}
		else if (state == REDGOOMBA_STATE_JUMPLOW)
		{
			vx = REDGOOMBA_JUMP_WALKING_SPEED;
			//vy = -REDGOOMBA_JUMPLOW_SPEED;
		}
		else if (state == REDGOOMBA_STATE_JUMP)
		{
			vx = REDGOOMBA_JUMP_WALKING_SPEED;
			//vy = -REDGOOMBA_JUMP_SPEED;
		}
		else if (state == REDGOOMBA_STATE_WALKING)
		{
			vx = REDGOOMBA_WALKING_SPEED;
		}
	}
	else if (x > rightBound)
	{
		x = rightBound;
		if (state == REDGOOMBA_STATE_JUMP_WALKING)
		{
			vx = -REDGOOMBA_JUMP_WALKING_SPEED;
		}
		else if (state == REDGOOMBA_STATE_JUMPLOW)
		{
			vx = -REDGOOMBA_JUMP_WALKING_SPEED;
			//vy = -REDGOOMBA_JUMPLOW_SPEED;
		}
		else if (state == REDGOOMBA_STATE_JUMP)
		{
			vx = -REDGOOMBA_JUMP_WALKING_SPEED;
			//vy = -REDGOOMBA_JUMP_SPEED;
		}
		else if (state == REDGOOMBA_STATE_WALKING)
		{
			vx = -REDGOOMBA_WALKING_SPEED;
		}
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
		DebugOut(L"Set state DIE");
	}
	else if (state == REDGOOMBA_STATE_JUMP_WALKING)
	{
		aniId = ID_ANI_REDGOOMBA_JUMP_WALKING;
		DebugOut(L"Set state JUMP WALKING");
	}
	else if (state == REDGOOMBA_STATE_JUMPLOW)
	{
		aniId = ID_ANI_REDGOOMBA_JUMPLOW;
	}
	else if (state == REDGOOMBA_STATE_JUMP)
	{
		aniId = ID_ANI_REDGOOMBA_JUMP;
	}
	else if (state == REDGOOMBA_STATE_WALKING)
	{
		aniId = ID_ANI_REDGOOMBA_WALKING;
		DebugOut(L"Set state WALKING");
	}
	else if (state == REDGOOMBA_STATE_OUT_GAME)
	{
		aniId = ID_ANI_REDGOOMBA_OUTGAME;
	}
	

	auto ani = CAnimations::GetInstance()->Get(aniId);
	if (ani)
	{
		ani->Render(x, y);
	}
	//RenderBoundingBox();
}

void CRedGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	startTime = GetTickCount64();
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
	case REDGOOMBA_STATE_JUMP_WALKING:
		if (vx > 0)
		{
			vx = REDGOOMBA_JUMP_WALKING_SPEED;
		}
		else
		{
			vx = -REDGOOMBA_JUMP_WALKING_SPEED;
		}
		break;
	case REDGOOMBA_STATE_JUMPLOW:
		this->count++;
		if (vx > 0)
		{
			vx = REDGOOMBA_JUMP_WALKING_SPEED;
		}
		else
		{
			vx = -REDGOOMBA_JUMP_WALKING_SPEED;
		}
		vy = -REDGOOMBA_JUMPLOW_SPEED;
		break;
	case REDGOOMBA_STATE_JUMP:
		this->count = 0;
		if (vx > 0)
		{
			vx = REDGOOMBA_JUMP_WALKING_SPEED;
		}
		else
		{
			vx = -REDGOOMBA_JUMP_WALKING_SPEED;
		}
		vy = -REDGOOMBA_JUMP_SPEED;
		break;
	case REDGOOMBA_STATE_OUT_GAME:
		vx = 0;
		ax = 0;
		vy = -REDGOOMBA_JUMP_DEFLECT_SPEED;
		break;
	}
	
	
}
