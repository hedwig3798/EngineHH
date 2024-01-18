#include "DemoObject.h"
#include "DemoProcess.h"
#include "ManagerSet.h"
#include "FObject.h"
#include "IGraphicsEngine.h"

DemoObject::DemoObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, lightCount(3)
	, managers(_manager)
	, isLocal(false)
	, testFMesh(nullptr)
{
	testFMesh = this->graphicsEngine->LoadFbxData("../Model/testMen.fbx");
	this->testFMesh->Initalize(this->graphicsEngine, path, texturePath, this->testFMesh->fData);
}

DemoObject::~DemoObject()
{
	delete this->testFMesh;
}

void DemoObject::Update(float _dt)
{
	if (this->managers->keyManager->GetKeyState(KEY::N_0) == KEY_STATE::DOWN)
	{
		lightCount = 0;
	}
	if (this->managers->keyManager->GetKeyState(KEY::N_1) == KEY_STATE::DOWN)
	{
		lightCount = 1;
	}
	if (this->managers->keyManager->GetKeyState(KEY::N_2) == KEY_STATE::DOWN)
	{
		lightCount = 2;
	}
	if (this->managers->keyManager->GetKeyState(KEY::N_3) == KEY_STATE::DOWN)
	{
		lightCount = 3;
	}
}


void DemoObject::Render(IGraphicsEngine* ge)
{
	this->testFMesh->Render();
}
