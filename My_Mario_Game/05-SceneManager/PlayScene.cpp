#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Ground.h"
#include "Tree.h"
#include "Cloud.h"
#include "Box.h"
#include "Pipe.h"
#include "debug.h"
#include "Koopa.h"
#include "GameData.h"
#include "RedGoomba.h"
#include "RedPlant.h"
#include "GreenPlant.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}
	
	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
	if (CSprites::GetInstance()->Get(ID) == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", ID);
		return;
	}
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_KOOPA: {
		float leftBound = (float)atof(tokens[3].c_str());
		float rightBound = (float)atof(tokens[4].c_str());
		obj = new CKoopa(x, y, leftBound, rightBound); break;
	}
	case OBJECT_TYPE_GOOMBA: {
		float leftBound = (float)atof(tokens[3].c_str());
		float rightBound = (float)atof(tokens[4].c_str());
		obj = new CGoomba(x, y, leftBound, rightBound); break;
	}
	case OBJECT_TYPE_REDGOOMBA: {
		float leftBound = (float)atof(tokens[3].c_str());
		float rightBound = (float)atof(tokens[4].c_str());
		obj = new CRedGoomba(x, y, leftBound, rightBound); break;
	}
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_GROUND:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_id = atoi(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		obj = new CGround(
			x, y,
			cell_width, cell_height, length,
			sprite_id, type
		);
		break;
	}
	case OBJECT_TYPE_TREE:
	{
		int spriteTreeID = atoi(tokens[3].c_str());
		obj = new CTree(x, y, spriteTreeID);
		break;
	}
	case OBJECT_TYPE_REDPLANT:
		obj = new CRedPlant(x, y);
		break;

	case OBJECT_TYPE_GREENPLANT:
		obj = new CGreenPlant(x, y);
		break;
	case OBJECT_TYPE_CLOUD:
	{
		int spriteCloudID = atoi(tokens[3].c_str());
		obj = new CCloud(x, y, spriteCloudID);
		break;
	}
	case OBJECT_TYPE_BOX:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_id = atoi(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		obj = new CBox(
			x, y,
			cell_width, cell_height, length,
			sprite_id, type
		);
		break;
	}

	case OBJECT_TYPE_PIPE:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_id = atoi(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		obj = new CPipe(
			x, y,
			cell_width, cell_height, length,
			sprite_id, type
		);
		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	int objectIndex = objects.size();
	int sceneID = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (GameData::GetInstance()->IsObjectDeleted(sceneID, objectIndex)) {
		objects.push_back(NULL);
		return;
	}
	objects.push_back(obj);

}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::AddObject(LPGAMEOBJECT object) {
	this->objects.push_back(object);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i] != NULL) {
		    coObjects.push_back(objects[i]);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i] != NULL) {
			objects[i]->Update(dt, &coObjects);
		}
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	float y1 = cy;

	CGame* game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= (game->GetBackBufferHeight() / 2);

	if (cx < 0) cx = 0;
	if (game->GetCurrentScene()->GetId() == 5 || game->GetCurrentScene()->GetId() == 1)
	{
		if (cx > 2463) cx = 2463;
	}
	else if (game->GetCurrentScene()->GetId() == 2)
	{
		if (cx > 127) cx = 127;
	}
	DebugOut(L"game->GetBackBufferHeight() : %d", game->GetBackBufferHeight());
	if (cy > 0) cy = 0;



	if (game->GetCurrentScene()->GetId() == 5)
	{
		CGame::GetInstance()->SetCamPos(cx, cy);

	}
	else {
		CGame::GetInstance()->SetCamPos(cx, 0);
	}

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		if (objects[i] != NULL) {
			objects[i]->Render();
		}
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		if (*it != NULL) {
			delete (*it);
		}
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		if (objects[i] != NULL)
		{
			delete objects[i];

			objects.clear();
			player = NULL;

			DebugOut(L"[INFO] Scene %d unloaded! \n", id);
		}
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	GameData* gameData = GameData::GetInstance();
	int sceneID = CGame::GetInstance()->GetCurrentScene()->GetId();
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] != NULL) {
			LPGAMEOBJECT o = objects[i];
			if (o->IsDeleted())
			{
				gameData->MarkObjectDeleted(sceneID, i);
				if (sceneID == 5) {
					gameData->MarkObjectDeleted(1, i);
				}
				delete o;
				objects[i] = NULL;
			}
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	/*objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());*/
}