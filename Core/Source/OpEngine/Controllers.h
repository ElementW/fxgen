//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Controllers.h
//! \brief	standart animation controllers
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
#ifndef CONTROLLERS_H
#define CONTROLLERS_H


//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "../../Include/CoreLib.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NObject;
	class NControllers;

//-----------------------------------------------------------------
//!	\class		NController
//!	\brief		Base class for an animation controller
//!
//!	\author		JN
//!	\version  1.0
//-----------------------------------------------------------------
class CORELIB_API NController :	public NObject
{
public:
	FDECLARE_CLASS();

	NController();
	virtual ~NController();

	float GetValue(float _fTime);

protected:
	// Variables Bloc
	NVarsBloc* m_pcvarsBloc;
};

#endif //CONTROLLERS_H
