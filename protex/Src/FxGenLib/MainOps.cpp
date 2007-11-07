//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MainOps.cpp
//! \brief	Main Operators
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
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "MainOps.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NStoreOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NStoreOp, NOperator);

static NVarsBlocDesc blocdescStoreOp[] =
{
	VAR(estring,	false, "Name", "",	"NStringProp")	//0
};

NStoreOp::NStoreOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescStoreOp, 1);
}

NStoreOp::~NStoreOp()
{
}

char* NStoreOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(0, 0, pszname);
	return pszname;
}

udword NStoreOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj, OBJRES_TYPE_STORED);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));

	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NStoreResultOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NStoreResultOp, NOperator);

static NVarsBlocDesc blocdescStoreResultOp[] =
{
	VAR(estring,	false, "Name", "",	"NStringProp")	//0
};

NStoreResultOp::NStoreResultOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescStoreResultOp, 1);
}

NStoreResultOp::~NStoreResultOp()
{
}

char* NStoreResultOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(0, 0, pszname);
	return pszname;
}


udword NStoreResultOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj, OBJRES_TYPE_FINALSTORED);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));

	return 0;
}

NBitmap* NStoreResultOp::GetBitmap()
{
	NBitmap* pbmp = null;
	if (m_pObj!=null)
	{
		if (strcmp(m_pObj->GetRTClass()->m_pszClassName, "NBitmap") == 0)
			pbmp = (NBitmap*)m_pObj;
	}

	return pbmp;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NLoadOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NLoadOp, NOperator);

static NVarsBlocDesc blocdescLoadOp[] =
{
	VAR(erefobj,	false, "Load",	"0", "NUseStoredOpsProp")	//0
};

NLoadOp::NLoadOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescLoadOp, 1);
}

char* NLoadOp::GetUserName()
{
		//Get Variables Values
	NOperator* popRef;
	m_pcvarsBloc->GetValue(0, 0, (NObject*&)popRef);

	//Process
	if (popRef)
		return popRef->GetUserName();

	return "";
}

udword NLoadOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=0)	//Load operator must'n have one input !
		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	//Get Variables Values
	NOperator* popRef;
	m_pcvarsBloc->GetValue(0, _ftime, (NObject*&)popRef);

	//Process
	if (popRef && popRef->m_pObj)
	{
		NBitmap* pSrc = (NBitmap*)popRef->m_pObj;
		NBitmap* pDst = (NBitmap*)m_pObj;

		udword w = pSrc->GetWidth();
		udword h = pSrc->GetHeight();
		pDst->SetSize(w,h);

		//Copy Source to This
		memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));
	}

	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NChannelAnimFX1Op class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NChannelAnimFX1Op, NOperator);

static NVarsBlocDesc blocdescChanAnimFX1Op[] =
{
	VAR(eubyte,	false, "Channel",			"0",								"NUbyteProp")	//0
	VAR(eubyte,	false, "Type",				"0,[Linear,Sine]",	"NUbyteComboProp")	//1
	VAR(efloat,	true, "Start Value",	"0.0",							"NFloatProp")	//2
	VAR(efloat,	true, "End Value",		"1.0",							"NFloatProp")	//3
	VAR(efloat,	true, "Factor",				"1.0",							"NFloatProp")	//4
	VAR(eubyte,	true, "Repeat",				"0",								"NUbyteProp")	//5
};

NChannelAnimFX1Op::NChannelAnimFX1Op()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescChanAnimFX1Op, 1);
}

udword NChannelAnimFX1Op::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=0)
	{
		NObject* pSrc = (NObject*)(*_pOpsInts)->m_pObj;
		m_pObj = pSrc;
	}

	//Get Variables Values
	ubyte byChannel, byType;
	float fFactor;
	m_pcvarsBloc->GetValue(0, _ftime, byChannel);
	m_pcvarsBloc->GetValue(1, _ftime, byType);
	m_pcvarsBloc->GetValue(4, _ftime, fFactor);

	//Calculate Channel Value
	NVarValue value;

	if (byType==1)	//sine
	{
		value.fVal = sin(_ftime) * fFactor;

	} else { //linear
		value.fVal = _ftime * fFactor;
	}

	//Update channel value
	NEngineOp::GetEngine()->SetChannelValue(byChannel, value);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NStoreOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NFloatVarOp, NOperator);

static NVarsBlocDesc blocdescNFloatVarOp[] =
{
	VAR(efloat,	true, "Var", "1.0",	"NFloatProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NFloatVarOp::NFloatVarOp()
{
	//Create variables bloc
	indx = 0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNFloatVarOp, 1);
}

NFloatVarOp::~NFloatVarOp()
{
}

char* NFloatVarOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

float NFloatVarOp::GetUserVal()
{
	m_pcvarsBloc->GetValue(0, 0, (float&)value);
	return value;
}

udword NFloatVarOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float curr = 0.0;
	m_pcvarsBloc->GetValue(0, 0, (float&)value);

	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	//Only one Input
	//if (m_byInputs!=1)		return (udword)-1;

	//InsureCommonInputsSize(_pOpsInts, _fDetailFactor);

	//Bitmap instance
	/*NEngineOp::GetEngine()->GetBitmap(&m_pObj, OBJRES_TYPE_STORED);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));*/

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NIntVarOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NUWordVarOp, NOperator);

static NVarsBlocDesc blocdescNIntVarOp[] =
{
	VAR(eudword,		true, "Value",			"1",	"NUwordProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NUWordVarOp::NUWordVarOp()
{
	//Create variables bloc
	indx = 0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNIntVarOp, 1);
}

NUWordVarOp::~NUWordVarOp()
{
}

char* NUWordVarOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

udword NUWordVarOp::GetUserVal()
{

	m_pcvarsBloc->GetValue(0, 0, (udword&)value);
	return value;
}

udword NUWordVarOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{

	m_pcvarsBloc->GetValue(0, 0, (udword&)value);

	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	//Only one Input
	//if (m_byInputs!=1)		return (udword)-1;

	//InsureCommonInputsSize(_pOpsInts, _fDetailFactor);



	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NLoadOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NLoadVarOp, NOperator);

static NVarsBlocDesc blocdescNLoadVarOp[] =
{
	VAR(efloat,	true, "Var", "1.0",	"NFloatProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NLoadVarOp::NLoadVarOp()
{
	//Create variables bloc
	indx = 0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNLoadVarOp, 1);
}

char* NLoadVarOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

float NLoadVarOp::GetUserVal()
{
	float ret;
	m_pcvarsBloc->GetValue(0, 0, (float&)ret);
	return ret;
}

udword NLoadVarOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float curr = 0.0;
	m_pcvarsBloc->GetValue(0, 0, (float&)value);

	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NIntVarOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVarsSumOp, NOperator);

static NVarsBlocDesc blocdescNUVarsSumOp[] =
{
	VAR(efloat,	true, "Var", "1.0",	"NFloatProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NVarsSumOp::NVarsSumOp()
{
	//Create variables bloc
	indx = 0;
	value = 0.0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNUVarsSumOp, 1);
}

NVarsSumOp::~NVarsSumOp()
{
}

char* NVarsSumOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

udword NVarsSumOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	NOperator *nOp = (NFloatVarOp*)*(_pOpsInts);
	NOperator *nOp2 = (NFloatVarOp*)*(_pOpsInts+1);

	float val1, val2;

	if(  !strcmp(nOp->GetRTClass()->m_pszClassName, "NUWordVarOp") )
	{
		NUWordVarOp *pvar1 = (NUWordVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}
	else
	{
		NFloatVarOp *pvar1 = (NFloatVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}

	value = val1 + val2;

	m_pcvarsBloc->SetValue(0, 0, value);


	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NVarsSubtractOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVarsSubtractOp, NOperator);

static NVarsBlocDesc blocdescNVarsSubtractOp[] =
{
	VAR(efloat,	true, "Var", "1.0",	"NFloatProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NVarsSubtractOp::NVarsSubtractOp()
{
	//Create variables bloc
	indx = 0;
	value = 0.0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNVarsSubtractOp, 1);
}

NVarsSubtractOp::~NVarsSubtractOp()
{
}

char* NVarsSubtractOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

udword NVarsSubtractOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	NOperator *nOp = (NFloatVarOp*)*(_pOpsInts);
	NOperator *nOp2 = (NFloatVarOp*)*(_pOpsInts+1);

	float val1, val2;

	if(  !strcmp(nOp->GetRTClass()->m_pszClassName, "NUWordVarOp") )
	{
		NUWordVarOp *pvar1 = (NUWordVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}
	else
	{
		NFloatVarOp *pvar1 = (NFloatVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}

	value = val1 - val2;

	m_pcvarsBloc->SetValue(0, 0, value);


	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	return 0;
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NVarsDivideOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVarsDivideOp, NOperator);

static NVarsBlocDesc blocdescNVarsDivideOp[] =
{
	VAR(efloat,	true, "Var", "1.0",	"NFloatProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NVarsDivideOp::NVarsDivideOp()
{
	//Create variables bloc
	indx = 0;
	value = 0.0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNVarsDivideOp, 1);
}

NVarsDivideOp::~NVarsDivideOp()
{
}

char* NVarsDivideOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

udword NVarsDivideOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	NOperator *nOp = (NFloatVarOp*)*(_pOpsInts);
	NOperator *nOp2 = (NFloatVarOp*)*(_pOpsInts+1);

	float val1, val2;

	if(  !strcmp(nOp->GetRTClass()->m_pszClassName, "NUWordVarOp") )
	{
		NUWordVarOp *pvar1 = (NUWordVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}
	else
	{
		NFloatVarOp *pvar1 = (NFloatVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}

	value = val1 / val2;

	m_pcvarsBloc->SetValue(0, 0, value);


	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NVarsMultiplyOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVarsMultiplyOp, NOperator);

static NVarsBlocDesc blocdescNVarsMultiplyOp[] =
{
	VAR(efloat,	true, "Var", "1.0",	"NFloatProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NVarsMultiplyOp::NVarsMultiplyOp()
{
	//Create variables bloc
	indx = 0;
	value = 0.0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNVarsMultiplyOp, 1);
}

NVarsMultiplyOp::~NVarsMultiplyOp()
{
}

char* NVarsMultiplyOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

udword NVarsMultiplyOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	NOperator *nOp = (NFloatVarOp*)*(_pOpsInts);
	NOperator *nOp2 = (NFloatVarOp*)*(_pOpsInts+1);

	float val1, val2;

	if(  !strcmp(nOp->GetRTClass()->m_pszClassName, "NUWordVarOp") )
	{
		NUWordVarOp *pvar1 = (NUWordVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}
	else
	{
		NFloatVarOp *pvar1 = (NFloatVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}

	value = val1 * val2;

	m_pcvarsBloc->SetValue(0, 0, value);


	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	return 0;
}





//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NVarsPowerOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVarsPowerOp, NOperator);

static NVarsBlocDesc blocdescNVarsNVarsPowerOp[] =
{
	VAR(efloat,	true, "Var", "1.0",	"NFloatProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NVarsPowerOp::NVarsPowerOp()
{
	//Create variables bloc
	indx = 0;
	value = 0.0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNVarsNVarsPowerOp, 1);
}

NVarsPowerOp::~NVarsPowerOp()
{
}

char* NVarsPowerOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

udword NVarsPowerOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	NOperator *nOp = (NFloatVarOp*)*(_pOpsInts);
	NOperator *nOp2 = (NFloatVarOp*)*(_pOpsInts+1);

	float val1, val2;

	if(  !strcmp(nOp->GetRTClass()->m_pszClassName, "NUWordVarOp") )
	{
		NUWordVarOp *pvar1 = (NUWordVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}
	else
	{
		NFloatVarOp *pvar1 = (NFloatVarOp*)*(_pOpsInts);

		if(  !strcmp(nOp2->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pvar2 = (NUWordVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
		else
		{
			NFloatVarOp *pvar2 = (NFloatVarOp*)*(_pOpsInts+1);

			val1 = (float)pvar1->GetUserVal();
			val2 = (float)pvar2->GetUserVal();
		}
	}

	value = pow(val1, val2);

	m_pcvarsBloc->SetValue(0, 0, value);


	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NVarsPowerOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVarsLogOp, NOperator);

static NVarsBlocDesc blocdescNVarsNVarsLogOp[] =
{
	VAR(efloat,	true, "Var", "1.0",	"NFloatProp")	//0
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NVarsLogOp::NVarsLogOp()
{
	//Create variables bloc
	indx = 0;
	value = 0.0;
	m_pcvarsBloc = AddVarsBloc(2, blocdescNVarsNVarsLogOp, 1);
}

NVarsLogOp::~NVarsLogOp()
{
}

char* NVarsLogOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(1, 0, pszname);
	return pszname;
}

udword NVarsLogOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{

	if (m_byInputs!=1)		return (udword)-1;

	NOperator *nOp = (NFloatVarOp*)*(_pOpsInts);

	float val1, val2;

	if(  !strcmp(nOp->GetRTClass()->m_pszClassName, "NUWordVarOp") )
	{
		NUWordVarOp *pvar1 = (NUWordVarOp*)*(_pOpsInts);
		val1 = (float)pvar1->GetUserVal();
	}
	else
	{
		NFloatVarOp *pvar1 = (NFloatVarOp*)*(_pOpsInts);
		val1 = (float)pvar1->GetUserVal();
	}

	value = log( val1 );

	m_pcvarsBloc->SetValue(0, 0, value);

	for( int i = 0; i < indx; i++ )
	{
		(*val[i]) = value;
	}

	return 0;
}
