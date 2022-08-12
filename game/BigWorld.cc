#include "BigWorld.hh"
#include "../memory/Addr.hh"
ImpCamera* ImpCamera::Instance() { return *(ImpCamera**)*(int*)Addr::g_imp_Camera; };

EntityManager*  EntityManager::instance()
{
	return reinterpret_cast<EntityManager*(__fastcall*)()>(Addr::pfn_EntityManager_instance)();
}


Entity* __thiscall EntityManager::getEntity( int id, bool check_cache)
{
	return reinterpret_cast<Entity*(__thiscall*)(EntityManager* _this,int id,bool checkcache)>(Addr::pfn_EntityManager_getEntity)(this,id, check_cache);
}
PyStringObject* PyHelpers::PyString_InternFromString( const char* str)
{
	return reinterpret_cast<PyStringObject*(__cdecl*)(const char* name)>(Addr::pfn_Phyton_PyString_InternFromString)(str); // ory create own class with str +0x14
}