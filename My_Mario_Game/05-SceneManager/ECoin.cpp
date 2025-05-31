#include "ECoin.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"
#include "Coin.h"
#include "PlayScene.h"
#include "Point.h"

ECoin::ECoin(float x, float y) : CGameObject(x, y)
{
	this->ay = 0;
	this->ax = 0;
	startY = y;
	topY = y - 45;
	SetState(ECOIN_STATE_JUMPED);
}

void ECoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = this->x - ECOIN_BBOX_WIDTH / 2;
	top = this->y - ECOIN_BBOX_HEIGHT / 2;
	right = left + ECOIN_BBOX_WIDTH;
	bottom = top + ECOIN_BBOX_HEIGHT;
}

void ECoin::RenderBoundingBox()
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

void ECoin::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void ECoin::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void ECoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());

	switch (state)
	{
	case ECOIN_STATE_JUMPED:
		if (this->y <= topY)
		{
			this->y = topY;
			SetState(ECOIN_STATE_FALLED);
		}
		break;
	case ECOIN_STATE_FALLED:
		if (this->y >= startY - 14)
		{
			this->y = startY - 14;
			this->Delete();
			CPoint* point = new CPoint(x, startY - 8 , 1, topY);
			playScene->AddObject(point);
			return;
		}
		break;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void ECoin::Render()
{
	int aniId;
		aniId = ID_ANI_ECOIN;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void ECoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ECOIN_STATE_JUMPED:
		vx = 0;
		vy = -ECOIN_JUMP_SPEED;
		break;
	case ECOIN_STATE_FALLED:
		vx = 0;
		vy = ECOIN_JUMP_SPEED;
		break;
	}
}