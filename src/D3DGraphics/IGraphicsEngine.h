#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <memory>
struct PipeLine;
class UObject;
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

	virtual std::vector<float> PickingWorldPoint(float _x, float _y) abstract;

	virtual void CreateCamera(std::string _name, float _w, float _h) abstract;
	virtual void SetMainCamera(std::string _name) abstract;
	virtual bool IsMainCamera(std::string _name) abstract;
	virtual std::weak_ptr<ICamera> GetCamera() abstract;
	// Debug object
	virtual void DrawDefaultLine() abstract;
	virtual void DrawDefaultAxes() abstract;

	//GetDT
	virtual void GetDT(float _dt) abstract;

	//For Effect OnOff
	virtual void SetFlashEffect(float deltaTime, bool _isOnOff) abstract;
	virtual void SetPixelateEffect() abstract;
	virtual void SetWhiteOutEffect(float deltaTime, bool _isOnOff) abstract;

	// resize window size
	virtual void Resize() abstract;

	// chaputer imgae
	virtual void ChaptuerScreen(std::string _name) abstract;
	virtual void ShowChaptueredImage(std::string _name, RECT _rect) abstract;

	virtual void CreateSkyBox(std::string _name, std::string _path) abstract;
	virtual void SetSkyBox(std::string _name) abstract;
};

void CreateGrapicsEngine(IGraphicsEngine** _output);