#pragma once

#include "Scene.h"

#define DISTANCE_TO_PICKUP_KOOPA 24.0f
#define DISTANCE_TO_PICKUP_TROOPA 24.0f


class CSampleKeyHandler: public CSceneKeyHandler
{
protected:
	bool isPressKeyA;
	bool isPressLeft;
	bool isPressRight;
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CSampleKeyHandler(LPSCENE s) :CSceneKeyHandler(s) {};
};
