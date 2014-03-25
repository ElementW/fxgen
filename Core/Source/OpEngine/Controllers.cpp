//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Controllers.cpp
//! \brief	Standart animation controllers
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
#include "EngineOp.h"
#include "Controllers.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NOpGraphModel class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NController, NObject);

static NVarsBlocDesc blocdescControllersOp[] =
{
	VAR(eubyte,	false, "Channel",	"0",		"NUbyteProp", 0.0f, 255.0f, 1.0f)	//0
	VAR(efloat,	false, "Factor",	"1.0",	"NFloatProp", 0.0f, 100.0f, 0.1f)	//1
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
	NEngineOp::GetInstance()->GetChannelValue(byChannel, value);

	//Calcul
	return value.fVal*fFactor;
}

