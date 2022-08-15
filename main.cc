#include "render/draw.h"
#include "game/BigWorld.hh"
#include "game/Hooks.hh"
#include "memory/Addr.hh"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include <iostream>
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int(__stdcall* oPresent)(LPDIRECT3DDEVICE9 _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) = 0;

HWND window = NULL;
WNDPROC oWndProc;
IDirect3DSwapChain9* pswapchain;
IDirect3DSurface9* surface;



bool show,test = true;
bool init = false;
void DrawEsp()
{


	auto entity_mngr = EntityManager::instance();
	if (!entity_mngr) return;
	auto camera = ImpCamera::Instance();
	if (!camera) return;

	if (!entity_mngr) return;
	myMap entities = entity_mngr->EntiredEntities();
	auto count = entities.count;
	if (!count) return;
	auto localplayer = entity_mngr->getEntity(entity_mngr->LocalPlayerId());
	if (!localplayer) return;

	auto  is_burning = localplayer->is_burning();
	auto  teamID = localplayer->teamID();
	auto  canTakeDamage = localplayer->canTakeDamage();
	auto  CanUse = localplayer->CanUse();
	auto  CanShoot = localplayer->CanShoot();
	auto  dead = localplayer->dead();

	auto list_position = entities._first_->_next;
	for (size_t i = 0; i < count; i++)
	{
		count = entities.count;
		if (!list_position) break;
		auto entity = entity_mngr->getEntity(list_position->EntityId);
		list_position = list_position->_next;
		if (!entity) continue;
		Vector3 position = entity->position();
		Vector2 ScreenPos;
		if ((!camera->WorldToScreen(position, &ScreenPos)) && init) continue;
	
		const char* shortName = entity->GetNameClass();
		if (strcmp((char*)shortName, (char*)"Avatar") != 0) continue;
		
					PyUnicodeObject* playername = entity->name();
				
					if (playername)
					{
						auto name = playername->str;
						if (name)
							CDraw::TextW(ScreenPos.x, ScreenPos.y, name);
					}
		
					
		

		//entity->DumpProperties();

	}
	init = true;

}

bool intizliaze = false;
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{

		window = FindWindowW(L"App", L"SO official game client ");
		if (!window)
			window = GetForegroundWindow();
		oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();
		intizliaze = true;
}

int  __stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (!intizliaze)
	{
		InitImGui(pDevice);
		CDraw::Setup(pDevice); // for custom render i use it to draw wchars
	}
	

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	DrawEsp();

	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                     

		ImGui::Text("This is some useful text.");

		if (ImGui::Button("Button"))                         
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}



	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);



	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}



void Init()	
{
	Addr::Setup();
	Hooks::Setup();

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

