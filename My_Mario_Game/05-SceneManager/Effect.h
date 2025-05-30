#include "GameObject.h"

#define EFFECT_MOVE_SPEED 0.09f
#define EFFECT_FALL_SPEED 0.18f

#define EFFECT_STATE_JUMP_LEFT 100
#define EFFECT_STATE_JUMP_RIGHT 200
#define EFFECT_STATE_FALL_LEFT 300
#define EFFECT_STATE_FALL_RIGHT 400

#define ID_ANI_EFFECT 6335


class CEffect : public CGameObject
{
protected:

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
	CEffect(float x, float y, int type, float topY);
	virtual void SetState(int state);
};

