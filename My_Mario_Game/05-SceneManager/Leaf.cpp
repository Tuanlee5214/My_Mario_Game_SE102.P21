#include "Leaf.h"
#include "QuestionBlock.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScene.h"

CLeaf::CLeaf(float x, float y, float leftBound, float rightBound) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	topY = this->y - 45;
	SetState(LEAF_STATE_RAISE);
	this->leftBound = leftBound;
	this->rightBound = rightBound;
}


void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;
}

void CLeaf::RenderBoundingBox()
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

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = (CMario*)playScene->GetPlayer();
	vy += ay * dt;
	vx += ax * dt;

	switch (state)
	{
	case LEAF_STATE_RAISE:
		if (this->y <= topY)
		{
			this->y = topY;
			SetState(LEAF_STATE_FALL_RIGHT);
		}
		break;
	case LEAF_STATE_FALL_RIGHT:
		if (abs(x - rightBound) <= 15) vy = -LEAF_FALL_SPEED;
		if (this->x >= rightBound)
		{
			this->x = rightBound;
			SetState(LEAF_STATE_FALL_LEFT);
		}
		break;
	case LEAF_STATE_FALL_LEFT:
		if (abs(x - leftBound) <= 15) vy = -LEAF_FALL_SPEED;
		if (this->x <= leftBound)
		{
			this->x = leftBound;
			SetState(LEAF_STATE_FALL_RIGHT);
		}
		break;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{
	int aniId;

	if (state == LEAF_STATE_RAISE || state == LEAF_STATE_FALL_LEFT)
		aniId = ID_ANI_LEAF_LEFT;
	else aniId = ID_ANI_LEAF_RIGHT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_RAISE:
		vx = 0;
		vy = -LEAF_RAISE_SPEED;
		break;
	case LEAF_STATE_FALL_LEFT:
		vx = -LEAF_MOVE_SPEED;
		vy = LEAF_FALL_SPEED;
		break;
	case LEAF_STATE_FALL_RIGHT:
		vx = LEAF_MOVE_SPEED;
		vy = LEAF_FALL_SPEED;
		break;
	}
}