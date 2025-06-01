#include "GameData.h"
#include "QuestionBlock.h"

GameData* GameData::_instance = NULL;

GameData* GameData::GetInstance()
{
	if (_instance == NULL) _instance = new GameData();
	return _instance;
}

void GameData:: MarkObjectDeleted(int sceneID, int objectIndex)
{
	if (deletedObjectsByScene.find(sceneID) == deletedObjectsByScene.end())
	{
		deletedObjectsByScene[sceneID] = vector<int>();
	}
	deletedObjectsByScene[sceneID].push_back(objectIndex);
}

bool GameData::IsObjectDeleted(int sceneID, int objectIndex)
{
	if (deletedObjectsByScene.find(sceneID) != deletedObjectsByScene.end())
	{
		vector<int> IsDeletedObject = deletedObjectsByScene[sceneID];
		for (int i = 0; i < IsDeletedObject.size(); i++)
		{
			if (objectIndex == IsDeletedObject[i])
				return true;
		}
	}
	return false;
}

void GameData::ClearAllDeletedObjects()
{

	deletedObjectsByScene.clear();
}