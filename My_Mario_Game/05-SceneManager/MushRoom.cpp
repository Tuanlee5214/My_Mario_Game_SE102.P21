#include "MushRoom.h"
#include "QuestionBlock.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScene.h"
#include <cstdlib>
#include <ctime>


CMushRoom::CMushRoom(float x, float y) : CGameObject(x, y)
{
	topY = this->y - 16;
	this->ax = 0;
	this->ay = 0;
	SetState(MUSHROOM_STATE_IDLE);
}

void CMushRoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushRoom::RenderBoundingBox()
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

void CMushRoom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushRoom::OnCollisionWith(LPCOLLISIONEVENT e)
{
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

void CMushRoom::InitRandom()
{
	std::srand((unsigned int)std::time(nullptr));
}

// Hàm random trả về 0 hoặc 1
int CMushRoom::RandomLeftOrRight()
{
	return std::rand() % 2;
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	switch (state)
	{
	case MUSHROOM_STATE_RAISE:
		if (this->y <= topY)
		{
			this->y = topY;
			vy = 0;
			
				SetState(MUSHROOM_STATE_RUNL);

		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushRoom::Render()
{
	int aniId;

	aniId = ID_ANI_MUSHROOM;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CMushRoom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case MUSHROOM_STATE_RAISE:
		vx = 0;
		vy = -MUSHROOM_RISE_SPEED;
		break;
	case MUSHROOM_STATE_RUNL:
		vx = -MUSHROOM_MOVE_SPEED;
		ay = MUSHROOM_GRAVITY;
		break;
	case MUSHROOM_STATE_RUNR:
		vx = MUSHROOM_MOVE_SPEED;
		ay = MUSHROOM_GRAVITY;
		break;
	}
}