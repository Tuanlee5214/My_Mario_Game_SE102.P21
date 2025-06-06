/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 05 - SCENE MANAGER

	This sample illustrates how to:

		1/ Read scene (textures, sprites, animations and objects) from files 
		2/ Handle multiple scenes in game

	Key classes/functions:
		CScene
		CPlayScene		


HOW TO INSTALL Microsoft.DXSDK.D3DX
===================================
1) Tools > NuGet package manager > Package Manager Console
2) execute command :  Install-Package Microsoft.DXSDK.D3DX


================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <list>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animation.h"
#include "Animations.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "Coin.h"
#include "Platform.h"
#include "Koopa.h"
#include "Scene.h"
#include "RedGoomba.h"
#include "RedPlant.h"
#include "Bullet.h"
#include "GreenPlant.h"
#include "Parinha.h"
#include "Troopa.h"
#include "ParaTroopa.h"
#include "SampleKeyEventHandler.h"
#include "Spawner.h"
#include "QuestionBlock.h"
#include "ECoin.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "Effect.h"
#include "Button.h"
#include "Point.h"

#include "AssetIDs.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(156.0f/255, 252.0f/255, 240.0f/255, 0.0f)
#define BACKGROUND_COLOR2 D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)

#define TEXTURES_DIR L"texture1"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MAINMAP TEXTURES_DIR "\\mainmap.png"
#define TEXTURE_PATH_MAP TEXTURES_DIR "\\map.png"
#define TEXTURE_PATH_ENEMIES1 TEXTURES_DIR "\\enemies1.png"
#define TEXTURE_PATH_MICS3 TEXTURES_DIR "\\misc-3.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc.png"
#define TEXTURE_PATH_GOAL TEXTURES_DIR "\\goal.png"
#define TEXTURE_PATH_MARIO3	TEXTURES_DIR "\\mariobros3.png"
#define TEXTURE_PATH_ENEMIES2 TEXTURES_DIR "\\enemies2.png"
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 230

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*	
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	CGame::GetInstance()->GetCurrentScene()->Update(dt);
}

/*
	Render a frame 
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	int idScene = g->GetCurrentScene()->GetId();
	if (idScene == 5 || idScene == 1)
	{
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);
	}
	else if (idScene == 2)
	{
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR2);
	}
	//pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	CGame::GetInstance()->GetCurrentScene()->Render();

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}


void LoadResourceForGame() {
	CTextures* textures = CTextures::GetInstance();
	textures->Add(ID_TEX_MAP, TEXTURE_PATH_MAP);
	textures->Add(ID_TEX_MAINMAP, TEXTURE_PATH_MAINMAP);
	textures->Add(ID_TEX_ENEMIES1, TEXTURE_PATH_ENEMIES1);
	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_MISC3, TEXTURE_PATH_MICS3);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_GOAL, TEXTURE_PATH_GOAL);
	textures->Add(ID_TEX_MARIO3, TEXTURE_PATH_MARIO3);
	textures->Add(ID_TEX_ENEMIES2, TEXTURE_PATH_ENEMIES2);

	LPTEXTURE texMainMap = textures->Get(ID_TEX_MAINMAP);
	LPTEXTURE texMap = textures->Get(ID_TEX_MAP);
	LPTEXTURE texEnemies1 = textures->Get(ID_TEX_ENEMIES1);
	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);
	LPTEXTURE texMisc3 = textures->Get(ID_TEX_MISC3);
	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	LPTEXTURE texGoal = textures->Get(ID_TEX_GOAL);
	LPTEXTURE texMario3 = textures->Get(ID_TEX_MARIO3);
	LPTEXTURE texEnemies2 = textures->Get(ID_TEX_ENEMIES2);

	CSprites* sprites = CSprites::GetInstance();

#pragma region SPRITE
	//Sprite Ground
	sprites->Add(ID_SPRITE_GROUND2, 1151, 417, 1503, 432, texMainMap);
	sprites->Add(ID_SPRITE_GROUND3, 1535, 417, 1615, 432, texMainMap);
	sprites->Add(ID_SPRITE_GROUND4, 1663, 417, 2239, 432, texMainMap);
	sprites->Add(ID_SPRITE_GROUND5, 2255, 417, 2815, 432, texMainMap);
	sprites->Add(ID_SPRITE_GROUND1_1, 1599, 367, 1616, 383, texMainMap);
	sprites->Add(ID_SPRITE_GROUND1_2, 1503, 335, 1536, 351, texMainMap);
	sprites->Add(ID_SPRITE_GROUND1_3, 1567, 400, 1618, 416, texMainMap);
	sprites->Add(ID_SPRITE_GROUND1_4, 1408, 191, 1471, 208, texMainMap);
	sprites->Add(ID_SPRITE_GROUND1_5, 1487, 174, 1697, 192, texMainMap);
	sprites->Add(ID_SPRITE_BLUEBRICK, 1968, 464, 1984, 480, texMainMap);
	sprites->Add(ID_SPRITE_GROUP_BLUEBRICK, 1967, 463, 2096, 656, texMainMap);

	sprites->Add(ID_SPRITE_QUESTION1, 415, 319, 432, 336, texMainMap);
	sprites->Add(ID_SPRITE_QUESTION2, 175, 351, 208, 368, texMainMap);

	//Sprite Tree
	sprites->Add(ID_SPRITE_TREE1, 15, 351, 143, 416, texMainMap);
	sprites->Add(ID_SPRITE_TREE3, 79, 26, 143, 74, texMap);
	sprites->Add(ID_SPRITE_TREE2, 0, 79, 128, 143, texMap);
	sprites->Add(ID_SPRITE_TREE4, 1889, 367, 1951, 416, texMainMap);
	sprites->Add(ID_SPRITE_TREE5, 2367, 287, 2463, 416, texMainMap);
	sprites->Add(ID_SPRITE_TransTREE1, 0, 208, 78, 304, texMap);
	sprites->Add(ID_SPRITE_TransTREE2, 3, 145, 50, 193, texMap);
	sprites->Add(ID_SPRITE_BLACKBACKGROUND, 2480, 0, 2816, 417, texMainMap);
	sprites->Add(ID_SPRITE_BUSH, 160, 399, 176, 416, texMainMap);
	sprites->Add(ID_SPRITE_BLACKBACKGROUND2, 1613, 466, 1904, 635, texMainMap);

	//Sprite Cloud
	sprites->Add(ID_SPRITE_CLOUD1, 126, 0, 159, 22, texMap);
	sprites->Add(ID_SPRITE_CLOUD2, 120, 272, 167, 295, texMainMap);
	sprites->Add(ID_SPRITE_CLOUD3, 728, 304, 791, 327, texMainMap);

	//Sprite Box
	sprites->Add(ID_SPRITE_BOX1, 240, 368, 287, 415, texMainMap);
	sprites->Add(ID_SPRITE_BOX2, 270, 336, 328, 415, texMainMap);
	sprites->Add(ID_SPRITE_BOX3, 400, 368, 479, 415, texMainMap);
	sprites->Add(ID_SPRITE_BOX4, 464, 336, 528, 415, texMainMap);
	sprites->Add(ID_SPRITE_BOX5, 343, 37, 416, 148, texMap);
	sprites->Add(ID_SPRITE_BOX6, 343, 117, 447, 149, texMap);
	sprites->Add(ID_SPRITE_BOX7, 151, 217, 262, 249, texMap);
	sprites->Add(ID_SPRITE_BOX8, 182, 185, 294, 249, texMap);
	sprites->Add(ID_SPRITE_BOX9, 215, 153, 336, 249, texMap);
	sprites->Add(ID_SPRITE_BOX10, 1312, 142, 1384, 182, texMainMap);
	sprites->Add(ID_SPRITE_BOX11, 2144, 367, 2191, 415, texMainMap);
	sprites->Add(ID_SPRITE_BOX12, 2176, 273, 2223, 415, texMainMap);
	sprites->Add(ID_SPRITE_BOX12, 2176, 272, 2232, 415, texMainMap);
	
	//Sprite Pipe
	sprites->Add(ID_SPRITE_PIPE1, 352, 369, 383, 416, texMainMap);
	sprites->Add(ID_SPRITE_PIPE2, 1792, 385, 1824, 417, texMainMap);
	sprites->Add(ID_SPRITE_PIPE3, 1856, 369, 1888, 416, texMainMap);
	sprites->Add(ID_SPRITE_PIPE4, 2257, 383, 2286, 416, texMainMap);
	sprites->Add(ID_SPRITE_PIPE5, 2255, 111, 2287, 304, texMainMap);
	sprites->Add(ID_SPRITE_PIPE6, 2320, 383, 2351, 416, texMainMap);
	sprites->Add(ID_SPRITE_BLACKPIPE, 2095.7, 463, 2129, 495, texMainMap);

	//Sprite Koopa
	sprites->Add(ID_SPRITE_KOOPA_WALKING_LEFT1, 26, 66, 45, 92, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_WALKING_LEFT2, 45, 66, 64, 92, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_WALKING_RIGHT1, 602, 66, 621, 92, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_WALKING_RIGHT2, 582, 66, 601, 92, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_DIE3L, 27, 95, 45, 111, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_DIE2L, 48, 95, 65, 111, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_DIE1L, 69, 94, 87, 109, texEnemies1);

	sprites->Add(ID_SPRITE_KOOPA_DIE3R, 28, 95, 45, 111, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_DIE2R, 581, 94, 598, 111, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_DIE1R, 70, 94, 87, 109, texEnemies1);
	

	//Sprite Coin
	sprites->Add(ID_SPRITE_COIN1, 227, 6, 241, 22, texMap);
	sprites->Add(ID_SPRITE_COIN2, 245, 6, 257, 22, texMap);
	sprites->Add(ID_SPRITE_COIN3, 258, 6, 264, 22, texMap);
	sprites->Add(ID_SPRITE_COIN4, 268, 6, 279, 22, texMap);
	sprites->Add(ID_SPRITE_COIN5, 283, 6, 297, 22, texMap);

	//Sprite RedGoomba
	sprites->Add(ID_SPRITE_REDGOOMBA_DIE1, 144, 68, 161, 85, texEnemies1);
	sprites->Add(ID_SPRITE_REDGOOMBA_WALK1_1, 102, 68, 119, 85, texEnemies1);
	sprites->Add(ID_SPRITE_REDGOOMBA_WALK1_2, 124, 68, 141, 85, texEnemies1);
	sprites->Add(ID_SPRITE_REDGOOMBA_WALK2_1, 140, 37, 158, 58, texEnemies1);
	sprites->Add(ID_SPRITE_REDGOOMBA_WALK2_2, 115, 37, 134, 58, texEnemies1);
	sprites->Add(ID_SPRITE_REDGOOMBA_FLY2_1, 88, 33, 109, 58, texEnemies1);
	sprites->Add(ID_SPRITE_REDGOOMBA_FLY2_2, 65, 33, 88, 58, texEnemies1);

	//Sprite RedPlant
	sprites->Add(ID_SPRITE_REDPLANT_LEFTBOTTOM, 6, 29, 23, 65, texEnemies1);
	sprites->Add(ID_SPRITE_REDPLANT_LEFTTOP, 6, 68, 23, 103, texEnemies1);
	sprites->Add(ID_SPRITE_REDPLANT_RIGHTTOP, 623, 68, 641, 103, texEnemies1);
	sprites->Add(ID_SPRITE_REDPLANT_RIGHTBOTTOM, 623, 29, 641, 64, texEnemies1);

	//Sprite GreenPlant
	sprites->Add(ID_SPRITE_GREENPLANT_LEFTTOP, 6, 192, 22, 217, texEnemies1);
	sprites->Add(ID_SPRITE_GREENPLANT_LEFTBOTTOM, 6, 164, 22, 188, texEnemies1);
	sprites->Add(ID_SPRITE_GREENPLANT_RIGHTTOP, 625, 193, 642, 218, texEnemies1);
	sprites->Add(ID_SPRITE_GREENPLANT_RIGHTBOTTOM, 625, 164, 642, 189, texEnemies1);

	//Sprite Parinha
	sprites->Add(ID_SPRITE_PARINHA_OPEN, 7, 106, 22, 130, texEnemies1);
	sprites->Add(ID_SPRITE_PARINHA_CLOSE, 6, 135, 22, 159, texEnemies1);

	//Sprite Bullet
	sprites->Add(ID_SPRITE_BULLET1, 34, 33, 44, 43, texEnemies1);
	sprites->Add(ID_SPRITE_BULLET2, 50, 33, 58, 43, texEnemies1);
	sprites->Add(ID_SPRITE_BULLET3, 33, 47, 43, 57, texEnemies1);
	sprites->Add(ID_SPRITE_BULLET4, 48, 47, 58, 57, texEnemies1);

	//Sprite Troopa
	sprites->Add(ID_SPRITE_TROOPA_WALKING_LEFT1, 30, 117, 46, 144, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_WALKING_LEFT2, 49, 117, 66, 144, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_WALKING_RIGHT1, 601, 117, 617, 144, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_WALKING_RIGHT2, 581, 117, 598, 144, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_DIE3L, 31, 147, 48, 163, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_DIE2L, 52, 146, 69, 163, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_DIE1L, 73, 146, 90, 163, texEnemies1);

	sprites->Add(ID_SPRITE_TROOPA_DIE3R, 31, 147, 48, 163, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_DIE2R, 579, 146, 595, 163, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_DIE1R, 73, 146, 90, 163, texEnemies1);
	sprites->Add(ID_SPRITE_TROOPA_FLIP_DIE, 326, 124, 343, 141, texEnemies1);

	//BlackBackground
	sprites->Add(ID_SPRITE_BLACKBACKGROUND1, 2619, 186, 2666, 234, texMainMap);

	//Sprite E-Coin
	sprites->Add(ID_SPRITE_ECOIN1, 293, 100, 302, 115, texMisc3);
	sprites->Add(ID_SPRITE_ECOIN2, 335, 100, 341, 115, texMisc3);
	sprites->Add(ID_SPRITE_ECOIN3, 309, 100, 312, 115, texMisc3);
	sprites->Add(ID_SPRITE_ECOIN4, 320, 100, 325, 115, texMisc3);

	//Sprite ParaTroopa
	sprites->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT1, 146, 128, 164, 155, texEnemies1);
	sprites->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT2, 125, 128, 142, 155, texEnemies1);
	sprites->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT3, 104, 128, 121, 155, texEnemies1);
	sprites->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT1, 484, 128, 501, 155, texEnemies1);
	sprites->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT2, 505, 128, 521, 155, texEnemies1);
	sprites->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT3, 527, 128, 543, 155, texEnemies1);

	//MushRoom
	sprites->Add(ID_SPRITE_MUSHROOMRED, 155, 120, 172, 136, texMisc3);
	sprites->Add(ID_SPRITE_MUSHROOMGREEN, 285, 120, 302, 136, texMisc3);

	//END GAME
	sprites->Add(ID_SPRITE_BOXWHITE, 57, 149, 88, 178, texMap);
	sprites->Add(ID_SPRITE_STAR, 89, 156, 105, 173, texMap);
	sprites->Add(ID_SPRITE_FLOWER, 107, 155, 125, 173, texMap);
	sprites->Add(ID_SPRITE_RED_MUSHROOM, 126, 154, 144, 172, texMap);

	//Sprite Mario
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_CARRY_LEFT, 247, 91, 264, 120, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_CARRY_RIGHT, 257, 183, 274, 212, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_CARRY_LEFT1, 265, 91, 283, 120, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_CARRY_LEFT2, 283, 91, 303, 120, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_CARRY_RIGHT1, 238, 183, 257, 212, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_CARRY_RIGHT2, 220, 183, 238, 212, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_KICKING_LEFT, 303, 91, 328, 120, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_KICKING_RIGHT, 193, 183, 216, 212, texMario3);

	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_CARRY_LEFT, 144, 13, 160, 32, texMario3);
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_CARRY_RIGHT, 362, 53, 377, 72, texMario3);
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_CARRY_LEFT, 162, 13, 179, 32, texMario3);
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_CARRY_RIGHT, 342, 53, 360, 72, texMario3);
	sprites->Add(ID_SPRITE_MARIO_SMALL_KICKING_LEFT, 180, 13, 198, 32, texMario3);
	sprites->Add(ID_SPRITE_MARIO_SMALL_KICKING_RIGHT, 323, 53, 342, 72, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_TRANS_LEFT, 0, 91, 17, 120, texMario3);
	sprites->Add(ID_SPRITE_MARIO_BIG_TRANS_RIGHT, 505, 183, 522, 214, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT, 208, 130, 224, 155, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT, 297, 223, 314, 248, texMario3);
	sprites->Add(ID_SPRITE_MARIO_SMALL_TRANS_LEFT, 1, 15, 16, 32, texMario3);
	sprites->Add(ID_SPRITE_MARIO_SMALL_TRANS_RIGHT, 506, 53, 521, 72, texMario3);
	sprites->Add(ID_SPRITE_MARIO_NOTHING, 182, 315, 198, 339, texMario);

	sprites->Add(ID_SPRITE_MARIO_MAX_IDLE_LEFT, 1, 348, 22, 379, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_IDLE_RIGHT, 498, 439, 521, 470, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_IDLE_CARRY_LEFT, 0, 382, 23, 415, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_IDLE_CARRY_RIGHT, 498, 473, 521, 504, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_WALKING_CARRY_LEFT1, 24, 382, 51, 415, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_WALKING_CARRY_LEFT2, 51, 382, 76, 415, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_WALKING_CARRY_RIGHT1, 470, 473, 499, 502, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_WALKING_CARRY_RIGHT2, 444, 473, 471, 502, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_WALKING_LEFT1, 25, 348, 50, 381, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_WALKING_LEFT2, 51, 347, 76, 380, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_WALKING_RIGHT1, 472, 439, 497, 470, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_WALKING_RIGHT2, 445, 437, 472, 470, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_KICKING_LEFT, 79, 382, 104, 413, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_KICKING_RIGHT, 417, 473, 442, 504, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_RUNNING_LEFT1, 179, 347, 208, 380, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_RUNNING_LEFT2, 207, 347, 234, 380, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_RUNNING_LEFT3, 233, 347, 260, 380, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_RUNNING_RIGHT1, 313, 437, 340, 470, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_RUNNING_RIGHT2, 288, 437, 314, 470, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_RUNNING_RIGHT3, 261, 437, 288, 470, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_JUMP_WALK_LEFT, 101, 348, 128, 377, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_JUMP_WALK_RIGHT, 392, 439, 419, 469, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_LEFT, 259, 347, 286, 380, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_RIGHT, 235, 436, 262, 469, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_SIT_LEFT, 77, 355, 103, 380, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_SIT_RIGHT, 419, 448, 444, 469, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_BRACE_LEFT, 337, 346, 356, 379, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_BRACE_RIGHT, 165, 436, 184, 469, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_TURN_LEFT, 215, 284, 242, 317, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_TURN_RIGHT, 264, 284, 293, 317, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_IS_IN_FRONT_OF, 356, 346, 375, 379, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_IS_BACK, 399, 347, 417, 378, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_FALL_LEFT1, 129, 346, 156, 379, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_FALL_LEFT2, 155, 346, 180, 379, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_FALL_RIGHT1, 366, 436, 393, 469, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_FALL_RIGHT2, 340, 436, 367, 469, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_LEFT1, 287, 348, 311, 377, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_LEFT2, 311, 348, 337, 377, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_RIGHT1, 210, 438, 236, 469, texMario3);
	sprites->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_RIGHT2, 185, 438, 210, 469, texMario3);






	//Leaf sprite
	sprites->Add(ID_SPRITE_LEAF_LEFT, 179 ,121 ,196, 138, texMisc3);
	sprites->Add(ID_SPRITE_LEAF_RIGHT, 382, 121, 400, 138, texMisc3);


	//Questions block
	sprites->Add(ID_SPRITE_QUESBLOCK1, 154, 186, 173, 204, texMisc3);
	sprites->Add(ID_SPRITE_QUESBLOCK2, 173, 186, 192, 204, texMisc3);
	sprites->Add(ID_SPRITE_QUESBLOCK3, 193, 186, 212, 204, texMisc3);
	sprites->Add(ID_SPRITE_QUESBLOCK4, 212, 186, 231, 204, texMisc3);
	sprites->Add(ID_SPRITE_NO_QUESBLOCK, 232, 186, 251, 204, texMisc3);

	//Brick sprite
	sprites->Add(ID_SPRITE_BRICK1, 273, 187, 291, 204, texMisc3);
	sprites->Add(ID_SPRITE_BRICK2, 292, 187, 310, 204, texMisc3);
	sprites->Add(ID_SPRITE_BRICK3, 311, 187, 329, 204, texMisc3);
	sprites->Add(ID_SPRITE_BRICK4, 329, 187, 348, 204, texMisc3);
	sprites->Add(ID_SPRITE_EFFECT_BRICK, 482, 118, 492, 129, texMisc);

	//Button sprite
	sprites->Add(ID_SPRITE_BUTTON1, 156, 142, 171, 162, texMisc3);
	sprites->Add(ID_SPRITE_BUTTON2, 176, 143, 194, 161, texMisc3);
	sprites->Add(ID_SPRITE_BUTTON_AFTER_USE, 198, 151, 217, 160, texMisc3);

	//Point sprite
	sprites->Add(ID_SPRITE_POINT_TYPE1, 457, 221, 468, 230, texMisc);
	sprites->Add(ID_SPRITE_POINT_TYPE2, 453, 231, 468, 240, texMisc);
	sprites->Add(ID_SPRITE_POINT_TYPE10, 470, 218, 489, 231, texMisc);
	sprites->Add(ID_SPRITE_1UP, 594, 170, 625, 190, texMisc);
	sprites->Add(ID_SPRITE_MUSH_EFFECT1, 255, 370, 275, 390, texGoal);
	sprites->Add(ID_SPRITE_MUSH_EFFECT2, 315, 371, 329, 390, texGoal);

	//SPRITE OUT GAME
	sprites->Add(ID_SPRITE_GOOMBA_OUT_GAME, 3, 21, 21, 42, texEnemies2);
	sprites->Add(ID_SPRITE_REDGOOMBA_OUT_GAME, 22, 22, 37, 42, texEnemies2);
	sprites->Add(ID_SPRITE_KOOPA_OUTGAME, 38, 21, 53, 42, texEnemies2);

	

#pragma endregion

	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION ani;

#pragma region ANIMATION

	//Point
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_POINT_TYPE1);
	animations->Add(ID_ANI_POINT_TYPE1, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_POINT_TYPE2);
	animations->Add(ID_ANI_POINT_TYPE2, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_POINT_TYPE10);
	animations->Add(ID_ANI_POINT_TYPE3, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_1UP);
	animations->Add(ID_ANI_1UP, ani);

	ani = new CAnimation(300);
	ani->Add(ID_SPRITE_MUSH_EFFECT1);
	ani->Add(ID_SPRITE_MUSH_EFFECT2);
	animations->Add(ID_ANI_MUSH_EFFEC, ani);
	
	//Button
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BUTTON1);
	animations->Add(ID_ANI_BUTTON_BEFORE_USE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BUTTON_AFTER_USE);
	animations->Add(ID_ANI_BUTTON_AFTER_USE, ani);

	//Goomba
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_GOOMBA_OUT_GAME);
	animations->Add(ID_ANI_GOOMBA_OUT_GAME, ani);

	//Brick
	ani = new CAnimation(300);
	ani->Add(ID_SPRITE_BRICK1);
	ani->Add(ID_SPRITE_BRICK2);
	ani->Add(ID_SPRITE_BRICK3);
	animations->Add(ID_ANI_BRICK_BEFORE_USE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_EFFECT_BRICK);
	animations->Add(ID_ANI_EFFECT, ani);

	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_STAR);
	ani->Add(ID_SPRITE_FLOWER);
	ani->Add(ID_SPRITE_RED_MUSHROOM);
	animations->Add(ID_ANI_MSF, ani);

	//QuestionBlock
	ani = new CAnimation(130);
	ani->Add(ID_SPRITE_QUESBLOCK1);
	ani->Add(ID_SPRITE_QUESBLOCK2);
	ani->Add(ID_SPRITE_QUESBLOCK3);
	ani->Add(ID_SPRITE_QUESBLOCK4);
	animations->Add(ID_ANI_QUESBLOCK_BEFORE_USE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_NO_QUESBLOCK);
	animations->Add(ID_ANI_QUESBLOCK_AFTER_USE, ani);

	//MushRoom ani
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MUSHROOMRED);
	animations->Add(ID_ANI_MUSHROOM, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MUSHROOMGREEN);
	animations->Add(ID_ANI_MUSHROOMGREEN, ani);

	//Leaf ani
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_LEAF_LEFT);
	animations->Add(ID_ANI_LEAF_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_LEAF_RIGHT);
	animations->Add(ID_ANI_LEAF_RIGHT, ani);

	//Mario ani
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_CARRY_LEFT);
	animations->Add(ID_ANI_MARIO_IDLE_CARRY_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_CARRY_RIGHT);
	animations->Add(ID_ANI_MARIO_IDLE_CARRY_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_CARRY_LEFT);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_CARRY_LEFT1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_CARRY_LEFT2);
	animations->Add(ID_ANI_MARIO_WALKING_CARRY_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_CARRY_RIGHT);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_CARRY_RIGHT1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_CARRY_RIGHT2);
	animations->Add(ID_ANI_MARIO_WALKING_CARRY_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_KICKING_LEFT);
	animations->Add(ID_ANI_MARIO_KICKING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_KICKING_RIGHT);
	animations->Add(ID_ANI_MARIO_KICKING_RIGHT, ani);

	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_LEFT);
	animations->Add(ID_ANI_MARIO_TRANSFORM_SMALL_TO_BIG_LEFT, ani);

	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_RIGHT);
	animations->Add(ID_ANI_MARIO_TRANSFORM_SMALL_TO_BIG_RIGHT, ani);


	ani = new CAnimation(40);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_NOTHING);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_NOTHING);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_NOTHING);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_RIGHT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_RIGHT);
	animations->Add(ID_ANI_MARIO_TRANSFORM_BIG_TO_SMALL_RIGHT, ani);

	ani = new CAnimation(40);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_NOTHING);
	ani->Add(ID_SPRITE_MARIO_BIG_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_NOTHING);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_NOTHING);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_MIDDLE_TRANS_LEFT);
	ani->Add(ID_SPRITE_MARIO_SMALL_TRANS_LEFT);
	animations->Add(ID_ANI_MARIO_TRANSFORM_BIG_TO_SMALL_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_LEFT);
	animations->Add(ID_ANI_MARIO_MAX_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_RIGHT);
	animations->Add(ID_ANI_MARIO_MAX_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_CARRY_RIGHT);
	animations->Add(ID_ANI_MARIO_MAX_IDLE_CARRY_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_CARRY_LEFT);
	animations->Add(ID_ANI_MARIO_MAX_IDLE_CARRY_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_LEFT);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_LEFT1);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_LEFT2);
	animations->Add(ID_ANI_MARIO_MAX_WALKING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_RIGHT1);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_RIGHT2);
	animations->Add(ID_ANI_MARIO_MAX_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_CARRY_LEFT);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_CARRY_LEFT1);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_CARRY_LEFT2);
	animations->Add(ID_ANI_MARIO_MAX_WALKING_CARRY_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_CARRY_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_CARRY_RIGHT1);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_CARRY_RIGHT2);
	animations->Add(ID_ANI_MARIO_MAX_WALKING_CARRY_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_KICKING_LEFT);
	animations->Add(ID_ANI_MARIO_MAX_KICKING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_KICKING_RIGHT);
	animations->Add(ID_ANI_MARIO_MAX_KICKING_RIGHT, ani);

	ani = new CAnimation(30);
	ani->Add(ID_SPRITE_MARIO_MAX_RUNNING_LEFT1);
	ani->Add(ID_SPRITE_MARIO_MAX_RUNNING_LEFT2);
	ani->Add(ID_SPRITE_MARIO_MAX_RUNNING_LEFT3);
	animations->Add(ID_ANI_MARIO_MAX_RUNNING_LEFT, ani);

	ani = new CAnimation(30);
	ani->Add(ID_SPRITE_MARIO_MAX_RUNNING_RIGHT1);
	ani->Add(ID_SPRITE_MARIO_MAX_RUNNING_RIGHT2);
	ani->Add(ID_SPRITE_MARIO_MAX_RUNNING_RIGHT3);
	animations->Add(ID_ANI_MARIO_MAX_RUNNING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_WALK_LEFT);
	animations->Add(ID_ANI_MARIO_MAX_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_WALK_RIGHT);
	animations->Add(ID_ANI_MARIO_MAX_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_FALL_LEFT1);
	animations->Add(ID_ANI_MARIO_MAX_FALL_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_FALL_RIGHT1);
	animations->Add(ID_ANI_MARIO_MAX_FALL_WALK_RIGHT, ani);

	ani = new CAnimation(60);
	ani->Add(ID_SPRITE_MARIO_MAX_FALL_LEFT1);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_LEFT2);
	ani->Add(ID_SPRITE_MARIO_MAX_FALL_LEFT2);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_LEFT2);
	animations->Add(ID_ANI_MARIO_MAX_FALL_SLOW_LEFT, ani);

	ani = new CAnimation(60);
	ani->Add(ID_SPRITE_MARIO_MAX_FALL_RIGHT1);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_RIGHT2);
	ani->Add(ID_SPRITE_MARIO_MAX_FALL_RIGHT2);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_RIGHT2);
	animations->Add(ID_ANI_MARIO_MAX_FALL_SLOW_RIGHT, ani);

	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_LEFT);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_LEFT1);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_LEFT2);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_LEFT1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_HIGH_L, ani);

	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_RIGHT1);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_RIGHT2);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_RIGHT1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_HIGH_R, ani);




	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_LEFT);
	animations->Add(ID_ANI_MARIO_MAX_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_JUMP_RUN_RIGHT);
	animations->Add(ID_ANI_MARIO_MAX_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_SIT_LEFT);
	animations->Add(ID_ANI_MARIO_MAX_SIT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_SIT_RIGHT);
	animations->Add(ID_ANI_MARIO_MAX_SIT_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_BRACE_RIGHT);
	animations->Add(ID_ANI_MARIO_MAX_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_MAX_BRACE_LEFT);
	animations->Add(ID_ANI_MARIO_MAX_BRACE_LEFT, ani);


	ani = new CAnimation(60);
	ani->Add(ID_SPRITE_MARIO_MAX_TURN_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MAX_IS_IN_FRONT_OF);
	ani->Add(ID_SPRITE_MARIO_MAX_TURN_LEFT);
	ani->Add(ID_SPRITE_MARIO_MAX_IS_BACK);
	ani->Add(ID_SPRITE_MARIO_MAX_TURN_RIGHT);
	animations->Add(ID_ANI_MARIO_MAX_TURN_RIGHT, ani);

	ani = new CAnimation(60);
	ani->Add(ID_SPRITE_MARIO_MAX_TURN_LEFT);
	ani->Add(ID_SPRITE_MARIO_MAX_IS_IN_FRONT_OF);
	ani->Add(ID_SPRITE_MARIO_MAX_TURN_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MAX_IS_BACK);
	ani->Add(ID_SPRITE_MARIO_MAX_TURN_LEFT);
	animations->Add(ID_ANI_MARIO_MAX_TURN_LEFT, ani);

	ani = new CAnimation(40);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_LEFT);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_LEFT1);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_LEFT2);
	animations->Add(ID_ANI_MARIO_MAX_WALK_TO_RUN_LEFT, ani);

	ani = new CAnimation(40);
	ani->Add(ID_SPRITE_MARIO_MAX_IDLE_RIGHT);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_RIGHT1);
	ani->Add(ID_SPRITE_MARIO_MAX_WALKING_RIGHT2);
	animations->Add(ID_ANI_MARIO_MAX_WALK_TO_RUN_RIGHT, ani);


	//ECoin
	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_ECOIN1);
	ani->Add(ID_SPRITE_ECOIN2);
	ani->Add(ID_SPRITE_ECOIN3);

	animations->Add(ID_ANI_ECOIN, ani);


	//Small mario
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_CARRY_LEFT);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_CARRY_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_CARRY_RIGHT);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_CARRY_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_CARRY_LEFT);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_CARRY_LEFT);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_CARRY_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_CARRY_RIGHT);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_CARRY_RIGHT);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_CARRY_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_KICKING_LEFT);
	animations->Add(ID_ANI_MARIO_SMALL_KICKING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_KICKING_RIGHT);
	animations->Add(ID_ANI_MARIO_SMALL_KICKING_RIGHT, ani);

	//Koopa ani
	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_KOOPA_WALKING_LEFT1);
	ani->Add(ID_SPRITE_KOOPA_WALKING_LEFT2);	
	animations->Add(ID_ANI_KOOPA_WALKING_L, ani);

	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_KOOPA_WALKING_RIGHT1);
	ani->Add(ID_SPRITE_KOOPA_WALKING_RIGHT2);
	animations->Add(ID_ANI_KOOPA_WALKING_R, ani);

	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_KOOPA_DIE1L);
	ani->Add(ID_SPRITE_KOOPA_DIE2L);
	ani->Add(ID_SPRITE_KOOPA_DIE3L);
	animations->Add(ID_ANI_KOOPA_DIE_RUN_L, ani);

	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_KOOPA_DIE1R);
	ani->Add(ID_SPRITE_KOOPA_DIE2R);
	ani->Add(ID_SPRITE_KOOPA_DIE3R);
	animations->Add(ID_ANI_KOOPA_DIE_RUN_R, ani);

	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_KOOPA_DIE3R);
	animations->Add(ID_ANI_KOOPA_DIE, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_KOOPA_OUTGAME);
	animations->Add(ID_ANI_KOOPA_OUT_GAME, ani);


	//Troopa ani
	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_TROOPA_WALKING_LEFT1);
	ani->Add(ID_SPRITE_TROOPA_WALKING_LEFT2);
	animations->Add(ID_ANI_TROOPA_WALKING_L, ani);

	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_TROOPA_WALKING_RIGHT1);
	ani->Add(ID_SPRITE_TROOPA_WALKING_RIGHT2);
	animations->Add(ID_ANI_TROOPA_WALKING_R, ani);

	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_TROOPA_DIE1L);
	ani->Add(ID_SPRITE_TROOPA_DIE2L);
	ani->Add(ID_SPRITE_TROOPA_DIE3L);
	animations->Add(ID_ANI_TROOPA_DIE_RUN_L, ani);

	ani = new CAnimation(80);
	ani->Add(ID_SPRITE_TROOPA_DIE1R);
	ani->Add(ID_SPRITE_TROOPA_DIE2R);
	ani->Add(ID_SPRITE_TROOPA_DIE3R);
	animations->Add(ID_ANI_TROOPA_DIE_RUN_R, ani);

	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_TROOPA_DIE3R);
	animations->Add(ID_ANI_TROOPA_DIE, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_TROOPA_FLIP_DIE);
	animations->Add(ID_ANI_TROOPA_OUT_GAME, ani);

	//ParaTroopa ani
	ani = new CAnimation(135);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT1);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT2);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT3);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT1);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT2);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT3);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT1);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT2);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_LEFT3);
	animations->Add(ID_ANI_PARATROOPA_WALKING_FLY_L, ani);

	ani = new CAnimation(135);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT1);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT2);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT3);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT1);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT2);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT3);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT1);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT2);
	ani->Add(ID_SPRITE_PARATROOPA_WALKING_RIGHT3);
	animations->Add(ID_ANI_PARATROOPA_WALKING_FLY_R, ani);



	

	//Coin ani
	ani = new CAnimation(210);
	ani->Add(ID_SPRITE_COIN1);
	ani->Add(ID_SPRITE_COIN2);
	ani->Add(ID_SPRITE_COIN3);
	ani->Add(ID_SPRITE_COIN4);
	animations->Add(ID_ANI_COIN, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_COIN1);
	animations->Add(ID_ANI_COIN1, ani);


	//RedGoomba ani
	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_REDGOOMBA_DIE1);
	animations->Add(ID_ANI_REDGOOMBA_DIE, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK1_1);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK1_2);
	animations->Add(ID_ANI_REDGOOMBA_WALKING, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_1);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_2);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_1);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_2);
	animations->Add(ID_ANI_REDGOOMBA_JUMP_WALKING, ani);

	ani = new CAnimation(150);
	ani->Add(ID_SPRITE_REDGOOMBA_FLY2_1);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_1);
	animations->Add(ID_ANI_REDGOOMBA_JUMPLOW, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_REDGOOMBA_OUT_GAME);
	animations->Add(ID_ANI_REDGOOMBA_OUTGAME, ani);

	
	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_REDGOOMBA_FLY2_1);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_1);
	ani->Add(ID_SPRITE_REDGOOMBA_FLY2_2);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_2);
	ani->Add(ID_SPRITE_REDGOOMBA_FLY2_1);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_1);
	ani->Add(ID_SPRITE_REDGOOMBA_FLY2_1);
	ani->Add(ID_SPRITE_REDGOOMBA_WALK2_1);
	animations->Add(ID_ANI_REDGOOMBA_JUMP, ani);

	//RedPlant ani
	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_REDPLANT_LEFTTOP);
	animations->Add(ID_ANI_REDPLANT_LEFTTOP, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_REDPLANT_LEFTBOTTOM);
	animations->Add(ID_ANI_REDPLANT_LEFTBOTTOM, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_REDPLANT_RIGHTTOP);
	animations->Add(ID_ANI_REDPLANT_RIGHTTOP, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_REDPLANT_RIGHTBOTTOM);
	animations->Add(ID_ANI_REDPLANT_RIGHTBOTTOM, ani);

	//Bullet ani
	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_BULLET1);
	ani->Add(ID_SPRITE_BULLET2);
	ani->Add(ID_SPRITE_BULLET3);
	ani->Add(ID_SPRITE_BULLET4);
	animations->Add(ID_ANI_BULLET, ani);

	//GreenPlant ani
	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_GREENPLANT_LEFTTOP);
	animations->Add(ID_ANI_GREENPLANT_LEFTTOP, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_GREENPLANT_LEFTBOTTOM);
	animations->Add(ID_ANI_GREENPLANT_LEFTBOTTOM, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_GREENPLANT_RIGHTTOP);
	animations->Add(ID_ANI_GREENPLANT_RIGHTTOP, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_GREENPLANT_RIGHTBOTTOM);
	animations->Add(ID_ANI_GREENPLANT_RIGHTBOTTOM, ani);

	//Parinha
	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_PARINHA_OPEN);
	ani->Add(ID_SPRITE_PARINHA_CLOSE);
	ani->Add(ID_SPRITE_PARINHA_OPEN);
	ani->Add(ID_SPRITE_PARINHA_CLOSE);
	animations->Add(ID_ANI_PARINHA_ATTACK, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_PARINHA_CLOSE);
	animations->Add(ID_ANI_PARINHA_IDLE, ani);
#pragma endregion
	
	if (sprites->Get(ID_SPRITE_GROUND2) == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", ID_SPRITE_GROUND2);
		return;
	}
	else
	{
		DebugOut(L"[INFO] Sprite ID %d loaded successfully\n", ID_SPRITE_GROUND2);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;
	CPlayScene* playScene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = 1;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = (DWORD)(now - frameStart);

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			CGame::GetInstance()->ProcessKeyboard();			
			Update(dt);
			Render();

			CGame::GetInstance()->SwitchScene();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	

	SetDebugWindow(hWnd);
	LPGAME game = CGame::GetInstance();
	game->Init(hWnd, hInstance);
	game->InitKeyboard();
	

	//IMPORTANT: this is the only place where a hardcoded file name is allowed ! 
	game->Load(L"mario-sample.txt");  
	LoadResourceForGame();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}