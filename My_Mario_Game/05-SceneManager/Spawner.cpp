#include "Spawner.h"
#include "PlayScene.h"
#include "Game.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "Goomba.h"
#include "Troopa.h"
#include "ParaTroopa.h"
#include "Koopa.h"

void CSpawner::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());

	CMario* mario = dynamic_cast<CMario*>(playScene->GetPlayer());

	float marioX, marioY;
	mario->GetPosition(marioX, marioY);

	bool isDistanceSpawn1 = abs(this->x - marioX) >= spawnRange;
	bool isDistanceSpawn2 = abs(this->x - marioX) <= 250.0f && abs(this->x - marioX) >= 225.0f;

	bool isTimePassed = (GetTickCount64() - lastSpawnTime) > spawnInterval;

	bool isEnemyDead = (currentEnemy == nullptr || currentEnemy->IsDeleted());

	if (isDistanceSpawn1 && isTimePassed && isEnemyDead && typeObjectToSpawn != OBJECT_TYPE_TROOPA ||
		isDistanceSpawn1 && isTimePassed && isEnemyDead && typeObjectToSpawn != OBJECT_TYPE_PARATROOPA)
	{
		Spawn(playScene);
		lastSpawnTime = GetTickCount64();
	}

	if (isDistanceSpawn2 && isEnemyDead && typeObjectToSpawn == OBJECT_TYPE_TROOPA ||
		isDistanceSpawn2 && isEnemyDead && typeObjectToSpawn == OBJECT_TYPE_PARATROOPA)
	{
		Spawn(playScene);
		lastSpawnTime = GetTickCount64();
	}
}

void CSpawner::Spawn(CPlayScene* playScene)
{
	LPGAMEOBJECT enemy = nullptr;

	switch (typeObjectToSpawn)
	{
	case OBJECT_TYPE_GOOMBA:
		enemy = new CGoomba(x, y, leftBound, rightBound);
		break;
	case OBJECT_TYPE_KOOPA:
		enemy = new CKoopa(x, y, leftBound, rightBound);
		break;
	case OBJECT_TYPE_TROOPA:
		enemy = new CTroopa(x, y, leftBound, rightBound);
		break;
	case OBJECT_TYPE_PARATROOPA:
		enemy = new CParaTroopa(x, y, leftBound, rightBound);
		break;	
	}

	if (enemy)
	{
		currentEnemy = enemy;
		playScene->AddObject(enemy);
	}
}

void CSpawner::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//left = top = right = bottom = 0;
}

void CSpawner::Render()
{

}