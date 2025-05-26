#include "Goomba.h"
#include "Koopa.h"
#include "GameObject.h"
#include "Mario.h"
#include "Goomba.h"
#include "PlayScene.h"
#include "Bullet.h"

CKoopa::CKoopa(float x, float y, float leftBound, float rightBound) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	this->leftBound = leftBound;
	this->rightBound = rightBound;
	SetState(KOOPA_STATE_WALKING);

}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_DIE || 
		state == KOOPA_STATE_DIE_RUNL || 
		state == KOOPA_STATE_DIE_RUNR)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::Set_Y(float y)
{
	this->y = this->y - y;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba && (e->nx != 0 || e->ny != 0) && state == KOOPA_STATE_DIE_RUNL ||
		goomba && (e->nx != 0 || e->ny != 0) && state == KOOPA_STATE_DIE_RUNR)
	{
		goomba->SetState(GOOMBA_STATE_OUT_GAME);
		return;
	}
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	if ((mario && e->nx != 0 && state == KOOPA_STATE_DIE_RUNL) ||
		(mario && e->nx != 0 && state == KOOPA_STATE_DIE_RUNR))
	{
		return;
	}
	CBullet* bullet = dynamic_cast<CBullet*>(e->obj);
	if ((bullet && e->nx != 0 && state == KOOPA_STATE_DIE_RUNL) ||
		(bullet && e->nx != 0 && state == KOOPA_STATE_DIE_RUNR))
	{
		return;
	}
	if (e->obj->IsBlocking())
	{
		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (!playScene) return;

	CMario* mario = dynamic_cast<CMario*>(playScene->GetPlayer());
	if (!mario) return;

	float marioX, marioY;
	mario->GetPosition(marioX, marioY);
	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if ((state == KOOPA_STATE_DIE_RUNL || state == KOOPA_STATE_DIE_RUNR) &&
		abs(this->x - marioX) > KOOPA_DISTANCE_DELETE)
	{
		isDeleted = true;
		return;
	}

	if (state == KOOPA_STATE_DIE && (GetTickCount64() - die_start > KOOPA_DIE_TO_WALK_TIMEOUT))
	{
		this->SetState(KOOPA_STATE_WALKING);
		this->SetBound(510, 600);
		this->Set_Y((KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE + 2) / 2);
	}

	if (x < leftBound) {
		x = leftBound;
		vx = KOOPA_WALKING_SPEED;
	}
	else if (x > rightBound) {
		x = rightBound;
		vx = -KOOPA_WALKING_SPEED;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Set_ax(float ax)
{
	this->ax = ax;
}

void CKoopa::Render()
{
	int aniId;

	if (state == KOOPA_STATE_DIE) {
		aniId = ID_ANI_KOOPA_DIE;
	}
	else if (state == KOOPA_STATE_DIE_RUNL)
	{
		aniId = ID_ANI_KOOPA_DIE_RUN_L;
	}
	else if (state == KOOPA_STATE_DIE_RUNR)
	{
		aniId = ID_ANI_KOOPA_DIE_RUN_R;
	}
	else {
		aniId = (vx > 0) ? ID_ANI_KOOPA_WALKING_R : ID_ANI_KOOPA_WALKING_L;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}
bool CKoopa::GetIsKoopaInDieRunState()
{
	return isKoopaInDieRunState;
}
void CKoopa::SetBound(float a, float b)
{
	this->leftBound = a; 
	this->rightBound = b;
}
void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = KOOPA_GRAVITY;
		ay = 0;
		break;
	case KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		ay = KOOPA_GRAVITY;
		break;
	case KOOPA_STATE_DIE_RUNL:
		vx = -KOOPA_WALKING_DIE_SPEED * 10;
		ay = KOOPA_GRAVITY * 2;
		//y += (TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE) / 2;
		break;
	case KOOPA_STATE_DIE_RUNR:
		vx = KOOPA_WALKING_DIE_SPEED * 10;
		ay = KOOPA_GRAVITY * 2;
		//y += (TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE) / 2;
		break;
	}
}
