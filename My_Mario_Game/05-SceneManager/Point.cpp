#include "Point.h"

void CPoint::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}

void CPoint::OnNoCollision(DWORD dt)
{

}

void CPoint::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

CPoint::CPoint(float x, float y, int type, float topY) : CGameObject(x, y)
{
	this->type = type;
	this->topY = topY;
	SetState(POINT_STATE_JUMP);
}

void CPoint::Render()
{
	int aniId;
	if (type == 1)
		aniId = ID_ANI_POINT_TYPE1;
	else if (type == 2)
		aniId = ID_ANI_POINT_TYPE2;
	else if (type == 4)
		aniId = ID_ANI_1UP;
	else if (type == 5)
		aniId = ID_ANI_MUSH_EFFEC;
	else
		aniId = ID_ANI_POINT_TYPE3;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CPoint::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case POINT_STATE_JUMP:
		vx = 0;
		vy = -POINT_JUMP_SPEED;
		break;
	}
}

void CPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	switch (state)
	{
	case POINT_STATE_JUMP:
		if (this->y <= topY)
		{
			this->y = topY;
			this->Delete();
		}
	}
}