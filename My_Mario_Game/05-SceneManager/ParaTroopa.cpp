#include "Goomba.h"
#include "ParaTroopa.h"
#include "GameObject.h"
#include "Mario.h"
#include "Game.h"
#include "Textures.h"
#include "Troopa.h"

CParaTroopa::CParaTroopa(float x, float y, float leftBound, float rightBound) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = PARATROOPA_GRAVITY;
	die_start = -1;
	isOnPlatform = true;
	this->leftBound = leftBound;
	this->rightBound = rightBound;
	SetState(PARATROOPA_STATE_WALKING_FLY);
}

void CParaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PARATROOPA_STATE_DIE || state == PARATROOPA_STATE_DIE_RUNR ||
		state == PARATROOPA_STATE_DIE_RUNL)
	{
		left = x - PARATROOPA_BBOX_WIDTH / 2;
		top = y - PARATROOPA_BBOX_HEIGHT_DIE / 2;
		right = left + PARATROOPA_BBOX_WIDTH;
		bottom = top + PARATROOPA_BBOX_HEIGHT_DIE;
	}
	else if (state == PARATROOPA_STATE_OUT_GAME)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x - PARATROOPA_BBOX_WIDTH / 2;
		top = y - PARATROOPA_BBOX_HEIGHT / 2;
		right = left + PARATROOPA_BBOX_WIDTH;
		bottom = top + PARATROOPA_BBOX_HEIGHT;
	}
}

void CParaTroopa::RenderBoundingBox()
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

void CParaTroopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
};

void CParaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CParaTroopa*>(e->obj) && e->nx != 0) vx = -vx;
	CTroopa* troopa = dynamic_cast<CTroopa*>(e->obj);

	if ((troopa && (e->nx != 0 || e->ny != 0) && state == PARATROOPA_STATE_DIE_RUNL) ||
		(troopa && (e->nx != 0 || e->ny != 0) && state == PARATROOPA_STATE_DIE_RUNR))
	{
		troopa->SetState(TROOPA_STATE_OUT_GAME);
		return;
	}
	

	if ((troopa && (e->ny != 0 || e->nx != 0) && state == PARATROOPA_STATE_WALKING_FLY) ||
		(troopa && (e->ny != 0 || e->nx != 0) && state == PARATROOPA_STATE_WALKING))
	{
		if (troopa->GetState() == TROOPA_STATE_DIE)
		{
			troopa->SetY((TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE + 2) / 2);
			troopa->SetState(TROOPA_STATE_WALKING);
			troopa->SetSpeed(TROOPA_WALKING_SPEED, TROOPA_GRAVITY);
			vx = -vx;
			return;
		}
	}

	CParaTroopa* paraTroopa = dynamic_cast<CParaTroopa*>(e->obj);
	if ((paraTroopa && (e->nx != 0 || e->ny != 0) && state == PARATROOPA_STATE_DIE_RUNL) ||
		(paraTroopa && (e->nx != 0 || e->ny != 0) && state == PARATROOPA_STATE_DIE_RUNR))
	{
		paraTroopa->SetState(PARATROOPA_STATE_OUT_GAME);
		return;
	}

	if (dynamic_cast<CTroopa*>(e->obj) && (e->nx != 0 || e->ny != 0)) vx = -vx;



	if (e->ny != 0)
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CParaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == PARATROOPA_STATE_WALKING_FLY && isOnPlatform)
	{
		vy = -PARATROOPA_JUMP_SPEED; // nhảy lên lại
		isOnPlatform = false; // reset lại trạng thái
	}
	else
	{
		vy += ay * dt;
	}
	vx += ax * dt;

	if ((state == PARATROOPA_STATE_DIE) && (GetTickCount64() - die_start > PARATROOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	float y = GetY();
	if (y > 200)
	{
		isDeleted = true;
		return;
	}

	if (x < leftBound) {
		x = leftBound;
		vx = PARATROOPA_WALKING_SPEED;
	}
	else if (x > rightBound) {
		x = rightBound;
		vx = -PARATROOPA_WALKING_SPEED;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

float CParaTroopa::GetY()
{
	return this->y;
}
void CParaTroopa::Render()
{
	int aniId;

	if (state == PARATROOPA_STATE_DIE) {
		aniId = ID_ANI_TROOPA_DIE;
	}
	else if (state == PARATROOPA_STATE_DIE_RUNL)
	{
		aniId = ID_ANI_TROOPA_DIE_RUN_L;
	}
	else if (state == PARATROOPA_STATE_DIE_RUNR)
	{
		aniId = ID_ANI_TROOPA_DIE_RUN_R;
	}
	else if (state == PARATROOPA_STATE_WALKING)
	{
		aniId = (vx > 0) ? ID_ANI_TROOPA_WALKING_R : ID_ANI_TROOPA_WALKING_L;
	}
	else if (state == PARATROOPA_STATE_OUT_GAME) aniId = ID_ANI_TROOPA_OUT_GAME;
	else
	{
		aniId = (vx > 0) ? ID_ANI_PARATROOPA_WALKING_FLY_R : ID_ANI_PARATROOPA_WALKING_FLY_L;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CParaTroopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARATROOPA_STATE_DIE:
		die_start = GetTickCount64();
		y += (PARATROOPA_BBOX_HEIGHT - PARATROOPA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = PARATROOPA_GRAVITY;
		//ay = 0;
		break;
	case PARATROOPA_STATE_WALKING:
		vx = -PARATROOPA_WALKING_SPEED;
		break;
	case PARATROOPA_STATE_DIE_RUNL:
		vx = -PARATROOPA_WALKING_SPEED * 8;
		ay = PARATROOPA_GRAVITY * 2;
		break;
	case PARATROOPA_STATE_DIE_RUNR:
		vx = PARATROOPA_WALKING_SPEED * 8;
		ay = PARATROOPA_GRAVITY * 2;
		break;
	case PARATROOPA_STATE_WALKING_FLY:
		vx = -PARATROOPA_WALKING_SPEED;
		vy = -PARATROOPA_JUMP_SPEED;
		break;
	case PARATROOPA_STATE_OUT_GAME:
		vx = 0;
		ax = 0;
		vy = -MARIO_JUMP_DEFLECT_SPEED / 2.5f;
		break;
	}
}
