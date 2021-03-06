//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Archive.h
//! \brief	Serializer for project archive loading and saving
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!					Anders Stenberg (anders.stenberg@gmail.com)
//!
//!	\date		23-07-2008
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef ARCHIVE_H
#define ARCHIVE_H

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Core.h"

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define		NSF_HEADERID				0x0a0b

//#define NSF_VERSION		0x1000	//1.0.0.0
#define		NSF_VERSION					0x1001	//1.0.0.1	(Class GUID Removed, Class name added)

#define		NSF_BUFSTARTSIZE		16000
#define		NSF_BUFGROWSIZE			16000

#define		NSF_RTCLASSESARRAYSIZE	32	//In elements
#define		NSF_RTCLASSESGROWSIZE		32	//In elements

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
	uword				RTClassesCount;
	uword				MappedObjsCount;
	udword			MappedObjsOffset;	//Offset from Start File
	udword			DatasOffset;			//Offset from Start File
	//Then RTClasses's Names ...
  //###TODO#### write properties struct declares
	//Then Datas ...
	//Then Mapped Objects ...
};

//-----------------------------------------------------------------
//! \struct NSFFieldSchema
//! \brief	Field schema
//-----------------------------------------------------------------
struct NSFFieldSchema
{
	sbyte			sbyType;
	char*			pszName;
	//value??
};

//-----------------------------------------------------------------
//	Prototypes
//-----------------------------------------------------------------
class NObject;
class NStream;
class NMemoryStream;

//-----------------------------------------------------------------
//!	\class	NArchive
//!	\brief	Serializer for data loading and saving
//-----------------------------------------------------------------
class CORELIB_API NArchive
{
public:
	//Constructor-Destructor
	NArchive(NStream *_stream);
	virtual	~NArchive();

	//Storage Methods
	bool  PrepareSave();
	bool  FinalizeSave();
	bool  Read();
	void  Clear();

	//Writing Methods
	void	PutClass(NObject* _c);

  NArchive &operator<<( const char* _val ); // Needed since the template tends to choose wrong overload on const char*
  template <class T> NArchive &operator<<( const T &_val ) { *(m_pBufferedStream == null ? static_cast<NMemoryStream*>(m_pStream) : m_pBufferedStream) << _val; return *this; }

  bool	PutData(void* _buf, udword _length);

	//Reading Methods
	NObject*	GetClass();

  NArchive &operator>>( char* _val ); // Needed since the template tends to choose wrong overload on const char*
  template <class T> NArchive &operator>>( T &_val ) { *(m_pBufferedStream == null ? static_cast<NMemoryStream*>(m_pStream) : m_pBufferedStream) >> _val; return *this; }

  bool	GetData(void* _buf, udword _length);

	//Mapped object Methods
	void			PutMappedObj(NObject* _c);
	NObject*	GetMappedObj();

	NStream* GetStream()								{ return m_pStream;					}
	NMemoryStream* GetBufferedStream()	{ return m_pBufferedStream; }

	//GetRTClassFieldFromName(const char* pszClassName, 

protected:
	//Methods
	uword			AddUniqueRTClass(NRTClass* _pRTClass);//!< Return ID index into m_pRTClassesArray
	NRTClass*	GetRTClassFromIdx(uword _idx);				//!< Return ID* from an m_pRTClassesArray's index
	uword			AddUniqueMappedObjs(NObject* _pobj);	//!< Return Nobject index into m_carrayMappedObjs

	void		PutObj(NObject* _c);									//!< Write class datas to archive

	void		WriteRTClassesSchema();
	bool		ReadRTClassesSchema();

	//Datas
	NStream*				m_pStream;				//!< Stream used for storage (memory or file)
	NMemoryStream*  m_pBufferedStream;//!< internal datas
	bool						m_bWriting;

	uword					m_wRTClassCount;		//!< Unique Guids count
	uword					m_wRTClassesSize;		//!< Unique Guids array size
	NRTClass**		m_pRTClassesArray;	//!< Unique RTClass array

	NObjectArray	m_carrayMappedObjs;	//!< Unique Mapped objects
};

#endif //ARCHIVE_H

