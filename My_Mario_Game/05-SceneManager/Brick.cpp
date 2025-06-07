#include "Brick.h"
#include "QuestionBlock.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScene.h"
#include "ECoin.h"
#include "Leaf.h"
#include "Effect.h"
#include "MushRoom.h"
#include "Button.h"
#include "Coin.h"
#include "Point.h"


CBrick::CBrick(float x, float y, int type) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	startX = x;
	startY = y;
	topY = y - 10;
	this->type = type;
	SetState(BRICK_STATE_INI);
}

void CBrick::RenderBoundingBox()
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

void CBrick::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMario* player = (CMario*)(playScene->GetPlayer());

	float marioX, marioY;
	if (mario && e->ny < 0 && state == BRICK_STATE_INI && type == 2)
	{
		SetState(BRICK_STATE_JUMPED);
		if (playScene)
		{
			CButton* button = new CButton(this->x, this->y);
			playScene->InsertObjectBefore(button, this);
		}
		mario->GetPosition(marioX, marioY);
		mario->SetPosition(marioX, marioY);
		mario->Set_vy(0);
	}
	else if (mario && e->ny < 0 && state == BRICK_STATE_INI && type == 3)
	{
		this->Delete();
		CPoint* point = new CPoint(this->x, this->y, 5, y - 110);
		playScene->InsertObjectBefore(point, this);
	}

	if (mario && (e->nx != 0 || e->ny != 0) && type == 1)
	{
		return;
	}

	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa && e->nx != 0 && state == BRICK_STATE_INI && 
		(koopa->GetState() == KOOPA_STATE_DIE_RUNL || 
		koopa->GetState() == KOOPA_STATE_DIE_RUNR) && type == 1)
	{
		this->Delete();
		koopa->Set_vx1();
		CEffect* effect1 = new CEffect(this->x, this->y, 1, this->y - 80);
		CEffect* effect2 = new CEffect(this->x, this->y, 1, this->y - 50);
		CEffect* effect3 = new CEffect(this->x, this->y, 2, this->y - 80);
		CEffect* effect4 = new CEffect(this->x, this->y, 2, this->y - 50);
		playScene->AddObject(effect1);
		playScene->AddObject(effect2);
		playScene->AddObject(effect3);
		playScene->AddObject(effect4);
	}

	if (e->obj->IsBlocking()) return;
}

void CBrick::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x - BRICK_BBOX_WIDTH/2 - 2;
	top = y - BRICK_BBOX_HEIGHT/2;
	right = left + BRICK_BBOX_WIDTH;
	bottom = top + BRICK_BBOX_HEIGHT - 2;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CButton* button = (CButton*)playScene->GetButton(playScene);
	switch (state)
	{
	case BRICK_STATE_INI:
		if (this->type == 1 && button && button->GetState() == BUTTON_STATE_AFTER_USE)
		{
			SetState(BRICK_STATE_SPAWN_COIN);
		}
	case BRICK_STATE_JUMPED:
		if (this->y <= topY)
		{
			this->y = topY;
			SetState(BRICK_STATE_FALLED);
		}
		break;
	case BRICK_STATE_FALLED:
		if (this->y >= startY)
		{
			this->y = startY;
			SetState(BRICK_STATE_USED);
		}
		break;
	case BRICK_STATE_USED:
		this->y = startY - 0.25f;
		this->x = startX - 1;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBrick::Render()
{
	int aniId;

	if (state == BRICK_STATE_INI && type == 3)
	{
		aniId = ID_ANI_MSF;
	}
	else if (state == BRICK_STATE_INI && type != 3) {
		aniId = ID_ANI_BRICK_BEFORE_USE;
	}
	else
	{
		aniId = ID_ANI_QUESBLOCK_AFTER_USE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::SetState(int state)
{
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_INI:
		vx = 0;
		vy = 0;
		break;
	case BRICK_STATE_JUMPED:
		vx = 0;
		vy = -BRICK_JUMP_SPEED;
		break;
	case BRICK_STATE_FALLED:
		vx = 0;
		vy = BRICK_JUMP_SPEED;
		break;
	case BRICK_STATE_USED:
		vx = 0;
		vy = 0;
		break;
	case BRICK_STATE_SPAWN_COIN:
		this->Delete();
		CCoin* coin = new CCoin(this->x, this->y, 2);
		playScene->InsertObjectBefore(coin, this);
	}
}