#include "RedPlant.h"
#include "GameObject.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Bullet.h"
#include "cmath"

#define REDPLANT_IDLE_TIME 3000
#define REDPLANT_AIM_TIME 1700
#define REDPLANT_SHOOT_TIME 700
#define REDPLANT_SAFE_DISTANCE 23

CRedPlant::CRedPlant(float x, float y) : CGameObject(x, y)
{
    SetState(REDPLANT_STATE_IDLE);
    start_Y = y;
    hasShoot = false;
    startTime = GetTickCount64();
}

void CRedPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - REDPLANT_BBOX_WIDTH / 2;
    top = y - REDPLANT_BBOX_HEIGHT / 2;
    right = left + REDPLANT_BBOX_WIDTH;
    bottom = top + REDPLANT_BBOX_HEIGHT;
}

void CRedPlant::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CRedPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CRedPlant*>(e->obj))
        return;

    if (e->ny != 0 && e->obj->IsBlocking())
    {
        //vy = 0;
    }
}

void CRedPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
    case REDPLANT_STATE_IDLE:
        if (abs(this->x - marioX) >= REDPLANT_SAFE_DISTANCE && GetTickCount64() - startTime > REDPLANT_IDLE_TIME)
        {
            SetState(marioX > this->x ? REDPLANT_STATE_RISE_RIGHT : REDPLANT_STATE_RISE_LEFT);
        }
        //else if (abs(this->x - marioX) <= REDPLANT_SAFE_DISTANCE || GetTickCount64() - startTime < REDPLANT_IDLE_TIME) 
           // SetState(REDPLANT_STATE_IDLE);
        break;

    case REDPLANT_STATE_RISE_LEFT:
    case REDPLANT_STATE_RISE_RIGHT:
        if (this->y <= top_Y)
        {
            y = top_Y;
            vy = 0;
            if (this->x > marioX)
            {
                SetState(marioY < top_Y ? REDPLANT_STATE_AIM_LEFTTOP : REDPLANT_STATE_AIM_LEFTBOTTOM);
                DebugOut(L"REDPLANT_STATE_RISE!!!!!!!!!!!!!!!!!");
            }
            else
            {
                SetState(marioY < top_Y ? REDPLANT_STATE_AIM_RIGHTTOP : REDPLANT_STATE_AIM_RIGHTBOTTOM);
                DebugOut(L"REDPLANT_STATE_RISE!!!!!!!!!!!!!!!!!");

            }
        }
        else if (this->y > top_Y) y -= speed_Y * dt;
        break;

    case REDPLANT_STATE_AIM_LEFTTOP:
        if (!hasShoot && GetTickCount64() - startTime > REDPLANT_SHOOT_TIME)
        {
            DebugOut(L"[INFO] Preparing to shoot bullet in state %d\n", state);
            ShootBullet(playScene);
        }
        if (GetTickCount64() - startTime > REDPLANT_AIM_TIME || abs(this->x - marioX) < REDPLANT_SAFE_DISTANCE)
        {
            SetState(marioX > this->x ? REDPLANT_STATE_HIDE_RIGHT : REDPLANT_STATE_HIDE_LEFT);
        }
        else if (marioY > top_Y && GetTickCount64() - startTime < REDPLANT_AIM_TIME && abs(this->x - marioX) > REDPLANT_SAFE_DISTANCE) 
            SetState(REDPLANT_STATE_AIM_LEFTBOTTOM);
        break;
    case REDPLANT_STATE_AIM_LEFTBOTTOM:
        if (!hasShoot && GetTickCount64() - startTime > REDPLANT_SHOOT_TIME)
        {
            ShootBullet(playScene);
            DebugOut(L"[INFO] Preparing to shoot bullet in state %d\n", state);
        }
        if (GetTickCount64() - startTime > REDPLANT_AIM_TIME || abs(this->x - marioX) < REDPLANT_SAFE_DISTANCE)
        {
            SetState(marioX > this->x ? REDPLANT_STATE_HIDE_RIGHT : REDPLANT_STATE_HIDE_LEFT);
        }
        else if (marioY < top_Y && GetTickCount64() - startTime < REDPLANT_AIM_TIME && abs(this->x - marioX) > REDPLANT_SAFE_DISTANCE) SetState(REDPLANT_STATE_AIM_LEFTTOP);
        break;
    case REDPLANT_STATE_AIM_RIGHTTOP:
        if (!hasShoot && GetTickCount64() - startTime > REDPLANT_SHOOT_TIME)
        {
            ShootBullet(playScene);
            DebugOut(L"[INFO] Preparing to shoot bullet in state %d\n", state);
        }
        if (GetTickCount64() - startTime > REDPLANT_AIM_TIME || abs(this->x - marioX) < REDPLANT_SAFE_DISTANCE)
        {
            SetState(marioX > this->x ? REDPLANT_STATE_HIDE_RIGHT : REDPLANT_STATE_HIDE_LEFT);
        }
        else if (marioY > top_Y && GetTickCount64() - startTime < REDPLANT_AIM_TIME && abs(this->x - marioX) > REDPLANT_SAFE_DISTANCE) SetState(REDPLANT_STATE_AIM_RIGHTBOTTOM);
        break;
    case REDPLANT_STATE_AIM_RIGHTBOTTOM:
        if (!hasShoot && GetTickCount64() - startTime > REDPLANT_SHOOT_TIME)
        {
            ShootBullet(playScene);
            DebugOut(L"[INFO] Preparing to shoot bullet in state %d\n", state);
        }
        if (GetTickCount64() - startTime > REDPLANT_AIM_TIME || abs(this->x - marioX) < REDPLANT_SAFE_DISTANCE)
        {
            SetState(marioX > this->x ? REDPLANT_STATE_HIDE_RIGHT : REDPLANT_STATE_HIDE_LEFT);
        }
        else if (marioY < top_Y && GetTickCount64() - startTime < REDPLANT_AIM_TIME && abs(this->x - marioX) > REDPLANT_SAFE_DISTANCE) SetState(REDPLANT_STATE_AIM_RIGHTTOP);
        break;

    case REDPLANT_STATE_HIDE_LEFT:
    case REDPLANT_STATE_HIDE_RIGHT:
        if (this->y >= start_Y)
        {
            y = start_Y;
            vy = 0;
            SetState(REDPLANT_STATE_IDLE);
        }
        else
        {
            y += speed_Y * dt;
        }
        break;
    }

    // Giới hạn vị trí y
    if (y <= top_Y) y = top_Y;
    if (y >= start_Y) y = start_Y;

    CGameObject::Update(dt, coObjects);
    if (coObjects && !coObjects->empty())
    {
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
}

void CRedPlant::ShootBullet(CPlayScene* playScene)
{
    float vx, vy;
    switch (state)
    {
    case REDPLANT_STATE_AIM_LEFTTOP:
        vx = -BULLET_SPEEDX;
        vy = -BULLET_SPEEDY;
        break;
    case REDPLANT_STATE_AIM_LEFTBOTTOM:
        vx = -BULLET_SPEEDX;
        vy = BULLET_SPEEDY;
        break;
    case REDPLANT_STATE_AIM_RIGHTTOP:
        vx = BULLET_SPEEDX;
        vy = -BULLET_SPEEDY;
        break;
    case REDPLANT_STATE_AIM_RIGHTBOTTOM:
        vx = BULLET_SPEEDX;
        vy = BULLET_SPEEDY;
        break;
    default:
        return;
    }

    //CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (playScene)
    {
        CBullet* bullet = new CBullet(this->x , this->y - 5, vx, vy);
        playScene->AddObject(bullet);
        DebugOut(L"Bullet succesfully created!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        hasShoot = true;
    }
}

void CRedPlant::Render()
{
    int aniId;
    switch (state)
    {
    case REDPLANT_STATE_IDLE:
    case REDPLANT_STATE_RISE_LEFT:
    case REDPLANT_STATE_HIDE_LEFT:
    case REDPLANT_STATE_AIM_LEFTBOTTOM:
        aniId = ID_ANI_REDPLANT_LEFTBOTTOM;
        break;
    case REDPLANT_STATE_RISE_RIGHT:
    case REDPLANT_STATE_HIDE_RIGHT:
    case REDPLANT_STATE_AIM_RIGHTBOTTOM:
        aniId = ID_ANI_REDPLANT_RIGHTBOTTOM;
        break;
    case REDPLANT_STATE_AIM_LEFTTOP:
        aniId = ID_ANI_REDPLANT_LEFTTOP;
        break;
    case REDPLANT_STATE_AIM_RIGHTTOP:
        aniId = ID_ANI_REDPLANT_RIGHTTOP;
        break;
    default:
        aniId = ID_ANI_REDPLANT_LEFTBOTTOM;
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

void CRedPlant::SetState(int state)
{
    CGameObject::SetState(state);
    startTime = GetTickCount64();
    switch (state)
    {
    case REDPLANT_STATE_IDLE:
        vx = 0;
        vy = 0;
        break;
    case REDPLANT_STATE_RISE_LEFT:
    case REDPLANT_STATE_RISE_RIGHT:
        vx = 0;
        vy = -speed_Y;
        break;
    case REDPLANT_STATE_AIM_LEFTTOP:
    case REDPLANT_STATE_AIM_LEFTBOTTOM:
    case REDPLANT_STATE_AIM_RIGHTTOP:
    case REDPLANT_STATE_AIM_RIGHTBOTTOM:
        vx = 0;
        vy = 0;
        break;
    case REDPLANT_STATE_HIDE_LEFT:
    case REDPLANT_STATE_HIDE_RIGHT:
        hasShoot = false;
        vx = 0;
        vy = speed_Y;
        break;
    }
}