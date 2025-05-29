#include "GameObject.h"

#define ECOIN_BBOX_HEIGHT 14
#define ECOIN_BBOX_WIDTH 8

#define ECOIN_JUMP_SPEED 0.13f

#define ECOIN_STATE_JUMPED 100
#define ECOIN_STATE_FALLED 200

#define ID_ANI_ECOIN 6555

class ECoin : public CGameObject
{
	float ay;
	float ax;
	float startY;
	float topY;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void RenderBoundingBox();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);


	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	ECoin(float x, float y);
	virtual void SetState(int state);
};

