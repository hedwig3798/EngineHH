#include "DemoUIObject.h"
#include "DemoProcess.h"
#include "ManagerSet.h"
#include "UObject.h"
#include "IGraphicsEngine.h"

DemoUIObject::DemoUIObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, managers(_manager)
	, isLocal(false)
	, testMesh(nullptr)
	, aniTime(0.0f)
{
	this->testMesh = new UObject();
	this->testMesh->Initalize(this->graphicsEngine, "keySetting", path, texturePath, 500, 500, 500, 500);
}

DemoUIObject::~DemoUIObject()
{
	delete this->testMesh;
}

void DemoUIObject::Update(float _dt)
{
	if (this->managers->keyManager->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
	{
		this->testMesh->AddPosition(_dt * 1000, 0.0f);
	}
	if (this->managers->keyManager->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
		this->testMesh->AddPosition(-_dt * 1000, 0.0f);
	}
	if (this->managers->keyManager->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
	{
		this->testMesh->AddPosition(0.0f, _dt * 1000);
	}
	if (this->managers->keyManager->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
	{
		this->testMesh->AddPosition(0.0f, -_dt * 1000);
	}
// 	if (this->managers->keyManager->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
// 	{
// 		this->teatAssimp->AddRotation(0.0f, 0.0f, _dt * 10);
// 	}
}

void DemoUIObject::Render(IGraphicsEngine* ge)
{
	this->testMesh->Render();
}
