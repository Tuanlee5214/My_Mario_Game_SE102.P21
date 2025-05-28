#include "GameObject.h"

class CQuestionBlock : public CGameObject
{
protected:
	float ay;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void RenderBoundingBox();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);


	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CQuestionBlock(float x, float y);
	virtual void SetState(int state);
	virtual void SetY(float y);
	virtual void Set_ay(float ay);
	virtual float GetY();
};

