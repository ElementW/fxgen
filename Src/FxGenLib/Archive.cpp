//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Archive.cpp
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

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Archive.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NArchive class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NArchive::NArchive()
{
	m_pStream			= null;
	m_pbyBuffer		= null;
	m_pGuidsArray	= null;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NArchive::~NArchive()
{
	Close();
}

//-----------------------------------------------------------------
//!	\brief	Open an archive for read or save
//!	\param	_filename	full file name
//!	\param	_writing	true if open for save
//!	\return	true si success
//-----------------------------------------------------------------
bool NArchive::Open(const char* _filename, bool _writing)
{
	m_bWriting = _writing;

	m_wGuidsSize		= NSF_GUIDSARRAYSIZE;
	m_wGuidsCount		= 0;
	m_pGuidsArray		= (ID*)NMemAlloc(sizeof(ID) * m_wGuidsSize);

	m_carrayMappedObjs.Clear();
	m_carrayMappedObjs.AddItem(null);	//at idx 0, null object reference

	///////////////////////////////////////////
	//For Writing
	if (_writing)
	{
		errno_t err = fopen_s(&m_pStream, _filename, "wb" );
		if (err!=0)													return false;

		//Init
		m_dwBufSize	= NSF_BUFSTARTSIZE;
		m_dwBufPos		= 0;
		m_pbyBuffer		= (ubyte*)NMemAlloc(m_dwBufSize);

	///////////////////////////////////////////
	//For Reading
	} else {
		errno_t err = fopen_s(&m_pStream, _filename, "rb" );
		if (err!=0)													return false;

		//Init
		m_dwBufSize	= NSF_BUFSTARTSIZE;
		m_dwBufPos		= 0;
		m_pbyBuffer		= (ubyte*)NMemAlloc(m_dwBufSize);

		//Lecture de l'entete et de la table des GUIDS
		if (!Read())			return false;

	}

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Close archive
//-----------------------------------------------------------------
void NArchive::Close()
{

	if (m_bWriting)	Save();


	if (m_pbyBuffer)			{ NMemFree(m_pbyBuffer); m_pbyBuffer=null; }
	if (m_pStream!=null)	{ fclose(m_pStream); m_pStream=null;	}
}

//-----------------------------------------------------------------
//!	\brief		Save archive
//!	\return		true if success
//-----------------------------------------------------------------

bool NArchive::Save()
{
	if (m_pStream==null)		return false;

	//Add Mapped Objects To Archive
	udword j;
	udword dwUserDatasEndPos = m_dwBufPos;

	for (j=0; j<m_carrayMappedObjs.Count(); j++)
	{
		NObject* pobj = m_carrayMappedObjs[j];
		if (pobj)		PutObj(pobj);
	}

	//Write Header
	NSFHeader						h;
	h.NSFId							= NSF_HEADERID;
	h.Version						= NSF_VERSION;
	h.GUIDCount					= m_wGuidsCount;
	h.MappedObjsCount		= (uword)m_carrayMappedObjs.Count();
	h.MappedObjsOffset	= sizeof(NSFHeader)+ (m_wGuidsCount * sizeof(ID)) + dwUserDatasEndPos;
	fwrite(&h, sizeof(NSFHeader),1, m_pStream);

	//Save GUIDs Table
	for (j=0; j<m_wGuidsCount; j++)
	{
		fwrite(&m_pGuidsArray[j], sizeof(ID),1,m_pStream);
	}

	//Save Datas block
	fwrite(m_pbyBuffer, m_dwBufPos, 1, m_pStream);


	return true;
}


//-----------------------------------------------------------------
//!	\brief	Read a part of Archive
//!	\return	True if success
//-----------------------------------------------------------------
bool NArchive::Read()
{
	if (m_pStream==null)				return false;

	//Read Header
	NSFHeader		h;
	fread(&h, sizeof(NSFHeader), 1, m_pStream);

	//Check if it's a NSF file
	if (h.NSFId!=NSF_HEADERID)		{ ERR("Not an NSF File",0); return false; }

	//Check version
	if (h.Version!=NSF_VERSION)		{ ERR("NSF Bad Version",0); return false; }

	//Read GUIDs Table
	m_wGuidsCount = h.GUIDCount;

	if (m_wGuidsCount>=m_wGuidsSize)
	{
		m_wGuidsSize=m_wGuidsCount+NSF_GUIDSGROWSIZE;
		m_pGuidsArray = (ID*)NMemRealloc(m_pGuidsArray, sizeof(ID) * (m_wGuidsSize));
	}

	udword i;
	for (i=0; i<h.GUIDCount; i++)
	{
		fread(&m_pGuidsArray[i], sizeof(ID), 1, m_pStream);
	}

	// Save current file pos then jump to Mapped Objects ...
	udword dwSavedPos = ftell(m_pStream);
	fseek(m_pStream, h.MappedObjsOffset, SEEK_SET);

	//Read Mapped Objects Table
	m_carrayMappedObjs.SetSize(h.MappedObjsCount);
	for (i=1; i<h.MappedObjsCount; i++)	//from 1 because idx0 = null object reference
	{
		NObject* pobj = GetClass();
		m_carrayMappedObjs.AddItem(pobj);
	}

	//Restore old file pos
	fseek(m_pStream, dwSavedPos, SEEK_SET);

	//Then Datas ...

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Add GUID into array
//!	\param	_guidClass	GUID to add
//!	\return	idx of unique GUID in array
//-----------------------------------------------------------------
uword NArchive::AddUniqueGUID(ID _guidClass)
{
	for (uword i=0; i<m_wGuidsCount; i++)
	{
		if (m_pGuidsArray[i]==_guidClass)		return i;
	}

	if (m_wGuidsCount+1>=m_wGuidsSize)
	{
		m_wGuidsSize+=NSF_GUIDSGROWSIZE;
		m_pGuidsArray = (ID*)NMemRealloc(m_pGuidsArray, sizeof(ID) * (m_wGuidsSize));
	}

	m_pGuidsArray[m_wGuidsCount++] = _guidClass;

	return m_wGuidsCount-1;
}

//-----------------------------------------------------------------
//!	\brief	Return a GUID from an idx
//!	\param	_idx indice
//!	\return	GUID
//-----------------------------------------------------------------
ID NArchive::GetGUIDFromIdx(uword _idx)
{
	if (_idx>=m_wGuidsCount)		return null;
	return m_pGuidsArray[_idx];
}

//-----------------------------------------------------------------
//!	\brief	Put datas into archive
//!	\param	_buf			buffer to add
//!	\param	_length		len of datas
//-----------------------------------------------------------------

void NArchive::PutDatas(void* _buf, udword _length)
{
	//Grow buffer size if necessary
	if ((m_dwBufPos+_length)>=m_dwBufSize){
		m_dwBufSize+=NSF_BUFGROWSIZE+_length;
		m_pbyBuffer=(ubyte*)NMemRealloc(m_pbyBuffer,m_dwBufSize);
	}

	//Save datas
	CopyMemory(m_pbyBuffer+m_dwBufPos, _buf, _length);
	m_dwBufPos+=_length;
}


//-----------------------------------------------------------------
//!	\brief	Get datas from archive
//!	\param	_buf			buffer to add
//!	\param	_length		len of datas
//-----------------------------------------------------------------
void NArchive::GetDatas(void* _buf, udword _length)
{
	fread(_buf, _length, 1, m_pStream);
}


//-----------------------------------------------------------------
//!	\brief	Put a class to archive
//!	\param	_c	class to put
//-----------------------------------------------------------------

void NArchive::PutClass(NObject* _c)
{
	//Not referenced Class
	if (_c->GetRefToMeCount()==0)
	{
		PutObj(_c);
	//Referenced Class
	} else {
		PutMappedObj(_c);
	}
}


//-----------------------------------------------------------------
//!	\brief	Put an object to archive
//!	\param	_c object to put
//-----------------------------------------------------------------

void NArchive::PutObj(NObject* _c)
{
	//Get Class ID idx
	uword id = AddUniqueGUID(_c->GetRTClass()->CLASSID);

	//Write Tag + ID idx
	*this << (ubyte)NSF_CLASS_TAG;
	*this << id;

	//Call Serialize Method
	_c->Save(this);
}



//-----------------------------------------------------------------
//!	\brief	Get a class from archive
//!	\return	object pointer
//-----------------------------------------------------------------
NObject* NArchive::GetClass()
{
	NObject* pobj=null;

	ubyte		tag;	*this >> tag;		//Get Tag
	uword		idx;

	//////////////////////////////////////////
	if (tag==NSF_CLASS_TAG)
	{
		//Get Class ID from idx
		*this >> idx;
		ID CLASSID = GetGUIDFromIdx(idx);
		if (!CLASSID)							ERR("Class's ID not registered",0);

		//Create Run-Time Class
		pobj = NRTClass::CreateByID(CLASSID);
		if (!pobj)								ERR("Can't create RTClass",0);

		//Serialize class
		if (pobj)	pobj->Load(this);

	//////////////////////////////////////////
	} else if (tag==NSF_MAPPEDCLASS_TAG) {

		//Get Object From Idx
		*this >> idx;
		pobj = m_carrayMappedObjs[idx];

		if (idx!=0 && !pobj)				ERR("Mapped object not found!", 0);

	//////////////////////////////////////////
	} else {

		ERR("Unsupported Tag",0);
	}

	return pobj;
}

//-----------------------------------------------------------------
//!	\brief	Add a mapped object into array
//!	\param	_pobj object to add
//!	\return	index into m_carrayMappedObjs
//-----------------------------------------------------------------
uword NArchive::AddUniqueMappedObjs(NObject* _pobj)
{
	for (udword i=0; i<m_carrayMappedObjs.Count(); i++)
	{
		if (m_carrayMappedObjs[i]==_pobj)		return (uword)i;
	}

	return (uword)m_carrayMappedObjs.AddItem(_pobj);
}

//-----------------------------------------------------------------
//!	\brief	Put a mapped object into array
//!	\param	_c mapped object to put
//-----------------------------------------------------------------

void	NArchive::PutMappedObj(NObject* _c)
{
	//Write Tag
	*this << (ubyte)NSF_MAPPEDCLASS_TAG;

	uword idx = 0;	//idx0 => obj=null
	if (_c!=null)	idx = AddUniqueMappedObjs(_c);
	*this<<idx;
}


//-----------------------------------------------------------------
//!	\brief	Get a mapped object from archive
//!	\return	Object pointer
//-----------------------------------------------------------------
NObject* NArchive::GetMappedObj()
{
	ubyte tag;	*this >> tag;		//Get Tag
	if (tag==NSF_MAPPEDCLASS_TAG)
	{
		uword idx;	*this >> idx;	//Get Object Idx	(rk. idx=0 obj=null)
		NObject* pobj = m_carrayMappedObjs[idx];
		if (idx!=0 && !pobj)				ERR("Mapped object not found!", 0);
		return pobj;
	}

	ERR("Unsupported Tag",0);
	return null;
}
