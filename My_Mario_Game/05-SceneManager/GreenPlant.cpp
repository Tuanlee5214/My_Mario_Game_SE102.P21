#include "GreenPlant.h"
#include "GameObject.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Bullet.h"
#include "cmath"

#define GREENPLANT_IDLE_TIME 3000
#define GREENPLANT_AIM_TIME 1700
#define GREENPLANT_SHOOT_TIME 700
#define GREENPLANT_SAFE_DISTANCE 23

CGreenPlant::CGreenPlant(float x, float y) : CGameObject(x, y)
{
    SetState(GREENPLANT_STATE_IDLE);
    start_Y = y;
    hasShoot = false;
    startTime = GetTickCount64();
}

void CGreenPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - GREENPLANT_BBOX_WIDTH / 2;
    top = y - GREENPLANT_BBOX_HEIGHT / 2;
    right = left + GREENPLANT_BBOX_WIDTH;
    bottom = top + GREENPLANT_BBOX_HEIGHT;
}

void CGreenPlant::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CGreenPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CGreenPlant*>(e->obj))
        return;

    if (e->ny != 0 && e->obj->IsBlocking())
    {
        //vy = 0;
    }
}

void CGreenPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
    case GREENPLANT_STATE_IDLE:
        if (abs(this->x - marioX) >= GREENPLANT_SAFE_DISTANCE && GetTickCount64() - startTime > GREENPLANT_IDLE_TIME)
        {
            SetState(marioX > this->x ? GREENPLANT_STATE_RISE_RIGHT : GREENPLANT_STATE_RISE_LEFT);
        }
        break;

    case GREENPLANT_STATE_RISE_LEFT:
    case GREENPLANT_STATE_RISE_RIGHT:
        if (this->y <= top_Y)
        {
            y = top_Y;
            vy = 0;
            if (this->x > marioX)
            {
                SetState(marioY < top_Y ? GREENPLANT_STATE_AIM_LEFTTOP : GREENPLANT_STATE_AIM_LEFTBOTTOM);
                DebugOut(L"GREENPLANT_STATE_RISE!!!!!!!!!!!!!!!!!");
            }
            else
            {
                SetState(marioY < top_Y ? GREENPLANT_STATE_AIM_RIGHTTOP : GREENPLANT_STATE_AIM_RIGHTBOTTOM);
                DebugOut(L"GREENPLANT_STATE_RISE!!!!!!!!!!!!!!!!!");
            }
        }
        else if (this->y > top_Y) y -= speed_Y * dt;
        break;

    case GREENPLANT_STATE_AIM_LEFTTOP:
        if (!hasShoot && GetTickCount64() - startTime > GREENPLANT_SHOOT_TIME)
        {
            DebugOut(L"[INFO] Preparing to shoot bullet in state %d\n", state);
            ShootBullet(playScene);
        }
        if (GetTickCount64() - startTime > GREENPLANT_AIM_TIME || abs(this->x - marioX) < GREENPLANT_SAFE_DISTANCE)
        {
            SetState(marioX > this->x ? GREENPLANT_STATE_HIDE_RIGHT : GREENPLANT_STATE_HIDE_LEFT);
        }
        else if (marioY > top_Y && GetTickCount64() - startTime < GREENPLANT_AIM_TIME && abs(this->x - marioX) > GREENPLANT_SAFE_DISTANCE)
            SetState(GREENPLANT_STATE_AIM_LEFTBOTTOM);
        break;

    case GREENPLANT_STATE_AIM_LEFTBOTTOM:
        if (!hasShoot && GetTickCount64() - startTime > GREENPLANT_SHOOT_TIME)
        {
            ShootBullet(playScene);
            DebugOut(L"[INFO] Preparing to shoot bullet in state %d\n", state);
        }
        if (GetTickCount64() - startTime > GREENPLANT_AIM_TIME || abs(this->x - marioX) < GREENPLANT_SAFE_DISTANCE)
        {
            SetState(marioX > this->x ? GREENPLANT_STATE_HIDE_RIGHT : GREENPLANT_STATE_HIDE_LEFT);
        }
        else if (marioY < top_Y && GetTickCount64() - startTime < GREENPLANT_AIM_TIME && abs(this->x - marioX) > GREENPLANT_SAFE_DISTANCE) SetState(GREENPLANT_STATE_AIM_LEFTTOP);
        break;

    case GREENPLANT_STATE_AIM_RIGHTTOP:
        if (!hasShoot && GetTickCount64() - startTime > GREENPLANT_SHOOT_TIME)
        {
            ShootBullet(playScene);
            DebugOut(L"[INFO] Preparing to shoot bullet in state %d\n", state);
        }
        if (GetTickCount64() - startTime > GREENPLANT_AIM_TIME || abs(this->x - marioX) < GREENPLANT_SAFE_DISTANCE)
        {
            SetState(marioX > this->x ? GREENPLANT_STATE_HIDE_RIGHT : GREENPLANT_STATE_HIDE_LEFT);
        }
        else if (marioY > top_Y && GetTickCount64() - startTime < GREENPLANT_AIM_TIME && abs(this->x - marioX) > GREENPLANT_SAFE_DISTANCE) SetState(GREENPLANT_STATE_AIM_RIGHTBOTTOM);
        break;

    case GREENPLANT_STATE_AIM_RIGHTBOTTOM:
        if (!hasShoot && GetTickCount64() - startTime > GREENPLANT_SHOOT_TIME)
        {
            ShootBullet(playScene);
            DebugOut(L"[INFO] Preparing to shoot bullet in state %d\n", state);
        }
        if (GetTickCount64() - startTime > GREENPLANT_AIM_TIME || abs(this->x - marioX) < GREENPLANT_SAFE_DISTANCE)
        {
            SetState(marioX > this->x ? GREENPLANT_STATE_HIDE_RIGHT : GREENPLANT_STATE_HIDE_LEFT);
        }
        else if (marioY < top_Y && GetTickCount64() - startTime < GREENPLANT_AIM_TIME && abs(this->x - marioX) > GREENPLANT_SAFE_DISTANCE) SetState(GREENPLANT_STATE_AIM_RIGHTTOP);
        break;

    case GREENPLANT_STATE_HIDE_LEFT:
    case GREENPLANT_STATE_HIDE_RIGHT:
        if (this->y >= start_Y)
        {
            y = start_Y;
            vy = 0;
            SetState(GREENPLANT_STATE_IDLE);
        }
        else
        {
            y += speed_Y * dt;
        }
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

void CGreenPlant::ShootBullet(CPlayScene* playScene)
{
    float vx, vy;
    switch (state)
    {
    case GREENPLANT_STATE_AIM_LEFTTOP:
        vx = -BULLET_SPEEDX;
        vy = -BULLET_SPEEDY;
        break;
    case GREENPLANT_STATE_AIM_LEFTBOTTOM:
        vx = -BULLET_SPEEDX;
        vy = BULLET_SPEEDY;
        break;
    case GREENPLANT_STATE_AIM_RIGHTTOP:
        vx = BULLET_SPEEDX;
        vy = -BULLET_SPEEDY;
        break;
    case GREENPLANT_STATE_AIM_RIGHTBOTTOM:
        vx = BULLET_SPEEDX;
        vy = BULLET_SPEEDY;
        break;
    default:
        return;
    }

    if (playScene)
    {
        CBullet* bullet = new CBullet(this->x, this->y - 5, vx, vy);
        playScene->AddObject(bullet);
        DebugOut(L"Bullet succesfully created!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        hasShoot = true;
    }
}

void CGreenPlant::Render()
{
    int aniId;
    switch (state)
    {
    case GREENPLANT_STATE_IDLE:
    case GREENPLANT_STATE_RISE_LEFT:
    case GREENPLANT_STATE_HIDE_LEFT:
    case GREENPLANT_STATE_AIM_LEFTBOTTOM:
        aniId = ID_ANI_GREENPLANT_LEFTBOTTOM;
        break;
    case GREENPLANT_STATE_RISE_RIGHT:
    case GREENPLANT_STATE_HIDE_RIGHT:
    case GREENPLANT_STATE_AIM_RIGHTBOTTOM:
        aniId = ID_ANI_GREENPLANT_RIGHTBOTTOM;
        break;
    case GREENPLANT_STATE_AIM_LEFTTOP:
        aniId = ID_ANI_GREENPLANT_LEFTTOP;
        break;
    case GREENPLANT_STATE_AIM_RIGHTTOP:
        aniId = ID_ANI_GREENPLANT_RIGHTTOP;
        break;
    default:
        aniId = ID_ANI_GREENPLANT_LEFTBOTTOM;
    }

    auto ani = CAnimations::GetInstance()->Get(aniId);
    if (ani)
    {
        ani->Render(x, y);
    }
    else
    {
        DebugOut(L"[ERROR] Animation ID %d not found\n", aniId);
    }
}

void CGreenPlant::SetState(int state)
{
    CGameObject::SetState(state);
    startTime = GetTickCount64();
    switch (state)
    {
    case GREENPLANT_STATE_IDLE:
        vx = 0;
        vy = 0;
        break;
    case GREENPLANT_STATE_RISE_LEFT:
    case GREENPLANT_STATE_RISE_RIGHT:
        vx = 0;
        vy = -speed_Y;
        break;
    case GREENPLANT_STATE_AIM_LEFTTOP:
    case GREENPLANT_STATE_AIM_LEFTBOTTOM:
    case GREENPLANT_STATE_AIM_RIGHTTOP:
    case GREENPLANT_STATE_AIM_RIGHTBOTTOM:
        vx = 0;
        vy = 0;
        break;
    case GREENPLANT_STATE_HIDE_LEFT:
    case GREENPLANT_STATE_HIDE_RIGHT:
        hasShoot = false;
        vx = 0;
        vy = speed_Y;
        break;
    }
}
