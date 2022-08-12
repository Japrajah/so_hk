#pragma once
#define ADD_PTR(NAME) static inline int NAME;

class Addr
{
public:
	static void Setup();
	ADD_PTR(pfn_EntityManager_instance);
	ADD_PTR(pfn_EntityManager_getEntity);
	ADD_PTR(pfn_Steam_Present);
	ADD_PTR(g_imp_Camera);
	ADD_PTR(pfn_Phyton_PyString_InternFromString); //


};

#undef ADD_PTR