//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		VectorOp.h
//! \brief	Vector Render Operator
//!
//!	\author Anders Stenberg (anders.stenberg@gmail.com)
//!
//!	\date		10-02-2008
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef VECTOROP_H
#define VECTOROP_H


//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "../../Src/FxGenLib/EngineOp.h"
#include "../../Src/FxGenLib/MainOps.h"
#include "../../Src/FxGenLib/Bitmap.h"

//#include "EngineOp.h"

extern const char* GetModuleName();  //!< Must had been defined in application and per dynamic libraries(dll, so)


//-----------------------------------------------------------------
//                   Forward declarations
//-----------------------------------------------------------------
/*namespace vgvm
{
	namespace cairo
	{
		class CairoContext;
	}
	class Program;
}*/


//-----------------------------------------------------------------
//!	\class		NVectorOp
//!	\brief		Render vector graphics
//-----------------------------------------------------------------
class NVectorOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NVectorOp();
	~NVectorOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Vector"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

//	void			SetVectorData(ubyte* _data, udword _length);

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

/*
private:
	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	vgvm::cairo::CairoContext*	m_pContext;
	vgvm::Program*			m_pProgram;
*/
};

#endif //VECTOROP_H
