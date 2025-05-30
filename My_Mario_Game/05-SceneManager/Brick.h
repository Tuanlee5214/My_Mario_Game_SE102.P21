#include "GameObject.h"

#define BRICK_BBOX_HEIGHT 16
#define BRICK_BBOX_WIDTH 16

#define BRICK_JUMP_SPEED 0.16f

#define BRICK_STATE_INI 100
#define BRICK_STATE_JUMPED 200
#define BRICK_STATE_FALLED 300
#define BRICK_STATE_USED 400

#define ID_ANI_BRICK_BEFORE_USE 6334

class CQuestionBlock : public CGameObject
{
protected:
	float ay;
	float ax;
	int type;
	float startY;
	float startX;
	float topY;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void RenderBoundingBox();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }

	virtual void OnNoCollision(DWORD dt);


	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CQuestionBlock(float x, float y, int type);
	virtual void SetState(int state);
	//virtual void SetY(float y);
	//virtual void Set_ay(float ay);
	//virtual float GetY();
};
