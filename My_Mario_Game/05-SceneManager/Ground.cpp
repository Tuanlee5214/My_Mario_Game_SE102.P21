#include "Ground.h"

#include "Sprite.h"
#include "Sprites.h"
#include "debug.h"

#include "Textures.h"
#include "Game.h"

void CGround::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CGround::Render()
{
	if (this->length <= 0) return;
	float xx = x;
	CSprites* s = CSprites::GetInstance();
	for (int i = 0; i < this->length; i++)
	{
		LPSPRITE X = s->Get(this->spriteGroundID);
		if (s->Get(61000) != NULL) {
			X->Draw(xx, y);
			DebugOut(L"[ERROR] 61000 %d found!\n");
		}
		xx += this->cellWidth;
	}
	//xx += this->cellWidth;
	//for (int i = 1; i < this->length - 1; i++)
	//{
	//	s->Get(this->spriteIdMiddle)->Draw(xx, y);
	//	xx += this->cellWidth;
	//}
	//if (length > 1)
		//s->Get(this->spriteIdEnd)->Draw(xx, y);

	//RenderBoundingBox();
}

void CGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}

int CGround::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1 && type == 0) return 1;
	if (nx == 0 && ny == -1 && type == 1) return 1;
	if (nx == 0 && (ny == 1 || ny == -1) && type == 2) return 1;
	if ((nx == 1 || nx == -1) && ny == 0 && type == 0) return 0;
	if ((nx == 1 || nx == -1) && ny == 0 && type == 1) return 1;
	if ((nx == 1 || nx == -1) && ny == 0 && type == 2) return 1;


	else return 0;
}