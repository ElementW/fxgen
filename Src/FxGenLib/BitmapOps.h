//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		BitmapOps.h
//! \brief	Bitmap Operators
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
#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "EngineOp.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//NObject
//	NOperator
//		NNopOp
//		NFlatOp
//		NRectOp
//		NPixelsOp
//		NAddOp
//		NGlowOp
//		NRotoZoomOp
//		NBlurOp
//		NColorsOp
//		NLightOp
//		NNormalsOp
//		NCrackOp
//		NCloudOp
//		NDistortOp
//		NSetSizeOp
//		NGradientOp

//-----------------------------------------------------------------
//!	\class		NNopOp
//!	\brief		Nop operator
//-----------------------------------------------------------------
class NNopOp : public NOperator
{
public:
	DECLARE_CLASS();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Nop"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Misc"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NFlatOp
//!	\brief		Flat rendering
//-----------------------------------------------------------------
class NFlatOp : public NOperator
{
public:
	DECLARE_CLASS();

	NFlatOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Flat"; }			//!< Operator's Name
	virtual char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NRectOp
//!	\brief		Rectangle rendering
//-----------------------------------------------------------------
class NRectOp : public NOperator
{
public:
	DECLARE_CLASS();

	NRectOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Rect"; }			//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};


//-----------------------------------------------------------------
//!	\class		NPixelsOp
//!	\brief		Pixels rendering
//-----------------------------------------------------------------
class NPixelsOp : public NOperator
{
public:
	DECLARE_CLASS();

	NPixelsOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Pixels"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NAddOp
//!	\brief		Add bitmaps
//-----------------------------------------------------------------
class NAddOp : public NOperator
{
public:
	DECLARE_CLASS();

	NAddOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Add"; }			//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NGlowOp
//!	\brief		Glow rendering
//-----------------------------------------------------------------
class NGlowOp : public NOperator
{
public:
	DECLARE_CLASS();

	NGlowOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Glow"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category


	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};

//-----------------------------------------------------------------
//!	\class		NRotoZoomOp
//!	\brief		RotoZoom filtering
//-----------------------------------------------------------------
class NRotoZoomOp : public NOperator
{
public:
	DECLARE_CLASS();

	NRotoZoomOp();
	virtual ~NRotoZoomOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Roto Zoom"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Distord"; }		//!< Operator's Category


	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NBlurOp
//!	\brief		Blur filtering
//-----------------------------------------------------------------
class NBlurOp : public NOperator
{
public:
	DECLARE_CLASS();

	NBlurOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Blur"; }			//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NColorsOp
//!	\brief		Colors filtering
//-----------------------------------------------------------------
class NColorsOp : public NOperator
{
public:
	DECLARE_CLASS();

	NColorsOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Colors"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};


//-----------------------------------------------------------------
//!	\class		NLightOp
//!	\brief		Lighting rendering
//-----------------------------------------------------------------
class NLightOp : public NOperator
{
public:
	DECLARE_CLASS();

	NLightOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Light"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};


//-----------------------------------------------------------------
//!	\class		NJPEGOp
//!	\brief		JPEG datas loader
//-----------------------------------------------------------------
/*
class NJPEGOp : public NOperator
{
public:
	DECLARE_CLASS();

	NJPEGOp();

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};
*/

//-----------------------------------------------------------------
//!	\class		NNormalsOp
//!	\brief		Compute 2D Normals
//-----------------------------------------------------------------
class NNormalsOp : public NOperator
{
public:
	DECLARE_CLASS();

	NNormalsOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Normals"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NCrackOp
//!	\brief		Compute cracked bitmap
//-----------------------------------------------------------------
class NCrackOp : public NOperator
{
public:
	DECLARE_CLASS();

	NCrackOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Crack"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

	//Methods
	float myfRandom();

};

//-----------------------------------------------------------------
//!	\class		NCloudOp
//!	\brief		Render clouds
//-----------------------------------------------------------------
class NCloudOp : public NOperator
{
public:
	DECLARE_CLASS();

	NCloudOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Cloud"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

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
//!	\class		NDistordOp
//!	\brief		Distort filtering
//-----------------------------------------------------------------
class NDistortOp : public NOperator
{
public:
	DECLARE_CLASS();

	NDistortOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Distort"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};

//-----------------------------------------------------------------
//!	\class		NSetSizeOp
//!	\brief		Change bitmap size
//-----------------------------------------------------------------
class NSetSizeOp : public NOperator
{
public:
	DECLARE_CLASS();

	NSetSizeOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "SetSize"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Misc"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};

//-----------------------------------------------------------------
//!	\class		NGradientOp
//!	\brief		Four corners gradient rendering
//-----------------------------------------------------------------
class NGradientOp : public NOperator
{
public:
	DECLARE_CLASS();

	NGradientOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Gradient"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Render"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};

