//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Archive.h
//! \brief	Serializer for datas loading and saving
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

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Core.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define		NSF_HEADERID				0x0a0b
#define		NSF_VERSION					0x1000

#define		NSF_BUFSTARTSIZE		16000
#define		NSF_BUFGROWSIZE			16000

#define		NSF_GUIDSARRAYSIZE	32
#define		NSF_GUIDSGROWSIZE		32

#define		NSF_CLASS_TAG				0x0001
#define		NSF_MAPPEDCLASS_TAG	0x0002

//-----------------------------------------------------------------
//! \struct NSFHeader
//! \brief	Header File Storage
//-----------------------------------------------------------------
struct NSFHeader
{
	uword				NSFId;
	uword				Version;
	uword				GUIDCount;
	uword				MappedObjsCount;
	udword			MappedObjsOffset;	//From Start File
	//Then GUIDs ...
	//Then Datas ...
	//Then Mapped Objects ...
};

//-----------------------------------------------------------------
//	Prototypes
//-----------------------------------------------------------------
class NObject;

//-----------------------------------------------------------------
//!	\class	NArchive
//!	\brief	Serializer for datas loading and saving
//-----------------------------------------------------------------
class FXGEN_API NArchive
{
public:
	//Constructor-Destructor
					NArchive();
	virtual	~NArchive();

	//File Methods
	bool		Open(const char* _filename, bool _writing=false);
	void		Close();

	//Writing Methods
	void	PutClass(NObject* _c);
	void	PutDatas(void* _buf, udword _length);

	void operator<<( uword _val	)	{ PutDatas(&_val, sizeof(uword));}
	void operator<<( sword _val	)	{ PutDatas(&_val, sizeof(sword));}
	void operator<<( udword _val)	{ PutDatas(&_val, sizeof(udword));}
	void operator<<( sdword _val)	{ PutDatas(&_val, sizeof(sdword));}
	void operator<<( ubyte _val	)	{ PutDatas(&_val, sizeof(ubyte));}
	void operator<<( sbyte _val	)	{ PutDatas(&_val, sizeof(sbyte));}
	void operator<<( float _val	)	{ PutDatas(&_val, sizeof(float));}
	void operator<<( bool _val	)	{ PutDatas(&_val, sizeof(bool));}
	void operator<<( char* _val	)	{ uword l=(uword)strlen(_val); PutDatas(&l, sizeof(uword)); PutDatas(_val, l);}


	//Reading Methods
	NObject*	GetClass();
	void			GetDatas(void* _buf, udword _length);

	void operator>>( uword &_val	)	{ GetDatas(&_val, sizeof(uword));}
	void operator>>( sword &_val	)	{ GetDatas(&_val, sizeof(sword));}
	void operator>>( udword &_val)	{ GetDatas(&_val, sizeof(udword));}
	void operator>>( sdword &_val)	{ GetDatas(&_val, sizeof(sdword));}
	void operator>>( ubyte &_val	)	{ GetDatas(&_val, sizeof(ubyte));}
	void operator>>( sbyte &_val	)	{ GetDatas(&_val, sizeof(sbyte));}
	void operator>>( float &_val	)	{ GetDatas(&_val, sizeof(float));}
	void operator>>( bool &_val	)	{ GetDatas(&_val, sizeof(bool));}
	void operator>>( char* _val	)	{ uword l; GetDatas(&l, sizeof(uword)); GetDatas(_val, l); _val[l]=0;}

	//Mapped object Methods
	void			PutMappedObj(NObject* _c);
	NObject*	GetMappedObj();

protected:
	//Methods
	uword		AddUniqueGUID(ID _guidClass);					//!< Return ID index into m_pGuidsArray
	ID			GetGUIDFromIdx(uword _idx);						//!< Return ID* from an m_pGuidsArray's index
	uword		AddUniqueMappedObjs(NObject* _pobj);	//!< Return Nobject index into m_carrayMappedObjs


	void		PutObj(NObject* _c);									//!< Write class datas to archive
	bool		Save();
	bool		Read();

	//Datas
	FILE*			m_pStream;		//###TOFIX###

	ubyte*		m_pbyBuffer;
	udword		m_dwBufSize;
	udword		m_dwBufPos;
	bool			m_bWriting;

	uword					m_wGuidsCount;			//!< Unique Guids count
	uword					m_wGuidsSize;				//!< Unique Guids array size
	ID*						m_pGuidsArray;			//!< Unique GUIDs array
	NObjectArray	m_carrayMappedObjs;	//!< Unique Mapped objects
};

