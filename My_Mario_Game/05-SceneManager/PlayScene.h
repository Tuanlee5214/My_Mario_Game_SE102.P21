#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Troopa.h"
#include "MushRoom.h"
#include "Button.h"
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;	
	LPGAMEOBJECT panel;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void AddObject(LPGAMEOBJECT e);
	virtual void Unload();
	void SetIsInSecret(bool a) { this->isInSecretArea = a; }
	bool GetIsInSecret() { return this->isInSecretArea; }

	CKoopa* GetFirstKoopa(CPlayScene* playScene);
	CTroopa* GetTroopa(CPlayScene* playScene);
	CButton* GetButton(CPlayScene* playScene);
	CKoopa* GetNearestKoopa(CPlayScene* playScene);

	void InsertObjectBefore(LPGAMEOBJECT newObj, LPGAMEOBJECT beforeObj);
	CMushRoom* GetMushRoomSamePosition(CPlayScene* playScene, float x);

	LPGAMEOBJECT GetPlayer() { return player; }
	std::vector<LPGAMEOBJECT> GetObjects()
	{
		return objects;
	}

	void InitiateObjectWithOutTxt();

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

