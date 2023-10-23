#include "DemoObject.h"
#include "DemoProcess.h"
#include "DemoCamera.h"
#include "ModelParser.h"
#include "ManagerSet.h"
#include "Mesh.h"

DemoObject::DemoObject(GraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager)
	: graphicsEngine(_graphicsEngine)
	, scene(_scene)
	, lightCount(1)
	, managers(_manager)
	, meshies{}
{
	std::vector<VertexT::Data> vertexInfo;
	std::vector<UINT> indexInfo;
	this->meshies = AseParser(L"../Model/box_material.ASE");

	for (auto& m : this->meshies)
	{
		m->CreatePipeline(this->graphicsEngine, this->path, this->texturePath);
	}
	dirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLights[0].Direction = XMFLOAT3(1.f, 0.f, 0.f);

	dirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	dirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	dirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	dirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	demoMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	demoMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	demoMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}

DemoObject::~DemoObject()
{
	for (auto& m : this->meshies)
	{
		delete m;
	}
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

void DemoObject::Render(GraphicsEngine* ge)
{
	this->graphicsEngine->BindMatrixParameter(
		DirectX::XMMatrixIdentity(),
		this->scene->getCamera()->GetViewTM(),
		this->scene->getCamera()->GetProjectionTM(),
		this->demoMat
	);

	this->graphicsEngine->BindLightingParameter(this->dirLights, lightCount, this->scene->getCamera()->GetPosition());

	// this->graphicsEngine->SetTexture(0, 1, &this->pipeline.textureView);
	for(auto &m : this->meshies) 
	{
		m->Render(graphicsEngine);
	}
}
