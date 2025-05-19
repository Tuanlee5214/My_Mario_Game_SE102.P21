#pragma once
#include <unordered_map>
#include <vector>

using namespace std;
class GameData
{
private:
	static GameData* _instance;
	unordered_map<int, vector<int>> deletedObjectsByScene;
	GameData() {}
public:
	static GameData* GetInstance();

	void MarkObjectDeleted(int sceneId, int objectIndex);
	bool IsObjectDeleted(int sceneId, int objectIndex);
	void ClearDeletedObjects(int sceneId);
	void ClearAllDeletedObjects();

};

