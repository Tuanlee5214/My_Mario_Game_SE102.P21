#include "Parinha.h"
#include "Animations.h"

#define PARINHA_IDLE_TIME 2500
#define PARINHA_TOP_TIME 1500
#define PARINHA_SAFE_DISTANCE 23
#define PARINHA_MAX_DISTANCE_ATTACK 120

CParinha::CParinha(float x, float y)
{
	start_Y = y;
	SetState(PARINHA_STATE_IDLE);
	startTime = GetTickCount64();
}

void CParinha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - PARINHA_BBOX_WIDTH / 2;
    top = y - PARINHA_BBOX_HEIGHT / 2;
    right = left + PARINHA_BBOX_WIDTH;
    bottom = top + PARINHA_BBOX_HEIGHT;
}

void CParinha::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CParinha::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CParinha*>(e->obj))
        return;

    if (e->ny != 0 && e->obj->IsBlocking())
    {
        //vy = 0;
    }
}

void CParinha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (!playScene)
        return;

    CMario* player = dynamic_cast<CMario*>(playScene->GetPlayer());
    if (!player)
        return;

    float marioX, marioY;
    player->GetPosition(marioX, marioY);

    switch (state)
    {
    case PARINHA_STATE_IDLE:
        if (abs(this->x - marioX) >= PARINHA_SAFE_DISTANCE &&
            GetTickCount64() - startTime > PARINHA_IDLE_TIME &&
            abs(this->x - marioX) <= PARINHA_MAX_DISTANCE_ATTACK)
        {
            SetState(PARINHA_STATE_RISE);
        }
        break;
    case PARINHA_STATE_RISE:
        if (this->y <= top_Y)
        {
            y = top_Y;
            vy = 0;
            SetState(PARINHA_STATE_TOP);
        }
        else if (this->y > top_Y) y -= speed_Y * dt;
        break;
    case PARINHA_STATE_TOP:
        if (GetTickCount64() - startTime > PARINHA_TOP_TIME || abs(this->x - marioX) <= PARINHA_SAFE_DISTANCE)
        {
            SetState(PARINHA_STATE_HIDE);
        }
        break;
    case PARINHA_STATE_HIDE:
        if (this->y >= start_Y)
        {
            y = start_Y;
            vy = 0;
            SetState(PARINHA_STATE_IDLE);
        }
        else if (this->y < start_Y) y += speed_Y * dt;
        break;

    }
    if (y <= top_Y) y = top_Y;
    if (y >= start_Y) y = start_Y;

    CGameObject::Update(dt, coObjects);
    if (coObjects && !coObjects->empty())
    {
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
}

void CParinha::Render()
{
    int aniId = ID_ANI_PARINHA_IDLE;
    switch (state)
    {
    case PARINHA_STATE_IDLE:
        aniId = ID_ANI_PARINHA_IDLE;
        break;
    case PARINHA_STATE_HIDE:
    case PARINHA_STATE_TOP:
    case PARINHA_STATE_RISE:
        aniId = ID_ANI_PARINHA_ATTACK;
        break;
    }

    auto ani = CAnimations::GetInstance()->Get(aniId);
    if (ani)
    {
        ani->Render(x, y);
    }
    else
    {
        DebugOut(L"ani ID %d render failed", aniId);
    }
}

void CParinha::SetState(int state)
{
    CGameObject::SetState(state);
    startTime = GetTickCount64();
    switch (state)
    {
    case PARINHA_STATE_IDLE:
        vx = 0;
        vy = 0;
        break;
    case PARINHA_STATE_RISE:
        vx = 0;
        vy = -speed_Y;
        break;
    case PARINHA_STATE_TOP:
        vx = 0; 
        vy = 0;
        break;
    case PARINHA_STATE_HIDE:
        vx = 0;
        vy = speed_Y;
        break;
    }
}