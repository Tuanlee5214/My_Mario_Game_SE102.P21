#include "GameObject.h"
#include "PlayScene.h"

#define top_Y 143
#define speed_Y 0.030

#define PARINHA_BBOX_HEIGHT 23
#define PARINHA_BBOX_WIDTH 16

#define PARINHA_STATE_IDLE 100
#define PARINHA_STATE_RISE 200
#define PARINHA_STATE_TOP 300
#define PARINHA_STATE_HIDE 400

#define ID_ANI_PARINHA_IDLE 5310
#define ID_ANI_PARINHA_ATTACK 5320

class CParinha: public CGameObject
{
protected:

	ULONGLONG startTime = 0;
	float start_Y;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CParinha(float x, float y);
	virtual void SetState(int state);
};

