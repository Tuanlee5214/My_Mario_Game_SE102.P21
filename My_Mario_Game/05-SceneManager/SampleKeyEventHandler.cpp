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
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		 mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
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

	CKoopa* koopa = playScene->GetFirstKoopa(playScene);
	float marioX, marioY;
	float koopaX, koopaY;

	if (mario)
		mario->GetPosition(marioX, marioY);

	if (koopa)
		koopa->GetPosition(koopaX, koopaY);

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() != KOOPA_STATE_DIE)
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);

		else if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
			abs(koopaX - marioX) > DISTANCE_TO_PICKUP_KOOPA)
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);

		else if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
			abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA &&
			abs(koopaY - marioY) <= DISTANCE_TO_PICKUP_KOOPA)
		{
			if (mario->GetLevel() > MARIO_LEVEL_SMALL)
			{
				koopa->SetPosition(marioX + 14, marioY + 1.5f);
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				koopa->SetPosition(marioX + 13, marioY - 2);
			}
			koopa->SetBound(300.0f, 700.0f);
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
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() != KOOPA_STATE_DIE)
			mario->SetState(MARIO_STATE_RUNNING_LEFT);

		else if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
			abs(koopaX - marioX) > DISTANCE_TO_PICKUP_KOOPA)
			mario->SetState(MARIO_STATE_RUNNING_LEFT);

		else if (game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
			abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA &&
			abs(koopaY - marioY) <= DISTANCE_TO_PICKUP_KOOPA)
		{
			if (mario->GetLevel() > MARIO_LEVEL_SMALL)
			{
				koopa->SetPosition(marioX - 14, marioY + 1.5f);
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				koopa->SetPosition(marioX - 13, marioY - 2);
			}
			koopa->SetBound(300.0f, 700.0f);
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}


		else if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() != KOOPA_STATE_DIE)
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
	else
		mario->SetState(MARIO_STATE_IDLE);

	if (!mario->GetIsOnPlatform() && game->IsKeyDown(DIK_A) &&
		koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA &&
		abs(koopaY - marioY) <= DISTANCE_TO_PICKUP_KOOPA)
	{
		if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			koopa->SetPosition(marioX + 14, marioY + 1.5f);
		}
		else if (mario->GetLevel() > MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			koopa->SetPosition(marioX - 14, marioY + 1.5f);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() > 0)
		{
			koopa->SetPosition(marioX + 13, marioY - 2);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_SMALL && mario->Get_nx() < 0)
		{
			koopa->SetPosition(marioX - 13, marioY - 2);
		}
		koopa->SetBound(300.0f, 700.0f);
	}

	if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA && (koopaY == marioY + 1.5f || koopaY == marioY - 2)
		&& mario->Get_nx() > 0)
	{
		koopa->SetBound(300.0f, 700.0f);
		koopa->SetState(KOOPA_STATE_DIE_RUNR);
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
	}


	if (!game->IsKeyDown(DIK_A) && koopa && koopa->GetState() == KOOPA_STATE_DIE &&
		abs(koopaX - marioX) <= DISTANCE_TO_PICKUP_KOOPA && (koopaY == marioY + 1.5f || koopaY == marioY - 2)
		&& mario->Get_nx() < 0)
	{
		koopa->SetBound(300.0f, 700.0f);
		koopa->SetState(KOOPA_STATE_DIE_RUNL);
		mario->SetState(MARIO_STATE_WALKING_LEFT);
		mario->SetIsRight1(true);
		mario->SetIsInKickStateNow(GetTickCount64());
	}
}