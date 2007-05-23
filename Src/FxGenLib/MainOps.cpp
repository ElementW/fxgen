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

udword NStoreOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj, true);	//Get a permanent bitmap

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));

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


udword NStoreResultOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj, true);	//Get a permanent bitmap

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));

	return 0;
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

udword NLoadOp::Process(float _ftime, NOperator** _pOpsInts)
{
	if (m_byInputs!=0)	//Load operator must'n have one input !
		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

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
		CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));
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

udword NChannelAnimFX1Op::Process(float _ftime, NOperator** _pOpsInts)
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
	gNFxGen_GetEngine()->SetChannelValue(byChannel, value);

	return 0;
}

