#include "GameObject.h"

#pragma once
class CBox : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteBoxID;
	int type; // 0: ground, 1: groungBig

public:
	CBox(float x, float y,
		float cell_width, float cell_height, int length,
		int spriteGroundID, int type) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteBoxID = spriteGroundID;
		this->type = type;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);

};

typedef CBox* LPBOX;

