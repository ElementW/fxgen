//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		BrickStone.cpp
//! \brief	BrickStone operator plug-in for FxGen
//!
//!	\author	Sebastian Olter (qduaty@gmail.com)
//!	\date		25-02-2008
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

#include "fxgen_pch.h"

//-----------------------------------------------------------------
//!	\class		NBrickStoneOp
//!	\brief		Render brick or stone wall
//-----------------------------------------------------------------
class FXGEN_API NBrickStoneOp : public NOperator
{
public:
	DECLARE_CLASS();

	NBrickStoneOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "BrickStone"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

IMPLEMENT_CLASS(NBrickStoneOp, NOperator);

// Description:
// Rows - how many bricks can fit into the texture vertically
// Cols - how many bricks can fit into the texture horizontally
// Variance X - how much can they differ in X axis (length): l +/- l * (1 - varX)
// Variance Y - how much can they differ in Y axis (height): h +/- d * varX
// Mortar - size of the gap between bricks, relative to the height of lines: d
// Brick shade -
// Gap shade -
//
static NVarsBlocDesc blocdescBrickStoneOp[] =
{
	VAR(eubyte,	false, "Width",			"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eubyte,	true, 	"Rows",			"8",	"NUbyteProp")	//2
	VAR(eubyte,	true, 	"Cols",			"3",	"NUbyteProp")	//3
	VAR(efloat, true, 	"Mortar",		"0.2",	"NCFloatProp")	//4
	VAR(efloat,	true, 	"Variance X",	"0.0",	"NCFloatProp")	//5
	VAR(efloat,	true, 	"Variance Y",	"0.0",	"NCFloatProp")	//6
	VAR(euword,	true, 	"Seed",			"5412",	"NUwordProp") //7
	VAR(efloat, true, 	"Brick Shade",	"0.0",	"NUFloatProp")	//8
	VAR(efloat, true, 	"Gap Shade",	"0.0",	"NFloatProp")	//9
};

NBrickStoneOp::NBrickStoneOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(10, blocdescBrickStoneOp, 1);
}


class RectangleCorner: public NTreeNode
{
	;
};


/* Algorithm:


*/
udword NBrickStoneOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//No Inputs
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;
	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);
	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	w=(udword)(w*_fDetailFactor);
	h=(udword)(h*_fDetailFactor);

	ubyte rows, cols;
	float varX, varY, mortar, bshade, gshade;
	uword seed;
	m_pcvarsBloc->GetValue(2, _ftime, rows);
	rows = max(1, rows);
	m_pcvarsBloc->GetValue(3, _ftime, cols);
	cols = max(1, cols);
	m_pcvarsBloc->GetValue(4, _ftime, mortar);
	m_pcvarsBloc->GetValue(5, _ftime, varX);
	m_pcvarsBloc->GetValue(6, _ftime, varY);
	m_pcvarsBloc->GetValue(7, _ftime, seed);
	m_pcvarsBloc->GetValue(8, _ftime, bshade);
	m_pcvarsBloc->GetValue(9, _ftime, gshade);

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);
	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w, h);
	RGBAArray arrDst(pDst->GetPixels(), w, h);

	SetSeedValue(seed);


//	// create texels using rows, cols, offsets, dist, varX, varY
//	int xoffset = w / cols / 2;
//	int yoffset = h / rows / 2;
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
		}
	}


	//Temporary buffeur to Texture
	RGBA black(0,0,0,255);
	RGBA gray(127,127,127,255);
	RGBA white(255,255,255,255);
	RGBA red(255,0,0,255);
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			arrDst(x,y) = black;
		}
	}

	return 0;
}
