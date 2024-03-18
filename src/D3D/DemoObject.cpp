#include "DemoObject.h"
#include "DemoProcess.h"
#include "ManagerSet.h"
#include "IGraphicsEngine.h"
#include "AObject.h"

DemoObject::DemoObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, lightCount(3)
	, managers(_manager)
	, isLocal(false)
	, aniTime(0.0f)
{
 //	testFMesh = this->graphicsEngine->LoadFbxData("../Model/DeferredTestMesh/test5.fbx");
// 	this->testFMesh->Initalize(this->graphicsEngine, path, texturePath, this->testFMesh->fData);

	this->teatAssimp = new AObject("sf_test_box.fbx", this->graphicsEngine, false, false, true);
}

DemoObject::~DemoObject()
{
	delete this->teatAssimp;
}

void DemoObject::Update(float _dt)
{
	
}


void DemoObject::Render(IGraphicsEngine* ge)
{
	this->teatAssimp->Render();
}
