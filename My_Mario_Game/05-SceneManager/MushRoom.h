#include "GameObject.h"

#define MUSHROOM_BBOX_HEIGHT 16
#define MUSHROOM_BBOX_WIDTH 16

#define MUSHROOM_RISE_SPEED 0.03f

#define MUSHROOM_STATE_IDLE 100
#define MUSHROOM_STATE_RAISE 200
#define MUSHROOM_STATE_RUNL 300
#define MUSHROOM_STATE_RUNR 400

#define ID_ANI_MUSHROOM 6666

class CMushRoom : public CGameObject
{
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
	CMushRoom(float x, float y);
	virtual void SetState(int state);
};

