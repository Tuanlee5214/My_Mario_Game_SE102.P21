#include "GameObject.h"

class CCloud : public CGameObject
{
protected:
	int spriteCloudID;
public:
	CCloud(float x, float y, int spriteCloudID) :CGameObject(x, y)
	{
		this->spriteCloudID = spriteCloudID;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b)
	{
	}
};

typedef CCloud* LPCLOUD;

