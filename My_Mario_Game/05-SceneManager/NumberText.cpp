#include "NumberText.h"

void CNumberText::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(idSprite)->Draw(x, y);
}

void CNumberText::SetIdSprite(int idSp)
{
	this->idSprite = idSp;
}