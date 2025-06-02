#include "GameObject.h"

class CSwitchPos : public CGameObject
{
	float width;
	float height;
	int type;
public:
	CSwitchPos(float l, float t, float r, float b, int type);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void RenderBoundingBox(void);
	int GetType() { return type; }
	int IsBlocking() { return 0; }
};

