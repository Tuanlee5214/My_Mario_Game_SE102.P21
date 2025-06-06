#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.3f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_IN_KICKSTATE_TIME 200
#define MARIO_TRANSFORM_SMALL_TO_BIG_TIMEOUT 900
#define MARIO_TRANSFORM_BIG_TO_SMALL_TIMEOUT 900
#define MARIO_TURN_TAIL_TIMEOUT 300


#define MARIO_JUMP_SPEED_Y		0.350f
#define MARIO_JUMP_RUN_SPEED_Y	0.443f

#define MARIO_GRAVITY			0.0008f

#define MARIO_JUMP_DEFLECT_SPEED  0.253f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_TRANSFORM_SMALL_TO_BIG	700
#define MARIO_STATE_TRANSFORM_BIG_TO_SMALL	800

#define MARIO_STATE_TURN_TAIL 900
#define MARIO_STATE_FALL_SLOW 901
#define MARIO_STATE_JUMP_HIGH 902
#define MARIO_STATE_GO_IN_PIPE_DOWN 903
#define MARIO_STATE_GO_IN_PIPE_UP 904


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401
#define ID_ANI_MARIO_IDLE_CARRY_LEFT 402
#define ID_ANI_MARIO_IDLE_CARRY_RIGHT 403

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501
#define ID_ANI_MARIO_WALKING_CARRY_LEFT 502
#define ID_ANI_MARIO_WALKING_CARRY_RIGHT 503
#define ID_ANI_MARIO_WALK_TO_RUN_LEFT 509
#define ID_ANI_MARIO_WALK_TO_RUN_RIGHT 508
#define ID_ANI_MARIO_KICKING_LEFT 504
#define ID_ANI_MARIO_KICKING_RIGHT 505

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701
#define ID_ANI_MARIO_FALL_WALK_LEFT 702
#define ID_ANI_MARIO_FALL_WALK_RIGHT 703

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801
#define ID_ANI_MARIO_JUMP_RUN_HIGH_R 802
#define ID_ANI_MARIO_JUMP_RUN_HIGH_L 803

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102
#define ID_ANI_MARIO_SMALL_IDLE_CARRY_LEFT 1104
#define ID_ANI_MARIO_SMALL_IDLE_CARRY_RIGHT 1106
#define ID_ANI_MARIO_TRANSFORM_SMALL_TO_BIG_LEFT 1108
#define ID_ANI_MARIO_TRANSFORM_SMALL_TO_BIG_RIGHT 1110

#define ID_ANI_MARIO_TRANSFORM_BIG_TO_SMALL_LEFT 1112
#define ID_ANI_MARIO_TRANSFORM_BIG_TO_SMALL_RIGHT 1114

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201
#define ID_ANI_MARIO_SMALL_WALK_TO_RUN_LEFT 1209
#define ID_ANI_MARIO_SMALL_WALK_TO_RUN_RIGHT 1208
#define ID_ANI_MARIO_SMALL_WALKING_CARRY_LEFT 1202
#define ID_ANI_MARIO_SMALL_WALKING_CARRY_RIGHT 1203
#define ID_ANI_MARIO_SMALL_KICKING_LEFT 1204
#define ID_ANI_MARIO_SMALL_KICKING_RIGHT 1205

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

//MAX MARIO
#define ID_ANI_MARIO_MAX_IDLE_RIGHT 100
#define ID_ANI_MARIO_MAX_IDLE_LEFT 101
#define ID_ANI_MARIO_MAX_IDLE_CARRY_LEFT 102
#define ID_ANI_MARIO_MAX_IDLE_CARRY_RIGHT 103

#define ID_ANI_MARIO_MAX_WALKING_RIGHT 123
#define ID_ANI_MARIO_MAX_WALKING_LEFT 104
#define ID_ANI_MARIO_MAX_WALKING_CARRY_LEFT 105
#define ID_ANI_MARIO_MAX_WALKING_CARRY_RIGHT 106
#define ID_ANI_MARIO_MAX_WALK_TO_RUN_LEFT 107
#define ID_ANI_MARIO_MAX_WALK_TO_RUN_RIGHT 108
#define ID_ANI_MARIO_MAX_KICKING_LEFT 109
#define ID_ANI_MARIO_MAX_KICKING_RIGHT 110

#define ID_ANI_MARIO_MAX_RUNNING_RIGHT 111
#define ID_ANI_MARIO_MAX_RUNNING_LEFT 112

#define ID_ANI_MARIO_MAX_JUMP_WALK_RIGHT 113
#define ID_ANI_MARIO_MAX_JUMP_WALK_LEFT 114
#define ID_ANI_MARIO_MAX_FALL_WALK_LEFT 115
#define ID_ANI_MARIO_MAX_FALL_WALK_RIGHT 116
#define ID_ANI_MARIO_MAX_FALL_SLOW_LEFT 126
#define ID_ANI_MARIO_MAX_FALL_SLOW_RIGHT 127
#define ID_ANI_MARIO_MAX_IN_PIPE 128

#define ID_ANI_MARIO_MAX_JUMP_RUN_RIGHT 117
#define ID_ANI_MARIO_MAX_JUMP_RUN_LEFT 118

#define ID_ANI_MARIO_MAX_SIT_RIGHT 119
#define ID_ANI_MARIO_MAX_SIT_LEFT 120

#define ID_ANI_MARIO_MAX_BRACE_RIGHT 121
#define ID_ANI_MARIO_MAX_BRACE_LEFT 122

#define ID_ANI_MARIO_MAX_TURN_LEFT 124
#define ID_ANI_MARIO_MAX_TURN_RIGHT 125


#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_MAX		5

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 900

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	ULONGLONG timeStartTransForm;
	BOOLEAN isOnPlatform;
	int coin;
	bool isRight1 = false;
	bool isHoldingKoopa = false;
	bool isHoldingTroopa = false;
	bool isTurn = false;
	bool isFlyHigh;
	bool isReadyToGoDown = false;
	bool isGoDownPipe = false;
	ULONGLONG timeStartFlyHigh;
	ULONGLONG startTurnTail;
	ULONGLONG isInKickStateNow;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithRedGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithRedPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithGreenPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithParinha(LPCOLLISIONEVENT e);
	void OnCollisionWithTroopa(LPCOLLISIONEVENT e);
	void OnCollisionWithParaTroopa(LPCOLLISIONEVENT e);
	void OnCollisionWithMushRoom(LPCOLLISIONEVENT e);
	void OnCollisionWihtLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithSwitchPos(LPCOLLISIONEVENT e);
	void OnCollisiosnWithPipe(LPCOLLISIONEVENT e);



public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		timeStartTransForm = -1;
		isOnPlatform = false;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetIsGoDownPipe(bool a) { isGoDownPipe = a; }
	int GetLevel()
	{
		return level;
	}

	void SetIsReadyToGoDown(bool a) { isReadyToGoDown = a; }
	bool GetIsReadyToGoDown() { return isReadyToGoDown; }
	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	void SetIsInKickStateNow(ULONGLONG a)
	{
		this->isInKickStateNow = a;
	}

	bool GetIsOnPlatform()
	{
		return isOnPlatform;
	}

	void SetIsFlyHigh(bool a) { isFlyHigh = a; }
	bool GetIsFlyHigh() { return isFlyHigh; }
	void SetTimeStartFly(ULONGLONG a) { timeStartFlyHigh = a; }
	void SetIsHoldingKoopa(float a)
	{
		this->isHoldingKoopa = a;
	}

	float Get_vy() { return vy; }

	bool GetIsHoldingKoopa()
	{
		return this->isHoldingKoopa;
	}

	void SetIsHoldingTroopa(float a)	 
	{
		this->isHoldingTroopa = a;
	}

	bool GetIsHoldingTroopa()
	{
		return this->isHoldingTroopa;
	}

	void Set_vy(float vy)
	{
		this->vy = vy;
	}

	void Set_ax(float ax)
	{
		this->ax = ax;
	}
	
	void Set_vx(float vx)
	{
		this->vx -= vx;
	}

	float Get_vx() { return this->vx; }
	float Get_ax() { return this->ax; }

	bool GetIsTurn() { return isTurn; }

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdMax();

	void SetIsRight1(bool a)
	{
		this->isRight1 = a;
	}

	bool GetIsRight1()
	{
		return isRight1;
	}

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};