#include <Windows.h>
#include "render/draw.h"
#include "memory/Addr.hh"

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
 
int(__stdcall* oPresent)(LPDIRECT3DDEVICE9 _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) = 0;

HWND window = NULL;
WNDPROC oWndProc;
IDirect3DSwapChain9* pswapchain;
IDirect3DSurface9* surface;



bool show,test = true;
bool init = false;
int  __stdcall hkPresent(LPDIRECT3DDEVICE9 _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{

	//if (!init)
	//{

	//	//InitImGui(_this);
	//	oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
	//	init = true;
	//	return oPresent(_this, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	//}

	


	if (GetAsyncKeyState(VK_INSERT) & 1)
		show = !show;
	
	CDraw::Setup(_this);
	CDraw::DrawLine(150, 200, 100, D3DCOLOR_ARGB(255, 255, 0, 0));


	

	return oPresent(_this, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}



void Init()	
{
	Addr::Setup();
	//window = FindWindowW(L"App", L"SO official game client ");
	//if (!window)
	//	window = GetForegroundWindow();
	auto Steam_PresentScene = *(uintptr_t**)(Addr::pfn_Steam_Present);
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

