#include "DemoSpriteObject.h"
#include "DemoProcess.h"
#include "ManagerSet.h"
#include "UObject.h"
#include "IGraphicsEngine.h"

DemoSpriteObject::DemoSpriteObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, managers(_manager)
	, isLocal(false)
	, testMesh(nullptr)
	, aniTime(0.0f)
{
	this->testMesh = new UObject();
	this->testMesh->AnimationInitailze(this->graphicsEngine, "meowAni", texturePath, 500, 400, 300, 300, 300, 4);
}

DemoSpriteObject::~DemoSpriteObject()
{
	delete this->testMesh;
}

void DemoSpriteObject::Update(float _dt)
{
	//this->testMesh->Update();
}

void DemoSpriteObject::Render(IGraphicsEngine* ge)
{
	this->testMesh->Render();
}
