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

#include "../../Include/CoreLib.h"
#include "MainOps.h"
#include "Bitmap.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NStoreOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NStoreOp, NOperator);

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

const char* NStoreOp::GetUserName()
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
	NEngineOp::GetInstance()->GetBitmap(&m_pObj, OBJRES_TYPE_STORED);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));

	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NStoreResultOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NStoreResultOp, NOperator);

static NMapVarsBlocDesc mapblocdescStoreResultOp[] =
{
	MAP(1, estring,	"0", "" )	//V1 => 0-name
};

static NVarsBlocDesc blocdescStoreResultOp[] =
{
	VAR(estring,	false, "Name", "",	"NStringProp")	//0
	VAR(eubyte,	false, "Type", "0,[Preview,Diffuse,Normal,Specular,Height]",	"NUbyteComboProp")	//1
};

NStoreResultOp::NStoreResultOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(2, blocdescStoreResultOp, 2);
	//To Keep compatibility with oldier blocs versions (will be removed after alpha)
	m_pcvarsBloc->SetMapVarBlocDesc(1, mapblocdescStoreResultOp);
}

NStoreResultOp::~NStoreResultOp()
{
}

const char* NStoreResultOp::GetUserName()
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
	NEngineOp::GetInstance()->GetBitmap(&m_pObj, OBJRES_TYPE_FINALSTORED);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));

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
FIMPLEMENT_CLASS(NLoadOp, NOperator);

static NVarsBlocDesc blocdescLoadOp[] =
{
	VAR(erefobj,	false, "Load",	"0", "NUseStoredOpsProp")	//0
};

NLoadOp::NLoadOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescLoadOp, 1);
}

const char* NLoadOp::GetUserName()
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
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

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
		memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));
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
FIMPLEMENT_CLASS(NChannelAnimFX1Op, NOperator);

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
	NEngineOp::GetInstance()->SetChannelValue(byChannel, value);

	return 0;
}
