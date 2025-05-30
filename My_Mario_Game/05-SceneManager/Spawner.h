#include "GameObject.h"
#include "PlayScene.h"

class CSpawner : public CGameObject
{
protected:

	DWORD lastSpawnTime;
	DWORD spawnInterval;
	int typeObjectToSpawn;
	float leftBound, rightBound;
	int count;
	LPGAMEOBJECT currentEnemy;
	float spawnRange = 200.0f;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	virtual void Spawn(CPlayScene* playScene);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& right, float& top, float& bottom);


public:
	CSpawner(float x, float y, int spawnInterval, int type, float leftBound, float rightBound)
	{
		this->x = x;
		this->y = y;
		this->count = 0;
		this->leftBound = leftBound;
		this->rightBound = rightBound;
		this->spawnInterval = spawnInterval;
		this->typeObjectToSpawn = type;
		currentEnemy = NULL;
		lastSpawnTime = GetTickCount64();
	}
};

