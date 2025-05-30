#include "Effect.h"

CEffect::CEffect(float x, float y, int type, float topY) : CGameObject(x, y)
{
	this->type = type;
	this->topY = topY;
	if (this->type == 1)
		SetState(EFFECT_STATE_JUMP_LEFT);
	else if (this->type == 2)
		SetState(EFFECT_STATE_JUMP_RIGHT);
}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	switch (state)
	{
	case EFFECT_STATE_JUMP_LEFT:
		if (this->y <= topY)
		{
			this->y = topY;
			SetState(EFFECT_STATE_FALL_LEFT);
		}
		break;
	case EFFECT_STATE_JUMP_RIGHT:
		if (this->y <= topY)
		{
			this->y = topY;
			SetState(EFFECT_STATE_FALL_RIGHT);
		}
		break;
	}

}

void CEffect::Render()
{
	int aniId;
	aniId = ID_ANI_EFFECT;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}


void CEffect::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EFFECT_STATE_JUMP_LEFT:
		vx = -EFFECT_MOVE_SPEED;
		vy = -EFFECT_FALL_SPEED;
		break;
	case EFFECT_STATE_JUMP_RIGHT:
		vx = EFFECT_MOVE_SPEED;
		vy = -EFFECT_FALL_SPEED;
		break;
	case EFFECT_STATE_FALL_LEFT:
		vx = -EFFECT_MOVE_SPEED;
		vy = EFFECT_FALL_SPEED;
		break;
	case EFFECT_STATE_FALL_RIGHT:
		vx = EFFECT_MOVE_SPEED;
		vy = EFFECT_FALL_SPEED;
		break;
	}
}

void CEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}

void CEffect::OnNoCollision(DWORD dt)
{

}

void CEffect::OnCollisionWith(LPCOLLISIONEVENT e)
{

}