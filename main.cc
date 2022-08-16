//#include "render/draw.h" legacy
#include <Windows.h>
#include "render/draw.h"

#include "game/BigWorld.hh"
#include "game/Hooks.hh"
#include "memory/Addr.hh"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include <iostream>
//#include <forward_list>
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int(__stdcall* oPresent)(LPDIRECT3DDEVICE9 _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) = 0;

HWND window = NULL;
WNDPROC oWndProc;
IDirect3DSwapChain9* pswapchain;
IDirect3DSurface9* surface;
//
//std::forward_list<int> Entityidmap;
//
//void CacheThread()
//{
//	while (true)
//	{
//
//		auto entity_mngr = EntityManager::instance();
//		
//		if (!entity_mngr) continue;
//		myMap entities = entity_mngr->EntiredEntities();
//		auto count = entities.count;
//		if (!count) continue;
//		auto localplayer = entity_mngr->getEntity(entity_mngr->LocalPlayerId());
//		if (!localplayer) continue;
//
//		auto  teamID = localplayer->teamID();
//		auto  dead = localplayer->dead();
//
//		auto list_position = entities._first_->_next;
//		for (size_t i = 0; i < count; i++)
//		{
//
//			if (!list_position) break;
//			//auto entity = entity_mngr->getEntity(list_position->EntityId);
//			auto entity = list_position->pp_Entity;
//
//			list_position = list_position->_next;
//			if (!entity) continue;
//			Vector3 position = entity->position();
//			Vector2 ScreenPos;
//			const char* shortName = entity->GetNameClass(); // to do use type id 
//			if (strcmp((char*)shortName, (char*)"Avatar") != 0) continue;
//
//			/*PyUnicodeObject* playername = entity->name();*/
//
//		//	ImGui::GetBackgroundDrawList()->AddText({ ScreenPos.x,ScreenPos.y }, ImColor{ 255,255,255,255 }, "test");
//
//			//entity->DumpProperties();
//
//		}
//	}
//
//
//}
//


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

	auto  teamID = localplayer->is_burning();

	auto  dead = localplayer->dead();

	auto list_position = entities._first_->_next;
	for (size_t i = 0; i < count; i++)
	{
		
		if (!list_position) break;
		//auto entity = entity_mngr->getEntity(list_position->EntityId);
		auto entity = list_position->pp_Entity;

		list_position = list_position->_next;
		if (!entity) continue;
		Vector3 position = entity->position();
		Vector2 ScreenPos;
		if ((!camera->WorldToScreen(position, &ScreenPos))) continue;
	
		const char* shortName = entity->GetNameClass(); // to do use type id 
		if (strcmp((char*)shortName, (char*)"Avatar") != 0) continue;
		
					PyUnicodeObject* playername = entity->name();
					CDraw::TextW(ScreenPos.x, ScreenPos.y , playername->str);
					
						
						
		
					
		

		//entity->DumpProperties();

	}
	

}

bool intizliaze = false;
bool showmenu;
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (GetAsyncKeyState(VK_F2) & 1)
		showmenu = !showmenu;

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
		CDraw::Setup(pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();
		intizliaze = true;
}


bool showesp;
int  __stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (!intizliaze)
		InitImGui(pDevice);
	


	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();



	if (showesp)
		DrawEsp();



	if (showmenu)
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Begin("Hello, world!");                     

		ImGui::SameLine();
		ImGui::Checkbox("esp_test", &showesp);
	
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

//TODO reverse process /sogame.exe+1A426A8  == disable ac /detection and find where it send bad request

void Init()	
{
	Addr::Setup();
	Hooks::Setup();
	
	auto p_PresentScene = *(uintptr_t**)(Addr::pfn_Present);
	*(uintptr_t*)&oPresent = *p_PresentScene;
	p_PresentScene[0] = (uintptr_t)&hkPresent;
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD r, LPVOID lpvReserved)
{
 
	if (r == 1)
	{
		Init();
	}

    return TRUE;
}

