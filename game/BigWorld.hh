#pragma once
#include <Windows.h>
#include "Vectors.hh"
#include "python.hh"
#include <map>
#include <vector>
#ifdef DEBUG
#include <iostream>
#endif
#define OFFSET( type, func, offset ) [[nodiscard]] std::add_lvalue_reference_t< type > func()  { return *reinterpret_cast< std::add_pointer_t< type > >( reinterpret_cast< std::uintptr_t >( this ) + offset ); }

class PyHelpers
{
public:
	 static PyStringObject* PyString_InternFromString(const char* name);
};

class Physics;
class Filter;
struct EntityType;


class ImpCamera //  umbra.dll+BE950 screen -> camera pos -> matrix
{
public:
	OFFSET(Matrix4x4, view_matirx, 0x1E0);
	OFFSET(float, Width, 0x30);
	OFFSET(float, Hight, 0x38);
	static ImpCamera* Instance();
	bool WorldToScreen(Vector3 position, Vector2* ScreenPos)
	{
		if (!this) return false;
		auto height = this->Hight();
		auto width = this->Width();
		Matrix4x4 matrix = this->view_matirx();
		float w = 0.0f;
		ScreenPos->x = matrix.m[0][0] * position.x + matrix.m[0][1] * position.y + matrix.m[0][2] * position.z + matrix.m[0][3];
		ScreenPos->y = matrix.m[1][0] * position.x + matrix.m[1][1] * position.y + matrix.m[1][2] * position.z + matrix.m[1][3];
		w = matrix.m[3][0] * position.x + matrix.m[3][1] * position.y + matrix.m[3][2] * position.z + matrix.m[3][3];
		if (w < 0.01f)
			return false;
		float invw = 1.0f / w;
		ScreenPos->x *= invw;
		ScreenPos->y *= invw;
		float x = width / 2;
		float y = height / 2;
		x += 0.5 * ScreenPos->x * width + 0.5;
		if (x > 10000 || x < -10000) return false;
		y -= 0.5 * ScreenPos->y * height + 0.5;
		if (y < -10000 || y > 10000)return false;

		*ScreenPos = { x , y };
		return true;
	}
	// need test after restarts↓
	//OFFSET(Umbra::Cell, 0x2e8, cell); //    Umbra::Cell : Umbra::ReferenceCount
	//OFFSET(Umbra::ImpCamera, 0x2EC, THIS); //   
	//OFFSET(Umbra::Camera, 0x2F0, camera); //   
};


//https://flagbot.ch/lesson6.pdf
class string_impl
{

private:
	const char* str_ptr; //0x0000
	char pad_0004[12]; //0x0004
	int length; //0x0010
	int capacity; //0x0014
public:
	const char* c_str()
	{
		if (this->capacity >= 0x10)
		return this->str_ptr;

		return (const char*)this;
	}
	size_t lenght() { return this->length; }
};

static_assert(sizeof(string_impl) == 24, "string_impl size changed");

template <typename T>
struct vector_impl
{
	T* start;
	T* end;
	T* max;

		bool at(int idxc, T* ref )
	     {
			if (!start) return 0;
			if (end < &start[idxc]) // out of array idx
				return false;
			if (start > &start[idxc]) // negative index
				return false;
			*ref = start[idxc];
			return true;
	     }
		int count()
		{
			if (!start) return 0;
			auto offset = 	 (uintptr_t)end - (uintptr_t)start;
			return offset / sizeof(T);
		}
		
};


struct VolatileInfo
{
	float	positionPriority_;
	float	yawPriority_;
	float	pitchPriority_;
	float	rollPriority_;
};

struct DataTypeMetadata
{
	int vtable;
	string_impl name_;
};
struct DataTypeImpl
{
	int vtable;
	int id;
	DataTypeMetadata* metadata;
	string_impl* GetName()
	{
		auto m_data = this->metadata;
		if (m_data)
			return &m_data->name_;

		return 0;
	}
};

class DataDescription
{
public:
	enum PARSE_OPTIONS
	{
		PARSE_DEFAULT,			// Parses all known sections.
		PARSE_IGNORE_FLAGS = 1	// Ignores the 'Flags' section.
	};
	string_impl name_;
	DataTypeImpl* /*DataTypePtr*/	pDataType_;
	int			dataFlags_;
	void*  /*PyObjectPtr*/	pInitialValue_;
	void*	pDefaultSection_;
	int			index_;
	int			localIndex_;		// Index into local prop value vector.
	int			eventStampIndex_;	// Index into time-stamp vector.
	int			clientServerFullIndex_;
	int			detailLevel_;
	int			databaseLength_;
	int unknown;
};






struct EntityDescription_vftbl {
	//void (__thiscall*Destructor_BaseUserDataObjectDescription)();
	//void (__thiscall*parse)(string_impl name,
	//	void* pSection, bool isFinal);	
	//DataDescription(__thiscall*findProperty)(string_impl name);
};

enum BWCompressionType : int
{
	BW_COMPRESSION_NONE,

	BW_COMPRESSION_ZIP_1,
	BW_COMPRESSION_ZIP_2,
	BW_COMPRESSION_ZIP_3,
	BW_COMPRESSION_ZIP_4,
	BW_COMPRESSION_ZIP_5,
	BW_COMPRESSION_ZIP_6,
	BW_COMPRESSION_ZIP_7,
	BW_COMPRESSION_ZIP_8,
	BW_COMPRESSION_ZIP_9,

	BW_COMPRESSION_ZIP_BEST_SPEED = BW_COMPRESSION_ZIP_1,
	BW_COMPRESSION_ZIP_BEST_COMPRESSION = BW_COMPRESSION_ZIP_9,

	BW_COMPRESSION_DEFAULT_INTERNAL,
	BW_COMPRESSION_DEFAULT_EXTERNAL,
};




struct EntityDescription {
	EntityDescription_vftbl* vtbl;
	string_impl  name; 
	vector_impl <DataDescription> properties_; // From  : BaseUserDataObjectDescription
	char padstdMap[8]; // std map // From  : BaseUserDataObjectDescription
	/// END : BaseUserDataObjectDescription
	// EntityDescription Begin
	int /*EntityTypeID*/		index_;
    int /*EntityTypeID*/		clientIndex_;
	std::string			clientName_;
	bool				hasCellScript_;
	bool				hasBaseScript_;
	bool				hasClientScript_;
	char pad[0x1]; // unused;
	VolatileInfo 		volatileInfo_;
	BWCompressionType	internalNetworkCompressionType_;
	BWCompressionType	externalNetworkCompressionType_;
};







//https://github.com/v2v3v4/BigWorld-Engine-2.0.1/blob/620e0f244739188a5b183f1f28b278f16d182370/src/client/entity_type.hpp
struct EntityType
{
	 EntityDescription* description_;
	 PyObject* pModule_;
	 PyTypeObject* pClass_;
	 PyTypeObject* pPlayerClass_;
	 
};
/*: public PyInstancePlus : public PyObject*/
class Entity : public  PyObject
{
public:
	OFFSET(int, id, 0x24); // 0x24 EntityID
	OFFSET(Vector3, position, 0x28); // 0x28 //  real type Position3D == Vector3[2]
	OFFSET(Vector3, velocity, 0x34); // 0x34
	OFFSET(Vector3, auxVolatile, 0x40);	//  0x40 	// 0 - [Head] Yaw of Entity. 	// 1 - Head Pitch of Entity.
	OFFSET(EntityType*, E_type, 0x4c); // 0x4c


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
	inline const char* GetNameClass() {
		auto etype = this->E_type();
		if (!etype) return nullptr;
		auto tp = etype->pClass_;
		if (!tp) return nullptr;
		return tp->GetName();
	}

#ifdef DEBUG
	void DumpProperties()
	{

		    auto etype = this->E_type();
		    if (!etype) return;
			auto pclass = etype->pClass_;
			if (!pclass) return;
			auto description = etype->description_;
			if (!description) return;
			auto properties = description->properties_;


			auto fullname = this->ob_type->GetFullName();

			std::cout << fullname << "\n{ \n";

			for (int bb = 0; bb < properties.count(); bb++)
			{
				DataDescription desc{};
				if (!properties.at(bb, &desc)) return;

				auto Data_type = desc.pDataType_;
				if (!Data_type)  continue;
				auto type_string = Data_type->GetName();
				if (!type_string) continue;


				std::cout << type_string->c_str() << " " << desc.name_.c_str() << " \n";

			}
			std::cout << "\n} \n";


	}
#endif
};

struct dymmy
{
	char pad[0xc];
	Entity* entity;
};
struct myMapEntry
{
	myMapEntry* _next; // pointer to _next
	dymmy* pp_Entity;
	int EntityId;
};

struct myMap
{
	myMapEntry* _first_ ; 
	size_t count;
};

class EntityManager
{
	
public:

	OFFSET(int, LocalPlayerId, 0x8);


	OFFSET(myMap, EntiredEntities, 0x10);


	static EntityManager* instance();
	Entity* __thiscall getEntity(int id, bool b_check_cache = false);
};
