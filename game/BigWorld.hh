#pragma once
#include <Windows.h>
#include "Vectors.hh"
#define OFFSET( type, func, offset )  type func()  { return *reinterpret_cast< type *> ( (uintptr_t )this  + offset ); };


class Physics;
class Filter;
struct EntityType;
//https://github.com/v2v3v4/BigWorld-Engine-2.0.1/blob/620e0f244739188a5b183f1f28b278f16d182370/src/client/entity_type.hpp
struct EntityType
{
	class EntityDescription* description_;
	class PyObject* pModule_;
	class PyTypeObject* pClass_;
	class PyTypeObject* pPlayerClass_;
};

class Entity /*: public PyInstancePlus : public PyObject*/
{
public:
	OFFSET(int, id_, 0x24); // 0x24 EntityID
	OFFSET(Position3D, position_, 0x28); // 0x28
	OFFSET(Vector3, velocity_, 0x34); // 0x34
	OFFSET(Vector3, auxVolatile_, 0x40);	//  0x40 	// 0 - [Head] Yaw of Entity. 	// 1 - Head Pitch of Entity.
	OFFSET(EntityType*, type_, 0x4c); // 0x4c

	 //int unk_gap  // 0x50
		//PyObject* pPyCell_; // PyServer 0x54
		//PyObject* pPyBase_; // PyServer 0x58

	OFFSET(Filter*, pFilter_, 0x5c);				// never NULL // 0x5c
	OFFSET(bool, isPoseVolatile_, 0x60); // 0x60
	OFFSET(bool, isDestroyed_, 0x61);


	//	ChunkEmbodimentPtr	primaryEmbodiment_; // 0x64

	//	ChunkEmbodiments	auxiliaryEmbodiments_;
	//	PySTLSequenceHolder<ChunkEmbodiments>	auxModelsHolder_;

	//	ChunkSpacePtr		pSpace_; // 0x9c
	//	EntityPtr			pVehicle_; // 0xa0

	OFFSET(Physics*, pPhysics_, 0xa4);
	// 0xa4
		//double		physicsCorrected_; // 0xa8
		//mutable EntityID	waitingForVehicle_;
		//bool		tickAdvanced_;

		//uint64		lastInvoked_;
		//int			enterCount_;

		//Capabilities	targetCapabilities_;
		//Capabilities	matchCapabilities_;

		//int			nextTrapID_;
		//struct TrapRecord
		//{
		//	int			id;
		//	int			num;
		//	float		radiusSquared;
		//	PyObject* function;
		//};
		//std::vector<TrapRecord>	traps_;

	OFFSET(Entity*, nextLeft_, 0xd8); // d8
	OFFSET(Entity*, nextRight_, 0xdc); //dc
	OFFSET(Entity*, nextUp_, 0xe0); // e0
	OFFSET(Entity*, nextDown_, 0xe4); // e4

	//	CacheStamps	cacheStamps_;
	OFFSET(int*, SkeletonCollider, 0x108); // e4
		// 0x108 SkeletonCollider
		// 0x10c SkeletonCollider

	//	PrerequisitesOrderPtr		prerequisitesOrder_;
		//ResourceRefs				resourceRefs_;

		//bool		targetFullBounds_;

		//bool invisible_;
		//double visiblityTime_;


};



class EntityManager
{

public:
	static EntityManager* instance();
	Entity* __thiscall getEntity(int id, bool b_check_cache);
};
