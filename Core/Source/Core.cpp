//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Core.cpp
//! \brief	Core
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
//                   Includes
//-----------------------------------------------------------------
#include "../Include/CoreLib.h"
#include "Core.h"
#include "OpEngine/Controllers.h"

//-----------------------------------------------------------------
//!	localeless atof() for proper interpretation of default values
//!	\author Sebastian Olter (qduaty@gmail.com)
//-----------------------------------------------------------------

//###JN### to remove from here

double my_atof(const char* s)
{
	const char* point = strchr(s, '.');
	int integer = atol(s);
	double frac = 0.;
	if(point)
	{
		frac = atol(++point);
		for(unsigned i = 0; i < strlen(point); i++)
			frac /= 10.;
	}
	return integer + frac;
}




//-----------------------------------------------------------------
//                   Variables
//-----------------------------------------------------------------
const char* GetModuleName()  { return "FxGenCore"; }

NRTClassModule* NRTClassModule::m_pFirstRTClassModule=null;
NRTClassModule* NRTClassModule::m_pLastRTClassModule=null;


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//									NVarsBloc class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NVarsBloc::NVarsBloc()
{
	m_paVarsValues		= null;
	m_pcnextVarsBloc	= null;
	m_pcvarsblocDesc	= null;
	m_dwVarsCount			= 0;
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NVarsBloc::~NVarsBloc()
{
	if (m_paVarsValues!=null)		NMemFree(m_paVarsValues);
}

//-----------------------------------------------------------------
//!	\brief	Serialize Variable bloc for saving
//!	\param	_s	archive container
//!	\return	True if success
//-----------------------------------------------------------------
bool NVarsBloc::Save(NArchive* _s)
{
	*_s<<m_byVersion;
	*_s<<m_dwVarsCount;
	for (udword i=0; i<m_dwVarsCount; i++)
	{
		NVarValue* pval = m_paVarsValues + i;
		NVarsBlocDesc* pdesc = m_pcvarsblocDesc + i;
		switch (pdesc->eType)
		{
			case eubyte:	*_s<<pval->byVal;									break;
			case euword:	*_s<<pval->wVal;									break;
			case eudword:	*_s<<pval->dwVal;									break;
			case efloat:	*_s<<pval->fVal;									break;
			case erefobj:	_s->PutMappedObj(pval->pcRefObj);	break;
			case estring:	*_s<<(const char*)pval->szVal; /* cast for GCC - Olter */	break;
			default:	assert(0);														break;
		}

		//Anim Control
		ubyte byAniCtrl = pval->pcCtrlObj!=null?1:0;
		*_s<<byAniCtrl;

		if (byAniCtrl)		_s->PutClass(pval->pcCtrlObj);

	}

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Serialize Variable bloc for loading
//!	\param	_l	archive container
//!	\return	True if success
//-----------------------------------------------------------------
bool NVarsBloc::Load(NArchive* _l)
{
	ubyte byFileVersion=0;
	*_l>>byFileVersion;

	if (m_byVersion!=byFileVersion && byFileVersion!=0)
	{
		//DoVarBlocVersion_Mapping(_l, byFileVersion);
	} else {
		*_l>>m_dwVarsCount;
		for (udword i=0; i<m_dwVarsCount; i++)
		{
			NVarValue* pval = m_paVarsValues + i;
			NVarsBlocDesc* pdesc = m_pcvarsblocDesc + i;
			switch (pdesc->eType)
			{
				case eubyte:	*_l>>pval->byVal;										break;
				case euword:	*_l>>pval->wVal;										break;
				case eudword:	*_l>>pval->dwVal;										break;
				case efloat:	*_l>>pval->fVal;										break;
				case erefobj:	SetValue(i, 0, _l->GetMappedObj());	break;
				case estring:	*_l>>pval->szVal;					break;
				default:	assert(0);															break;
			}

			//Anim Control
			if (byFileVersion>=1)
			{
				ubyte byAniCtrl;
				*_l>>byAniCtrl;

				if (byAniCtrl)		pval->pcCtrlObj = _l->GetClass();
				else							pval->pcCtrlObj = null;
			} else {
				pval->pcCtrlObj = null;
			}

		}

	}

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Initialize a variable bloc
//!	\param	_dwVarsCount		Variables count
//!	\param	_pvarsBlocDesc	Variables bloc description
//!	\param	_powner					Object for this variable bloc
//!	\param	_byVersion			Version for serialization
//-----------------------------------------------------------------
void NVarsBloc::Init(udword _dwVarsCount, NVarsBlocDesc* _pvarsBlocDesc, NObject* _powner, ubyte _byVersion)
{
	//Store Values
	m_dwVarsCount			= _dwVarsCount;
	m_pcvarsblocDesc  = _pvarsBlocDesc;
	m_powner					= _powner;
	m_byVersion				= _byVersion;

	//Create variables for this bloc
	m_paVarsValues = (NVarValue*)NMemAlloc(_dwVarsCount*sizeof(NVarValue));

	//Init default values
	for (udword i=0; i<m_dwVarsCount; i++)
	{
		NVarValue* pval = m_paVarsValues + i;
		NVarsBlocDesc* pdesc = m_pcvarsblocDesc + i;
		switch (pdesc->eType)
		{
			case eubyte:	pval->byVal=(ubyte)atol(pdesc->pszDefValue);	break;
			case euword:	pval->wVal=(uword)atol(pdesc->pszDefValue);		break;
			case eudword:	pval->dwVal=(udword)atof(pdesc->pszDefValue);	break;
			case efloat:	pval->fVal=(float)my_atof(pdesc->pszDefValue);		break;
			case erefobj:	pval->pcRefObj=null;	break;
			case estring:	strcpy_s(pval->szVal, sizeof(pval->szVal), pdesc->pszDefValue);			break;
			default:	assert(0);																				break;
		}

		pval->pcCtrlObj = null;
	}
}

//-----------------------------------------------------------------
//!	\brief	Remove a referenced object from variables (see erefobj)
//!	\param	_pobj	Object referenced by erefobj vars
//-----------------------------------------------------------------
void NVarsBloc::RemoveVarsRef(NObject* _pobj)
{
	for (udword i=0; i<m_dwVarsCount; i++)
	{
		NVarValue* pval = m_paVarsValues + i;
		NVarsBlocDesc* pdesc = m_pcvarsblocDesc + i;
		if (pdesc->eType==erefobj && pval->pcRefObj==_pobj)
			SetValue(i, 0, (NObject*)null);
	}

}

//-----------------------------------------------------------------
//!	\brief	Return true if one variable is animated
//!	\return	True if one variable is animated
//-----------------------------------------------------------------
bool NVarsBloc::IsOneValueAnimated()
{
	for (udword i=0; i<m_dwVarsCount; i++)
	{
		NVarValue* pval = m_paVarsValues + i;
		if (pval->pcCtrlObj!=null)			return true;
	}
	return false;
}

//-----------------------------------------------------------------
//!	\brief	Return true if variable is animated
//!	\return	True if variable is animated
//-----------------------------------------------------------------
bool NVarsBloc::IsValueAnimated(udword _idx)
{
	NVarValue* pval = m_paVarsValues + _idx;
	if (pval->pcCtrlObj!=null)			return true;
	return false;
}

//-----------------------------------------------------------------
//!	\brief	Return controler for a value
//!	\return	NObject* for controler
//-----------------------------------------------------------------
NObject* NVarsBloc::GetValueControler(udword _idx)
{
	NVarValue* pval = m_paVarsValues + _idx;
	return pval->pcCtrlObj;
}

//-----------------------------------------------------------------
//!	\brief	Return value's description
//!	\return	NVarsBlocDesc* for a value
//-----------------------------------------------------------------
NVarsBlocDesc* NVarsBloc::GetValueDesc(udword _idx)
{
	return m_pcvarsblocDesc+_idx;
}

//-----------------------------------------------------------------
//!	\brief	Return variable's value by indice
//!	\param	_idx		Indice of variable
//!	\param	_fTime	Time for evaluation
//!	\param	_val		Returned Value
//-----------------------------------------------------------------
void NVarsBloc::GetValue(udword _idx, float _fTime, ubyte& _val)
{
	if (m_paVarsValues[_idx].pcCtrlObj)
		_val = (ubyte)((NController*)m_paVarsValues[_idx].pcCtrlObj)->GetValue(_fTime);
	else
		_val = m_paVarsValues[_idx].byVal;
}

void NVarsBloc::GetValue(udword _idx, float _fTime, uword&	_val)
{
	if (m_paVarsValues[_idx].pcCtrlObj)
		_val = (uword)((NController*)m_paVarsValues[_idx].pcCtrlObj)->GetValue(_fTime);
	else
		_val = m_paVarsValues[_idx].wVal;

}

void NVarsBloc::GetValue(udword _idx, float _fTime, udword&	_val)
{
	if (m_paVarsValues[_idx].pcCtrlObj)
		_val = (udword)((NController*)m_paVarsValues[_idx].pcCtrlObj)->GetValue(_fTime);
	else
		_val = m_paVarsValues[_idx].dwVal;
}

void NVarsBloc::GetValue(udword _idx, float _fTime, float& _val)
{
	if (m_paVarsValues[_idx].pcCtrlObj)
		_val = ((NController*)m_paVarsValues[_idx].pcCtrlObj)->GetValue(_fTime);
	else
		_val = m_paVarsValues[_idx].fVal;
}

void NVarsBloc::GetValue(udword _idx, float _fTime, NObject*&	_val)
{
	_val = m_paVarsValues[_idx].pcRefObj;
}

void NVarsBloc::GetValue(udword _idx, float _fTime, char*& _val)
{
	_val = m_paVarsValues[_idx].szVal;
}

//-----------------------------------------------------------------
//!	\brief	Change a variable's value by indice
//!	\param	_idx		Indice of variable
//!	\param	_fTime	Time for changed
//!	\param	_val		New Value
//-----------------------------------------------------------------
void NVarsBloc::SetValue(udword _idx, float _fTime, ubyte _val)
{
	m_paVarsValues[_idx].byVal = _val;
}

void NVarsBloc::SetValue(udword _idx, float _fTime, uword	_val)
{
	m_paVarsValues[_idx].wVal = _val;
}

void NVarsBloc::SetValue(udword _idx, float _fTime, udword	_val)
{
	m_paVarsValues[_idx].dwVal = _val;
}

void NVarsBloc::SetValue(udword _idx, float _fTime, float _val)
{
	m_paVarsValues[_idx].fVal = _val;
}

void NVarsBloc::SetValue(udword _idx, float _fTime, NObject* _val)
{
	//Remove old reference link
	if (m_paVarsValues[_idx].pcRefObj != null)
	{
		m_powner->RemoveRef( m_paVarsValues[_idx].pcRefObj );
	}

	//Set New reference
	m_paVarsValues[_idx].pcRefObj = _val;
	if (_val)	m_powner->AddRef(_val);
}

void NVarsBloc::SetValue(udword _idx, float _fTime, const char*	_val)
{
	strcpy_s(m_paVarsValues[_idx].szVal, sizeof(m_paVarsValues[_idx].szVal), _val);
}

//-----------------------------------------------------------------
//!	\brief	Set a mapping variable bloc to manage blocs versions
//!	\param	_dwMapVarsCount		Mapped variables count
//!	\param	_pmapVarsBlocDesc	Mapping variables bloc description
//-----------------------------------------------------------------
/*void NVarsBloc::SetMapVarBlocDesc(udword _dwMapVarsCount, NMapVarsBlocDesc* _pmapVarsBlocDesc)
{
	m_pcmapVarsBlocDesc = _pmapVarsBlocDesc;
	m_dwMapVarsCount  = _dwMapVarsCount;
}*/

//-----------------------------------------------------------------
//!	\brief	Change a variable's value by indice
//!	\param	_byVersion		File version (different to this)
//-----------------------------------------------------------------
/*void NVarsBloc::DoVarBlocVersion_Mapping(NArchive* _l, ubyte _byVersion)
{
	udword dwOldVarsCount;
	*_l>>dwOldVarsCount;

//	NMapVarsBlocDesc* pcmapvarsBloc = m_pcmapVarsBlocDesc;

	for (udword i=0; i<m_dwMapVarsCount; i++)
	{
		NMapVarsBlocDesc* pmapdesc = m_pcmapVarsBlocDesc + i;
		if (pmapdesc->byVersion==_byVersion)
		{
			const char* pszMapToIdx = pmapdesc->pszMapping;
			sdword dwLen = strlen(pszMapToIdx);

			NVarValue* pval			= null;
			NVarValue* pvalPrev = null;
			NVarValue arVal;

			while (dwLen>0)
			{
				udword j = atol(pszMapToIdx);
				pval = m_paVarsValues + j;

				if (pvalPrev==null)
				{
					switch (pmapdesc->eType)
					{
						case eubyte:
							*_l>>arVal.byVal;
							MapValueTo((double)arVal.byVal, j, pmapdesc->pszExpression);
						break;

						case euword:
							*_l>>arVal.wVal;
							MapValueTo((double)arVal.wVal, j, pmapdesc->pszExpression);
						break;

						case eudword:
							*_l>>arVal.dwVal;
							MapValueTo((double)arVal.dwVal, j, pmapdesc->pszExpression);
						break;

						case efloat:
							*_l>>arVal.fVal;
							MapValueTo((double)arVal.fVal, j, pmapdesc->pszExpression);
						break;

						case erefobj:
							arVal.pcRefObj = _l->GetMappedObj();
							MapValueTo(arVal.pcRefObj, j);
						break;

						case estring:
							*_l>>arVal.szVal;
							MapValueTo(arVal.szVal, j);
						break;

						default:	assert(0);	break;
					} //Switch

					//Anim Control
					if (_byVersion>=1)
					{
						ubyte byAniCtrl;
						*_l>>byAniCtrl;

						if (byAniCtrl)		pval->pcCtrlObj = _l->GetClass();
						else							pval->pcCtrlObj = null;

					} else {
						pval->pcCtrlObj = null;
					}

					pvalPrev = pval;
				} else {

					memcpy(pval, pvalPrev, sizeof(NVarValue));

				}

				//Next Idx
				for (; dwLen>0; --dwLen)
				{
					sbyte c = *pszMapToIdx++;
					if (c==',')		 { dwLen--;  break;}
				}

			}//While

		}//Version

	}

}

void NVarsBloc::MapValueTo(double _fval, udword _idx, const char* _pszExpression)
{
	double fVal = _fval;

	if (_pszExpression[0]=='*')		fVal*= atof(_pszExpression+1);
	if (_pszExpression[0]=='+')		fVal+= atof(_pszExpression+1);

	NVarValue* pval = m_paVarsValues + _idx;
	NVarsBlocDesc* pdesc = m_pcvarsblocDesc + _idx;
	switch (pdesc->eType)
	{
		case eubyte:	pval->byVal=(ubyte)fVal;	break;
		case euword:	pval->wVal=(uword)fVal;		break;
		case eudword:	pval->dwVal=(udword)fVal;	break;
		case efloat:	pval->fVal=(float)fVal;		break;
		default:	assert(0); break;
	}
}

void NVarsBloc::MapValueTo(NObject* _val, udword _idx)
{
//	NVarValue* pval = m_paVarsValues + _idx;
	NVarsBlocDesc* pdesc = m_pcvarsblocDesc + _idx;

	assert(pdesc->eType==erefobj);
	SetValue(_idx, 0.0f, _val);
}

void NVarsBloc::MapValueTo(const char* _val, udword _idx)
{
	NVarValue* pval = m_paVarsValues + _idx;
	NVarsBlocDesc* pdesc = m_pcvarsblocDesc + _idx;

	assert(pdesc->eType==estring);
	strcpy_s(pval->szVal, sizeof(pval->szVal), _val);
}
*/

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//									NObject class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NObject, NObject);

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NObject::NObject()
{
	m_pcfirstVarsBloc = null;
	m_dwLastUsedTime	= 0;
	m_szName[0]				= 0;
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NObject::~NObject()
{
	//Update erefobj Variables, in objects that reference Me (RefMakers)
	while (m_carrayRefMakers.Count())
		RemoveRefToMe( m_carrayRefMakers[0] );

	//Remove objects referenced by this object (RefTargets)
	while (m_carrayRefTargets.Count())
		RemoveRef( m_carrayRefTargets[0] );

	//Delete Variables Blocs
	NVarsBloc* pcur = m_pcfirstVarsBloc;
	while (pcur)
	{
		NVarsBloc* ptoDel = pcur;
		pcur = pcur->m_pcnextVarsBloc;
		delete ptoDel;
	}

}

//-----------------------------------------------------------------
//!	\brief	Serialize Variable bloc for saving
//!	\param	_s	archive container
//!	\return	True if success
//-----------------------------------------------------------------
bool NObject::Save(NArchive* _s)
{
	//Save object's name
	ubyte len = (ubyte)strlen(m_szName);
	*_s<<len;
	_s->PutData(&m_szName, len);

	//Save Variables Bloc's Values
	NVarsBloc* pcurvarbloc = m_pcfirstVarsBloc;
	while (pcurvarbloc)
	{
		pcurvarbloc->Save(_s);
		pcurvarbloc = m_pcfirstVarsBloc->m_pcnextVarsBloc;
	}

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Serialize Variable bloc for loading
//!	\param	_l	archive container
//!	\return	True if success
//-----------------------------------------------------------------
bool NObject::Load(NArchive* _l)
{
	//Load object's name
	ubyte len;
	*_l>>len;
	_l->GetData(&m_szName, len);
	m_szName[len]=0;

	//Load Variables Bloc's Values
	NVarsBloc* pcurvarbloc = m_pcfirstVarsBloc;
	while (pcurvarbloc)
	{
		pcurvarbloc->Load(_l);
		pcurvarbloc = m_pcfirstVarsBloc->m_pcnextVarsBloc;
	}

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Duplicate this object
//!	\return	Object duplicated pointer
//-----------------------------------------------------------------
NObject* NObject::Duplicate()
{
	NObject* pobjClone = NRTClass::CreateByName( GetRTClass()->m_pszClassName );
	if (pobjClone)
	{
		//Duplicate object name
		pobjClone->SetName( GetName() );

		//Duplicate Variables Bloc's Values
		NVarsBloc* pcurvarbloc			= m_pcfirstVarsBloc;
		NVarsBloc* pcurvarblocClone = pobjClone->m_pcfirstVarsBloc;
		while (pcurvarbloc)
		{
			memcpy(pcurvarblocClone->m_paVarsValues, pcurvarbloc->m_paVarsValues, pcurvarbloc->Count() * sizeof(NVarValue));
			pcurvarbloc = m_pcfirstVarsBloc->m_pcnextVarsBloc;
			pcurvarblocClone = pobjClone->m_pcfirstVarsBloc->m_pcnextVarsBloc;
		}

	}

	return pobjClone;
}


//-----------------------------------------------------------------
//!	\brief	Add a variable bloc to this object
//!	\param	_dwVarCount		Variables count
//!	\param	_pdesc				Variables bloc description
//!	\param	_byVersion		Version for serialization
//!	\return	Created variable bloc pointer from description
//-----------------------------------------------------------------
NVarsBloc* NObject::AddVarsBloc(udword _dwVarCount, NVarsBlocDesc* _pdesc, ubyte _byVersion)
{
	//Create bloc
	NVarsBloc* pvarbloc = new NVarsBloc;
	pvarbloc->Init(_dwVarCount, _pdesc, this, _byVersion);

	if (m_pcfirstVarsBloc==null)
	{
		m_pcfirstVarsBloc = pvarbloc;

	} else {

		//Add at end
		NVarsBloc* plastvarbloc = m_pcfirstVarsBloc;
		while (m_pcfirstVarsBloc->m_pcnextVarsBloc)
			plastvarbloc = m_pcfirstVarsBloc->m_pcnextVarsBloc;

		plastvarbloc->m_pcnextVarsBloc = pvarbloc;

	}

	return pvarbloc;
}

//-----------------------------------------------------------------
//!	\brief	Update erefobj Variables
//!	\param
//-----------------------------------------------------------------
void NObject::RemoveVarsRef(NObject* _pobj)
{
	NVarsBloc* pcurvarbloc = m_pcfirstVarsBloc;
	while (pcurvarbloc)
	{
		pcurvarbloc->RemoveVarsRef(_pobj);
		pcurvarbloc = m_pcfirstVarsBloc->m_pcnextVarsBloc;
	}

}

//-----------------------------------------------------------------
//!	\brief	Add an object to reference by this
//!	\param	_pobj		object to reference
//-----------------------------------------------------------------
void NObject::AddRef(NObject* _pobj)
{
	m_carrayRefTargets.AddItem(_pobj);
	_pobj->AddRefToMe(this);
}

//-----------------------------------------------------------------
//!	\brief	Remove object referenced by this
//!	\param	_pobj	object referenced
//-----------------------------------------------------------------
void NObject::RemoveRef(NObject* _pobj)
{
	for (udword i=0; i<m_carrayRefTargets.Count(); i++)
	{
		if(_pobj==m_carrayRefTargets[i])
		{
			m_carrayRefTargets.RemoveItem(i);
			_pobj->RemoveRefToMe(this);
			break;
		}
	}

}


//-----------------------------------------------------------------
//!	\brief	Add an object that reference this
//!	\param	_pobj		object that reference this
//-----------------------------------------------------------------
void NObject::AddRefToMe(NObject* _pobj)
{
	m_carrayRefMakers.AddItem(_pobj);
}

//-----------------------------------------------------------------
//!	\brief	Remove object that reference this
//!	\param	_pobj	object that reference this
//-----------------------------------------------------------------
void NObject::RemoveRefToMe(NObject* _pobj)
{
	for (udword i=0; i<m_carrayRefMakers.Count(); i++)
	{
		if(_pobj==m_carrayRefMakers[i])
		{
			m_carrayRefMakers.RemoveItem(i);
			_pobj->RemoveRef(this);
			_pobj->RemoveVarsRef(this);		//###NEW###
			break;
		}
	}
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//									NObjectArray class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NObjectArray::NObjectArray()
{
	m_dwSize			= 16;
	m_dwCount			=	0;
	m_bManagedDel = false;

	m_pBuffer = (NObject**)NMemAlloc(m_dwSize * sizeof(NObject*));
	memset(m_pBuffer, 0, m_dwSize * sizeof(NObject*));
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NObjectArray::~NObjectArray()
{
	Clear();
	if (m_pBuffer) NMemFree(m_pBuffer);
	m_pBuffer=null;
}

//-----------------------------------------------------------------
//!	\brief	Serialize object's array for saving
//!	\param	_s	archive container
//!	\return	True if success
//-----------------------------------------------------------------

bool NObjectArray::Save(NArchive* _s)
{
	//Save Objects Count
	udword dwCount = Count();
	*_s<<dwCount;

	//Save Objects
	for (udword i=0; i<dwCount; i++)
		_s->PutClass( m_pBuffer[i] );

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Serialize object's array for loading
//!	\param	_l	archive container
//!	\return	True if success
//-----------------------------------------------------------------
bool NObjectArray::Load(NArchive* _l)
{
	//Clear Array (delete objects if necessary)
	Clear();

	//Load Objects Count
	udword dwCount;
	*_l>>dwCount;
	SetSize(dwCount);

	//Load Objects
	for (udword i=0; i<dwCount; i++)
	{
		NObject* pobj = _l->GetClass();
		if (pobj==null)
			return false;

		AddItem( pobj );
	}

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Clear array
//!	\note		if items are managed, objets are deleted
//-----------------------------------------------------------------
void NObjectArray::Clear()
{
	if (m_bManagedDel)
		for (udword i=0; i<m_dwCount; i++)
			if (m_pBuffer[i])	delete m_pBuffer[i];

	memset(m_pBuffer, 0, m_dwSize * sizeof(NObject*));
	m_dwCount=0;
}

//-----------------------------------------------------------------
//!	\brief	Add an item at index
//!	\param	_item		object pointer to add
//!	\return	_idx		indice location or -1 to add at the end
//-----------------------------------------------------------------
udword NObjectArray::AddItem(NObject* _item, udword _idx)
{

	//Add an item at the end
	if (_idx==(udword)-1)
	{
		if (m_dwCount+1>=m_dwSize)
		{
			m_dwSize+=OBJARRAY_GROWSIZE;
			m_pBuffer = (NObject**)NMemRealloc(m_pBuffer, sizeof(NObject*) * m_dwSize);
		}

		m_pBuffer[m_dwCount] = _item;

	//Insert an item in array
	} else {

		if (_idx>=m_dwSize)
		{
			m_dwSize=_idx+OBJARRAY_GROWSIZE;
			m_pBuffer = (NObject**)NMemRealloc(m_pBuffer, m_dwSize * sizeof(NObject*));
		}

		memcpy(m_pBuffer+_idx+1, m_pBuffer+_idx, (m_dwSize-_idx) * sizeof(NObject*));

		m_pBuffer[_idx] = _item;

	}

	m_dwCount++;

	return m_dwCount-1;
}

//-----------------------------------------------------------------
//!	\brief	Remove an item from idx
//!	\param	_idx	indice
//-----------------------------------------------------------------
void NObjectArray::RemoveItem(udword _idx)
{
	m_dwCount--;
//	if (m_bManagedDel)	m_pBuffer[_idx];
	memcpy(m_pBuffer+_idx, m_pBuffer+_idx+1, (m_dwSize-_idx-1) * sizeof(NObject*));
}

//-----------------------------------------------------------------
//!	\brief	Set array's count
//!	\param	_c	new count
//!	\note		if items are managed, remained objets are deleted
//-----------------------------------------------------------------
void NObjectArray::SetCount(udword _c)
{
	if (m_bManagedDel && _c<m_dwCount)
		for (udword i=_c; i<m_dwCount; i++)
			if (m_pBuffer[i])		delete m_pBuffer[i];

	if (_c>=m_dwSize)
	{
		m_pBuffer = (NObject**)NMemRealloc(m_pBuffer, sizeof(NObject*) * (_c + OBJARRAY_GROWSIZE));
		m_dwSize = _c + OBJARRAY_GROWSIZE;
	}
	m_dwCount = _c;
}

//-----------------------------------------------------------------
//!	\brief	Changed array's size
//!	\param	_s	new size
//-----------------------------------------------------------------
void NObjectArray::SetSize(udword _s)
{
	if (_s==0)	return;

	if (m_bManagedDel && _s<m_dwCount)
		for (udword i=_s; i<m_dwCount; i++)
			if (m_pBuffer[i])		delete m_pBuffer[i];

	m_pBuffer = (NObject**)NMemRealloc(m_pBuffer, sizeof(NObject*) * _s);
	m_dwSize = _s;
}

//-----------------------------------------------------------------
//!	\brief	Add an array to this
//!	\param	_array	Array to add
//-----------------------------------------------------------------
void NObjectArray::AddArray(NObjectArray& _array)
{
	udword dwOldCount = m_dwCount;
	SetSize(m_dwCount+_array.Count()+OBJARRAY_GROWSIZE);
	memcpy(m_pBuffer + dwOldCount, _array.m_pBuffer, _array.m_dwCount * sizeof(NObject*));
	m_dwCount+=_array.m_dwCount;
}

//-----------------------------------------------------------------
//!	\brief	Find an object into array
//!	\param	_item	object to find
//!	\return	object index
//-----------------------------------------------------------------
udword NObjectArray::Find(NObject* _item)
{
	for (udword i=0; i<m_dwCount; i++)
		if (m_pBuffer[i] == _item)		return i;	//Found

	return -1;	//Not found
}

//-----------------------------------------------------------------
//!	\brief	Objects sorting
//!	\param	_cmp	compare function
//-----------------------------------------------------------------
void NObjectArray::Sort(CompareFnc _cmp)
{
	//Quicksort algorithm
	qsort(m_pBuffer, m_dwCount, sizeof(NObject*), _cmp);
}

//-----------------------------------------------------------------
//
//									NRTClassModule class implementation
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Constructor
//!	\param	_pszModuleName				Module name for RTClasses
//-----------------------------------------------------------------
NRTClassModule::NRTClassModule(const char* _pszModuleName)
{
  if (m_pFirstRTClassModule==null)  m_pFirstRTClassModule = this;
  if (m_pLastRTClassModule)         m_pLastRTClassModule->m_pNextRTClassModule = this;

  m_pszModuleName = _pszModuleName;
  m_pNextRTClassModule=null;

  m_pFirstRTClass = null;
  m_pLastRTClass = null;

  m_pLastRTClassModule=this;
}


//-----------------------------------------------------------------
//
//									NRTClass class implementation
//
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//!	\brief	Constructor
//!	\param	_pcreateCB					Function pointer for object creation
//!	\param	_pszClassName				Class name for object
//!	\param	_pszSuperClassName	Super-Class name for object
//!	\param	_pmod               RTClass's Module
//-----------------------------------------------------------------
NRTClass::NRTClass(RTCLASS_HANDLER*	_pcreateCB, const char* _pszClassName, const char* _pszSuperClassName, const char* _pszModuleName)
{
  NRTClassModule* pmod = NRTClassModule::RegisterModule(_pszModuleName);

	if (pmod->m_pFirstRTClass==null)	pmod->m_pFirstRTClass = this;
	if (pmod->m_pLastRTClass)					pmod->m_pLastRTClass->m_pNextRTC = this;

	m_pCreateCB					= _pcreateCB;
	m_pszClassName			= _pszClassName;
	m_pszSuperClassName	= _pszSuperClassName;
	m_pNextRTC					= null;
  m_pRTClassModule    = pmod;

	pmod->m_pLastRTClass = this;
}

//-----------------------------------------------------------------
//!	\brief	Create an object from class Name
//!	\param	_pszClassName	Class name
//!	\return	Created object
//-----------------------------------------------------------------
NObject* NRTClass::CreateByName(const char* _pszClassName)
{
	NRTClass*	prt = NRTClass::GetRTClassByName(_pszClassName);
	if (prt)
		return prt->m_pCreateCB();

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Return RTClass* from class Name
//!	\param	_pszClassName	Class name
//!	\return	NRTClass*
//-----------------------------------------------------------------
NRTClass*	NRTClass::GetRTClassByName(const char* _pszClassName)
{
  NRTClassModule* pmod = NRTClassModule::m_pFirstRTClassModule;
  while (pmod)
  {
    NRTClass* pcurRTC = pmod->m_pFirstRTClass;

    while (pcurRTC!=null)
    {
      if (strcmp(pcurRTC->m_pszClassName, _pszClassName) == 0)
        return pcurRTC;

      pcurRTC = pcurRTC->m_pNextRTC;
    }

    pmod=pmod->m_pNextRTClassModule;
  }

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Return First RTClass from a super class name
//-----------------------------------------------------------------
NRTClass* NRTClass::GetFirstClassBySuperClass(const char* _pszSuperClassName)
{
  NRTClassModule* pmod = NRTClassModule::m_pFirstRTClassModule;
  while (pmod)
  {
    NRTClass* pcurRTC = pmod->m_pFirstRTClass;
    while (pcurRTC!=null)
    {
      if (strcmp(pcurRTC->m_pszSuperClassName, _pszSuperClassName) == 0)
        return pcurRTC;
      pcurRTC = pcurRTC->m_pNextRTC;
    }

    pmod=pmod->m_pNextRTClassModule;
  }

	return null;
}


//-----------------------------------------------------------------
//!	\brief	Return Next RTClass from a super class name
//-----------------------------------------------------------------
NRTClass* NRTClass::GetNextClassBySuperClass(const char* _pszSuperClassName, NRTClass* _prtclass)
{
  NRTClassModule* pmod = _prtclass->m_pRTClassModule;
  while (pmod)
  {
    _prtclass = _prtclass->m_pNextRTC;

    while (_prtclass!=null)
    {
      if (strcmp(_prtclass->m_pszSuperClassName, _pszSuperClassName) == 0)
        return _prtclass;

      _prtclass = _prtclass->m_pNextRTC;
    }

    pmod=pmod->m_pNextRTClassModule;
		if (pmod)
			_prtclass = pmod->m_pFirstRTClass;
  }

	return null;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NTreeNode class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NTreeNode, NObject);

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NTreeNode::NTreeNode()
{
	m_pFirstSon	= null;
	m_ppLastSon	= &m_pFirstSon;
	m_pBrother	= null;
	m_carrayObjects.SetManageDelete(true);
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NTreeNode::~NTreeNode()
{
	DeleteAllSons();
}


//-----------------------------------------------------------------
//!	\brief	Save object into archive
//!	\param	s archive
//!	\return True if success
//-----------------------------------------------------------------

bool NTreeNode::Save(NArchive* s)
{
	NObject::Save(s);

	//Save objects
	m_carrayObjects.Save(s);

	//Save hierarchie
	udword dwCount = GetSonsCount();
	*s<<dwCount;

	NTreeNode* pcurnode = m_pFirstSon;
	while (pcurnode)
	{
		pcurnode->Save(s);
		pcurnode = pcurnode->m_pBrother;
	}

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Load object from archive
//!	\param	_l archive
//!	\return True if success
//-----------------------------------------------------------------
bool NTreeNode::Load(NArchive* _l)
{
	if (!NObject::Load(_l))
		return false;

	//Load objects
	if (!m_carrayObjects.Load(_l))
		return false;

	//Load hierarchie
	udword dwCount;
	*_l>>dwCount;

	while (dwCount--)
	{
		NTreeNode* pcurnode = new NTreeNode;
		if (!pcurnode->Load(_l))
			return false;

		AddSon(pcurnode);
	}

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Add a node as son
//!	\param	_pnode	node to add
//!	\param	_idx		child index
//-----------------------------------------------------------------
void NTreeNode::AddSon(NTreeNode* _pnode, udword _idx)
{
	//Add a son at the End
	if (_idx==(udword)-1)
	{
		_pnode->m_pBrother = null;

		NTreeNode** pnode = m_ppLastSon;

		m_ppLastSon = &(_pnode->m_pBrother);

		*pnode = _pnode;


	//Add a son at n Pos
	} else {

		udword idx=0;
		NTreeNode** ppnode = &m_pFirstSon;
		while (*ppnode && idx!=_idx)
		{
			ppnode=&((*ppnode)->m_pBrother);
			idx++;
		}

		_pnode->m_pBrother = *ppnode;
		if (*ppnode==null)	m_ppLastSon = &(_pnode->m_pBrother);
		*ppnode = _pnode;

	 }

}

//-----------------------------------------------------------------
//!	\brief		Return sons count
//!	\return		sons count
//-----------------------------------------------------------------
udword NTreeNode::GetSonsCount()
{
	udword idx=0;
	NTreeNode* pnode = m_pFirstSon;
	while (pnode)
	{
		pnode=pnode->m_pBrother;
		idx++;
	}
	return idx;
}

//-----------------------------------------------------------------
//!	\brief		Delete a sons at a given index
//!	\param		Index of the objet to delete
//-----------------------------------------------------------------
void NTreeNode::DeleteSon(udword _idx)
{
	udword idx=0;
	NTreeNode** ppnode = &m_pFirstSon;
	while (*ppnode)
	{
		if (idx==_idx)
		{
			NTreeNode* pnodeToDel = *ppnode;
			*ppnode = pnodeToDel->m_pBrother;	//Unlink
			if (*ppnode==null)	m_ppLastSon = ppnode;
			delete pnodeToDel;
			return;
		}
		ppnode=&( (*ppnode)->m_pBrother );
		idx++;
	}
}

//-----------------------------------------------------------------
//!	\brief		Delete all sons
//-----------------------------------------------------------------
void NTreeNode::DeleteAllSons()
{
	NTreeNode* pcurnode = m_pFirstSon;
	while (pcurnode)
	{
		NTreeNode* pcurnodeToDel = pcurnode->m_pBrother;
		delete pcurnode;
		pcurnode = pcurnodeToDel;

	}
	m_pFirstSon=null;
}

//-----------------------------------------------------------------
//!	\brief		Find son from parent  pointer
//!	\param		_pnode	parent node pointer
//!	\return		son idx else -1
//-----------------------------------------------------------------
udword NTreeNode::FindSon(NTreeNode* _pnode)
{
	udword idx=0;
	NTreeNode* pnode = m_pFirstSon;
	while (pnode)
	{
		if (pnode==_pnode)
			return idx;

		pnode=pnode->m_pBrother;
		idx++;
	}
	return -1;
}

//-----------------------------------------------------------------
//!	\brief		Find son from name
//!	\param		_pszNodeName	Son's name
//!	\return		son NTreeNode ptr else null
//-----------------------------------------------------------------
NTreeNode* NTreeNode::GetSonFromName(const char* _pszNodeName)
{
	NTreeNode* pnodeFound = null;

	NTreeNode* pnode = m_pFirstSon;
	while (pnode)
	{
		if (strcmp(pnode->GetName(), _pszNodeName)==0)
		{
			pnodeFound=pnode;
			break;
		}

		pnode=pnode->m_pBrother;
	}

	return pnodeFound;
}

NObject* NTreeNode::_FindNodeFromClassName(NTreeNode* _pParent, char* _pszClassName)
{
	//Sons
	NTreeNode* pnode = _pParent->GetSon();
	while (pnode)
	{
		//Objects array
		NObjectArray& array = pnode->GetObjsArray();
		for (udword i=0; i<array.Count(); i++)
		{
			NObject* pobj = array[i];
			if (strcmp(pobj->GetRTClass()->m_pszClassName, _pszClassName) == 0)
				return pobj;
		}

		//Son Nodes
		NObject* pfind = _FindNodeFromClassName(pnode, _pszClassName);
		if (pfind)
			return pfind;

		//Next brothers nodes
		pnode = pnode->GetBrother();
	}

	return null;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NObjectGarbage class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NObjectGarbage::NObjectGarbage()
{
	m_pszClassName=null;
	m_dwCount=0;
	m_dwSize=0;
	m_aobjects=null;
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NObjectGarbage::~NObjectGarbage()
{
	if (m_aobjects)
		NDELETEARRAY(m_aobjects);
}

//-----------------------------------------------------------------
//!	\brief	Set ClassID Object managed by this class
//!	\param	_CLASSID	class identifier
//-----------------------------------------------------------------
void NObjectGarbage::SetManagedClass(const char* _pszClassName)
{
	m_pszClassName = _pszClassName;

	m_dwSize		= 256;	//!< 256 objects managed by default
	m_aobjects = (NObjGarbageDesc*)NNEWARRAY(NObjGarbageDesc, m_dwSize);
}

//-----------------------------------------------------------------
//!	\brief		Delete all objects by types and time validity
//!	\param		_byTypeMask					Delete objects if object type and bits masking is true
//!	\param		_dwTimevalidityMs		Max time in milliseconds for objets's validities
//-----------------------------------------------------------------
void NObjectGarbage::Compact(ubyte _byTypeMask, udword _dwTimevalidityMs)
{
	udword dwTick = clock();
	udword	dwCompacted = 0;
	udword	dwCount = 0;
	bool bToKeep = false;

	NObjGarbageDesc*	aobjectsCompact = (NObjGarbageDesc*)NNEWARRAY(NObjGarbageDesc, m_dwSize);

	//Delete un-used entries and keep needed entries
	for (udword i=0; i<m_dwCount; i++)
	{
		bToKeep = true;
		NObjGarbageDesc* pdesc = &m_aobjects[i];
		if (*pdesc->ppobj!=null && (pdesc->byType&_byTypeMask))
		{
			udword dwDelta = dwTick - (*pdesc->ppobj)->m_dwLastUsedTime;
			if (dwDelta > _dwTimevalidityMs)
			{
				NDELETE(*pdesc->ppobj, NObject);
				*pdesc->ppobj = null;
				dwCompacted++;
				bToKeep = false;
			}
		}

		if (bToKeep)
		{
			NMemCopy(aobjectsCompact+dwCount, pdesc, sizeof(NObjGarbageDesc));
			dwCount++;
		}

	}

	//Switching Arrays
	NObjGarbageDesc* ptoFree = m_aobjects;
	m_aobjects = aobjectsCompact;
	NDELETEARRAY(ptoFree);
	m_dwCount = dwCount;

#ifdef _DEBUG
	TRACE("NObjectGarbage::Compact CompactedCount<%d> NewCount<%d>\n", dwCompacted, dwCount);
#endif

}

//-----------------------------------------------------------------
//!	\brief	Return an object instance
//!	\param	_ppobj			object pointer
//!	\param	_byAsType		Set object of a type (let free to developer)
//-----------------------------------------------------------------
void NObjectGarbage::Asset(NObject** _ppobj, ubyte _byAsType)
{
	if (m_aobjects)
	{
		//Create a new object and update _ppobj pointer
		if (*_ppobj==null)
		{
			*_ppobj = NRTClass::CreateByName(m_pszClassName);
			if (*_ppobj!=null)
			{
				m_aobjects[m_dwCount].ppobj		= _ppobj;
				m_aobjects[m_dwCount].byType	= _byAsType;

				m_dwCount++;
				if (m_dwCount>=m_dwSize)
				{
          udword dwnewSize = m_dwCount + 256;
          NObjGarbageDesc*	pnewBuffer = NNEWARRAY(NObjGarbageDesc, dwnewSize);
          NMemCopy(pnewBuffer, m_aobjects, m_dwSize*sizeof(NObjGarbageDesc));
          NDELETEARRAY(m_aobjects);
          m_aobjects=pnewBuffer;
          m_dwSize=dwnewSize;

					//m_dwSize+= m_dwCount + 256;
					//m_aobjects = (NObjGarbageDesc*)NMemRealloc(m_aobjects, m_dwSize*sizeof(NObjGarbageDesc));
				}

#ifdef _DEBUG
				TRACE("NObjectGarbage::Asset Count<%d> ObjPtr<0x%X>\n", m_dwCount, _ppobj);
#endif
			}
		}

		//Update used object time
		if (*_ppobj!=null)
			(*_ppobj)->m_dwLastUsedTime = clock();

	}
}

//-----------------------------------------------------------------
//!	\brief	Remove an object instance
//!	\param	_ppobj			object pointer
//-----------------------------------------------------------------
void NObjectGarbage::RemoveEntry(NObject** _ppobj)
{
	for (udword i=0; i<m_dwCount; i++)
	{
		NObjGarbageDesc* pdesc = &m_aobjects[i];
		if (pdesc->ppobj==_ppobj)
		{
			if (*pdesc->ppobj!=null)
			{
				NDELETE(*pdesc->ppobj, NObject);
			}

			NMemMove(m_aobjects+i, m_aobjects+i+1, (m_dwSize-i-1) * sizeof(NObjGarbageDesc*));
			m_dwCount--;
#ifdef _DEBUG
			TRACE("NObjectGarbage::RemoveEntry Count<%d>\n", m_dwCount);
#endif
			break;
		}
	}

}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//									NErrors class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

NErrors* g_perrors = null;
extern NErrors* gGetErrors()
{
	if (g_perrors==null)
		g_perrors = new NErrors();
	return g_perrors;
}

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NErrors::NErrors()
{
	m_dwStringSize=m_dwStringPos=0;
	m_pszErrors=null;
}
NErrors::~NErrors()
{
	if (m_pszErrors)	NMemFree(m_pszErrors);
}

void NErrors::AddError(udword _code, const char* _fmt, ... )
{
	if (m_dwStringPos==0 && m_pszErrors!=null)
		ZeroMemory(m_pszErrors, m_dwStringSize);

	char buf[256];
	wvsprintf(buf, _fmt, (char *)(&_fmt+1));
	udword len = strlen(buf);

	if ((m_dwStringPos+len) >= m_dwStringSize)
	{
		m_dwStringSize=m_dwStringPos+len+4096;
		m_pszErrors = (char*)NMemRealloc(m_pszErrors, m_dwStringSize);
	}

	udword ret = sprintf(m_pszErrors+m_dwStringPos, "<%d> %s", _code, buf);
	if (ret!=-1)		m_dwStringPos+=ret;
}

char* NErrors::GetErrors()
{
	m_dwStringPos = 0;
	return m_pszErrors;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//								Globals Fonctions
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Write text into visual C++ debug output window
//!	\param	_fmt	format
//-----------------------------------------------------------------
#ifdef _WIN32
void gDebugLog(const char* _fmt, ... )
{
	char buf[256];
	wvsprintf(buf, _fmt, (char *)(&_fmt+1));
	OutputDebugString(buf);
}
#else
void gDebugLog(const char* _fmt, ... )
{
}
#endif



//-----------------------------------------------------------------
//!	\brief	Register a new RTClassModule
//!	\param	_pszModuleName  Module's name
//! \return Created module pointer
//-----------------------------------------------------------------
NRTClassModule* NRTClassModule::RegisterModule(const char* _pszModuleName)
{
  NRTClassModule* pmodFound = null;

  //First check if module don't already exist
  NRTClassModule* pmod = NRTClassModule::m_pFirstRTClassModule;
  while (pmod)
  {
    if (strcmp(pmod->m_pszModuleName, _pszModuleName)==0)
    {
      pmodFound = pmod;
      break;
    }
    pmod=pmod->m_pNextRTClassModule;
  }

  //If not found
  if (pmodFound==null)
    pmodFound = new NRTClassModule(_pszModuleName);

  return pmodFound;
}

