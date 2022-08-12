#include "render/draw.h"
#include "game/BigWorld.hh"
#include "memory/Addr.hh"
#include <iostream>
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
 
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
	
	auto list_position = entities._first_;
	for (size_t i = 0; i < count; i++)
	{
		count = entities.count +2;
		if (!list_position) break;
		auto entity = entity_mngr->getEntity(list_position->EntityId);
		list_position = list_position->_next;
		if (!entity) continue;
		Vector3 position = entity->position();
		Vector2 ScreenPos;
		if ((!camera->WorldToScreen(position, &ScreenPos)) && init) continue;
		auto obtype = entity->ob_type;
		if (!obtype) continue;

		const char* shortName = obtype->GetName();
		if (!strcmp(shortName ,"Avatar"));
		{
			if (obtype->tp_getattro)
			{
				auto py_STR = PyHelpers::PyString_InternFromString("name");
				if (py_STR)
				{

					PyUnicodeObject* playername = obtype->GetAttribute<PyUnicodeObject*>(entity, py_STR);
					if (playername)
					{
						if (playername->str)
						{
							CDraw::TextW(ScreenPos.x, ScreenPos.y, playername->str);
						}
					}
					
				}

			

				
			
			}
			
		}


		
		//entity->DumpProperties();

	}
	init = true;

}




int  __stdcall hkPresent(LPDIRECT3DDEVICE9 _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	CDraw::Setup(_this);

	


	DrawEsp();


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

