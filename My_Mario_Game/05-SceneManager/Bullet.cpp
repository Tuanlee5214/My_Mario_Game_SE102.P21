#include "Bullet.h"
#include "Game.h"
#include "Animations.h"
#include "debug.h"

CBullet::CBullet(float x, float y, float vx, float vy)
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;

}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_BBOX_WIDTH / 2;
	top = y - BULLET_BBOX_HEIGHT / 2;
	right = left + BULLET_BBOX_WIDTH;
	bottom = top + BULLET_BBOX_HEIGHT;
}

void CBullet::OnNoCollision(DWORD dt)
{
	//x += vx * dt;
	//y += vy * dt;
}

void CBullet::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CBullet*>(e->obj))
		return;

	if (e->obj->IsBlocking())
	{
		//isDeleted = true;
	}
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBullet::Render()
{
	auto ani = CAnimations::GetInstance()->Get(ID_ANI_BULLET);
	if (ani)
	{
		ani->Render(x, y);
	}
	else
	{
		DebugOut(L"Animation Bullet ID %d load failed\n", ID_ANI_BULLET);
	}
}