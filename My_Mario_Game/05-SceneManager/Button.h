#ifndef BUTTON_H
#define BUTTON_H

#include "GameObject.h"

#define BUTTON_JUMP_SPEED 0.4f
#define BUTTON_BBOX_HEIGHT 16
#define BUTTON_BBOX_HEIGHT_AFTER_USE 8
#define BUTTON_BBOX_WIDTH 16

#define BUTTON_STATE_JUMP 100
#define BUTTON_STATE_TOP 200
#define BUTTON_STATE_AFTER_USE 300

#define ID_ANI_BUTTON_BEFORE_USE 6435
#define ID_ANI_BUTTON_AFTER_USE 6436


class CButton : public CGameObject
{
protected:
	float ay;
	float ax;
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
	CButton(float x, float y);
	virtual void SetState(int state);

};

#endif // BUTTON_H