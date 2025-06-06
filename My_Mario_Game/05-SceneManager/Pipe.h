#include "GameObject.h"

#pragma once
class CPipe : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spritePipeID;
	bool isBlocking = true;
	int type; // 0: ground, 1: groungBig

public:
	CPipe(float x, float y,
		float cell_width, float cell_height, int length,
		int spriteGroundID, int type) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spritePipeID = spriteGroundID;
		this->type = type;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	void SetBlocking(bool block) { isBlocking = block; }
	int IsBlocking() override { return isBlocking ? 1 : 0; }

	int GetType() { return type; }
	int IsDirectionColliable(float nx, float ny);

};

typedef CPipe* LPPIPE;

