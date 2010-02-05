//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Core.h
//! \brief	Core
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef CORE_H
#define CORE_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "../Include/CoreLib.h"


//-----------------------------------------------------------------
// Class Prototypes
//-----------------------------------------------------------------
  class NRTClassModule;
	class NRTClass;
	class NArchive;
	class NObject;
	class NVarsBloc;
	class NObjectArray;
	class NObjectGarbage;
	//class NErrors;

//-----------------------------------------------------------------
// Types
//-----------------------------------------------------------------

	//typedef udword						ID;			// Identifier	// ###JN### will be removed

	//Runtime class
	typedef	NObject* (RTCLASS_HANDLER)(void);	//Callback for class creation

//-----------------------------------------------------------------
//!	\struct		NFieldDesc
//!	\brief		field description struct
//-----------------------------------------------------------------
struct NFieldDesc
{
	ubyte				byType;
	const char*	pszName;
	udword			dwDataOffset;
	float				fMin;
	float				fMax;
	float				fStep;
	const char*	pszDef;
};


//-----------------------------------------------------------------
//!	\class		NRTClassModule
//!	\brief		RuntimeClass Module description
//-----------------------------------------------------------------
class CORELIB_API NRTClassModule
{
public:
  NRTClassModule();

  //Methods
  static NRTClassModule* RegisterModule(const char* _pszModuleName);
  static void UnRegisterAllModules();

  //Datas
	const char*			m_pszModuleName;				//!< Module's name

	NRTClassModule* m_pNextRTClassModule;   //!< Next Run-Time class Module

	static NRTClassModule* m_pFirstRTClassModule;   //!< First Run-Time class Module
	static NRTClassModule* m_pLastRTClassModule;    //!< Last Run-Time class Module

  //Datas for RTClasses for this module
	NRTClass* m_pFirstRTClass;	//!< First RTClass description for this module
	NRTClass* m_pLastRTClass;	  //!< Last RTClass description for this module
};

extern const char* GetModuleName();  //!< Must had been defined in application and per dynamic libraries(dll, so)


//-----------------------------------------------------------------
//!	\class		NRTClass
//!	\brief		RuntimeClass description
//-----------------------------------------------------------------
class CORELIB_API NRTClass
{
public:

//Constructor
	NRTClass(RTCLASS_HANDLER*	_pcreateCB, const char* _pszClassName, const char* _pszSuperClassName, const char* _pszModuleName, NFieldDesc* _paFieldsDesc);

//Methods
	static NObject*		CreateByName(const char* _pszClassName);
	static NRTClass*	GetRTClassByName(const char* _pszClassName);

	static NRTClass*  GetFirstClassDerivedFrom(const char* _pszSuperClassName);
	static NRTClass*  GetNextClassDerivedFrom(const char* _pszSuperClassName, NRTClass* _prtclass);

//Datas
	RTCLASS_HANDLER*	m_pCreateCB;					//!< CallBack for class creation
	const char*				m_pszClassName;				//!< Class's name
	const char*				m_pszSuperClassName;	//!< Super class's name

	NFieldDesc*				m_paFieldsDesc;				//!< Fields description array for this class level

	NRTClass*				m_pNextRTC;					//!< Next Run-Time class belong to one module
	NRTClassModule* m_pRTClassModule;   //!< Run-Time class Module for this Class
};



//-----------------------------------------------------------------
//!	\class		NRTClassFields
//!	\brief		RuntimeClass Fields description
//-----------------------------------------------------------------
/*class CORELIB_API NRTClassFields
{
public:
//Methods
	NFieldDesc* GetFieldDescByName(const char* _pszFieldName);	//!< Parse bases classes too

	static NRTClassFields*	GetRTFieldsByName(const char* _pszClassName);

//Datas
	const char*				m_pszClassName;				//!< Class's name
	NRTClassFields*		m_pSuperClassRTField;	//!< Super class's RTField
	NFieldDesc*				m_paFieldsDesc;				//!< Fields description array for this class level
};*/


//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define		MAX_NAMELEN					256
#define		MAX_PATHLEN					256
#define		OBJARRAY_GROWSIZE		16

//#ifndef max
#define nmax(a,b)            (((a) > (b)) ? (a) : (b))
//#endif

//#ifndef min
#define nmin(a,b)            (((a) < (b)) ? (a) : (b))
//#endif

//-----------------------------------------------------------------
// Macros
//-----------------------------------------------------------------

#define ERR		printf

//Runtime class
#define FDECLARE_CLASS() \
	static NRTClass	m_RTClass;\
	virtual NRTClass* GetRTClass()		{ return &m_RTClass; } \
	static NFieldDesc	m_Table[];

#define FIMPLEMENT_CLASS(class_name, superclass_name) \
	extern NObject* class_name##CB()	{ return NNEW(class_name); }\
	NRTClass	class_name::m_RTClass((RTCLASS_HANDLER*)&class_name##CB, #class_name, #superclass_name, GetModuleName(), class_name::m_Table );\
	NFieldDesc	class_name::m_Table[] = { 

#define FIMPLEMENT_CLASS_END() \
	{ -1, "end", 0, 0.0,0.0,0.0, "" } };

//Fields Macros
/*#define		FBEGIN_FIELDS_CLASS(class_name) \
				NRTClassFields		class_name::m_RTField = { NULL, class_name::m_Table }; \
				NFieldDesc	class_name::m_Table[] = {

#define		FBEGIN_FIELDS_SUBCLASS(class_name, superclass_name) \
				NRTClassFields		class_name::m_RTField = { &superclass_name::m_RTField, class_name::m_Table }; \
				NFieldDesc	class_name::m_Table[] = {
*/
//#define		FEND_FIELDS() \
//			{ -1, "end", 0 } };

/*#define		FDECLARE_FIELDS() \
				static NFieldDesc	m_Table[]; \
				static NRTClassFields		m_RTField; \
				virtual NRTClassFields* GetRTField()	{ return &m_RTField; }*/

#define		OFFSET(c,m)	(size_t)&(((c *)0)->m)
#define		NEWFIELD(type, name, classname, classmember, min, max, step, def)	{ type, name, OFFSET(classname, classmember), min, max, step, def	}

//-----------------------------------------------------------------
//!	\enum		eFieldType
//!	\brief	field types
//-----------------------------------------------------------------
enum eFieldType
{
	eInteger = 0,		//"Def,Min,Max,Step"
	eFloat,					//"Def,Min,Max,Step"
	eString,				//"MaxLen"
	eBool,					//"Def"
	eRefObj,				//""
	eRgb,						//"Def(R,G,B)"
	eRgba,					//"Def(R,G,B,A)"
	eIVec2,					//"Def(x,y),Min,Max,Step"    => Integers XY
	eIVec3,					//"Def(x,y,z),Min,Max,Step"  => Integers XYZ
	eIRect,					//"Def(x1,y1,x2,y2),Min,Max,Step"    => Integers (X1,Y1)-(X2,Y2)
	eFVec2,					//"Def(x,y),Min,Max,Step"    => Floats XY
	eFVec3,					//"Def(x,y,z),Min,Max,Step"  => Floats XYZ
	eFRect,					//"Def(x1,y1,x2,y2),Min,Max,Step"    => Floats (X1,Y1)-(X2,Y2)
	eFile,					//"MaxLen"
	eCombo, 				//"Def,[1|2|3...]"
	eBlobText,			//"MaxLen"
  eBlobBinary,		//"MaxSize"
  ePow            //Def
  //eTexture ...
	//...
};

//-----------------------------------------------------------------
//!	\class	NObjectArray Core.h
//!	\brief	Class for objects collections
//-----------------------------------------------------------------

//The return value of CompareFnc is show below in the relationship of elem1 to elem2:
//	< 0		if elem1 less than elem2
//	  0		if elem1 identical to elem2
//	> 0		if elem1 greater than elem2

typedef int(*CompareFnc) (const void *_elem1, const void *_elem2);


class CORELIB_API NObjectArray
{
public:
	NObjectArray();
	virtual	~NObjectArray();

	//Serialization
	bool Save(NArchive* _s);	//!< Save object
	bool Load(NArchive* _l);	//!< Load object

	//Methods
	void SetManageDelete(bool _bAutoDelete)		{ m_bManagedDel=_bAutoDelete; }	//!< false by default

	void		Clear();		//!< Clear array

	udword	Count()			{ return m_dwCount;	}	//!< Return used entries
	udword	Size()			{ return m_dwSize;	}	//!< Return array's buffer size

  udword	AddItem(NObject* _item, udword _idx=0xFFFFFFFF);	//!< Add an item at index into the array ( return index)
	void		RemoveItem(udword _idx);									//!< Remove an item
	void		SetCount(udword _c);
	void		SetSize(udword _s);

	void		AddArray(NObjectArray& _array);

	udword	Find(NObject* _item);							//!< Return index if found else -1

	void		Sort(CompareFnc _cmp);					//Sorts the array using the compare function

	//Operators
	NObject*	operator[]	(udword _index)			{ return m_pBuffer[_index];	}

protected:
	//Datas
	bool			m_bManagedDel;	//!< Auto managed objects delete (Default false)
	NObject**	m_pBuffer;			//!< NObject** array
	udword		m_dwCount;			//!< used items in array
	udword		m_dwSize;				//!< Size of array (in items)
};



//-----------------------------------------------------------------
//!	\class	NObject Core.h
//!	\brief	Base class for all objects
//!	\note
//!		.Automatic variables serialisation
//!		.References management
//-----------------------------------------------------------------
class CORELIB_API NObject
{
public:
	//Constructor-Destructor
	NObject();
	virtual	~NObject();

	FDECLARE_CLASS();

	//Methods
	virtual NObject* Duplicate();
	virtual const char*	GetName()	{ return null;}	//!< Return object name

	//Serialization
	virtual bool Save(NArchive* _s);
	virtual	bool Load(NArchive* _l);

	//A reference creates a record of the dependency between a Reference Master and a Reference Target
	//rk. The system automatically takes care of loading and saving references when an object is saved to disk.

	//Objects referenced by Me (this = Reference Maker)
	//	i.e This object (Reference Maker) that reference another objets (References Targets)
	udword		GetRefCount()						{ return m_carrayRefTargets.Count();	}
	NObject*	GetRef(udword _idx)			{ return m_carrayRefTargets[_idx];			}
	void			AddRef(NObject* _pobj);
	void			RemoveRef(NObject* _pobj);

	//Objects that references Me (this = Reference Target)
	//	i.e This object (Reference Target) that is referenced by another objets (References Makers)
	udword		GetRefToMeCount()						{ return m_carrayRefMakers.Count();	}
	NObject*	GetRefToMe(udword _idx)			{ return m_carrayRefMakers[_idx];			}
	void			AddRefToMe(NObject* _pobj);
	void			RemoveRefToMe(NObject* _pobj);

protected:
	//Datas
	NObjectArray			m_carrayRefTargets;				//!< Objects referenced by this object
	NObjectArray			m_carrayRefMakers;				//!< Objects that reference this object

public:
	udword						m_dwLastUsedTime;					//!< See garbage
};



//-----------------------------------------------------------------
//!	\class		NTreeNode
//!	\brief		NObject collections in hierarchy form
//-----------------------------------------------------------------
// +Parent
//	+Son/Brother idx0
//	+Son/Brother idx1

class CORELIB_API NTreeNode : public NObject
{
public:
	NTreeNode();
	virtual ~NTreeNode();

	FDECLARE_CLASS();

	//Methods
	NObjectArray& GetObjsArray()		{ return m_carrayObjects; }	//!< Objects Array Access

  void AddSon(NTreeNode* _pnode, udword _idx=0xFFFFFFFF);
	void DeleteSon(udword _idx);
	void DeleteAllSons();

	NTreeNode*	GetSon()			{ return m_pFirstSon;		}
	NTreeNode*	GetBrother()	{ return m_pBrother;		}

	void	SetName(const char* _pszName)	{ strncpy(m_szName, _pszName, sizeof(m_szName)); }	//!< Affecte le nom de l'objet
	virtual const char*	GetName()								{ return m_szName;}	//!< Return object name

	NTreeNode*	GetSonFromName(const char* _pszNodeName);

	udword GetSonsCount();

	udword FindSon(NTreeNode* _pnode);
	static NObject* _FindNodeFromClassName(NTreeNode* _pParent, char* _pszClassName);

	//Serialization
	virtual bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object and its hierarchie

protected:
	//Datas
	NObjectArray	m_carrayObjects;	//!< Objects array
	NTreeNode*		m_pBrother;
	NTreeNode*		m_pFirstSon;
	NTreeNode**		m_ppLastSon;
	char					m_szName[MAX_NAMELEN+1];	//!< Node's name
};

//-----------------------------------------------------------------
//!	\class		NObjectGarbage
//!	\brief		Object garbage collector
//-----------------------------------------------------------------
struct NObjGarbageDesc
{
	NObject** ppobj;
	ubyte			byType;
};

class CORELIB_API NObjectGarbage
{
public:
	NObjectGarbage();
	~NObjectGarbage();

	void SetManagedClass(const char* _pszClassName);
	void Compact(ubyte _byTypeMask, udword _dwTimevalidityMs);
	void GetInstance(NObject** _ppobj, ubyte _byAsType=1);	//!< Return an instance
	void RemoveEntry(NObject** _ppobj);

	//SetMaxUsedMemory()	//!< if max used memory reached, Compact() is called while GetInstance()

	udword		GetCount()	{	return m_dwCount; }
//	NObject*	GetObjectAt(udword _idx);

protected:
	NObjGarbageDesc*	m_aobjects;
	udword						m_dwCount;
	udword						m_dwSize;
	const char*				m_pszClassName;
};




//-----------------------------------------------------------------
//	File System
//-----------------------------------------------------------------
/*
NFILEHANDLE *NFileOpen(void *data, signed char type, int length);
void				NFileClose(NFILEHANDLE *handle);
int					NFileRead(void *buffer, int size, NFILEHANDLE *handle);
void				NFileSeek(NFILEHANDLE *handle, int pos, signed char mode);
int					NFileTell(NFILEHANDLE *handle);
*/

//-----------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------
	#include "MemoryMgr.h"
	#include "Stream.h"
	#include "Archive.h"

#endif //CORE_H
