#include "BigWorld.hh"
#include "../memory/Addr.hh"


EntityManager*  EntityManager::instance()
{
	return reinterpret_cast<EntityManager*(*)()>(Addr::pfn_EntityManager_instance)();
}


Entity* __thiscall EntityManager::getEntity( int id, bool check_cache)
{
	return reinterpret_cast<Entity*(__thiscall*)(EntityManager* _this,int id,bool checkcache)>(Addr::pfn_EntityManager_instance)(this,id, check_cache);
}