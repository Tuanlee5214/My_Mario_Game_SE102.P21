#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "Koopa.h"
#include "Collision.h"
#include "RedGoomba.h"
#include "RedPlant.h"
#include "Bullet.h"
#include "Parinha.h"
#include "GreenPlant.h"
#include "Troopa.h"
#include "ParaTroopa.h"
#include "PlayScene.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "SwitchPos.h"
#include "Point.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;


	if (abs(vx) > abs(maxVx)) vx = maxVx;
	if (state == MARIO_STATE_RUNNING_RIGHT)
	{
		if (abs(ax) > abs(MARIO_ACCEL_RUN_X)) ax = MARIO_ACCEL_RUN_X;
	}
	else if (state == MARIO_STATE_RUNNING_LEFT)
	{
		if (abs(ax) > abs(MARIO_ACCEL_RUN_X)) ax = -MARIO_ACCEL_RUN_X;
	}



	if (abs(ax) == 0 && abs(vx) != 0)
	{
		if (vx > 0 && vx <= MARIO_WALKING_SPEED)
		{
			vx -= 0.008f;
			if (vx < 0)
			{
				vx = 0;
			}
		}
		else if (vx > MARIO_WALKING_SPEED && vx <= MARIO_RUNNING_SPEED)
		{
			vx -= 0.011f;
			if (vx < 0)
			{
				vx = 0;
			}
		}
		else if (vx < 0 && vx >= -MARIO_WALKING_SPEED)
		{
			vx += 0.008f;
			if (vx > 0)
			{
				vx = 0;
			}
		}
		else if (vx < -MARIO_WALKING_SPEED && vx >= -MARIO_RUNNING_SPEED)
		{
			vx += 0.011f;
			if (vx > 0) vx = 0;
		}
	}
	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isTurn && GetTickCount64() - startTurnTail > MARIO_TURN_TAIL_TIMEOUT)
	{
		isTurn = false;
		if (vx != 0) return;
		else 
			SetState(MARIO_STATE_IDLE);
	}




	if (GetTickCount64() - isInKickStateNow > MARIO_IN_KICKSTATE_TIME && isRight1)
	{
		isRight1 = false;
	}

	if (level == 3 && GetTickCount64() - timeStartTransForm > MARIO_TRANSFORM_SMALL_TO_BIG_TIMEOUT)
	{
		SetLevel(MARIO_LEVEL_BIG);
	}

	if (level == 4 && GetTickCount64() - timeStartTransForm > MARIO_TRANSFORM_BIG_TO_SMALL_TIMEOUT)
	{
		SetLevel(MARIO_LEVEL_SMALL);
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CRedGoomba*>(e->obj))
		OnCollisionWithRedGoomba(e);
	else if (dynamic_cast<CRedPlant*>(e->obj))
		OnCollisionWithRedPlant(e);
	else if (dynamic_cast<CBullet*>(e->obj))
		OnCollisionWithBullet(e);
	else if (dynamic_cast<CParinha*>(e->obj))
		OnCollisionWithParinha(e);
	else if (dynamic_cast<CGreenPlant*>(e->obj))
		OnCollisionWithGreenPlant(e);
	else if (dynamic_cast<CTroopa*>(e->obj))
		OnCollisionWithTroopa(e);
	else if (dynamic_cast<CParaTroopa*>(e->obj))
		OnCollisionWithParaTroopa(e);
	else if (dynamic_cast<CMushRoom*>(e->obj))
		OnCollisionWithMushRoom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWihtLeaf(e);
	else if (dynamic_cast<CSwitchPos*>(e->obj))
		OnCollisionWithSwitchPos(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			float goombaX, goombaY;
			goomba->GetPosition(goombaX, goombaY);
			goomba->SetState(GOOMBA_STATE_DIE);
			CPoint* point = new CPoint(goombaX, goombaY - 3, 1, goombaY - 35);
			playScene->InsertObjectBefore(point, goomba);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else if (goomba->GetState() != GOOMBA_STATE_DIE && state == MARIO_STATE_TURN_TAIL && (e->nx != 0 || e->ny != 0))
	{
		float x, y;
		goomba->GetPosition(x, y);
		goomba->SetState(GOOMBA_STATE_OUT_GAME);
		CPoint* point = new CPoint(x, y - 4, 1, y - 50);
		playScene->AddObject(point);
		return;
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithRedGoomba(LPCOLLISIONEVENT e)
{
	CRedGoomba* redGoomba = dynamic_cast<CRedGoomba*>(e->obj);
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	float x, y;
	redGoomba->GetPosition(x, y);
	if (e->ny < 0)
	{
		if (redGoomba->GetState() == REDGOOMBA_STATE_JUMP_WALKING || redGoomba->GetState() == REDGOOMBA_STATE_JUMPLOW ||
			redGoomba->GetState() == REDGOOMBA_STATE_JUMP)
		{
			redGoomba->SetState(REDGOOMBA_STATE_WALKING);
			CPoint* point = new CPoint(x, y - 3, 1, y - 50);
			playScene->AddObject(point);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (redGoomba->GetState() == REDGOOMBA_STATE_WALKING)
		{
			redGoomba->SetState(REDGOOMBA_STATE_DIE);
			CPoint* point = new CPoint(x, y - 3, 1, y - 50);
			playScene->AddObject(point);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else if (redGoomba->GetState() != REDGOOMBA_STATE_DIE && state == MARIO_STATE_TURN_TAIL && (e->nx != 0 || e->ny != 0))
	{
		float x, y;
		redGoomba->GetPosition(x, y);
		redGoomba->SetState(REDGOOMBA_STATE_OUT_GAME);
		CPoint* point = new CPoint(x, y - 4, 1, y - 50);
		playScene->AddObject(point);
		return;
	}
	else 
	{
		if (untouchable == 0)
		{
			if (redGoomba->GetState() != REDGOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	float x, y; 
	koopa->GetPosition(x, y);
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	DebugOut(L"[INFO] Mario va cham Koopa, ny = %d\n", e->ny);
	// jump on top >> kill Koopa and deflect a bit 
	if (e->ny < 0)
	{
		if (koopa->GetState() == KOOPA_STATE_WALKING)
		{
			koopa->SetState(KOOPA_STATE_DIE);
			CPoint* point = new CPoint(x, y - 2, 1, y - 35);
			playScene->AddObject(point);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPA_STATE_DIE_RUNL ||
				koopa->GetState() == KOOPA_STATE_DIE_RUNR)
		{
			koopa->Set_Y((KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE + 2) / 2);
			koopa->SetState(KOOPA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPA_STATE_DIE)
		{
			if (nx > 0)
			{
				koopa->SetBound(-10.0f, 10000.0f);
				koopa->Set_Y((KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE + 2) / 2);
				CPoint* point = new CPoint(x, y - 2, 2, y - 35);
				playScene->AddObject(point);
				koopa->SetState(KOOPA_STATE_DIE_RUNR);
				this->SetIsRight1(true);
				this->isInKickStateNow = GetTickCount64();
			}
			else
			{
				koopa->SetBound(-10.0f, 10000.0f);
				koopa->Set_Y((KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE + 2) / 2);
				CPoint* point = new CPoint(x, y - 2, 2, y - 35);
				playScene->AddObject(point);
				koopa->SetState(KOOPA_STATE_DIE_RUNL);
				this->SetIsRight1(true);
				this->isInKickStateNow = GetTickCount64();
			}
			//vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else if (koopa->GetState() != KOOPA_STATE_DIE && state == MARIO_STATE_TURN_TAIL && (e->nx != 0 || e->ny != 0))
	{
		float x, y;
		koopa->GetPosition(x, y);
		koopa->SetState(KOOPA_STATE_OUT_GAME);
		CPoint* point = new CPoint(x, y - 4, 1, y - 50);
		playScene->AddObject(point);
		return;
	}
	else // hit by Koopa
	{
		if (untouchable == 0)
		{
			if (koopa->GetState() != KOOPA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
			else if (koopa->GetState() == KOOPA_STATE_DIE)
			{
				if (e->nx < 0)
				{
					koopa->SetBound(-10.0f, 10000.0f);
					koopa->SetState(KOOPA_STATE_DIE_RUNR);
					CPoint* point = new CPoint(x, y - 2, 2, y - 35);
					playScene->AddObject(point);
					this->SetIsRight1(true);
					this->isInKickStateNow = GetTickCount64();
				}
				else if (e->nx > 0)
				{
					koopa->SetBound(-10.0f, 10000.0f);
					koopa->SetState(KOOPA_STATE_DIE_RUNL);
					CPoint* point = new CPoint(x, y - 2, 2, y - 35);
					playScene->AddObject(point);
					this->SetIsRight1(true);
					this->isInKickStateNow = GetTickCount64();
				}
			}
		}
	}
}

void CMario::OnCollisionWithMushRoom(LPCOLLISIONEVENT e)
{
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMushRoom* mushRoom = dynamic_cast<CMushRoom*>(e->obj);
	float x, y;
	mushRoom->GetPosition(x, y);
	if ((e->nx != 0 || e->ny != 0) && mushRoom->GetType() == 1)
	{
		SetState(MARIO_STATE_TRANSFORM_SMALL_TO_BIG);
		mushRoom->Delete();
		CPoint* point = new CPoint(x, y, 3, y - 40);
		playScene->AddObject(point);

	}
	else if ((e->nx != 0 || e->ny != 0) && mushRoom->GetType() == 2)
	{
		mushRoom->Delete();
		CPoint* point = new CPoint(x, y, 4, y - 40);
		playScene->AddObject(point);
	}
}

void CMario::OnCollisionWithRedPlant(LPCOLLISIONEVENT e)
{
	CRedPlant* redPlant = dynamic_cast<CRedPlant*>(e->obj);
	if (e->ny != 0 || e->nx != 0)
	{
		if (untouchable == 0)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
				StartUntouchable();
			}
			else
			{
				DebugOut(L"Mario die >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithParaTroopa(LPCOLLISIONEVENT e)
{
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	float x, y;
	CParaTroopa* paraTroopa = dynamic_cast<CParaTroopa*>(e->obj);
	paraTroopa->GetPosition(x, y);
	if (e->ny < 0)
	{
			if (paraTroopa->GetState() == PARATROOPA_STATE_WALKING_FLY)
			{
				paraTroopa->SetState(PARATROOPA_STATE_WALKING);
				CPoint* point = new CPoint(x, y - 2, 1, y - 35);
				playScene->AddObject(point);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (paraTroopa->GetState() == PARATROOPA_STATE_WALKING)
			{
				paraTroopa->SetState(PARATROOPA_STATE_DIE);
				CPoint* point = new CPoint(x, y - 2, 1, y - 35);
				playScene->AddObject(point);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (paraTroopa->GetState() == PARATROOPA_STATE_DIE_RUNL ||
				paraTroopa->GetState() == PARATROOPA_STATE_DIE_RUNR)
			{
				paraTroopa->SetY((PARATROOPA_BBOX_HEIGHT - PARATROOPA_BBOX_HEIGHT_DIE + 2) / 2);
				paraTroopa->SetState(PARATROOPA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (paraTroopa->GetState() == PARATROOPA_STATE_DIE)
			{
				if (nx > 0)
				{
					paraTroopa->SetBound(0.0f, 10000.0f);
					paraTroopa->SetY((PARATROOPA_BBOX_HEIGHT - PARATROOPA_BBOX_HEIGHT_DIE + 2) / 2);
					paraTroopa->SetState(PARATROOPA_STATE_DIE_RUNR);
					CPoint* point = new CPoint(x, y - 2, 2, y - 35);
					playScene->AddObject(point);
					this->SetIsRight1(true);
					this->isInKickStateNow = GetTickCount64();
				}
				else
				{
					paraTroopa->SetBound(0.0f, 10000.0f);
					paraTroopa->SetY((PARATROOPA_BBOX_HEIGHT - PARATROOPA_BBOX_HEIGHT_DIE + 2) / 2);
					paraTroopa->SetState(PARATROOPA_STATE_DIE_RUNL);
					CPoint* point = new CPoint(x, y - 2, 2, y - 35);
					playScene->AddObject(point);
					this->SetIsRight1(true);
					this->isInKickStateNow = GetTickCount64();
				}
				//vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
	}
	else if (paraTroopa->GetState() != PARATROOPA_STATE_DIE && state == MARIO_STATE_TURN_TAIL && (e->nx != 0 || e->ny != 0))
	{
		float x, y;
		paraTroopa->GetPosition(x, y);
		paraTroopa->SetState(PARATROOPA_STATE_OUT_GAME);
		CPoint* point = new CPoint(x, y - 4, 1, y - 50);
		playScene->AddObject(point);
		return;
	}
	else
	{
		if (untouchable == 0)
		{
			if (paraTroopa->GetState() != PARATROOPA_STATE_DIE){
				if (level > MARIO_LEVEL_SMALL)
				{
					SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
			else if (paraTroopa->GetState() == PARATROOPA_STATE_DIE)
			{
				if (e->nx < 0)
				{
					paraTroopa->SetState(PARATROOPA_STATE_DIE_RUNR);
					paraTroopa->SetBound(0.0f, 10000.0f);
					CPoint* point = new CPoint(x, y - 2, 2, y - 35);
					playScene->AddObject(point);
					this->SetIsRight1(true);
					this->isInKickStateNow = GetTickCount64();
				}
				else if (e->nx > 0)
				{
					paraTroopa->SetState(PARATROOPA_STATE_DIE_RUNL);
					this->SetIsRight1(true);
					paraTroopa->SetBound(0.0f, 10000.0f);
					CPoint* point = new CPoint(x, y - 2, 2, y - 35);
					playScene->AddObject(point);
					this->isInKickStateNow = GetTickCount64();
				}
			}
		}
	}
}

void CMario::OnCollisionWithTroopa(LPCOLLISIONEVENT e)
{
	CTroopa* troopa = dynamic_cast<CTroopa*>(e->obj);
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	float x, y;
	troopa->GetPosition(x, y);
	DebugOut(L"[INFO] Mario va cham Koopa, ny = %d\n", e->ny);
	// jump on top >> kill troopa and deflect a bit 
	if (e->ny < 0)
	{
		if (troopa->GetState() == TROOPA_STATE_WALKING)
		{
			troopa->SetState(TROOPA_STATE_DIE);
			CPoint* point = new CPoint(x, y - 2, 1, y - 35);
			playScene->AddObject(point);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (troopa->GetState() == TROOPA_STATE_DIE_RUNL ||
			troopa->GetState() == TROOPA_STATE_DIE_RUNR)
		{
			troopa->SetY((TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE + 2) / 2);
			troopa->SetState(TROOPA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (troopa->GetState() == TROOPA_STATE_DIE)
		{
			if (nx > 0)
			{
				troopa->SetBound(-10.0f, 10000.0f);
				troopa->SetY((TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE + 2) / 2);
				troopa->SetState(TROOPA_STATE_DIE_RUNR);
				CPoint* point = new CPoint(x, y - 2, 2, y - 35);
				playScene->AddObject(point);
				this->SetIsRight1(true);
				this->isInKickStateNow = GetTickCount64();
			}
			else
			{
				troopa->SetBound(-10.0f, 10000.0f);
				troopa->SetY((TROOPA_BBOX_HEIGHT - TROOPA_BBOX_HEIGHT_DIE + 2) / 2);
				troopa->SetState(TROOPA_STATE_DIE_RUNL);
				CPoint* point = new CPoint(x, y - 2, 2, y - 35);
				playScene->AddObject(point);
				this->SetIsRight1(true);
				this->isInKickStateNow = GetTickCount64();
			}
			//vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else if (troopa->GetState() != TROOPA_STATE_DIE && state == MARIO_STATE_TURN_TAIL && (e->nx != 0 || e->ny != 0))
	{
		float x, y;
		troopa->GetPosition(x, y);
		troopa->SetState(TROOPA_STATE_OUT_GAME);
		CPoint* point = new CPoint(x, y - 4, 1, y - 50);
		playScene->AddObject(point);
		return;
	}
	else // hit by troopa
	{
		if (untouchable == 0)
		{
			if (troopa->GetState() != TROOPA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
			else if (troopa->GetState() == TROOPA_STATE_DIE)
			{
				if (e->nx < 0)
				{
					troopa->SetBound(-10.0f, 10000.0f);
					troopa->SetState(TROOPA_STATE_DIE_RUNR);
					CPoint* point = new CPoint(x, y - 2, 2, y - 35);
					playScene->AddObject(point);
					this->SetIsRight1(true);
					this->isInKickStateNow = GetTickCount64();
				}
				else if (e->nx > 0)
				{
					troopa->SetBound(-10.0f, 10000.0f);
					troopa->SetState(TROOPA_STATE_DIE_RUNL);
					CPoint* point = new CPoint(x, y - 2, 2, y - 35);
					playScene->AddObject(point);
					this->SetIsRight1(true);
					this->isInKickStateNow = GetTickCount64();
				}
				
			}
		}
	}
}

void CMario::OnCollisionWithParinha(LPCOLLISIONEVENT e)
{
	CParinha* parinha = dynamic_cast<CParinha*>(e->obj);
	if (e->ny != 0 || e->nx != 0)
	{
		if (untouchable == 0)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
				StartUntouchable();
			}
			else
			{
				DebugOut(L"Mario die >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithGreenPlant(LPCOLLISIONEVENT e)
{

	CGreenPlant* greenPlant = dynamic_cast<CGreenPlant*>(e->obj);
	if (e->ny != 0 || e->nx != 0)
	{
		if (untouchable == 0)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
				StartUntouchable();
			}
			else
			{
				DebugOut(L"Mario die >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWihtLeaf(LPCOLLISIONEVENT e)
{
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	float x, y;
	leaf->GetPosition(x, y);
	if (e->nx != 0 || e->ny != 0)
	{
		SetLevel(MARIO_LEVEL_MAX);
		leaf->Delete();
		CPoint* point = new CPoint(x, y - 3, 3, y - 50);
		playScene->AddObject(point);
	}
}

void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
	CBullet* bullet = dynamic_cast<CBullet*>(e->obj);
	if (e->ny != 0 || e->nx != 0)
	{
		if (untouchable == 0)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				SetState(MARIO_STATE_TRANSFORM_BIG_TO_SMALL);
				StartUntouchable();
			}
			else
			{
				DebugOut(L"Mario die >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	//CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithSwitchPos(LPCOLLISIONEVENT e)
{
	CSwitchPos* p = (CSwitchPos*)e->obj;
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	if (p->GetType() == 1)
	{
		this->SetPosition(146, 238);
		playScene->SetIsInSecret(true);

	}
	else if (p->GetType() == 2)
	{
		this->SetPosition(2366, 142);
		playScene->SetIsInSecret(false);
	}
}
//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	CPlayScene* playScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CKoopa* koopa = playScene->GetFirstKoopa(playScene);
	bool isHoldKoopa = this->GetIsHoldingKoopa();
	CTroopa* troopa = playScene->GetTroopa(playScene);
	bool isHoldTroopa = this->GetIsHoldingTroopa();
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0 && !isHoldKoopa && !isHoldTroopa)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else if(nx >= 0 && (isHoldKoopa || isHoldTroopa))
				aniId = ID_ANI_MARIO_SMALL_IDLE_CARRY_RIGHT;
			else if (nx < 0 && (isHoldKoopa || isHoldTroopa))
				aniId = ID_ANI_MARIO_SMALL_IDLE_CARRY_LEFT;
			else if(nx < 0 && !isHoldKoopa && !isHoldTroopa)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (isRight1)
				{
					aniId = nx > 0 ? ID_ANI_MARIO_SMALL_KICKING_RIGHT : ID_ANI_MARIO_SMALL_KICKING_LEFT;
				}
				else if (nx > 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else if (nx > 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_SMALL_IDLE_CARRY_RIGHT;
				else if (nx < 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
				else if (nx < 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_SMALL_IDLE_CARRY_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax > 0 && ax < MARIO_ACCEL_RUN_X && state == MARIO_STATE_RUNNING_RIGHT)
					aniId = ID_ANI_MARIO_SMALL_WALK_TO_RUN_RIGHT;
				else if (ax == 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
				else if (ax == 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_SMALL_WALKING_CARRY_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X && state != MARIO_STATE_RUNNING_RIGHT)
				{
					if ((isHoldKoopa || isHoldTroopa) && !isRight1)
					{
						aniId = ID_ANI_MARIO_SMALL_WALKING_CARRY_RIGHT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && !isRight1)
					{
						aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
					}
					else if ((isHoldKoopa || isHoldTroopa) && isRight1)
					{
						aniId = ID_ANI_MARIO_SMALL_KICKING_RIGHT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && isRight1)
					{
						aniId = ID_ANI_MARIO_SMALL_KICKING_RIGHT;
					}

					//if (!isRight1) DebugOut(L"ISRIGHT FALSEEEEEEEEEEEEEEEEEE");
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax < 0 && ax > -MARIO_ACCEL_RUN_X && state == MARIO_STATE_RUNNING_LEFT)
					aniId = ID_ANI_MARIO_SMALL_WALK_TO_RUN_LEFT;
				else if (ax == 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
				else if (ax == 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_SMALL_WALKING_CARRY_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X && state != MARIO_STATE_RUNNING_LEFT)
				{
					if ((isHoldKoopa || isHoldTroopa) && !isRight1)
					{
						aniId = ID_ANI_MARIO_SMALL_WALKING_CARRY_LEFT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && !isRight1)
					{
						aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
					}
					else if ((isHoldKoopa || isHoldTroopa) && isRight1)
					{
						aniId = ID_ANI_MARIO_SMALL_KICKING_LEFT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && isRight1)
					{
						aniId = ID_ANI_MARIO_SMALL_KICKING_LEFT;
					}
				}
			}

	if (aniId == -1 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	CPlayScene* playScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CKoopa* koopa = playScene->GetFirstKoopa(playScene);
	CTroopa* troopa = playScene->GetTroopa(playScene);
	bool isHoldTroopa = this->GetIsHoldingTroopa();
	bool isHoldKoopa = this->GetIsHoldingKoopa();
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0 && !isHoldKoopa && !isHoldTroopa && vy < 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else if (nx >= 0 && (isHoldKoopa || isHoldTroopa) && vy < 0)
				aniId = ID_ANI_MARIO_IDLE_CARRY_RIGHT;
			else if (nx < 0 && (isHoldKoopa || isHoldTroopa) && vy < 0)
				aniId = ID_ANI_MARIO_IDLE_CARRY_LEFT;
			else if (nx < 0 && !isHoldKoopa && !isHoldTroopa && vy < 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
			else if (nx >= 0 && !isHoldKoopa && !isHoldTroopa && vy > 0)
				aniId = ID_ANI_MARIO_FALL_WALK_RIGHT;
			else if (nx >= 0 && (isHoldKoopa || isHoldTroopa) && vy > 0)
				aniId = ID_ANI_MARIO_IDLE_CARRY_RIGHT;
			else if (nx < 0 && (isHoldKoopa || isHoldTroopa) && vy > 0)
				aniId = ID_ANI_MARIO_IDLE_CARRY_LEFT;
			else if (nx < 0 && !isHoldKoopa && !isHoldTroopa && vy > 0)
				aniId = ID_ANI_MARIO_FALL_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (isRight1)
				{
					aniId = nx > 0 ? ID_ANI_MARIO_KICKING_RIGHT : ID_ANI_MARIO_KICKING_LEFT;
				}
				else if (nx > 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else if (nx > 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_IDLE_CARRY_RIGHT;
				else if (nx < 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_IDLE_LEFT;
				else if (nx < 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_IDLE_CARRY_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax > 0 && ax < MARIO_ACCEL_RUN_X && state == MARIO_STATE_RUNNING_RIGHT)
					aniId = ID_ANI_MARIO_WALK_TO_RUN_RIGHT;
				else if (ax == 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_WALKING_RIGHT;
				else if (ax == 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_WALKING_CARRY_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X && state != MARIO_STATE_RUNNING_RIGHT)
				{
					if ((isHoldKoopa || isHoldTroopa) && !isRight1)
					{
						aniId = ID_ANI_MARIO_WALKING_CARRY_RIGHT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && !isRight1)
					{
						aniId = ID_ANI_MARIO_WALKING_RIGHT;
					}
					else if ((isHoldKoopa || isHoldTroopa) && isRight1)
					{
						aniId = ID_ANI_MARIO_KICKING_RIGHT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && isRight1)
					{
						aniId = ID_ANI_MARIO_KICKING_RIGHT;
					}

					//if (!isRight1) DebugOut(L"ISRIGHT FALSEEEEEEEEEEEEEEEEEE");
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax < 0 && ax > -MARIO_ACCEL_RUN_X && state == MARIO_STATE_RUNNING_LEFT)
					aniId = ID_ANI_MARIO_WALK_TO_RUN_LEFT;
				else if (ax == 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_WALKING_LEFT;
				else if (ax == 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_WALKING_CARRY_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X && state != MARIO_STATE_RUNNING_LEFT)
				{
					if ((isHoldKoopa || isHoldTroopa) && !isRight1)
					{
						aniId = ID_ANI_MARIO_WALKING_CARRY_LEFT;
					}
					else if(!isHoldKoopa && !isHoldTroopa && !isRight1)
					{
						aniId = ID_ANI_MARIO_WALKING_LEFT;
					}
					else if ((isHoldKoopa || isHoldTroopa) && isRight1)
					{
						aniId = ID_ANI_MARIO_KICKING_LEFT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && isRight1)
					{
						aniId = ID_ANI_MARIO_KICKING_LEFT;
					}
				}
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdMax()
{
	CPlayScene* playScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CKoopa* koopa = playScene->GetFirstKoopa(playScene);
	CTroopa* troopa = playScene->GetTroopa(playScene);
	bool isHoldTroopa = this->GetIsHoldingTroopa();
	bool isHoldKoopa = this->GetIsHoldingKoopa();
	int aniId = -1;
	if (!isSitting && !isHoldingKoopa && !isHoldingTroopa && isTurn)
	{
		aniId = nx > 0 ? ID_ANI_MARIO_MAX_TURN_RIGHT : ID_ANI_MARIO_MAX_TURN_LEFT;
		return aniId;
	}
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_MAX_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_MAX_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0 && !isHoldKoopa && !isHoldTroopa && vy < 0)
				aniId = ID_ANI_MARIO_MAX_JUMP_WALK_RIGHT;
			else if (nx >= 0 && (isHoldKoopa || isHoldTroopa) && vy < 0)
				aniId = ID_ANI_MARIO_MAX_IDLE_CARRY_RIGHT;
			else if (nx < 0 && (isHoldKoopa || isHoldTroopa) && vy < 0)
				aniId = ID_ANI_MARIO_MAX_IDLE_CARRY_LEFT;
			else if (nx < 0 && !isHoldKoopa && !isHoldTroopa && vy < 0)
				aniId = ID_ANI_MARIO_MAX_JUMP_WALK_LEFT;
			else if (nx >= 0 && !isHoldKoopa && !isHoldTroopa && vy > 0)
				aniId = ID_ANI_MARIO_MAX_FALL_WALK_RIGHT;
			else if (nx >= 0 && (isHoldKoopa || isHoldTroopa) && vy > 0)
				aniId = ID_ANI_MARIO_MAX_IDLE_CARRY_RIGHT;
			else if (nx < 0 && (isHoldKoopa || isHoldTroopa) && vy > 0)
				aniId = ID_ANI_MARIO_MAX_IDLE_CARRY_LEFT;
			else if (nx < 0 && !isHoldKoopa && !isHoldTroopa && vy > 0)
				aniId = ID_ANI_MARIO_MAX_FALL_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_MAX_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_MAX_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (isRight1)
				{
					aniId = nx > 0 ? ID_ANI_MARIO_MAX_KICKING_RIGHT : ID_ANI_MARIO_MAX_KICKING_LEFT;
				}
				else if (nx > 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_MAX_IDLE_RIGHT;
				else if (nx > 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_MAX_IDLE_CARRY_RIGHT;
				else if (nx < 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_MAX_IDLE_LEFT;
				else if (nx < 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_MAX_IDLE_CARRY_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_MAX_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_MAX_RUNNING_RIGHT;
				else if (ax > 0 && ax < MARIO_ACCEL_RUN_X && state == MARIO_STATE_RUNNING_RIGHT)
					aniId = ID_ANI_MARIO_MAX_WALK_TO_RUN_RIGHT;
				else if (ax == 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_MAX_WALKING_RIGHT;
				else if (ax == 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_MAX_WALKING_CARRY_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X && state != MARIO_STATE_RUNNING_RIGHT)
				{
					if ((isHoldKoopa || isHoldTroopa) && !isRight1)
					{
						aniId = ID_ANI_MARIO_MAX_WALKING_CARRY_RIGHT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && !isRight1)
					{
						aniId = ID_ANI_MARIO_MAX_WALKING_RIGHT;
					}
					else if ((isHoldKoopa || isHoldTroopa) && isRight1)
					{
						aniId = ID_ANI_MARIO_MAX_KICKING_RIGHT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && isRight1)
					{
						aniId = ID_ANI_MARIO_MAX_KICKING_RIGHT;
					}

					//if (!isRight1) DebugOut(L"ISRIGHT FALSEEEEEEEEEEEEEEEEEE");
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_MAX_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_MAX_RUNNING_LEFT;
				else if (ax < 0 && ax > -MARIO_ACCEL_RUN_X && state == MARIO_STATE_RUNNING_LEFT)
					aniId = ID_ANI_MARIO_MAX_WALK_TO_RUN_LEFT;
				else if (ax == 0 && !isHoldKoopa && !isHoldTroopa) aniId = ID_ANI_MARIO_MAX_WALKING_LEFT;
				else if (ax == 0 && (isHoldKoopa || isHoldTroopa)) aniId = ID_ANI_MARIO_MAX_WALKING_CARRY_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X && state != MARIO_STATE_RUNNING_LEFT)
				{
					if ((isHoldKoopa || isHoldTroopa) && !isRight1)
					{
						aniId = ID_ANI_MARIO_MAX_WALKING_CARRY_LEFT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && !isRight1)
					{
						aniId = ID_ANI_MARIO_MAX_WALKING_LEFT;
					}
					else if ((isHoldKoopa || isHoldTroopa) && isRight1)
					{
						aniId = ID_ANI_MARIO_MAX_KICKING_LEFT;
					}
					else if (!isHoldKoopa && !isHoldTroopa && isRight1)
					{
						aniId = ID_ANI_MARIO_MAX_KICKING_LEFT;
					}
				}
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_MAX_IDLE_RIGHT;

	return aniId;
}
void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	
	if (level == 3)
	{
		if (this->nx > 0) aniId = ID_ANI_MARIO_TRANSFORM_SMALL_TO_BIG_RIGHT;
		else aniId = ID_ANI_MARIO_TRANSFORM_SMALL_TO_BIG_LEFT;
	}
	else if (level == 4)
	{
		if (this->nx > 0) aniId = ID_ANI_MARIO_TRANSFORM_BIG_TO_SMALL_RIGHT;
		else aniId = ID_ANI_MARIO_TRANSFORM_BIG_TO_SMALL_LEFT;
	}
	else  if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_MAX)
		aniId = GetAniIdMax();
	 
	

	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		if (ax < MARIO_ACCEL_WALK_X) ax = MARIO_ACCEL_WALK_X;
		ax += 0.000005f;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		if (ax > -MARIO_ACCEL_WALK_X) ax = -MARIO_ACCEL_WALK_X;
		ax -= 0.000005f;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;
	case MARIO_STATE_TURN_TAIL:
		if (isSitting) break;
		if (isHoldingKoopa || isHoldingTroopa) break;
		isTurn = true;
		startTurnTail = GetTickCount64();
		break;
	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_TRANSFORM_SMALL_TO_BIG:
		timeStartTransForm = GetTickCount64();
		SetLevel(3);
		vy = 0;
		vx = 0;
		break;
	case MARIO_STATE_TRANSFORM_BIG_TO_SMALL:
		timeStartTransForm = GetTickCount64();
		SetLevel(4);
		vy = 0;
		vx = 0;
		break;

	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if(level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT + 1.5f;
	}
	else if (level == 3 || level == 4)
	{
		left = x - MARIO_BIG_BBOX_WIDTH / 2;
		top = y - MARIO_BIG_BBOX_HEIGHT / 2;
		right = left + MARIO_BIG_BBOX_WIDTH;
		bottom = top + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_MAX)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2 - 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH + 2;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else if (state == MARIO_STATE_TURN_TAIL)
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2 - 10;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH + 10;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2 - 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH + 4;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

