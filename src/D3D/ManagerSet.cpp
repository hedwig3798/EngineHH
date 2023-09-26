#include "ManagerSet.h"


ManagerSet::ManagerSet()
{
	this->keyManager = new KeyManager();
	this->timeManager = new TimeManager();
}

ManagerSet::~ManagerSet()
{
	delete this->keyManager;
	delete this->timeManager;
}

void ManagerSet::Initialize(HWND _hwnd)
{
	this->keyManager->Initalize(_hwnd);
	this->timeManager->Initalize();
}

void ManagerSet::Update()
{
	this->keyManager->Update();
	this->timeManager->Update();
}
