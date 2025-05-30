#include "GameObject.h"

#define LEAF_BBOX_HEIGHT 16
#define LEAF_BBOX_WIDTH 16

#define LEAF_FALL_SPEED 0.018f
#define LEAF_RAISE_SPEED 0.15f
#define LEAF_MOVE_SPEED 0.065f

#define LEAF_STATE_RAISE 200
#define LEAF_STATE_FALL_LEFT 300
#define LEAF_STATE_FALL_RIGHT 400

#define ID_ANI_LEAF_LEFT 6667
#define ID_ANI_LEAF_RIGHT 6668

class CLeaf : public CGameObject
{
	float topY;
	float ay;
	float ax;
	float leftBound;
	float rightBound;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void RenderBoundingBox();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CLeaf(float x, float y, float leftBound, float rightBound);
	virtual void SetState(int state);
};

