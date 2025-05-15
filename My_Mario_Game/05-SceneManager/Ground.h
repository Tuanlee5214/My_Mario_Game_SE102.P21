#include "GameObject.h"

#pragma once
class CGround : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteGroundID;

public:
	CGround(float x, float y,
		float cell_width, float cell_height, int length,
		int spriteGroundID) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteGroundID = spriteGroundID;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);

};

typedef CGround* LPGROUND;

