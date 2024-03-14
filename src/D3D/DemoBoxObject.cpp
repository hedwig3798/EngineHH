#include "DemoBoxObject.h"
#include "DemoProcess.h"
#include "ManagerSet.h"
#include "IGraphicsEngine.h"
#include "AObject.h"

DemoBoxObject::DemoBoxObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, lightCount(3)
	, managers(_manager)
	, isLocal(false)
	, aniTime(0.0f)
{
	this->teatAssimp = new AObject("adult_dog.fbx", this->graphicsEngine, true, true, true);
}

DemoBoxObject::~DemoBoxObject()
{
	delete this->teatAssimp;
}

void DemoBoxObject::Update(float _dt)
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

	if (this->managers->keyManager->GetKeyState(KEY::LEFT) == KEY_STATE::DOWN)
	{
		this->teatAssimp->SetPosition(0.f, 0.f, 10.f);
	}

	if (this->managers->keyManager->GetKeyState(KEY::RIGHT) == KEY_STATE::DOWN)
	{
		this->teatAssimp->SetPosition(0.f, 0.f, -10);
	}

	if (this->managers->keyManager->GetKeyState(KEY::UP) == KEY_STATE::DOWN)
	{
		this->teatAssimp->SetPosition(10.f, 0.f, 0.f);
	}

	if (this->managers->keyManager->GetKeyState(KEY::DOWN) == KEY_STATE::DOWN)
	{
		this->teatAssimp->SetPosition(-10.f , 0.f, 0.f);
	}

	aniTime += _dt;

	this->teatAssimp->ApplyAnimation("dog_stand_to_sit", aniTime);
}

void DemoBoxObject::Render(IGraphicsEngine* ge)
{
	this->teatAssimp->Render();
}
