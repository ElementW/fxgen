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
NArchive::NArchive(NStream *_stream)
{
  m_pBufferedStream = null;
  m_pStream = _stream;
  m_pGuidsArray = null;
  Clear();
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NArchive::~NArchive()
{
  if (m_pBufferedStream != null)
    delete m_pBufferedStream;

  if (m_pGuidsArray != null)
    NMemFree(m_pGuidsArray);
}

//-----------------------------------------------------------------
//!	\brief	Resets the archive
//-----------------------------------------------------------------
void NArchive::Clear()
{
  if (m_pGuidsArray != null)
    NMemFree(m_pGuidsArray);

  m_wGuidsSize		= NSF_GUIDSARRAYSIZE;
  m_wGuidsCount		= 0;
  m_pGuidsArray		= (ID*)NMemAlloc(sizeof(ID) * m_wGuidsSize);

  m_carrayMappedObjs.Clear();
  m_carrayMappedObjs.AddItem(null);	//at idx 0, null object reference
}

//-----------------------------------------------------------------
//!	\brief		Finalize saving of the archive
//!	\return		true if success
//-----------------------------------------------------------------
bool NArchive::PrepareSave()
{
  if (m_pBufferedStream != null)
    delete m_pBufferedStream;

  m_pBufferedStream = new NMemoryStream();

  return true;
}

//-----------------------------------------------------------------
//!	\brief		Finalize saving of the archive
//!	\return		true if success
//-----------------------------------------------------------------
bool NArchive::FinalizeSave()
{
	if (m_pBufferedStream == null || m_pStream==null)		return false;

	//Add Mapped Objects To Archive
	udword j;
	udword dwUserDatasEndPos = m_pBufferedStream->Tell();

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
	m_pStream->PutData(&h, sizeof(NSFHeader));

	//Save GUIDs Table
	m_pStream->PutData(m_pGuidsArray, sizeof(ID)*m_wGuidsCount);

	//Save Datas block
	m_pStream->PutData(m_pBufferedStream->GetBuffer(), m_pBufferedStream->Tell());

	delete m_pBufferedStream;
	m_pBufferedStream = null;

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
        m_pStream->GetData(&h, sizeof(NSFHeader));

	//Check if it's a NSF file
	if (h.NSFId!=NSF_HEADERID)
	{
#ifdef _DEBUG
		ERR("Not an NSF File",0);
#endif
		return false;
	}

	//Check version
	if (h.Version!=NSF_VERSION)
	{
#ifdef _DEBUG
		ERR("NSF Bad Version",0);
#endif
		return false;
	}

	//Read GUIDs Table
	m_wGuidsCount = h.GUIDCount;

	if (m_wGuidsCount>=m_wGuidsSize)
	{
		m_wGuidsSize=m_wGuidsCount+NSF_GUIDSGROWSIZE;
		m_pGuidsArray = (ID*)NMemRealloc(m_pGuidsArray, sizeof(ID) * (m_wGuidsSize));
	}

        m_pStream->GetData(m_pGuidsArray, sizeof(ID)*h.GUIDCount);

	// Save current file pos then jump to Mapped Objects ...
	udword dwSavedPos = m_pStream->Tell();
	m_pStream->Seek(h.MappedObjsOffset);

	//Read Mapped Objects Table
	udword i;
	m_carrayMappedObjs.SetSize(h.MappedObjsCount);
	for (i=1; i<h.MappedObjsCount; i++)	//from 1 because idx0 = null object reference
	{
		NObject* pobj = GetClass();
		m_carrayMappedObjs.AddItem(pobj);
	}

	//Restore old file pos
	m_pStream->Seek(dwSavedPos);

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
//!	\brief	Put data into archive
//!	\param	_buf			buffer to add
//!	\param	_length		len of datas
//-----------------------------------------------------------------

bool NArchive::PutData(void* _buf, udword _length)
{
  return (m_pBufferedStream != null ? m_pBufferedStream : m_pStream)->PutData(_buf, _length);
}


//-----------------------------------------------------------------
//!	\brief	Get data from archive
//!	\param	_buf			buffer to add
//!	\param	_length		len of datas
//-----------------------------------------------------------------
bool NArchive::GetData(void* _buf, udword _length)
{
  return (m_pBufferedStream != null ? m_pBufferedStream : m_pStream)->GetData(_buf, _length);
}


NArchive &NArchive::operator <<(const char *_val)
{
  *(m_pBufferedStream != null ? m_pBufferedStream : m_pStream) << _val;
  return *this;
}

NArchive &NArchive::operator >>(char *_val)
{
  *(m_pBufferedStream != null ? m_pBufferedStream : m_pStream) >> _val;
  return *this;
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
#ifdef _DEBUG
		if (!CLASSID)							ERR("Class's ID not registered",0);
#endif
		//Create Run-Time Class
		pobj = NRTClass::CreateByID(CLASSID);
#ifdef _DEBUG
		if (!pobj)								ERR("Can't create RTClass",0);
#endif
		//Serialize class
		if (pobj)	pobj->Load(this);

	//////////////////////////////////////////
	} else if (tag==NSF_MAPPEDCLASS_TAG) {

		//Get Object From Idx
		*this >> idx;
		pobj = m_carrayMappedObjs[idx];

#ifdef _DEBUG
		if (idx!=0 && !pobj)				ERR("Mapped object not found!", 0);
#endif
	//////////////////////////////////////////
	} else {

#ifdef _DEBUG
		ERR("Unsupported Tag",0);
#endif
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
	*this << idx;
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
#ifdef _DEBUG
		if (idx!=0 && !pobj)				ERR("Mapped object not found!", 0);
#endif
		return pobj;
	}

#ifdef _DEBUG
	ERR("Unsupported Tag",0);
#endif
	return null;
}
