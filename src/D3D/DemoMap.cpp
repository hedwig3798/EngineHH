#include "DemoMap.h"
#include "MapLoader.h"
#include "ManagerSet.h"

DemoMap::DemoMap(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, lightCount(3)
	, managers(_manager)
	, isLocal(false)
	, testFMesh(nullptr)
	, aniTime(0.0f)
	, testMapRender(testMap)
{
	testMap = MapLoader::LoadMapData("stage_2_tapdance_final.fbx", _graphicsEngine);
	testMap2 = MapLoader::LoadMapData("stage_2_tapdance_final.fbx", _graphicsEngine);

	for (auto& a : this->testMap)
	{
		if (a->GetName() == "bigball")
		{
			testBall = a;
			//ABoundingBox temp = testBall->GetBoundingBox();
			break;
		}
	}

	for (auto& a : this->testMap)
	{
		ABoundingBox temp = a->GetBoundingBox();
	}
}

DemoMap::~DemoMap()
{
	for (auto& a : this->testMap)
	{
		delete a;
	}
	for (auto& a : this->testMap2)
	{
		delete a;
	}
}

void DemoMap::Update(float _dt)
{
	if (this->managers->keyManager->GetKeyState(KEY::N_4) == KEY_STATE::DOWN)
	{
		this->testMapRender = testMap2;
	}
}

void DemoMap::Render(IGraphicsEngine* ge)
{
	for (auto& a : this->testMapRender)
	{
		a->Render();
	}
}
