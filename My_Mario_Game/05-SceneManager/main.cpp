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

#include "SampleKeyEventHandler.h"

#include "AssetIDs.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(156.0f/255, 252.0f/255, 240.0f/255, 0.0f)
#define BACKGROUND_COLOR2 D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)

#define TEXTURES_DIR L"texture1"
//#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MAINMAP TEXTURES_DIR "\\mainmap.png"
#define TEXTURE_PATH_MAP TEXTURES_DIR "\\map.png"
#define TEXTURE_PATH_ENEMIES1 TEXTURES_DIR "\\enemies1.png"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

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
	if (idScene == 5)
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

	LPTEXTURE texMainMap = textures->Get(ID_TEX_MAINMAP);
	LPTEXTURE texMap = textures->Get(ID_TEX_MAP);
	LPTEXTURE texEnemies1 = textures->Get(ID_TEX_ENEMIES1);

	CSprites* sprites = CSprites::GetInstance();
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
	sprites->Add(ID_SPRITE_PIPE2, 1792, 384, 1824, 416, texMainMap);
	sprites->Add(ID_SPRITE_PIPE3, 1856, 368.5, 1888, 416, texMainMap);
	sprites->Add(ID_SPRITE_PIPE4, 2257, 383, 2286, 416, texMainMap);
	sprites->Add(ID_SPRITE_PIPE5, 2255, 111, 2287, 304, texMainMap);
	sprites->Add(ID_SPRITE_PIPE6, 2320, 383, 2351, 416, texMainMap);
	sprites->Add(ID_SPRITE_BLACKPIPE, 2095.7, 463, 2129, 495, texMainMap);

	//Sprite Koopa
	sprites->Add(ID_SPRITE_KOOPA_WALKING_LEFT1, 28, 65, 44, 91, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_WALKING_LEFT2, 46, 65, 63, 91, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_WALKING_RIGHT1, 603, 66, 620, 91, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_WALKING_RIGHT2, 584, 64, 600, 91, texEnemies1);
	sprites->Add(ID_SPRITE_KOOPA_DIE, 71, 95, 88, 110, texEnemies1);

	//Sprite Question
	sprites->Add(ID_SPRITE_QUESTION1, 415, 319, 432, 336, texMainMap);
	sprites->Add(ID_SPRITE_QUESTION2, 175, 351, 208, 368, texMainMap);


	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION ani;
	
	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_KOOPA_WALKING_LEFT1);
	ani->Add(ID_SPRITE_KOOPA_WALKING_LEFT2);	
	animations->Add(ID_ANI_KOOPA_WALKING_L, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_KOOPA_WALKING_RIGHT1);
	ani->Add(ID_SPRITE_KOOPA_WALKING_RIGHT2);
	animations->Add(ID_ANI_KOOPA_WALKING_R, ani);

	ani = new CAnimation(200);
	ani->Add(ID_SPRITE_KOOPA_DIE);
	animations->Add(ID_ANI_KOOPA_DIE, ani);

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

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

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