#include "IManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

IManager::IManager()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
}

IManager::~IManager()
{

}
