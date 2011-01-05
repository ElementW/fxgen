//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RenderOps.h
//! \brief	Render Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
//!					Sebastian Olter (qduaty@gmail.com)
//!
//!	\date		07-05-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef RENDEROPS_H
#define RENDEROPS_H


//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "EngineOp.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//NObject
//	NOperatorFx
//		NFlatOp
//		NCloudOp
//		NGradientOp
//		NCellOp
//		NNoiseOp

//-----------------------------------------------------------------
//!	\class		NFlatOp
//!	\brief		Flat rendering
//-----------------------------------------------------------------
class CORELIB_API NFlatOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NFlatOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Flat"; }			//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperatorFx** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NCloudOp
//!	\brief		Render clouds
//-----------------------------------------------------------------
class CORELIB_API NCloudOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NCloudOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Cloud"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperatorFx** _pOpsInts, float _fDetailFactor);

	//Methods
	void	Cloud(ubyte byoctaves, float fampMod);
	void	AddRandom(float fampMod);
	void	ImageResize(udword w, udword h);
	void	BoxBlur(udword bw, udword bh);
	void	Normalize();

	//Datas
	udword m_dwWidth, m_dwHeight;	//!< Temporary image size
	sdword* m_pbyPxTps1, *m_pbyPxTps2;	//!< the two Temporary buffers
	sdword* m_pbyPxTps;	//!< pointeur for memfree

};

//-----------------------------------------------------------------
//!	\class		NGradientOp
//!	\brief		Four corners gradient rendering
//-----------------------------------------------------------------
class CORELIB_API NGradientOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NGradientOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Gradient"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperatorFx** _pOpsInts, float _fDetailFactor);

};

//-----------------------------------------------------------------
//!	\class		NCellOp
//!	\brief		Render cells
//-----------------------------------------------------------------
class CORELIB_API NCellOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NCellOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Cell"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperatorFx** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NNoiseOp
//!	\brief		Render noise
//-----------------------------------------------------------------
class CORELIB_API NNoiseOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NNoiseOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Noise"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperatorFx** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NVectorOp
//!	\brief		Render vector graphics
//-----------------------------------------------------------------
/*class CORELIB_API NVectorOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NVectorOp();
	~NVectorOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Vector"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	void			SetVectorData(ubyte* _data, udword _length);

	//Processing methods
	virtual udword Process(float _ftime, NOperatorFx** _pOpsInts, float _fDetailFactor);

private:
	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	vgvm::cairo::CairoContext*	m_pContext;
	vgvm::Program*			m_pProgram;
};*/

#endif //RENDEROPS_H
