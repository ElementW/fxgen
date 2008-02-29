//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Archive.h
//! \brief	Serializer for project archive loading and saving
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!             Anders Stenberg (anders.stenberg@gmail.com)
//!
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
#ifndef ARCHIVE_H
#define ARCHIVE_H

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Core.h"

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------
#ifndef STATIC_BUILD
#ifdef FXGEN_EXPORTS
	#define FXGEN_API __declspec(dllexport)
#else
	#define FXGEN_API __declspec(dllimport)
#endif
#else // STATIC_BUILD
#define FXGEN_API
#endif // STATIC_BUILD

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
class NStream;
class NMemoryStream;

//-----------------------------------------------------------------
//!	\class	NArchive
//!	\brief	Serializer for data loading and saving
//-----------------------------------------------------------------
class FXGEN_API NArchive
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

        NStream* GetStream() { return m_pStream; }

protected:
	//Methods
	uword		AddUniqueGUID(ID _guidClass);					//!< Return ID index into m_pGuidsArray
	ID			GetGUIDFromIdx(uword _idx);						//!< Return ID* from an m_pGuidsArray's index
	uword		AddUniqueMappedObjs(NObject* _pobj);	//!< Return Nobject index into m_carrayMappedObjs


	void		PutObj(NObject* _c);									//!< Write class datas to archive

	//Datas
	NStream*	m_pStream;
        NMemoryStream*  m_pBufferedStream;

	bool			m_bWriting;

	uword					m_wGuidsCount;			//!< Unique Guids count
	uword					m_wGuidsSize;				//!< Unique Guids array size
	ID*						m_pGuidsArray;			//!< Unique GUIDs array
	NObjectArray	m_carrayMappedObjs;	//!< Unique Mapped objects
};

#endif //ARCHIVE_H

