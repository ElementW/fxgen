//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Controllers.cpp
//! \brief	Standart animation controllers
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
#include "Controllers.h"
#include "EngineOp.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NOperatorsPage class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NController, NObject);

static NVarsBlocDesc blocdescControllersOp[] =
{
	VAR(eubyte,	false, "Channel",			"0",								"NUbyteProp")	//0
	VAR(efloat,	false, "Factor",			"1.0",							"NFloatProp")	//1
};

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NController::NController()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(2, blocdescControllersOp, 1);
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NController::~NController()
{
}

//-----------------------------------------------------------------
//!	\brief	Return value
//!	\param	_fTime	time
//!	\return return value
//-----------------------------------------------------------------
float NController::GetValue(float _fTime)
{
	//Get Variables Values
	float fFactor;
	ubyte byChannel;
	m_pcvarsBloc->GetValue(0, _fTime, byChannel);
	m_pcvarsBloc->GetValue(1, _fTime, fFactor);

	//Get Channel Value
	NVarValue value;
	NEngineOp::GetEngine()->GetChannelValue(byChannel, value);

	//Calcul
	return value.fVal*fFactor;
}

