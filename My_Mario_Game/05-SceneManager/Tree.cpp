#include "Tree.h"
#include "Sprite.h"

void CTree::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(this->spriteTreeID)->Draw(x, y);
}
