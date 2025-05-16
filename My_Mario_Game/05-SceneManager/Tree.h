#include "GameObject.h"

class CTree : public CGameObject
{
protected:
	int spriteTreeID;
public:
	CTree(float x, float y, int spriteTreeID) :CGameObject(x, y)
	{
		this->spriteTreeID = spriteTreeID;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b)
	{}
};

typedef CTree* LPTREE;

