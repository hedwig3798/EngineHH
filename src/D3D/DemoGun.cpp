#include "DemoGun.h"
#include "DemoProcess.h"
#include "ManagerSet.h"
#include "IGraphicsEngine.h"
#include "AObject.h"

DemoGun::DemoGun(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, lightCount(3)
	, managers(_manager)
	, isLocal(false)
{
	this->teatAssimp = new AObject("gun.fbx", this->graphicsEngine, false, false, true);
}

DemoGun::~DemoGun()
{
	delete this->teatAssimp;
}

void DemoGun::Update(float _dt)
{
	std::vector<float> tempMove = { _dt * 10, _dt * 10, _dt * 10 };
	std::vector<float> tempMovenega = { -_dt * 10, -_dt * 10, -_dt * 10 };
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

	if (this->managers->keyManager->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
	{
	//	this->testFMesh->AddPosition(tempMovenega);
	}
	if (this->managers->keyManager->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
	//	this->testFMesh->AddPosition(tempMove);
	}
	/*this->testFMesh->Update();*/
}

void DemoGun::Render(IGraphicsEngine* ge)
{
	//this->testFMesh->Render();
	this->teatAssimp->Render();
}
