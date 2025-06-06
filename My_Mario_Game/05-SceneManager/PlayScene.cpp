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
#include "Parinha.h"
#include "Troopa.h"
#include "ParaTroopa.h"
#include "MushRoom.h"
#include "Spawner.h"
#include "Brick.h"
#include "SwitchPos.h"
#include "QuestionBlock.h"

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
	if (tokens.size() <= 2) return;

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
		obj = new CMario(x ,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_KOOPA: {
		float leftBound = (float)atof(tokens[3].c_str());
		float rightBound = (float)atof(tokens[4].c_str());
		obj = new CKoopa(x, y, leftBound, rightBound); break;
	}
	case OBJECT_TYPE_TROOPA: {
		float leftBound = (float)atof(tokens[3].c_str());
		float rightBound = (float)atof(tokens[4].c_str());
		obj = new CTroopa(x, y, leftBound, rightBound); break;
	}
	case OBJECT_TYPE_PARATROOPA: {
		float leftBound = (float)atof(tokens[3].c_str());
		float rightBound = (float)atof(tokens[4].c_str());
		obj = new CParaTroopa(x, y, leftBound, rightBound); break;
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
	case OBJECT_TYPE_COIN: {
		int type = atoi(tokens[3].c_str());
		obj = new CCoin(x, y, type); break;
	}

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
	case OBJECT_TYPE_PARINHA:
		obj = new CParinha(x, y);
		break;

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
	case OBJECT_TYPE_QUESTIONBLOCK:
	{
		int type = atoi(tokens[3].c_str());
		obj = new CQuestionBlock(x, y, type);
		break;
	}
	case OBJECT_TYPE_BRICK:
	{
		int type = atoi(tokens[3].c_str());
		obj = new CBrick(x, y, type);
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
	 objects.push_back(obj);

}

void CPlayScene::InitiateObjectWithOutTxt()
{
	CSpawner* spawn1 = new CSpawner(602, 143, 10, OBJECT_TYPE_KOOPA, 510, 600);
	CSpawner* spawn2 = new CSpawner(600, 175, 1000, OBJECT_TYPE_GOOMBA, 350, 750);
	CSpawner* spawn3 = new CSpawner(1520, 120, 5000, OBJECT_TYPE_TROOPA, -10, 10000);
	CSpawner* spawn4 = new CSpawner(1390, 80, 5000, OBJECT_TYPE_PARATROOPA, 900, 1700);
	CSpawner* spawn5 = new CSpawner(1440, 80, 5000, OBJECT_TYPE_PARATROOPA, 900, 1700);
	CSpawner* spawn6 = new CSpawner(1490, 80, 5000, OBJECT_TYPE_PARATROOPA, 900, 1700);
	CSpawner* spawn7 = new CSpawner(900, 159, 1000, OBJECT_TYPE_GOOMBA, 600, 1065);
	CSpawner* spawn8 = new CSpawner(950, 159, 1000, OBJECT_TYPE_GOOMBA, 600, 1065);
	CSpawner* spawn9 = new CSpawner(1000, 159, 1000, OBJECT_TYPE_REDGOOMBA, 600, 1065);
	CSpawner* spawn10 = new CSpawner(2130, 129, 10, OBJECT_TYPE_KOOPA, 2122, 2138);
	CPlatform* platform = new CPlatform(10, 230, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform1 = new CPlatform(10, 246, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform2 = new CPlatform(10, 262, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform3 = new CPlatform(10, 278, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform4 = new CPlatform(10, 294, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform5 = new CPlatform(10, 310, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform6 = new CPlatform(10, 326, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform7 = new CPlatform(10, 342, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform8 = new CPlatform(10, 358, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform9 = new CPlatform(10, 374, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform10 = new CPlatform(10, 390, 16, 16, 32, 60001, 60001, 60001);
	CPlatform* platform11 = new CPlatform(10, 406, 16, 16, 32, 60001, 60001, 60001);

	CPlatform* platform12 = new CPlatform(266, 374, 16, 16, 16, 60001, 60001, 60001);
	CPlatform* platform13 = new CPlatform(282, 358, 16, 16, 15, 60001, 60001, 60001);
	CPlatform* platform14 = new CPlatform(298, 342, 16, 16, 14, 60001, 60001, 60001);
	CPlatform* platform15 = new CPlatform(314, 326, 16, 16, 13, 60001, 60001, 60001);
	CPlatform* platform16 = new CPlatform(330, 310, 16, 16, 12, 60001, 60001, 60001);
	CPlatform* platform17 = new CPlatform(346, 294, 16, 16, 11, 60001, 60001, 60001);
	CPlatform* platform18 = new CPlatform(394, 278, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform19 = new CPlatform(394, 262, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform20 = new CPlatform(394, 246, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform21 = new CPlatform(394, 230, 16, 16, 8, 60001, 60001, 60001);
	CPlatform* platform22 = new CPlatform(170, 230, 16, 16, 12, 60001, 60001, 60001);
	CPipe* pipe1 = new CPipe(146, 238, 32, 32, 1, 77600, 3);
	CPipe* pipe2 = new CPipe(369, 238, 32, 32, 1, 77600, 3);
	CPipe* pipe3 = new CPipe(2304, -13,	32,	193, 1,	77400, 2);

	CSwitchPos* pos1 = new CSwitchPos(2304, -61, 2320, -20, 1);
	CSwitchPos* pos2 = new CSwitchPos(368, 238, 382, 250, 2);
	CTree* b = new CTree(250, 300, ID_SPRITE_BLACKBACKGROUND2);
	CCoin* coin1 = new CCoin(220, 270, 1);
	CCoin* coin2 = new CCoin(220, 334, 1);
	CCoin* coin3 = new CCoin(236, 254, 1);
	CCoin* coin4 = new CCoin(236, 302, 1);
	CCoin* coin5 = new CCoin(236, 350, 1);
	CCoin* coin6 = new CCoin(252, 254, 1);
	CCoin* coin7 = new CCoin(252, 302, 1);
	CCoin* coin8 = new CCoin(252, 350, 1);
	CCoin* coin9 = new CCoin(268, 270, 1);
	CCoin* coin10 = new CCoin(268, 334, 1);
	CCoin* coin11 = new CCoin(268, 286, 1);
	CCoin* coin12 = new CCoin(268, 318, 1);




	if (this->GetId() == 5)
	{
		this->InsertObjectBefore(b, player);
		this->AddObject(spawn1);
		this->AddObject(spawn2);
		this->AddObject(spawn3);
		this->AddObject(spawn4);
		this->AddObject(spawn5);
		this->AddObject(spawn6);
		this->AddObject(spawn7);
		this->AddObject(spawn8);
		this->AddObject(spawn9);
		this->AddObject(spawn10);
		this->AddObject(platform);
		this->AddObject(platform1);
		this->AddObject(platform2);
		this->AddObject(platform3);
		this->AddObject(platform4);
		this->AddObject(platform5);
		this->AddObject(platform6);
		this->AddObject(platform7);
		this->AddObject(platform8);
		this->AddObject(platform9);
		this->AddObject(platform10);
		this->AddObject(platform11);
		this->AddObject(platform12);
		this->AddObject(platform13);
		this->AddObject(platform14);
		this->AddObject(platform15);
		this->AddObject(platform16);
		this->AddObject(platform17);
		this->AddObject(platform18);
		this->AddObject(platform19);
		this->AddObject(platform20);
		this->AddObject(platform21);
		this->AddObject(platform22);
		this->AddObject(pipe1);
		this->AddObject(pipe2);
		this->AddObject(pos1);
		this->AddObject(pos2);
		this->AddObject(coin1);
		this->AddObject(coin2);
		this->AddObject(coin3);
		this->AddObject(coin4);
		this->AddObject(coin5);
		this->AddObject(coin6);
		this->AddObject(coin7);
		this->AddObject(coin8);
		this->AddObject(coin9);
		this->AddObject(coin10);
		this->AddObject(coin11);
		this->AddObject(coin12);
		this->AddObject(pipe3);
	}
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

void CPlayScene::InsertObjectBefore(LPGAMEOBJECT newObj, LPGAMEOBJECT beforeObj) {
	auto it = std::find(objects.begin(), objects.end(), beforeObj);
	if (it != objects.end()) {
		objects.insert(it, newObj);
	}
	else {
		objects.push_back(newObj); // fallback nếu không tìm thấy
	}
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
	this->InitiateObjectWithOutTxt();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

CKoopa* CPlayScene::GetFirstKoopa(CPlayScene* playScene)
{
		for (LPGAMEOBJECT obj : playScene->GetObjects())
		{
			CKoopa* koopa = dynamic_cast<CKoopa*>(obj);
			if (koopa != NULL)
			{
				return koopa;
				DebugOut(L"\nTRoopa is get succccessss");

			}
		}
		return NULL;
}

CTroopa* CPlayScene::GetTroopa(CPlayScene* playScene)
{
	for (LPGAMEOBJECT obj : playScene->GetObjects())
	{
		CTroopa* troopa = dynamic_cast<CTroopa*>(obj);
		if (troopa != NULL)
		{
			return troopa;
			DebugOut(L"TRoopa is get succccessss");
		}
	}
	return NULL;
}

CKoopa* CPlayScene::GetNearestKoopa(CPlayScene* playScene)
{
	CMario* player = (CMario*)(playScene->GetPlayer());
	float marioX, marioY;
	float koopaX, koopaY;
	for (LPGAMEOBJECT obj : playScene->GetObjects())
	{
		CKoopa* koopa = dynamic_cast<CKoopa*>(obj);
		
		if (koopa != NULL && player != NULL)
		{
			koopa->GetPosition(koopaX, koopaY);
			player->GetPosition(marioX, marioY);
			if (abs(marioX - koopaX) <= 100)
			{
				return koopa;
			}
		}
	}
	return NULL;
}

CButton* CPlayScene::GetButton(CPlayScene* playScene)
{
	for (LPGAMEOBJECT obj : playScene->GetObjects())
	{
		CButton* button = dynamic_cast<CButton*>(obj);
		if (button != NULL)
		{
			return button;
			DebugOut(L"TRoopa is get succccessss");
		}
	}
	return NULL;
}

CMushRoom* CPlayScene::GetMushRoomSamePosition(CPlayScene* playScene, float x)
{
	float mushRoomX, mushRoomY;
	for (LPGAMEOBJECT obj : playScene->GetObjects())
	{
		CMushRoom* mushRoom = dynamic_cast<CMushRoom*>(obj);
		if (mushRoom != NULL)
		{
			mushRoom->GetPosition(mushRoomX, mushRoomY);
			if(mushRoomX == x) return mushRoom;
		}
	}
	return NULL;
}


void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		    coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
			objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	float y = cy;

	CGame* game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= (game->GetBackBufferHeight() / 2);

	if (cx < 0) cx = 0;
	if (game->GetCurrentScene()->GetId() == 5)
	{
		if (cx > 2463) cx = 2463;
	}

	if (y > 200 && this->GetIsInSecret())
	{
		if (cx > 127) cx = 127;
		cy = 210;
	}
	else if (y > 0 && y <= 200) cy = 0;
	else if (y > 200 && !this->GetIsInSecret()) cy = 0;


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
			objects[i]->Render();
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
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

