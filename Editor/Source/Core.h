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
//                   Macros
//-----------------------------------------------------------------

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

  #define null				0

	//type definitions
	typedef unsigned char			ubyte;	// Unsigned 8 bit value
	typedef signed char				sbyte;	// Signed 8 bit value
	typedef unsigned short		uword;	// Unsigned 16 bit value
	typedef signed short			sword;	// Signed 16 bit value
#ifndef _WIN32
	typedef int32_t				    sdword;	// Signed 32 bit value
	typedef u_int32_t			    udword;	// Unsigned 32 bit value
	typedef int64_t		        sqword;	// Signed 64 bit value
	typedef u_int64_t	        uqword; // Unsigned 64 bit value
#else
	typedef unsigned long			udword;	// Unsigned 32 bit value
	typedef signed long				sdword;	// Signed 32 bit value
	typedef signed __int64		sqword;	// Signed 64 bit value
	typedef unsigned __int64	uqword; // Unsigned 64 bit value
#endif
	typedef udword						ID;			// Identifier	// ###JN### will be removed

	//Runtime class
	typedef	NObject* (RTCLASS_HANDLER)(void);	//Callback for class creation


//-----------------------------------------------------------------
//!	\class		NRTClassModule
//!	\brief		RuntimeClass Module description
//-----------------------------------------------------------------
class  NRTClassModule
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
class  NRTClass
{
public:

//Constructor
	NRTClass(RTCLASS_HANDLER*	_pcreateCB, const char* _pszClassName, const char* _pszSuperClassName, const char* _pszModuleName);

//Methods
	static NObject*		CreateByID(ID _CLASSID);    // ###JN### will be removed
	static NObject*		CreateByName(const char* _pszClassName);
	static NRTClass*	GetRTClassByName(const char* _pszClassName);
	static NRTClass*	GetRTClassByID(ID _CLASSID);// ###JN### will be removed

	static ID					MakeClassID(const char* _pszClassName); // ###JN### will be removed
	static NRTClass*  GetFirstClassBySuperClass(const char* _pszSuperClassName);
	static NRTClass*  GetNextClassBySuperClass(const char* _pszSuperClassName, NRTClass* _prtclass);

//Datas
	RTCLASS_HANDLER*	m_pCreateCB;		//!< CallBack for class creation
	const char*				m_pszClassName;				//!< Class's name
	const char*				m_pszSuperClassName;	//!< Super class's name

	NRTClass*		m_pNextRTC;						//!< Next Run-Time class belong to one module
	ID					CLASSID;							//!< Class ID (Generate from name)// ###JN### will be removed
	ID					SUPERCLASSID;					//!< Super class ID (Generate from name)// ###JN### will be removed
	NRTClassModule* m_pRTClassModule;   //!< Run-Time class Module for this Class
};



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

#ifdef _DEBUG
	#define TRACE	gDebugLog
	void  gDebugLog(const char* fmt, ... );
#else
	#define TRACE
#endif

//Error ###TODO### Error management
				//TRACE("Code:%d Msg:%s\n", errcode, msg);
//#define ERR		gGetErrors()->AddError
#define ERR		printf

//Runtime class
#define FDECLARE_CLASS() \
	static NRTClass	m_RTClass;\
	virtual NRTClass* GetRTClass()		{ return &m_RTClass; }

#define FIMPLEMENT_CLASS(class_name, superclass_name) \
	extern NObject* class_name##CB()	{ return NNEW(class_name); }\
	NRTClass	class_name::m_RTClass((RTCLASS_HANDLER*)&class_name##CB, #class_name, #superclass_name, GetModuleName() );


//Variables
#define		VAR(type,	bCanBeAnimate, pszName,	pszDefValue, pszCLASSGUI) { type,	bCanBeAnimate, pszName,	pszDefValue, pszCLASSGUI },
#define		MAP(version, type, pszMapping,	pszExpression )	{ version,	type, pszMapping,	pszExpression },


//-----------------------------------------------------------------
//!	\class	NErrors Core.h
//!	\brief	Errors manager
//-----------------------------------------------------------------
/*class  NErrors
{
public:
	NErrors();
	~NErrors();

	void	AddError(udword _code, const char* _fmt, ... );
	char* GetErrors();	//!< Errors are clear after this call

protected:
	udword m_dwStringSize;
	udword m_dwStringPos;
	char* m_pszErrors;
};
extern  NErrors* gGetErrors();
*/

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
	erefobj//,
//	erefobj2		// This 'Hack' will allow loading and saving
//					// L_ImageRenderer, L_Curve and L_Terrace
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
	const char*			pszName;
	const char*			pszDefValue;	//!< One or more for combo-box edition
	const char*			pszCLASSGUI;	//!< Graphic control for editing
};

//-----------------------------------------------------------------
//!	\struct NMapVarsBlocDesc
//! \brief	Mapping Variable description for variable bloc
//-----------------------------------------------------------------
struct NMapVarsBlocDesc
{
	ubyte					byVersion;			//!< Bloc Version
	eVarType			eType;					//!< Type of variables (eubyte, euword ...)
	const char*		pszMapping;			//!< ie (">2>3") => transfert this old variable bloc version
																//!								to new variable bloc index 2 and 3
	const char*		pszExpression;	//!< value conversion (* or +)
																//!  ie ("*2.0") to multiply by 2.0
};


//-----------------------------------------------------------------
//!	\class	NVarsBloc Core.h
//!	\brief	Bloc of variables
//-----------------------------------------------------------------
class  NVarsBloc
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

	NMapVarsBlocDesc*	m_pcmapVarsBlocDesc;
	udword				m_dwMapVarsCount;	//!< Map Values Count

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

typedef int(*CompareFnc) (const void *_elem1, const void *_elem2);


class  NObjectArray
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
class  NObject
{
public:
	//Constructor-Destructor
	NObject();
	virtual	~NObject();

	FDECLARE_CLASS();

	//Methods
	virtual void	SetName(const char* _pszName)	{ strncpy(m_szName, _pszName, sizeof(m_szName)); }	//!< Affecte le nom de l'objet
	virtual const char*	GetName()								{ return m_szName;}	//!< Return object name

	virtual NObject* Duplicate();

	//Serialization
	virtual bool Save(NArchive* _s);
	virtual	bool Load(NArchive* _l);

	//Variables methods
	NVarsBloc* AddVarsBloc(udword _dwVarCount, NVarsBlocDesc* _pdesc, ubyte _byVersion=1);
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

class  NTreeNode : public NObject
{
public:
	NTreeNode();
	virtual ~NTreeNode();

	FDECLARE_CLASS();

	//Methods
	NObjectArray& GetObjsArray()		{ return m_carrayObjects; }	//!< Objects Array Access

	void AddSon(NTreeNode* _pnode, udword _idx=-1);
	void DeleteSon(udword _idx);
	void DeleteAllSons();

	NTreeNode*	GetSon()			{ return m_pFirstSon;		}
	NTreeNode*	GetBrother()	{ return m_pBrother;		}

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

class  NObjectGarbage
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


//-----------------------------------------------------------------
//! \brief Mutex-like class for safe thread operation
//! \note Usage: static bool mutex; { NMutexLock lock(mutex); /* will unlock at end of the scope */ }
//! \author Sebastian Olter (qduaty@gmail.com)
//-----------------------------------------------------------------

//###JN### To remove from here
/*
class NMutexLock
{
	protected:
	bool myfault; // can release only if it is set
	bool& _mutex; // global data (i.e. actual mutex)
	bool _debug; // very useful variable
	public:
	NMutexLock(bool& mutex, bool _lock = true, bool debug = false): myfault(false), _mutex(mutex), _debug(debug) { if(_lock) lock(); }
	void lock();
	bool trylock();
	void release();
	~NMutexLock();
};
*/

#endif //CORE_H