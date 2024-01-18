#pragma once
#include <windows.h>
#include <vector>
#include <string>

struct PipeLine;
class FObject;
class ICamera;

class IGraphicsEngine abstract
{
public:
	IGraphicsEngine() {};
	virtual ~IGraphicsEngine() {};

	virtual void Initialize(HWND _hwnd) abstract;

	virtual void begineDraw() abstract;
	virtual void Render(PipeLine& _pipline, int _indexSize) abstract;
	virtual void endDraw() abstract;

	virtual void BindPipeline(PipeLine& _pipline) abstract;

	virtual void WriteText(int _x, int _y, float _rgba[4], TCHAR* _text) abstract;
	virtual FObject* LoadFbxData(std::string _path) abstract;

	virtual void CreateCamera(ICamera** _camera, float _w, float _h) abstract;
	virtual void SetMainCamera(ICamera* _camera) abstract;

	// Debug object
	virtual void DrawDefaultLine() abstract;
	virtual void DrawDefaultAxes() abstract;
};

void CreateGrapicsEngine(IGraphicsEngine** _output);