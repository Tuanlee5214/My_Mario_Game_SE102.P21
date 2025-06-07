#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define NUMBER_TEXT_0 530000
#define NUMBER_TEXT_1 530001
#define NUMBER_TEXT_2 530002
#define NUMBER_TEXT_3 530003
#define NUMBER_TEXT_4 530004
#define NUMBER_TEXT_5 530005
#define NUMBER_TEXT_6 530006
#define NUMBER_TEXT_7 530007
#define NUMBER_TEXT_8 530008
#define NUMBER_TEXT_9 530009


class CNumberText : public CGameObject {
protected:
	int idSprite;
public:
	CNumberText(float x, float y) : CGameObject(x, y) {
		idSprite = NUMBER_TEXT_0;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	int IsBlocking() { return 0; }
	int IsCollidable() { return 0; }
	void SetIdSprite(int idSp);
};