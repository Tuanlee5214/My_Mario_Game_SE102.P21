#include "UIManager.h"

CUIManager* CUIManager::__instance = NULL;

CUIManager* CUIManager::GetInstance()
{
	if (__instance == NULL) __instance = new CUIManager(0, 0);
	return __instance;
}