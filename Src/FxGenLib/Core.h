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
#pragma once

#ifdef __GNUC__
#include "gcccompat/gcccompat.h"
#endif

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------
#ifdef FXGEN_EXPORTS
	#define FXGEN_API __declspec(dllexport)
#else
	#define FXGEN_API __declspec(dllimport)
#endif

//-----------------------------------------------------------------
// Class Prototypes
//-----------------------------------------------------------------
	class NRTClass;
	class NArchive;
	class NObject;
	class NVarsBloc;
	class NObjectArray;
	class NObjectGarbage;

//-----------------------------------------------------------------
// Types
//-----------------------------------------------------------------

	#define null				0

	//type definitions
	typedef unsigned char			ubyte;	// Unsigned 8 bit value
	typedef signed char				sbyte;	// Signed 8 bit value
	typedef unsigned short		uword;	// Unsigned 16 bit value
	typedef signed short			sword;	// Signed 16 bit value
	typedef unsigned long			udword;	// Unsigned 32 bit value
	typedef signed long				sdword;	// Signed 32 bit value
	typedef signed __int64		sqword;	// Signed 64 bit value
	typedef unsigned __int64	uqword; // Unsigned 64 bit value
	typedef udword						ID;			// Identifier

	//Runtime class
	typedef	NObject* (__cdecl RTCLASS_HANDLER)(void);	//Callback for class creation

//-----------------------------------------------------------------
//!	\class		NRTClass
//!	\brief		RuntimeClass description
//-----------------------------------------------------------------
class FXGEN_API NRTClass
{
public:

//Constructor
	NRTClass(RTCLASS_HANDLER*	_pcreateCB, const char* _pszClassName, const char* _pszSuperClassName);

//Methods
	static NObject*		CreateByID(ID _CLASSID);
	static NObject*		CreateByName(const char* _pszClassName);
	static ID					MakeClassID(const char* _pszClassName);

//Datas
	RTCLASS_HANDLER*	m_pCreateCB;		//!< CallBack for class creation
	const char*				m_pszClassName;				//!< Class's name
	const char*				m_pszSuperClassName;	//!< Super class's name

	NRTClass*		m_pNextRTC;						//!< Next Run-Time class
	ID					CLASSID;							//!< Class ID (Generate from name)
	ID					SUPERCLASSID;					//!< Super class ID (Generate from name)

	static NRTClass* m_pFirstRTClass;	//!< First RTClass description
	static NRTClass* m_pLastRTClass;	//!< Last RTClass description
};



//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define		MAX_NAMELEN					256
#define		MAX_PATHLEN					256
#define		OBJARRAY_GROWSIZE		16

#ifdef _DEBUG
#define TRACE	gDebugLog
void FXGEN_API gDebugLog(const char* fmt, ... );
#else
#define TRACE
#endif


//-----------------------------------------------------------------
// Macros
//-----------------------------------------------------------------

//Log and error
#define ERR(msg, errcode)		{	TCHAR buffer[256]; \
				wsprintf( buffer, ("File: %s\n\rLine: %ld\n\r Msg:%s\n\rCode:%d"), __FILE__, (udword)__LINE__, msg, errcode  ); \
				::MessageBox(null, buffer, "FxGen Error", MB_OK|MB_ICONERROR|MB_APPLMODAL|MB_TOPMOST); \
			}

//Runtime class
#define DECLARE_CLASS() \
	static NRTClass	m_RTClass;\
	virtual NRTClass* GetRTClass()		{ return &m_RTClass; }

#define IMPLEMENT_CLASS(class_name, superclass_name) \
	extern NObject* class_name##CB()	{ return new class_name(); }\
	NRTClass	class_name::m_RTClass((RTCLASS_HANDLER*)&class_name##CB, #class_name, #superclass_name );


//Variables
#define		VAR(type,	bCanBeAnimate, pszName,	pszDefValue, pszCLASSGUI) type,	bCanBeAnimate, pszName,	pszDefValue, pszCLASSGUI,
#define		MAP(version, type, pszMapping,	pszExpression )	version,	type, pszMapping,	pszExpression,


//-----------------------------------------------------------------
//!	\enum		eVarType
//!	\brief	variable types
//-----------------------------------------------------------------
enum eVarType
{
	eubyte = 0,
	euword,
	eudword,
	efloat,
	estring,
	erefobj,
};

//-----------------------------------------------------------------
//!	\struct NVarValue
//! \brief	A variable value desription
//-----------------------------------------------------------------
struct NVarValue
{
public:
	//Value
	union
	{
		ubyte			byVal;
		uword			wVal;
		udword		dwVal;
		float			fVal;
		char			szVal[256];
		NObject*	pcRefObj;
	};

	//Anim control
	NObject*	pcCtrlObj;	//!< different to null if animated
};

//-----------------------------------------------------------------
//!	\struct NVarsBlocDesc
//! \brief	Variable description for variable bloc
//-----------------------------------------------------------------
struct NVarsBlocDesc
{
	eVarType	eType;					//!< Type of variables (eubyte, euword ...)
	bool			bCanBeAnimate;	//!< True if this variables can be animate

	//Variables for Editing ...
	char*			pszName;
	char*			pszDefValue;	//!< One or more for combo-box edition
	char*			pszCLASSGUI;	//!< Graphic control for editing
};

//-----------------------------------------------------------------
//!	\struct NMapVarsBlocDesc
//! \brief	Mapping Variable description for variable bloc
//-----------------------------------------------------------------
struct NMapVarsBlocDesc
{
	ubyte			byVersion;			//!< Bloc Version
	eVarType	eType;					//!< Type of variables (eubyte, euword ...)
	char*			pszMapping;			//!< ie (">2>3") => transfert this old variable bloc version
														//!								to new variable bloc index 2 and 3
	char*			pszExpression;	//!< value conversion (* or +)
														//!  ie ("*2.0") to multiply by 2.0
};


//-----------------------------------------------------------------
//!	\class	NVarsBloc Core.h
//!	\brief	Bloc of variables
//-----------------------------------------------------------------
class FXGEN_API NVarsBloc
{
public:
	NVarsBloc();
	~NVarsBloc();

	//Serialization
	bool Save(NArchive* _s);	//!< Save Object
	bool Load(NArchive* _l);	//!< Load Object

	//Methods
	void	Init(udword _dwVarsCount, NVarsBlocDesc* _pvarsBlocDesc, NObject* _powner, ubyte _byVersion);
	void	SetMapVarBlocDesc(udword _dwVarsCount, NMapVarsBlocDesc* _pmapVarsBlocDesc);

	udword					Count()				{ return m_dwVarsCount;			}
	NVarsBlocDesc*	GetBlocDesc()	{ return m_pcvarsblocDesc;	}
	NVarValue*			GetValues()		{ return m_paVarsValues;		}

	bool	IsAnimated();	//!< return true if one variable is animated

	void	RemoveVarsRef(NObject* _pobj);	//!< Remove a referenced object from variables (erefobj)

	void	GetValue(udword _idx, float _fTime, ubyte&		_val);	//!< Return variable value
	void	GetValue(udword _idx, float _fTime, uword&		_val);	//!< Return variable value
	void	GetValue(udword _idx, float _fTime, udword&		_val);	//!< Return variable value
	void	GetValue(udword _idx, float _fTime, float&		_val);	//!< Return variable value
	void	GetValue(udword _idx, float _fTime, NObject*&	_val);	//!< Return variable value
	void	GetValue(udword _idx, float _fTime, char*&		_val);	//!< Return variable value

	void	SetValue(udword _idx, float _fTime, ubyte			_val);	//!< Change variable value
	void	SetValue(udword _idx, float _fTime, uword			_val);	//!< Change variable value
	void	SetValue(udword _idx, float _fTime, udword		_val);	//!< Change variable value
	void	SetValue(udword _idx, float _fTime, float			_val);	//!< Return variable value
	void	SetValue(udword _idx, float _fTime, NObject*	_val);	//!< Change variable value
	void	SetValue(udword _idx, float _fTime, const char*			_val);	//!< Change variable value

	NObject *GetOwner() { return m_powner; }

protected:
	//Methods
	void DoVarBlocVersion_Mapping(NArchive* _l, ubyte _byVersion);
	void MapValueTo(double _val, udword _idx, const char* _pszExpression);
	void MapValueTo(NObject* _val, udword _idx);
	void MapValueTo(const char* _val, udword _idx);

	//Datas
	NObject*						m_powner;						//!< Object that contain this varsbloc
	NVarsBlocDesc*			m_pcvarsblocDesc;		//!< Variables Descriptions
	udword							m_dwVarsCount;			//!< Values Count
	NVarValue*					m_paVarsValues;			//!< Values array
	ubyte								m_byVersion;				//!< Bloc version (for serialization)

	NMapVarsBlocDesc*		m_pcmapVarsBlocDesc;
	udword							m_dwMapVarsCount;	//!< Map Values Count

	NVarsBloc*					m_pcnextVarsBloc;		//!< Pointer on next variables bloc

	friend class NObject;
};


//-----------------------------------------------------------------
//!	\class	NObjectArray Core.h
//!	\brief	Class for objects collections
//-----------------------------------------------------------------

//The return value of CompareFnc is show below in the relationship of elem1 to elem2:
//	< 0		if elem1 less than elem2
//	  0		if elem1 identical to elem2
//	> 0		if elem1 greater than elem2

typedef int( __cdecl *CompareFnc) (const void *_elem1, const void *_elem2);


class FXGEN_API NObjectArray
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

	udword	AddItem(NObject* _item, udword _idx=-1);	//!< Add an item at index into the array ( return index)
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
class FXGEN_API NObject
{
public:
	//Constructor-Destructor
	NObject();
	virtual	~NObject();

	DECLARE_CLASS();

	//Methods
	virtual void	SetName(const char* _pszName)	{ strncpy_s(m_szName, sizeof(m_szName), _pszName, sizeof(m_szName)); }	//!< Affecte le nom de l'objet
	virtual const char*	GetName()								{ return m_szName;}	//!< Return object name

	virtual NObject* Duplicate();

	//Serialization
	virtual bool Save(NArchive* _s);
	virtual	bool Load(NArchive* _l);

	//Variables methods
	NVarsBloc* AddVarsBloc(udword _dwVarCount, NVarsBlocDesc* _pdesc, ubyte _byVersion);
	NVarsBloc* GetFirstVarsBloc()		{ return m_pcfirstVarsBloc; }
	void			 RemoveVarsRef(NObject* _pobj);

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
	char							m_szName[MAX_NAMELEN+1];	//!< Objet's name
	NObjectArray			m_carrayRefTargets;				//!< Objects referenced by this object
	NObjectArray			m_carrayRefMakers;				//!< Objects that reference this object
	NVarsBloc*				m_pcfirstVarsBloc;				//!< Pointer on first variables bloc

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

class FXGEN_API NTreeNode : public NObject
{
public:
	NTreeNode();
	virtual ~NTreeNode();

	DECLARE_CLASS();

	//Methods
	NObjectArray& GetObjsArray()		{ return m_carrayObjects; }	//!< Objects Array Access

	void AddSon(NTreeNode* _pnode, udword _idx=-1);
	void DeleteSon(udword _idx);
	void DeleteAllSons();

	NTreeNode*	GetSon()			{ return m_pFirstSon;		}
	NTreeNode*	GetBrother()	{ return m_pBrother;		}

	udword GetSonsCount();

	udword FindSon(NTreeNode* _pnode);

	//Serialization
	virtual bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object and its hierarchie

protected:
	//Datas
	NObjectArray	m_carrayObjects;	//!< Objects array
	NTreeNode*		m_pBrother;
	NTreeNode*		m_pFirstSon;
	NTreeNode**		m_ppLastSon;

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

class FXGEN_API NObjectGarbage
{
public:
	NObjectGarbage();
	~NObjectGarbage();

	void SetManagedClassID(ID _CLASSID);
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
	ID								m_CLASSID;
};


//-----------------------------------------------------------------
//	Memory System
//-----------------------------------------------------------------
#define NMemFree(_ptr)					free(_ptr)					//!< Free memory
#define NMemAlloc(_len)					calloc(_len, 1)			//!< Allocate memory
#define NMemRealloc(_ptr, _len)	realloc(_ptr, _len)	//!< Reallocate memory

//NMemCopy
//NMemFill

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
	#include "Maths.h"
	#include "Stream.h"
	#include "Archive.h"
	#include "Bitmap.h"
	#include "Controllers.h"
