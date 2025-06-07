#include "GameObject.h"

#define POINT_JUMP_SPEED 0.075f

#define POINT_STATE_JUMP 100


#define ID_ANI_POINT_TYPE1 6777
#define ID_ANI_POINT_TYPE2 6778
#define ID_ANI_POINT_TYPE3 6779
#define ID_ANI_1UP 6776
#define ID_ANI_MUSH_EFFEC 6775


class CPoint : public CGameObject
{
	int type;
	float topY;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }

	virtual void OnNoCollision(DWORD dt);


	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPoint(float x, float y, int type, float topY);
	virtual void SetState(int state);
};

