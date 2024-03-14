#pragma once
#include "pch.h"

class IGraphicsEngine;
class DemoProcess;
class ManagerSet;
class UObject;

class DemoUIObject
{
	IGraphicsEngine* graphicsEngine;

	DemoProcess* scene;

	std::wstring texturePath = L"..\\Resource\\Texture\\UI\\Stage1_Memo.png";

	std::wstring path[2]
	{
		L"../Shader/compiled/UIVertexShader.cso",
		L"../Shader/compiled/UIPixelShader.cso",
	};

	ManagerSet* managers;


	bool isLocal;
	float aniTime;

public:
	UObject* testMesh;
	DemoUIObject(IGraphicsEngine* _graphicsEngine, DemoProcess* _scene, ManagerSet* _manager);
	~DemoUIObject();

	void Update(float _dt);
	void Render(IGraphicsEngine* ge);
};

