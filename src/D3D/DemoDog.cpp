#include "DemoDog.h"
#include "DemoProcess.h"
#include "ManagerSet.h"
#include "IGraphicsEngine.h"
#include "AObject.h"


DemoDog::DemoDog(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, lightCount(3)
	, managers(_manager)
	, isLocal(false)
	, aniTime(0.0f)
{
	this->teatAssimp = new AObject("adult_dog6.fbx", this->graphicsEngine, true, true, false);
}

DemoDog::~DemoDog()
{
	delete this->teatAssimp;
}

void DemoDog::Update(float _dt)
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
		this->teatAssimp->AddRotation(_dt * 10, 0.0f, 0.0f);
	}
	if (this->managers->keyManager->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
	{
		this->teatAssimp->AddRotation(0.0f, _dt * 10, 0.0f);
	}
	if (this->managers->keyManager->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
	{
		this->teatAssimp->AddRotation(0.0f, 0.0f, _dt * 10);
	}
	/*this->testFMesh->Update();*/
	aniTime += _dt;

	this->teatAssimp->ApplyAnimation("dog_sit_to_lie", aniTime);
}

void DemoDog::Render(IGraphicsEngine* ge)
{
	/*this->testFMesh->Render();*/
	this->teatAssimp->Render();
}
