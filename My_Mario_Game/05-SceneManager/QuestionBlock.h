#include "GameObject.h"



#define QUESBLOCK_BBOX_HEIGHT 16
#define QUESBLOCK_BBOX_WIDTH 16

#define QUESBLOCK_STATE_INI 100
#define QUESBLOCK_STATE_JUMPED 200
#define QUESBLOCK_STATE_USED 300

#define ID_ANI_QUESBLOCK_BEFORE_USE 6111
#define ID_ANI_QUESBLOCK_AFTER_USE	6112
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

