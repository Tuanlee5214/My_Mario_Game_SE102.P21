#include "Cloud.h"
#include "Sprite.h"

void CCloud::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(this->spriteCloudID)->Draw(x, y);
}
