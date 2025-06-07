#include "PanelUI.h"
#include "PlayScene.h"
#include "debug.h"
#include "UIManager.h"
#include "NumberText.h"
#include "AssetIDs.h"

void CPanelUI::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(this->spriteId)->Draw(x, y);
	//RenderBoundingBox();
}

void CPanelUI::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - this->width / 2 + 1;
	t = y - this->height / 2 + 1;
	r = l + this->width;
	b = t + this->height;
}


void CPanelUI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (!isInitUI)
	{
		this->InitUI();
	}
	else {
		UpdateElements(timer, CUIManager::GetInstance()->timer);
		UpdateElements(coins, CUIManager::GetInstance()->coins);
		UpdateElements(points, CUIManager::GetInstance()->points);
		UpdateElements(lifes, CUIManager::GetInstance()->lifes);
	}
	CUIManager::GetInstance()->Update(dt, coObjects);
	CGameObject::Update(dt, coObjects);
}

void CPanelUI::InitUI()
{
	isInitUI = true;
	LPSCENE s = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* p = dynamic_cast<CPlayScene*>(s);
	
}

void CPanelUI::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	if (timer.size() > 0)
	{
		for (int i = 0; i < timer.size(); i++)
		{
			timer[i]->SetPosition(x + i * 8 + 10, y - 4);
		}
	}
	for (int i = 0; i < coins.size(); i++)
	{
		coins[i]->SetPosition(x + i * 8 + 18, y - 12);
	}
	for (int i = 0; i < points.size(); i++)
	{
		points[i]->SetPosition(x + i * 8 - 64, y - 4);
	}
	for (int i = 0; i < lifes.size(); i++)
	{
		lifes[i]->SetPosition(x + i * 8 - 86, y - 4);
	}
	
}

void CPanelUI::UpdateElements(vector<LPGAMEOBJECT>& elements, DWORD value)
{
	std::string valueStr = std::to_string(value);
	while (valueStr.length() < elements.size())
	{
		valueStr = "0" + valueStr;
	}

	int index = elements.size() - 1;
	for (int i = valueStr.length() - 1; i >= 0 && index >= 0; --i, --index)
	{
		int digit = valueStr[i] - '0';
		dynamic_cast<CNumberText*>(elements[index])->SetIdSprite(NUMBER_TEXT_0 + digit);
	}
}