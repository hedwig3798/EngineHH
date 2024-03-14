#include "DemoBackObject.h"
#include "DemoProcess.h"
#include "ManagerSet.h"
#include "IGraphicsEngine.h"
#include "AObject.h"

DemoBackObject::DemoBackObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, managers(_manager)
	, isLocal(false)
	, testUMesh(nullptr)
{
	//testUMesh = this->graphicsEngine->LoadFbxDatawithBackground("../Model/wall1.fbx");
	//this->testUMesh->Initalize(this->graphicsEngine, path, texturePath, this->testUMesh->bData);
	this->teatAssimp = new AObject("adult_dog6.fbx", this->graphicsEngine, true, true, true);
}

DemoBackObject::~DemoBackObject()
{
	delete this->testUMesh;
}

void DemoBackObject::Update(float _dt)
{

}

void DemoBackObject::Render(IGraphicsEngine* ge)
{
	this->testUMesh->Render();
}
