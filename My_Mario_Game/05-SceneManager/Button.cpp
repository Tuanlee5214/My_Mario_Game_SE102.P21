#include "Button.h"
#include "Brick.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScene.h"


CButton::CButton(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->topY = this->y - 16;
	SetState(BUTTON_STATE_JUMP);
}

void CButton::RenderBoundingBox()
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

void CButton::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CButton::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	if (mario && state == BUTTON_STATE_TOP)
	{
		SetState(BUTTON_STATE_AFTER_USE);
		
	}

}

void CButton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == BUTTON_STATE_AFTER_USE)
	{
		left = x - BUTTON_BBOX_WIDTH / 2;
		top = y - BUTTON_BBOX_HEIGHT_AFTER_USE / 2;
		right = left + BUTTON_BBOX_WIDTH;
		bottom = top + BUTTON_BBOX_HEIGHT_AFTER_USE;
	}
	else
	{
		left = x - BUTTON_BBOX_WIDTH / 2;
		top = y - BUTTON_BBOX_HEIGHT / 2;
		right = left + BUTTON_BBOX_WIDTH;
		bottom = top + BUTTON_BBOX_HEIGHT;
	}
}

void CButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;


	switch (state)
	{
	case BUTTON_STATE_JUMP:
		if (this->y <= topY)
		{
			this->y = topY;
			SetState(BUTTON_STATE_TOP);
		}
		break;
	case BUTTON_STATE_AFTER_USE:
		this->y = topY + 5;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CButton::Render()
{
	int aniId;

	if (state == BUTTON_STATE_JUMP || state == BUTTON_STATE_TOP) {
		aniId = ID_ANI_BUTTON_BEFORE_USE;
	}
	else
	{
		aniId = ID_ANI_BUTTON_AFTER_USE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CButton::SetState(int state)
{
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();
	CGameObject::SetState(state);
	switch (state)
	{
	case BUTTON_STATE_JUMP:
		vx = 0;
		vy = -BUTTON_JUMP_SPEED;
		break;
	case BUTTON_STATE_TOP:
		vx = 0;
		vy = 0;
		break;
	case BUTTON_STATE_AFTER_USE:
		vx = 0;
		vy = 0;
		break;
	}
}