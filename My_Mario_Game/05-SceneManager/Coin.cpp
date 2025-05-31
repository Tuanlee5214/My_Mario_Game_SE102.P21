#include "Coin.h"
#include "ECoin.h"
#include "GameObject.h"

void CCoin::Render()
{

	int aniId;
	if (type == 1)
	{
		aniId = ID_ANI_COIN;
	}
	else aniId = ID_ANI_COIN1;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

CCoin::CCoin(float x, float y, int type) : CGameObject(x, y)
{
	this->type = type;
}
void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}