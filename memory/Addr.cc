#include "Addr.hh"
#include "utils.hh"

void Addr::Setup()
{


	Addr::pfn_EntityManager_instance = utils::read_address(utils::sigscan("E8 ? ? ? ? 83 78 04 00 75 07 8B 0E E8"),1,5 );
	Addr::pfn_EntityManager_getEntity = utils::read_address(utils::sigscan("8B 40 04 8B 70 6C E8 ? ? ? ? 6A 00 56 8B C8 E8 ? ? ? ? 5E C3") +16, 1, 5);
	Addr::pfn_Steam_Present = utils::sigscan("6A 01 68 ? ? ? ? 68 ? ? ? ? FF 76 44 E8", "GameOverlayRenderer.dll") + 3;
	Addr::g_imp_Camera = (utils::sigscan("89 3D ? ? ? ? D9 1D", "umbra.dll")+2);

}