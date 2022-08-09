#include <Windows.h>
#include <d3d9.h>
#include <dxgi.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "memory/utils.hh"
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

//// HRESULT Present(
//[in] const RECT* pSourceRect,
//[in] const RECT* pDestRect,
//[in] HWND          hDestWindowOverride,
//[in] const RGNDATA* pDirtyRegion
//);
// IDirect3DDevice9
//  STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) PURE;
  
int(__stdcall* oPresent)(LPDIRECT3DDEVICE9 _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) = 0;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND window = NULL;
WNDPROC oWndProc;
IDirect3DSwapChain9* swapchain;
IDirect3DSurface9* surface;
void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
	ImGui_ImplDX9_CreateDeviceObjects();
}
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}




bool show,test = true;
bool init = false;
int  __stdcall hkPresent(LPDIRECT3DDEVICE9 _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{

	if (!init)
	{

		InitImGui(_this);
		oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
		init = true;
		return oPresent(_this, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		show = !show;
	

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	
	ImGui::NewFrame();

	if (show)
	ImGui::ShowDemoWindow(&test);


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	_this->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	return oPresent(_this, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}



void Init()	
{
	window = FindWindowW(L"App", L"SO official game client ");
	if (!window)
		window = GetForegroundWindow();
	auto Steam_PresentScene = *(uintptr_t**)( (uintptr_t)utils::sigscan("6A 01 68 ? ? ? ? 68 ? ? ? ? FF 76 44 E8", "GameOverlayRenderer.dll") + 3);
	*(uintptr_t*)&oPresent = *Steam_PresentScene;
   Steam_PresentScene[0] = (uintptr_t)&hkPresent; 
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD r, LPVOID lpvReserved)
{
 
	if (r == 1)
	{
		Init();
	}

    return TRUE;
}

