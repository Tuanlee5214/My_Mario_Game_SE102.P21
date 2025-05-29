#include "QuestionBlock.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"

CQuestionBlock::CQuestionBlock(float x, float y, int type) : CGameObject(x, y)
{
	this->ay = 0;
	this->ax = 0;
	startY = y;
	topY = y - 9;
	this->type = type;
	SetState(QUESBLOCK_STATE_INI);
}

void CQuestionBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = this->x - QUESBLOCK_BBOX_WIDTH / 2;
	top = this->y - QUESBLOCK_BBOX_HEIGHT / 2;
	right = left + QUESBLOCK_BBOX_WIDTH;
	bottom = top + QUESBLOCK_BBOX_HEIGHT;
}

void CQuestionBlock::RenderBoundingBox()
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

void CQuestionBlock::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CQuestionBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	float marioX, marioY;
	if (mario && e->ny < 0 && state == QUESBLOCK_STATE_INI)
	{
		SetState(QUESBLOCK_STATE_JUMPED);
		mario->GetPosition(marioX, marioY);
		mario->SetPosition(marioX, marioY);
		mario->Set_vy(0);
	}
	else if (mario && e->ny < 0 && state == QUESBLOCK_STATE_USED)
	{
		vy = 0;
	}
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	switch (state)
	{
	case QUESBLOCK_STATE_JUMPED:
		if (this->y <= topY)
		{
			this->y = topY;
			SetState(QUESBLOCK_STATE_FALLED);
		}
		break;
	case QUESBLOCK_STATE_FALLED:
		if (this->y >= startY)
		{
			this->y = startY;
			SetState(QUESBLOCK_STATE_USED);
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CQuestionBlock::Render()
{
	int aniId;

	if (state == QUESBLOCK_STATE_INI) {
		aniId = ID_ANI_QUESBLOCK_BEFORE_USE;
	}
	else
	{
		aniId = ID_ANI_QUESBLOCK_AFTER_USE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CQuestionBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESBLOCK_STATE_INI:
		vx = 0;
		vy = 0;
		break;
	case QUESBLOCK_STATE_JUMPED:
		vx = 0;
		vy = -QUESBLOCK_JUMP_SPEED;
		break;
	case QUESBLOCK_STATE_FALLED:
		vx = 0;
		vy = QUESBLOCK_JUMP_SPEED;
		break;
	case QUESBLOCK_STATE_USED:
		vx = 0;
		vy = 0;
		break;
	}
}
