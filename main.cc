#include "memory/Addr.hh"
#include "render/draw.h"
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
	CDraw::Setup(_this);

	CDraw::Text(350, 250, "TEST TEXT");

	

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

