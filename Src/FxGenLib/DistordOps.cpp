//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DistordOps.cpp
//! \brief	Distord Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		17-05-2007
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
#include "DistordOps.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRotoZoomOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NRotoZoomOp, NOperator);

static NVarsBlocDesc blocdescRotoZoomOp[] =
{
	VAR(eubyte,	false, "Set Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Set Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(efloat,		true, "CenterX",	"0.5",		"NFloatProp")	//2
	VAR(efloat,		true, "CenterY",	"0.5",		"NFloatProp")	//3
	VAR(efloat,		true, "Rotate",		"0.0",		"NFloatProp")	//4
	VAR(efloat,		true, "ZoomX",		"1.0",		"NFloatProp")	//5
	VAR(efloat,		true, "ZoomY",		"1.0",		"NFloatProp")	//6
};

NRotoZoomOp::NRotoZoomOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescRotoZoomOp);
}

NRotoZoomOp::~NRotoZoomOp()
{
}

udword NRotoZoomOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword ws = pSrc->GetWidth();
	udword hs = pSrc->GetHeight();

	//Get Variables Values
	ubyte byVal;

	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);

	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	pDst->SetSize(w, h);

	float fCenterX, fCenterY, fRotate, fZoom;
	m_pcvarsBloc->GetValue(0, _ftime, fCenterX);	//-0.5 <> +0.1
	m_pcvarsBloc->GetValue(1, _ftime, fCenterY);	//-0.5 <> +0.1
	m_pcvarsBloc->GetValue(2, _ftime, fRotate);		//0.0 <> 1.0 => 0 => Pi
	m_pcvarsBloc->GetValue(3, _ftime, fZoom);			//

	//Rotate
	fRotate = fRotate * nv_two_pi;

	//Zoom
	fZoom=0.5f - fZoom;
	fZoom=exp(fZoom*6.0f);

	//CenterX and CenterY
/*	uword CenterX = byCenterX;		//CenterX 0-255 (128=center)
	uword CenterY = byCenterY;		//CenterY 0-255	(128=center)
	CenterX<<=8;
	CenterY<<=8;

	//Process RotoZoom
	RGBA* pPxlSrc = pSrc->GetPixels();
	RGBA* pPxlDst = pDst->GetPixels();

	uword	c = (uword) (cos(fRotate) * fZoom * 256.0f);
	uword	s = (uword) (sin(fRotate) * fZoom * 256.0f);

	uword	ys = (uword)(s*-128);
	uword	yc = (uword)(c*-128);
	RGBI Color;

	for (udword y=0; y<h; y++)
	{
		uword	u = ((c*-128)-ys) + CenterX;			// x' = cos(x)-sin(y) + Center X;
		uword	v = ((s*-128)+yc) + CenterY;			// y' = sin(x)+cos(y) + Center Y;

		for (udword x=0; x<w; x++)
		{
			uword	uf			= u&0xff;	//Fraction
			uword	vf			= v&0xff;	//Fraction
			uword  inv_uf	= 255-uf;
			uword  inv_vf	= 255-vf;
			uword	ut			= (u&0xff00)>>8;
			uword	vt			= (v&0xff00)>>8;
			//Texels
			// 1 | 2
			//-------
			// 3 | 4

			//Texel1
			uword WeightFactors =	(inv_uf * inv_vf)>>8;	//(inv_uf * inv_vf);

			//RGBA* ptexel = pPxlSrc + ((v&0xff00) + (u>>8));
			RGBA* ptexel = pPxlSrc + ((vt%h)*w) + (ut%w);
			Color.r = ((udword)ptexel->r	* WeightFactors)>>8;
			Color.g = ((udword)ptexel->g	* WeightFactors)>>8;
			Color.b = ((udword)ptexel->b	* WeightFactors)>>8;

			//Texel2
			WeightFactors = (uf * inv_vf)>>8;	//(uf * inv_vf);
			//ptexel = pPxlSrc + ((v&0xff00) + ((u+0xff)>>8));
			ptexel = pPxlSrc + ((vt%h)*w) + ((ut+1)%w);
			Color.r	+= ((udword)ptexel->r	* WeightFactors)>>8;
			Color.g	+= ((udword)ptexel->g	* WeightFactors)>>8;
			Color.b	+= ((udword)ptexel->b	* WeightFactors)>>8;

			//Texel3
			WeightFactors = (inv_uf*vf)>>8;	//(inv_uf * vf);
			//ptexel = pPxlSrc + (((v+0xff)&0xff00) + (u>>8));
			ptexel = pPxlSrc + (((vt+1)%h)*w) + (ut%w);
			Color.r	+= ((udword)ptexel->r	* WeightFactors)>>8;
			Color.g	+= ((udword)ptexel->g	* WeightFactors)>>8;
			Color.b	+= ((udword)ptexel->b	* WeightFactors)>>8;

			//Texel4
			WeightFactors = (uf*vf)>>8;	//(uf * vf);
			//ptexel = pPxlSrc + (((v+0xff)&0xff00) + ((u+0xff)>>8));
			ptexel = pPxlSrc + (((vt+1)%h)*w) + ((ut+1)%w);
			Color.r	+= ((udword)ptexel->r	* WeightFactors)>>8;
			Color.g	+= ((udword)ptexel->g	* WeightFactors)>>8;
			Color.b	+= ((udword)ptexel->b	* WeightFactors)>>8;

			//Pixel
			pPxlDst->r = (ubyte)Color.r;
			pPxlDst->g = (ubyte)Color.g;
			pPxlDst->b = (ubyte)Color.b;
			pPxlDst->a = 255;
			pPxlDst++;

			//Vectors
			u+=c;	v+=s;
		};

		//Vectors
		ys+=s; yc+=c;
	};
*/

	return 1;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NDistortOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NDistortOp, NOperator);

static NVarsBlocDesc blocdescDistortOp[] =
{
	VAR(eudword,	true, "Power",	"0", "NUbyteProp")	//0
};

NDistortOp::NDistortOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescDistortOp);

}

udword NDistortOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Two inputs (texture, normal)
	if (m_byInputs!=2)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input Texture and Normal
	NBitmap* pSrc	= (NBitmap*)(*_pOpsInts)->m_pObj;
	_pOpsInts++;
	NBitmap* pNorm = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst	= (NBitmap*)m_pObj;

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();

	/////////////////////////////////////////
	//Get Variables Values
	ubyte byPower;
	m_pcvarsBloc->GetValue(0, _ftime, byPower);

	float fPower = (float)byPower;	//0.0<->8.0

	/////////////////////////////////////////
	//Process operator
	RGBA* pPxNorm = pNorm->GetPixels();
	RGBA* pPxSrc	= pSrc->GetPixels();
	RGBA* pPxDst	= pDst->GetPixels();

	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			//Normalized normal map
			vec3 n;
			n.x = ((float)pPxNorm->x - 127.0f);
			n.y = ((float)pPxNorm->y - 127.0f);
			n.z = ((float)pPxNorm->z - 127.0f);
			n.normalize();

			//Get texel
			udword u = (x + (sdword)(n.x*fPower)) % w;
			udword v = (y + (sdword)(n.y*fPower)) % h;

			*pPxDst++ = pPxSrc[u + (v*w)];

			pPxNorm++;
		}
	}

	return 0;
}

