//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MainOps.cpp
//! \brief	Main Operators
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
FIMPLEMENT_CLASS(NStoreOp, NOperatorFx);

static NVarsBlocDesc blocdescStoreOp[] =
{
	VAR(estring,	false, "Name", "",	"NStringProp", 0.0f, 0.0f, 0.0f)	//0
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

udword NStoreOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj, OBJRES_TYPE_STORED);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

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
//							NOutputOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NOutputOp, NOperatorFx);


static NVarsBlocDesc blocdescOutputOp[] =
{
	VAR(estring,	false, "Name", "",	"NStringProp", 0.0f, 0.0f, 0.0f)	//0
	VAR(eubyte,	false, "Type", "0,[Preview,Diffuse,Normal,Specular,Height]",	"NUbyteComboProp", 0.0f, 0.0f, 0.0f)	//1
};

NOutputOp::NOutputOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(2, blocdescOutputOp, 1);
}

NOutputOp::~NOutputOp()
{
}

const char* NOutputOp::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(0, 0, pszname);
	return pszname;
}


udword NOutputOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj, OBJRES_TYPE_FINALSTORED);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));

	return 0;
}

N2DBitmap* NOutputOp::GetBitmap()
{
	N2DBitmap* pbmp = null;
	if (m_pObj!=null)
	{
		if (strcmp(m_pObj->GetRTClass()->m_pszClassName, "N2DBitmap") == 0)
			pbmp = (N2DBitmap*)m_pObj;
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
FIMPLEMENT_CLASS(NLoadOp, NOperatorFx);

static NVarsBlocDesc blocdescLoadOp[] =
{
	VAR(erefobj,	false, "Load",	"0", "NUseStoredOpsProp", 0.0f, 0.0f, 0.0f)	//0
};

NLoadOp::NLoadOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescLoadOp, 1);
}

const char* NLoadOp::GetUserName()
{
		//Get Variables Values
	NOperatorFx* popRef;
	m_pcvarsBloc->GetValue(0, 0, (NObject*&)popRef);

	//Process
	if (popRef)
		return popRef->GetUserName();

	return "";
}

udword NLoadOp::Process(float _ftime, SEngineState& _state)
{
	if (m_byInputs!=0)	//Load operator must'n have one input !
		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get Variables Values
	NOperatorFx* popRef;
	m_pcvarsBloc->GetValue(0, _ftime, (NObject*&)popRef);

	//Process
	if (popRef && popRef->GetResource())
	{
		N2DBitmap* pSrc = (N2DBitmap*)popRef->GetResource();
		N2DBitmap* pDst = (N2DBitmap*)m_pObj;

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
FIMPLEMENT_CLASS(NChannelAnimFX1Op, NOperatorFx);

static NVarsBlocDesc blocdescChanAnimFX1Op[] =
{
	VAR(eubyte,	false, "Channel",			"0",								"NUbyteProp", 0.0f, 0.0f, 0.0f)	//0
	VAR(eubyte,	false, "Type",				"0,[Linear,Sine]",	"NUbyteComboProp", 0.0f, 0.0f, 0.0f)	//1
	VAR(efloat,	true, "Start Value",	"0.0",							"NFloatProp", -1.0f, 1.0f, 0.001f)	//2
	VAR(efloat,	true, "End Value",		"1.0",							"NFloatProp", -1.0f, 1.0f, 0.001f)	//3
	VAR(efloat,	true, "Factor",				"1.0",							"NFloatProp", 0.0f, 1000.0f, 0.001f)	//4
	VAR(eubyte,	true, "Repeat",				"0",								"NUbyteProp", 0.0f, 255.0f, 1.0f)	//5
};

NChannelAnimFX1Op::NChannelAnimFX1Op()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescChanAnimFX1Op, 1);
}

udword NChannelAnimFX1Op::Process(float _ftime, SEngineState& _state)
{
	if (m_byInputs!=0)
	{
		NResourceFx* pSrc = (NResourceFx*)_state.apInputs[0];
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

