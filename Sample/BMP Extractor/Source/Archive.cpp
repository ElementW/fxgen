//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Archive.cpp
//! \brief	Serializer for datas loading and saving
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
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

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
//#include "pch.h"
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
  m_pRTClassesArray = null;
  Clear();
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NArchive::~NArchive()
{
  if (m_pBufferedStream != null)
    NDELETE(m_pBufferedStream, NMemoryStream);

  if (m_pRTClassesArray != null)
    NDELETEARRAY(m_pRTClassesArray);
}

//-----------------------------------------------------------------
//!	\brief	Resets the archive
//-----------------------------------------------------------------
void NArchive::Clear()
{
  if (m_pRTClassesArray != null)
    NDELETEARRAY(m_pRTClassesArray);

  m_wRTClassesSize		= NSF_RTCLASSESARRAYSIZE;
  m_wRTClassCount			= 0;
  m_pRTClassesArray		= (NRTClass**)NNEWARRAY(NRTClass*, m_wRTClassesSize);

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
    NDELETE(m_pBufferedStream, NMemoryStream);

  m_pBufferedStream = NNEW(NMemoryStream);

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
	h.RTClassesCount			= m_wRTClassCount;
	h.MappedObjsCount		= (uword)m_carrayMappedObjs.Count();
	//h.RTClassesOffset Updated later...
	//h.MappedObjsOffset	Updated later...

	m_pStream->PutData(&h, sizeof(NSFHeader));	//Just for seeking

	//Write RTClasses Module + class Names
	for (j=0; j<(udword)m_wRTClassCount; j++)
	{
		*m_pStream<<m_pRTClassesArray[j]->m_pRTClassModule->m_pszModuleName;
		*m_pStream<<m_pRTClassesArray[j]->m_pszClassName;
	}

	//Update Header
	udword dwEndOfRTClasses = m_pStream->Tell();
	udword dwRTClassesNameSize = dwEndOfRTClasses - sizeof(NSFHeader);
	h.DatasOffset				= sizeof(NSFHeader) + dwRTClassesNameSize;
	h.MappedObjsOffset	= sizeof(NSFHeader) + dwRTClassesNameSize + dwUserDatasEndPos;

	m_pStream->Seek(0);
	m_pStream->PutData(&h, sizeof(NSFHeader));
	m_pStream->Seek(dwEndOfRTClasses);

	//Save Datas block
	m_pStream->PutData(m_pBufferedStream->GetBuffer(), m_pBufferedStream->Tell());

	NDELETE(m_pBufferedStream, NMemoryStream);
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

	udword i;

	//Read Header
	NSFHeader		h;
	m_pStream->GetData(&h, sizeof(NSFHeader));

	//Check if it's a NSF file
	if (h.NSFId!=NSF_HEADERID)
	{
		ERR(0, "Not an NSF File\n");
		return false;
	}

	//Check version
	if (h.Version!=NSF_VERSION && h.Version!=0x1000)
	{
		ERR(0, "NSF Bad Version\n");
		return false;
	}

	//Alloc Class's Names Table
	m_wRTClassCount = h.RTClassesCount;

	if (m_wRTClassCount>=m_wRTClassesSize)
	{
	  //Reallocate Array
	  uword wnewSize = m_wRTClassCount+NSF_RTCLASSESGROWSIZE;
    NRTClass** pnew = (NRTClass**)NNEWARRAY(NRTClass*, wnewSize);
    NMemCopy(pnew, m_pRTClassesArray, sizeof(NRTClass*) * m_wRTClassesSize);
    NDELETEARRAY(m_pRTClassesArray);

    m_pRTClassesArray=pnew;
		m_wRTClassesSize=wnewSize;
	}


	//For old Version 1.0.0.0 Classes are saved by ID	###TOREMOVE### after alpha version
	if (h.Version==0x1000)
	{
		m_pStream->Seek(sizeof(h) - sizeof(h.DatasOffset));

		for (i=0; i<m_wRTClassCount; i++)
		{
			ID id;
			m_pStream->GetData(&id, sizeof(ID));
			m_pRTClassesArray[i] = NRTClass::GetRTClassByID(id);
		}

	} else {

		//Read Class's Names Table and make RTClass Table
		char szModuleName[256];
		char szClassName[256];
		for (i=0; i<m_wRTClassCount; i++)
		{
			*m_pStream>>szModuleName;
			//###TODO### control if module is loaded...
			*m_pStream>>szClassName;
			m_pRTClassesArray[i] = NRTClass::GetRTClassByName(szClassName);
			if (m_pRTClassesArray[i]==null)
			{
				ERR(0, "RTClass Mod<%s> Name<%s> not found\n", szModuleName, szClassName);
				return false;
			}
		}

	}

	// Save current file pos then jump to Mapped Objects ...
	if (h.Version==0x1000)
		h.DatasOffset = m_pStream->Tell();

	m_pStream->Seek(h.MappedObjsOffset);

	//Read Mapped Objects Table
	m_carrayMappedObjs.SetSize(h.MappedObjsCount);
	for (i=1; i<h.MappedObjsCount; i++)	//from 1 because indice 0 = null object reference
	{
		NObject* pobj = GetClass();
		if (pobj==null)
		{
			ERR(0, "Mapped Obj not found\n");
			return false;
		}

		m_carrayMappedObjs.AddItem(pobj);
	}

	//Restore old file pos
	m_pStream->Seek(h.DatasOffset);

	//Then Datas ...

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Add Unique RTCLass* into array
//!	\param	_pRTClass	RTClass Ptr to add
//!	\return	idx of unique RTClass in array
//-----------------------------------------------------------------
uword NArchive::AddUniqueRTClass(NRTClass* _pRTClass)
{
	for (uword i=0; i<m_wRTClassCount; i++)
	{
		if (m_pRTClassesArray[i]==_pRTClass)		return i;
	}

	if (m_wRTClassCount+1>=m_wRTClassesSize)
	{
	  uword wnewSize = m_wRTClassesSize+NSF_RTCLASSESGROWSIZE;

    NRTClass** pnew = (NRTClass**)NNEWARRAY(NRTClass*, wnewSize);
    NMemCopy(pnew, m_pRTClassesArray, sizeof(NRTClass*) * m_wRTClassesSize);
    NDELETEARRAY(m_pRTClassesArray);

    m_pRTClassesArray=pnew;
		m_wRTClassesSize=wnewSize;
	}

	m_pRTClassesArray[m_wRTClassCount++] = _pRTClass;

	return m_wRTClassCount-1;
}

//-----------------------------------------------------------------
//!	\brief	Return a RTClass from an idx
//!	\param	_idx indice
//!	\return	RTClass*
//-----------------------------------------------------------------
NRTClass* NArchive::GetRTClassFromIdx(uword _idx)
{
	if (_idx>=m_wRTClassCount)		return null;
	return m_pRTClassesArray[_idx];
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
	uword id = AddUniqueRTClass( _c->GetRTClass() );

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
		//Get Runtime Class from idx
		*this >> idx;
		NRTClass* prtc = GetRTClassFromIdx(idx);
		if (!prtc)
		{
			ERR(0, "RTClass not found\n");
			return null;
		}

		//Create Class
		pobj = prtc->m_pCreateCB();
		if (!pobj)
		{
			ERR(0, "Can't create Object from RTClass\n");
			return null;
		}

		//Serialize class
		if (!pobj->Load(this))
			return null;

	//////////////////////////////////////////
	} else if (tag==NSF_MAPPEDCLASS_TAG) {

		//Get Object From Idx
		*this >> idx;
		pobj = m_carrayMappedObjs[idx];

		if (idx!=0 && !pobj)
		{
			ERR(0, "Mapped object not found!\n");
		}

	//////////////////////////////////////////
	} else {

		ERR(0, "Unsupported Tag\n");

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

		if (idx!=0 && !pobj)
		{
			ERR(0, "Mapped object not found!\n");
		}

		return pobj;
	}

	ERR(0, "Unsupported Tag\n");

	return null;
}
