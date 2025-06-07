#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

#include "Koopa.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	LPGAME game = CGame::GetInstance();
	CPlayScene* playScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	CKoopa* koopa = playScene->GetFirstKoopa(playScene);
	float marioX, marioY;
	float koopaX, koopaY;

	if (mario)
		mario->GetPosition(marioX, marioY);

	if (koopa)
		koopa->GetPosition(koopaX, koopaY);


	switch (KeyCode)
	{
	case DIK_DOWN:
		if (mario->GetIsReadyToGoDown())
		{
			mario->SetState(MARIO_STATE_GO_IN_PIPE_DOWN);
			mario->SetIsGoDownPipe(true);
		}
		else mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->GetState() == MARIO_STATE_FALL_RUN)
		{
			mario->SetState(MARIO_STATE_FALL_SLOW);
			mario->SetIsFlyHigh(false);
			mario->SetIsFallRunState(false);
		}
		if (isPressKeyA && mario->Get_vy() > 0)
		{
			if (isPressLeft || isPressRight)
			{
				mario->SetState(MARIO_STATE_FALL_SLOW);
				mario->SetIsFlyHigh(false);
				mario->SetIsFallRunState(false);
			}
			else
			{
				mario->SetState(MARIO_STATE_FALL_SLOW);
				mario->SetIsFlyHigh(false);
				mario->SetIsFallRunState(false);
			}
		}

		if ((isPressLeft || isPressRight) && mario->Get_vy() > 0)
		{
			mario->SetState(MARIO_STATE_FALL_SLOW);
			mario->SetIsFlyHigh(false);
			mario->SetIsFallRunState(false);
		}
		if (mario->GetLevel() == MARIO_LEVEL_MAX && !mario->GetIsOnPlatform() &&
			mario->Get_vy() > 0)
		{
			mario->SetState(MARIO_STATE_FALL_SLOW);
			mario->SetIsFlyHigh(false);
			mario->SetIsFallRunState(false);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_MAX	&&
				(mario->GetState() == MARIO_STATE_RUNNING_LEFT || 
				 mario->GetState() == MARIO_STATE_RUNNING_RIGHT) &&
				 mario->Get_vy() < 0)
		{
			mario->SetIsFlyHigh(true);
			mario->SetTimeStartFly(GetTickCount64());
		}
		else if (mario->GetLevel() == MARIO_LEVEL_MAX &&
			mario->GetIsFlyHigh() && mario->Get_vy() < 0)
		{
			mario->SetIsFlyHigh(true);
			mario->SetTimeStartFly(GetTickCount64());
		}
		else mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_MAX && !mario->GetIsTurn());
		{
			mario->SetState(MARIO_STATE_TURN_TAIL);
		}
		break;

	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_MAX);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	LPGAME game = CGame::GetInstance();
	CPlayScene* playScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	CKoopa* koopa = playScene->GetFirstKoopa(playScene);
	float marioX, marioY;
	float koopaX, koopaY;

	if (mario)
		mario->GetPosition(marioX, marioY);

	if (koopa)
		koopa->GetPosition(koopaX, koopaY);

	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CPlayScene* playScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	CKoopa* koopa = playScene->GetNearestKoopa(playScene);
	CTroopa* troopa = playScene->GetTroopa(playScene);

	float marioX, marioY;
	float koopaX, koopaY;
	float troopaX, troopaY;

	if (mario)
		mario->GetPosition(marioX, marioY);

	if (koopa)
		koopa->GetPosition(koopaX, koopaY);

	if (troopa)
		troopa->GetPosition(troopaX, troopaY);

	if (game->IsKeyDown(DIK_RIGHT))
	{
		isPressRight = true;
		if (koopa)
		{
			if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() != KOOPA_STATE_DIE && !mario->GetIsHoldingTroopa())
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);

			else if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
				abs(koopaX - marioX) > DISTANCE_TO_PICKUP_KOOPA && !mario->GetIsHoldingTroopa())
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);

			else if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
				abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA &&
				abs(koopaY - marioY) <= DISTANCE_TO_PICKUP_KOOPA &&
				(GetTickCount64() - koopa->GetKoopaDieStart() > KOOPA_TIME_TO_PICKUP))
			{
				if (mario->GetLevel() > MARIO_LEVEL_SMALL)
				{
					koopa->SetPosition(marioX + 14, marioY + 1.5f);
					mario->SetIsHoldingKoopa(true);
				}
				else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				{
					koopa->SetPosition(marioX + 13, marioY - 2);
					mario->SetIsHoldingKoopa(true);
				}
				koopa->SetBound(-10.0f, 10000.0f);
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}

			else if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() != KOOPA_STATE_DIE)
				mario->SetState(MARIO_STATE_WALKING_RIGHT);

			else if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
				abs(koopaX - marioX) > DISTANCE_TO_PICKUP_KOOPA)
				mario->SetState(MARIO_STATE_WALKING_RIGHT);

			else if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
				abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA)
				mario->SetState(MARIO_STATE_WALKING_RIGHT);



			else if (game->IsKeyDown(DIK_A) && !koopa)
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);

			else if (!game->IsKeyDown(DIK_A) && !koopa)
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
			//
		}
		
		if (troopa)
		{
		 if (game->IsKeyDown(DIK_A) && troopa && troopa->GetState() != TROOPA_STATE_DIE && !mario->GetIsHoldingKoopa())
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);

		//1
		else if (game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
			abs(troopaX - marioX) > DISTANCE_TO_PICKUP_TROOPA && !mario->GetIsHoldingKoopa())
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);

		else if (game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
			abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA &&
			abs(troopaY - marioY) <= DISTANCE_TO_PICKUP_TROOPA &&
			(GetTickCount64() - troopa->GetTroopaDieStart() > TROOPA_TIME_TO_PICKUP))
		{
			if (mario->GetLevel() > MARIO_LEVEL_SMALL)
			{
				troopa->SetPosition(marioX + 14, marioY + 1.5f);
				mario->SetIsHoldingTroopa(true);
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				troopa->SetPosition(marioX + 13, marioY - 2);
				mario->SetIsHoldingTroopa(true);
			}
			troopa->SetBound(-10.0f, 10000.0f);
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}

		else if (!game->IsKeyDown(DIK_A) && troopa && troopa->GetState() != TROOPA_STATE_DIE)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);

		else if (!game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
			abs(troopaX - marioX) > DISTANCE_TO_PICKUP_TROOPA)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);

		else if (!game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
			abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);

		else if (game->IsKeyDown(DIK_A) && !troopa)
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);

		else if (!game->IsKeyDown(DIK_A) && !troopa)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}

		if (!troopa && !koopa)
		{
			if (game->IsKeyDown(DIK_A)) mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			else mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		isPressLeft = true;
		if (koopa)
		{
			if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() != KOOPA_STATE_DIE && !mario->GetIsHoldingTroopa())
				mario->SetState(MARIO_STATE_RUNNING_LEFT);

			else if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
				abs(koopaX - marioX) > DISTANCE_TO_PICKUP_KOOPA && !mario->GetIsHoldingTroopa())
				mario->SetState(MARIO_STATE_RUNNING_LEFT);

			else if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
				abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA &&
				abs(koopaY - marioY) <= DISTANCE_TO_PICKUP_KOOPA &&
				(GetTickCount64() - koopa->GetKoopaDieStart() > KOOPA_TIME_TO_PICKUP))
			{
				if (mario->GetLevel() > MARIO_LEVEL_SMALL)
				{
					koopa->SetPosition(marioX - 14, marioY + 1.5f);
					mario->SetIsHoldingKoopa(true);
				}
				else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				{
					koopa->SetPosition(marioX - 13, marioY - 2);
					mario->SetIsHoldingKoopa(true);
				}
				koopa->SetBound(-10.0f, 10000.0f);
				mario->SetState(MARIO_STATE_WALKING_LEFT);
			}


			else if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() != KOOPA_STATE_DIE && !mario->GetIsHoldingKoopa())
				mario->SetState(MARIO_STATE_WALKING_LEFT);

			else if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
				abs(koopaX - marioX) > DISTANCE_TO_PICKUP_KOOPA)
				mario->SetState(MARIO_STATE_WALKING_LEFT);

			else if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
				abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA)
				mario->SetState(MARIO_STATE_WALKING_LEFT);



			else if (game->IsKeyDown(DIK_A) && !koopa)
				mario->SetState(MARIO_STATE_RUNNING_LEFT);

			else if (!game->IsKeyDown(DIK_A) && !koopa)
				mario->SetState(MARIO_STATE_WALKING_LEFT);
		}

		if (troopa)
		{
			if (game->IsKeyDown(DIK_A) && troopa && troopa->GetState() != TROOPA_STATE_DIE && !mario->GetIsHoldingKoopa())
				mario->SetState(MARIO_STATE_RUNNING_LEFT);

			else if (game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
				abs(troopaX - marioX) > DISTANCE_TO_PICKUP_TROOPA && !mario->GetIsHoldingKoopa())
				mario->SetState(MARIO_STATE_RUNNING_LEFT);

			else if (game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
				abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA &&
				abs(troopaY - marioY) <= DISTANCE_TO_PICKUP_TROOPA &&
				(GetTickCount64() - troopa->GetTroopaDieStart() > TROOPA_TIME_TO_PICKUP))
			{
				if (mario->GetLevel() > MARIO_LEVEL_SMALL)
				{
					troopa->SetPosition(marioX - 14, marioY + 1.5f);
					mario->SetIsHoldingTroopa(true);
				}
				else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				{
					troopa->SetPosition(marioX - 13, marioY - 2);
					mario->SetIsHoldingTroopa(true);
				}
				troopa->SetBound(-10.0f, 10000.0f);
				mario->SetState(MARIO_STATE_WALKING_LEFT);
			}

			else if (!game->IsKeyDown(DIK_A) && troopa && troopa->GetState() != TROOPA_STATE_DIE)
				mario->SetState(MARIO_STATE_WALKING_LEFT);

			else if (!game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
				abs(troopaX - marioX) > DISTANCE_TO_PICKUP_TROOPA)
				mario->SetState(MARIO_STATE_WALKING_LEFT);

			else if (!game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
				abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA)
				mario->SetState(MARIO_STATE_WALKING_LEFT);

			else if (game->IsKeyDown(DIK_A) && !troopa)
				mario->SetState(MARIO_STATE_RUNNING_LEFT);

			else if (!game->IsKeyDown(DIK_A) && !troopa)
				mario->SetState(MARIO_STATE_WALKING_LEFT);

		}

		if (!troopa && !koopa)
		{
			if (game->IsKeyDown(DIK_A)) mario->SetState(MARIO_STATE_RUNNING_LEFT);
			else mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
			//troopa
		 
	}
	else
	{
		mario->Set_ax(0);
	}

	if (!mario->GetIsOnPlatform() && game->IsKeyDown(DIK_A) &&
		koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA &&
		abs(koopaY - marioY) <= DISTANCE_TO_PICKUP_KOOPA &&
		(GetTickCount64() - koopa->GetKoopaDieStart() > KOOPA_TIME_TO_PICKUP))
	{
		if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			koopa->SetPosition(marioX + 14, marioY + 1.5f);
			mario->SetIsHoldingKoopa(true);
		}
		else if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			koopa->SetPosition(marioX - 14, marioY + 1.5f);
			mario->SetIsHoldingKoopa(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			koopa->SetPosition(marioX + 13, marioY - 2);
			mario->SetIsHoldingKoopa(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			koopa->SetPosition(marioX - 13, marioY - 2);
			mario->SetIsHoldingKoopa(true);
		}
		koopa->SetBound(-10.0f, 10000.0f);
	}
	else if (mario->GetIsOnPlatform() && game->IsKeyDown(DIK_A) &&
		koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA &&
		abs(koopaY - marioY) <= DISTANCE_TO_PICKUP_KOOPA &&
		(GetTickCount64() - koopa->GetKoopaDieStart() > KOOPA_TIME_TO_PICKUP))
	{
		if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			koopa->SetPosition(marioX + 14, marioY + 1.5f);
			mario->SetIsHoldingKoopa(true);
		}
		else if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			koopa->SetPosition(marioX - 14, marioY + 1.5f);
			mario->SetIsHoldingKoopa(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			koopa->SetPosition(marioX + 13, marioY - 2);
			mario->SetIsHoldingKoopa(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			koopa->SetPosition(marioX - 13, marioY - 2);
			mario->SetIsHoldingKoopa(true);
		}
		koopa->SetBound(-10.0f, 10000.0f);
	}

	if (mario->GetIsOnPlatform() && !game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA && mario->GetIsHoldingKoopa()
		&& mario->Get_nx() > 0)
	{
		koopa->SetBound(-10.0f, 10000.0f);
		koopa->SetState(KOOPA_STATE_DIE_RUNR);
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
		mario->SetIsHoldingKoopa(false);
	}
	else if (mario->GetIsOnPlatform() && !game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA && mario->GetIsHoldingKoopa()
		&& mario->Get_nx() < 0)
	{
		koopa->SetBound(-10.0f, 1000.0f);
		koopa->SetState(KOOPA_STATE_DIE_RUNL);
		mario->SetState(MARIO_STATE_WALKING_LEFT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
		mario->SetIsHoldingKoopa(false);
	}
	else if (!mario->GetIsOnPlatform() && !game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA && mario->GetIsHoldingKoopa()
		&& mario->Get_nx() > 0)
	{
		koopa->SetBound(-10.0f, 10000.0f);
		koopa->SetState(KOOPA_STATE_DIE_RUNR);
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
		mario->SetIsHoldingKoopa(false);
	}
	else if (!mario->GetIsOnPlatform() && !game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA && mario->GetIsHoldingKoopa()
		&& mario->Get_nx() < 0)
	{
		koopa->SetBound(-10.0f, 10000.0f);
		koopa->SetState(KOOPA_STATE_DIE_RUNL);
		mario->SetState(MARIO_STATE_WALKING_LEFT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
		mario->SetIsHoldingKoopa(false);
	}


	//troopa
	if (!mario->GetIsOnPlatform() && game->IsKeyDown(DIK_A) &&
		troopa && troopa->GetState() == TROOPA_STATE_DIE &&
		abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA &&
		abs(troopaY - marioY) <= DISTANCE_TO_PICKUP_TROOPA &&
		(GetTickCount64() - troopa->GetTroopaDieStart() > TROOPA_TIME_TO_PICKUP))
	{
		if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			troopa->SetPosition(marioX + 14, marioY + 1.5f);
			mario->SetIsHoldingTroopa(true);
		}
		else if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			troopa->SetPosition(marioX - 14, marioY + 1.5f);
			mario->SetIsHoldingTroopa(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			troopa->SetPosition(marioX + 13, marioY - 2);
			mario->SetIsHoldingTroopa(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			troopa->SetPosition(marioX - 13, marioY - 2);
			mario->SetIsHoldingTroopa(true);
		}
		troopa->SetBound(-10.0f, 10000.0f);
	}
	else if (mario->GetIsOnPlatform() && game->IsKeyDown(DIK_A) &&
		troopa && troopa->GetState() == TROOPA_STATE_DIE &&
		abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA &&
		abs(troopaY - marioY) <= DISTANCE_TO_PICKUP_TROOPA &&
		(GetTickCount64() - troopa->GetTroopaDieStart() > TROOPA_TIME_TO_PICKUP))
	{
		if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			troopa->SetPosition(marioX + 14, marioY + 1.5f);
			mario->SetIsHoldingTroopa(true);
		}
		else if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			troopa->SetPosition(marioX - 14, marioY + 1.5f);
			mario->SetIsHoldingTroopa(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			troopa->SetPosition(marioX + 13, marioY - 2);
			mario->SetIsHoldingTroopa(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			troopa->SetPosition(marioX - 13, marioY - 2);
			mario->SetIsHoldingTroopa(true);
		}
		troopa->SetBound(-10.0f, 10000.0f);
	}

	if (mario->GetIsOnPlatform() && !game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
		abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA && mario->GetIsHoldingTroopa()
		&& mario->Get_nx() > 0)
	{
		troopa->SetBound(-10.0f, 10000.0f);
		troopa->SetState(TROOPA_STATE_DIE_RUNR);
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
		mario->SetIsHoldingTroopa(false);
	}

	else if (mario->GetIsOnPlatform() && !game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
		abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA && mario->GetIsHoldingTroopa()
		&& mario->Get_nx() < 0)
	{
		troopa->SetBound(-10.0f, 1000.0f);
		troopa->SetState(TROOPA_STATE_DIE_RUNL);
		mario->SetState(MARIO_STATE_WALKING_LEFT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
		mario->SetIsHoldingTroopa(false);
	}

	else if (!mario->GetIsOnPlatform() && !game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
		abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA && mario->GetIsHoldingTroopa()
		&& mario->Get_nx() > 0)
	{
		troopa->SetBound(-10.0f, 10000.0f);
		troopa->SetState(TROOPA_STATE_DIE_RUNR);
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
		mario->SetIsHoldingTroopa(false);
	}

	else if (!mario->GetIsOnPlatform() && !game->IsKeyDown(DIK_A) && troopa && troopa->GetState() == TROOPA_STATE_DIE &&
		abs(troopaX - marioX) <= DISTANCE_TO_PICKUP_TROOPA && mario->GetIsHoldingTroopa()
		&& mario->Get_nx() < 0)
	{
		troopa->SetBound(-10.0f, 10000.0f);
		troopa->SetState(TROOPA_STATE_DIE_RUNL);
		mario->SetState(MARIO_STATE_WALKING_LEFT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
		mario->SetIsHoldingTroopa(false);
	}

	if (game->IsKeyDown(DIK_A)) isPressKeyA = true;
	else if (!game->IsKeyDown(DIK_A)) isPressKeyA = false;

	if (!game->IsKeyDown(DIK_LEFT)) isPressLeft = false;
	if (!game->IsKeyDown(DIK_RIGHT)) isPressRight = false;
}
