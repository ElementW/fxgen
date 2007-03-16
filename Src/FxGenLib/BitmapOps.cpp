//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		BitmapOps.h
//! \brief	Bitmap operators
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
#include "bitmapOps.h"
//#include "JPEGLoader.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NNopOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NNopOp,	NOperator);

udword NNopOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	//Copy Source to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), pDst->GetWidth() * pDst->GetHeight() * sizeof(RGBA));

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NFlatOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NFlatOp, NOperator);

static NVarsBlocDesc blocdescFlatOp[] =
{
	VAR(eudword,	true, "Color",	"4278190080", "NColorProp")	//0
};

NFlatOp::NFlatOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescFlatOp);
}

udword NFlatOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//No Inputs!
	if (m_byInputs!=0)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get Variables Values
	RGBA col;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)col);

	//Process operator
	NBitmap* pSrc = (NBitmap*)m_pObj;
	RGBA* pPxDst = pSrc->GetPixels();
	for (udword y=0; y<pSrc->GetHeight(); y++)
		for (udword x=0; x<pSrc->GetWidth(); x++)
			*pPxDst++ = col;

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRectOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NRectOp, NOperator);

static NVarsBlocDesc blocdescRectOp[] =
{
	VAR(eudword,	true, "Color",	"-1",		"NColorProp")	//0
	VAR(eubyte,		true, "x1",			"0",		"NUbyteProp")	//1
	VAR(eubyte,		true, "y1",			"0",		"NUbyteProp")	//2
	VAR(eubyte,		true, "x2",			"128",	"NUbyteProp")	//3
	VAR(eubyte,		true, "y2",			"128",	"NUbyteProp")	//4
};

NRectOp::NRectOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(5, blocdescRectOp);
}

udword NRectOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	//Get Variables Values
	RGBA col;
	ubyte x1,y1,x2,y2;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)col);
	m_pcvarsBloc->GetValue(1, _ftime, x1);
	m_pcvarsBloc->GetValue(2, _ftime, y1);
	m_pcvarsBloc->GetValue(3, _ftime, x2);
	m_pcvarsBloc->GetValue(4, _ftime, y2);

	//Copy Source to this bitmap
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), pDst->GetWidth() * pDst->GetHeight() * sizeof(RGBA));

	//Process operator
	for (udword y=y1; y<=y2; y++)
	{
		RGBA* pPixelsD = pDst->GetPixels() + (y*pDst->GetWidth()) + x1;
		for (udword x=x1; x<=x2; x++)
			*pPixelsD++ = col;
	}

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NPixelsOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NPixelsOp,NOperator);

static NVarsBlocDesc blocdescPixelsOp[] =
{
	VAR(eudword,	true, "Color",	"-1",			"NColorProp")	//0
	VAR(euword,		true, "Seed",		"5412",		"NUwordProp")	//1
	VAR(eubyte,		true, "Count",	"128",		"NUbyteProp")	//2
};

NPixelsOp::NPixelsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(3, blocdescPixelsOp);

}

udword NPixelsOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	//Get Variables Values
	RGBA col;
	ubyte byCount;
	uword wSeed;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)col);
	m_pcvarsBloc->GetValue(1, _ftime, wSeed);
	m_pcvarsBloc->GetValue(2, _ftime, byCount);

	//Copy Source to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), pDst->GetWidth() * pDst->GetHeight() * sizeof(RGBA));

	//Process operator
	SetSeedValue(wSeed);

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();
	udword dwCount = 2<<(byCount>>4);
	RGBA* pPxDst = pDst->GetPixels();
	while (--dwCount)
	{
		udword x=myRandom() % w;
		udword y=myRandom() % h;
		pPxDst[x + y*w] = col;
	}

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NAddOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NAddOp, NOperator);

static NVarsBlocDesc blocdescAddOp[] =
{
	VAR(eubyte,		false, "Mode",				"0,[Add Clamp,Add Wrap,Sub Clamp,Sub Wrap,Multiply,Multiply x2,Blend,Alpha]", "NUbyteComboProp")	//0
	VAR(eudword,	true,	 "RGB Percent",	"-1" , "NColorProp")			//1
};

NAddOp::NAddOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(2, blocdescAddOp);
}

udword NAddOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//x Inputs
	if (m_byInputs<1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();

	//Copy Source 1 to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));

	//Get Variables Values
	RGBA colPercent;
	ubyte byMode;
	m_pcvarsBloc->GetValue(0, _ftime, byMode);
	m_pcvarsBloc->GetValue(1, _ftime, (udword&)colPercent);

	udword rcolPercent = colPercent.r;
	udword gcolPercent = colPercent.g;
	udword bcolPercent = colPercent.b;
	udword acolPercent = 255;

	///////////////////////////////////////////////////////////
	//Add n Sources
	for (udword i=1; i<(udword)m_byInputs; i++)
	{
		_pOpsInts++;
		NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
		if (pSrc==null)		break;

		RGBA* pPxSrc = pSrc->GetPixels();
		RGBA* pPxDst = pDst->GetPixels();

		//////////////////////////////////////////////
		//Add Clamp
		if (byMode==0)
		{
			for (udword y=0; y<h; y++)
			{
				for (udword x=0; x<w; x++)
				{
					udword r = (udword)pPxDst->r + (((udword)pPxSrc->r * rcolPercent)>>8);
					udword g = (udword)pPxDst->g + (((udword)pPxSrc->g * gcolPercent)>>8);
					udword b = (udword)pPxDst->b + (((udword)pPxSrc->b * bcolPercent)>>8);
					pPxDst->r = (ubyte) ((r<255)?r:255);
					pPxDst->g = (ubyte) ((g<255)?g:255);
					pPxDst->b = (ubyte) ((b<255)?b:255);
					pPxDst++;	pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Add Wrap
		} else if (byMode==1)	{

			for (udword y=0; y<h; y++)
			{
				for (udword x=0; x<w; x++)
				{
					udword r = (udword)pPxDst->r + (((udword)pPxSrc->r * rcolPercent)>>8);
					udword g = (udword)pPxDst->g + (((udword)pPxSrc->g * gcolPercent)>>8);
					udword b = (udword)pPxDst->b + (((udword)pPxSrc->b * bcolPercent)>>8);
					pPxDst->r = (ubyte) (r%255);
					pPxDst->g = (ubyte) (g%255);
					pPxDst->b = (ubyte) (b%255);
					pPxDst++;	pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Sub Clamp
		} else if (byMode==2)	{

			for (udword y=0; y<h; y++)
			{
				for (udword x=0; x<w; x++)
				{
					sdword r = (sdword)pPxDst->r - (((sdword)pPxSrc->r * rcolPercent)>>8);
					sdword g = (sdword)pPxDst->g - (((sdword)pPxSrc->g * gcolPercent)>>8);
					sdword b = (sdword)pPxDst->b - (((sdword)pPxSrc->b * bcolPercent)>>8);
					pPxDst->r = (ubyte) ((r>0)?r:0);
					pPxDst->g = (ubyte) ((g>0)?g:0);
					pPxDst->b = (ubyte) ((b>0)?b:0);
					pPxDst++;	pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Sub Wrap
		} else if (byMode==3)	{

			for (udword y=0; y<h; y++)
			{
				for (udword x=0; x<w; x++)
				{
					udword r = (udword)pPxDst->r - (((udword)pPxSrc->r * rcolPercent)>>8);
					udword g = (udword)pPxDst->g - (((udword)pPxSrc->g * gcolPercent)>>8);
					udword b = (udword)pPxDst->b - (((udword)pPxSrc->b * bcolPercent)>>8);
					pPxDst->r = (ubyte) (r%255);
					pPxDst->g = (ubyte) (g%255);
					pPxDst->b = (ubyte) (b%255);
					pPxDst++;	pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Multiply
		} else if (byMode==4)	{

			for (udword y=0; y<h; y++)
			{
				for (udword x=0; x<w; x++)
				{
					udword r = (udword)pPxDst->r * (((udword)pPxSrc->r * rcolPercent)>>8);
					udword g = (udword)pPxDst->g * (((udword)pPxSrc->g * gcolPercent)>>8);
					udword b = (udword)pPxDst->b * (((udword)pPxSrc->b * bcolPercent)>>8);

					pPxDst->r = (ubyte) (r>>8);
					pPxDst->g = (ubyte) (g>>8);
					pPxDst->b = (ubyte) (b>>8);

					pPxDst++;		pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Multiply X2
		} else if (byMode==5)	{

			for (udword y=0; y<h; y++)
			{
				for (udword x=0; x<w; x++)
				{
					udword r = (udword)pPxDst->r * (((udword)pPxSrc->r * rcolPercent)>>8);
					udword g = (udword)pPxDst->g * (((udword)pPxSrc->g * gcolPercent)>>8);
					udword b = (udword)pPxDst->b * (((udword)pPxSrc->b * bcolPercent)>>8);
					r>>=7;	g>>=7;	b>>=7;

					pPxDst->r = (ubyte) ((r<255)?r:255);
					pPxDst->g = (ubyte) ((g<255)?g:255);
					pPxDst->b = (ubyte) ((b<255)?b:255);

					pPxDst++;		pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Blend
		}	else if (byMode==6)	{

			for (udword y=0; y<h; y++)
			{
				for (udword x=0; x<w; x++)
				{
					udword r = (udword)pPxDst->r + (((udword)pPxSrc->r * rcolPercent)>>8);
					udword g = (udword)pPxDst->g + (((udword)pPxSrc->g * gcolPercent)>>8);
					udword b = (udword)pPxDst->b + (((udword)pPxSrc->b * bcolPercent)>>8);
					//udword a = (udword)pPxDst->a + (((udword)pPxSrc->a * acolPercent)>>8);

					pPxDst->r = (ubyte) (r >> 1);
					pPxDst->g = (ubyte) (g >> 1);
					pPxDst->b = (ubyte) (b >> 1);
					//pPxDst->a = (ubyte) (((udword)pPxDst->a + a) >> 1);

					pPxDst++;
					pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Alpha
		}	else if (byMode==7)	{

			for (udword y=0; y<h; y++)
			{
				for (udword x=0; x<w; x++)
				{
					float r = (float)pPxSrc->r * 0.299;
					float g = (float)pPxSrc->g * 0.587;
					float b = (float)pPxSrc->b * 0.114;

					pPxDst->a = (ubyte) (( (udword)pPxDst->a + (udword)(r+g+b) )>>1);

					pPxDst++;
					pPxSrc++;
				}
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NGlowOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NGlowOp, NOperator);

static NVarsBlocDesc blocdescGlowOp[] =
{
	VAR(eudword,	true, "Color",		"-1",			"NColorProp")	//0
	VAR(eubyte,		true, "CenterX",	"128",		"NUbyteProp")	//1
	VAR(eubyte,		true, "CenterY",	"128",		"NUbyteProp")	//2
	VAR(eubyte,		true, "RayX",			"64",			"NUbyteProp")	//3
	VAR(eubyte,		true, "RayY",			"64",			"NUbyteProp")	//4
	VAR(eubyte,		true, "Alpha",		"128",		"NUbyteProp")	//5
	VAR(eubyte,		true, "Gamma",		"128",		"NUbyteProp")	//6
};


NGlowOp::NGlowOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescGlowOp);

}

udword NGlowOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	sdword w = pDst->GetWidth();
	sdword h = pDst->GetHeight();

	//Get Variables Values
	RGBA col;
	ubyte byCenterX, byCenterY, byRayX, byRayY, byAlpha, byGamma;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)col);
	m_pcvarsBloc->GetValue(1, _ftime, byCenterX);
	m_pcvarsBloc->GetValue(2, _ftime, byCenterY);
	m_pcvarsBloc->GetValue(3, _ftime, byRayX);
	m_pcvarsBloc->GetValue(4, _ftime, byRayY);
	m_pcvarsBloc->GetValue(5, _ftime, byAlpha);
	m_pcvarsBloc->GetValue(6, _ftime, byGamma);

	//Process operator
	sdword	dwCenterX	= byCenterX;
	sdword  dwCenterY	= byCenterY;
	sdword  dwRadiusX	= byRayX;
	sdword  dwRadiusY	= byRayY;
	float		fAlpha		= (float)byAlpha/128.0f;
	float		fGamma		= (float)byGamma/128.0f;

	float fRed	= col.r;
	float fGreen= col.g;
	float fBlue	= col.b;

	float	f1_RadiusX = 1.0f/(float)dwRadiusX;
	float	f1_RadiusY = 1.0f/(float)dwRadiusY;

	RGBA* pPxSrc = pSrc->GetPixels();
	RGBA* pPxDst = pDst->GetPixels();

	//Process
	for (sdword y=0; y<h; y++)
	{

		float dy = (float)(y-dwCenterY) * f1_RadiusY;
		float dy_2 = dy*dy;

		for (sdword x=0; x<w; x++)
		{
			//Calcul distance
			float dx = (float)(x-dwCenterX) * f1_RadiusX;
			float d = sqrt(dx*dx + dy_2);

			if (d>1.0f)		d=1.0f;
			d=1.0f-d;

			//Calcul Couleur du pixel
			udword r = (udword) (((float)pPxSrc->r) + ( (fGamma * d * fRed	) * fAlpha ));
			udword g = (udword) (((float)pPxSrc->g) + ( (fGamma * d * fGreen) * fAlpha ));
			udword b = (udword) (((float)pPxSrc->b) + ( (fGamma * d * fBlue ) * fAlpha ));

			//Affectation couleur
			pPxDst->r = (ubyte) ((r<255)?r:255);
			pPxDst->g = (ubyte) ((g<255)?g:255);
			pPxDst->b = (ubyte) ((b<255)?b:255);
			pPxDst->a = 255;

			pPxSrc++;
			pPxDst++;
		}
	}

	return 1;
}

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
	VAR(eubyte,		true, "CenterX",	"128",		"NUbyteProp")	//0
	VAR(eubyte,		true, "CenterY",	"128",		"NUbyteProp")	//1
	VAR(eubyte,		true, "Rotate",		"0",			"NUbyteProp")	//2
	VAR(eubyte,		true, "Zoom",			"128",		"NUbyteProp")	//3
};

NRotoZoomOp::NRotoZoomOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescRotoZoomOp);
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

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();

	//Get Variables Values
	ubyte byCenterX, byCenterY, byRotate, byZoom;
	m_pcvarsBloc->GetValue(0, _ftime, byCenterX);
	m_pcvarsBloc->GetValue(1, _ftime, byCenterY);
	m_pcvarsBloc->GetValue(2, _ftime, byRotate);
	m_pcvarsBloc->GetValue(3, _ftime, byZoom);

	//Rotate
	float fAngle= (float)byRotate;
	fAngle = fAngle * nv_two_pi / 255.0f;

	//Zoom
	float fZoom= (float)byZoom;
	fZoom=0.5f - (fZoom/256.0f);
	fZoom=exp(fZoom*6.0f);

	//CenterX and CenterY
	uword CenterX = byCenterX;		//CenterX 0-255 (128=center)
	uword CenterY = byCenterY;		//CenterY 0-255	(128=center)
	CenterX<<=8;
	CenterY<<=8;

	//Process RotoZoom
	RGBA* pPxlSrc = pSrc->GetPixels();
	RGBA* pPxlDst = pDst->GetPixels();

	uword	c = (uword) (cos(fAngle) * fZoom * 256.0f);
	uword	s = (uword) (sin(fAngle) * fZoom * 256.0f);

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
			pPxlDst->r = Color.r;
			pPxlDst->g = Color.g;
			pPxlDst->b = Color.b;
			pPxlDst->a = 255;
			pPxlDst++;

			//Vectors
			u+=c;	v+=s;
		};

		//Vectors
		ys+=s; yc+=c;
	};

	return 1;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NBlurOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NBlurOp, NOperator);

static NVarsBlocDesc blocdescBlurOp[] =
{
	VAR(eubyte,		true, "Width",		"4",		"NUbyteProp")	//0
	VAR(eubyte,		true, "Height",		"4",		"NUbyteProp")	//1
	VAR(eubyte,		true, "Amplify",	"16",		"NUbyteProp")	//2
};

NBlurOp::NBlurOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(3, blocdescBlurOp);
}

udword NBlurOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	sdword w = pDst->GetWidth();
	sdword h = pDst->GetHeight();

	//Get Variables Values
	ubyte byWidth, byHeight, byAmplify;
	m_pcvarsBloc->GetValue(0, _ftime, byWidth);
	m_pcvarsBloc->GetValue(1, _ftime, byHeight);
	m_pcvarsBloc->GetValue(2, _ftime, byAmplify);

	//Radius
	float radiusW= (float)byWidth / 2.0f;
	float radiusH= (float)byHeight /2.0f;

	//Amplify
	float amplify= (float)byAmplify;
	sdword amp = sdword(floor(amplify*16.0f));

	/////////////////////////////////////////////////////////
	// Blur Horizontal
	sdword bw = (sdword) (floor(radiusW)*2);

	if (bw>0)
	{

		for(sdword y=0;y<h;y++)
		{
			RGBA* pPxSrc	= pSrc->GetPixels() + (y*w);
			RGBA* pAccu		= pPxSrc;
			RGBA* pPxDst	= pDst->GetPixels() + (y*w);

			// Accumulation precalc
			sdword x = 0;
			RGBI sum;
			ZeroMemory(&sum, sizeof(RGBI));
			while (x<bw)
			{
				sum.r+=(sdword)pAccu->r;
				sum.g+=(sdword)pAccu->g;
				sum.b+=(sdword)pAccu->b;
				pAccu++;
				++x;
			}

			pAccu = pPxSrc;

			// Blur
			x=0;
			while (x<w)
			{
				sdword r = (sum.r*amp)/(bw<<8);
				sdword g = (sum.g*amp)/(bw<<8);
				sdword b = (sum.b*amp)/(bw<<8);

				pPxDst[(x+bw/2)%w].r= (ubyte) ((r<255)?r:255);
				pPxDst[(x+bw/2)%w].g= (ubyte) ((g<255)?g:255);
				pPxDst[(x+bw/2)%w].b= (ubyte) ((b<255)?b:255);
				pPxDst[(x+bw/2)%w].a= 255;

				sum.r-=(sdword)pPxSrc->r;
				sum.g-=(sdword)pPxSrc->g;
				sum.b-=(sdword)pPxSrc->b;
				pPxSrc++;

				sum.r+=(sdword)pAccu[(x+bw)%w].r;
				sum.g+=(sdword)pAccu[(x+bw)%w].g;
				sum.b+=(sdword)pAccu[(x+bw)%w].b;

				++x;
			}


		}

	}

	/////////////////////////////////////////////////////////
	// Allocation d'un buffer intermediare//###TOFIX
	RGBA* pPxInter = null;
	if (bw>0)
	{
		pPxInter = (RGBA*)NMemAlloc(w*h*sizeof(RGBA));	//###TOFIX

		//Copy Destination to Intermediate buffer (used as source)
		CopyMemory(pPxInter, pDst->GetPixels(), w*h*sizeof(RGBA));
	} else {
		pPxInter = pSrc->GetPixels();
	}

	/////////////////////////////////////////////////////////
	// Blur Vertical
	sdword bh = (sdword) (floor(radiusH)*2);

	if (bh>0)
	{

		for(sdword x=0;x<w;x++)
		{
			RGBA* pPxSrc	= pPxInter + x;
			RGBA* pAccu		= pPxSrc;
			RGBA* pPxDst	= pDst->GetPixels() + x;

			// Accumulation Precalc
			sdword y = 0;
			RGBI sum;
			ZeroMemory(&sum, sizeof(RGBI));
			while (y<bh)
			{
				sum.r+=(sdword)pAccu->r;
				sum.g+=(sdword)pAccu->g;
				sum.b+=(sdword)pAccu->b;
				pAccu+=w;
				++y;
			}

			pAccu = pPxSrc;

			// Blur
			y=0;
			while (y<h)
			{
				sdword r = (sum.r*amp)/(bh<<8);
				sdword g = (sum.g*amp)/(bh<<8);
				sdword b = (sum.b*amp)/(bh<<8);

				pPxDst[((y+bh/2)%h)*w].r= (ubyte) ((r<255)?r:255);
				pPxDst[((y+bh/2)%h)*w].g= (ubyte) ((g<255)?g:255);
				pPxDst[((y+bh/2)%h)*w].b= (ubyte) ((b<255)?b:255);
				pPxDst[((y+bh/2)%h)*w].a= 255;

				sum.r-=(sdword)pPxSrc->r;
				sum.g-=(sdword)pPxSrc->g;
				sum.b-=(sdword)pPxSrc->b;
				pPxSrc+=w;

				sum.r+=(sdword)pAccu[((y+bh)%h)*w].r;
				sum.g+=(sdword)pAccu[((y+bh)%h)*w].g;
				sum.b+=(sdword)pAccu[((y+bh)%h)*w].b;

				++y;
			}
		}
	}

	if (pPxInter && bw>0)		NMemFree(pPxInter);		//###TOFIX###

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NColorsOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NColorsOp, NOperator);

static NVarsBlocDesc blocdescColorsOp[] =
{
	//eubyte,		false,	"Mode",				"0,[RGB,HLS]",	"NUbyteComboProp",	//0
	VAR(eudword,	true,		"Color Base",			"0",						"NColorProp")				//0
	VAR(eudword,	true,		"Color Percent",	"-1",						"NColorProp")				//1
	VAR(eubyte,		true,		"Brithness",			"127",					"NUbyteProp")				//2
	VAR(eubyte,		true,		"Contrast",				"127",					"NUbyteProp")				//3
};


NColorsOp::NColorsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescColorsOp);
}

udword NColorsOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();

	//Get Variables Values
	ubyte byMode=1;
	RGBA ColorBase, ColorPer;
	ubyte byBrihtness, byContrast;
	//m_pcvarsBloc->GetValue(0, _ftime, byMode);
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)ColorBase);
	m_pcvarsBloc->GetValue(1, _ftime, (udword&)ColorPer);
	m_pcvarsBloc->GetValue(2, _ftime, byBrihtness);
	m_pcvarsBloc->GetValue(3, _ftime, byContrast);

	//Process operator
	sdword	brithness = (((sdword)byBrihtness)*2) - 256;	//-255 <> +255
	sdword	contrast	= (((sdword)byContrast));						//0 <> 255

	float fconstrast = (float)contrast/128.0f;
	if (contrast>64)		fconstrast = fconstrast*fconstrast*fconstrast;
	contrast=(sdword)(fconstrast*256.0f);

	RGBA* pPxSrc = pSrc->GetPixels();
	RGBA* pPxDst = pDst->GetPixels();

	///////////////////////////////////////////////
	// RGB Process + Brithness + Contrast
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			//Color Base + Color Percent + Brithness
			sdword r = (sdword)ColorBase.r + (((sdword)pPxSrc->r * (sdword)ColorPer.r)>>8) + brithness;
			sdword g = (sdword)ColorBase.g + (((sdword)pPxSrc->g * (sdword)ColorPer.g)>>8) + brithness;
			sdword b = (sdword)ColorBase.b + (((sdword)pPxSrc->b * (sdword)ColorPer.b)>>8) + brithness;

			//Contrast
			sdword c = (sdword) (((r - 127) * contrast)>>8) + 127;
			r = (c < 0x00) ? 0x00 : (c > 0xff) ? 0xff : c;

			c = (sdword) (((g - 127) * contrast)>>8) + 127;
			g = (c < 0x00) ? 0x00 : (c > 0xff) ? 0xff : c;

			c = (sdword) (((b - 127) * contrast)>>8) + 127;
			b = (c < 0x00) ? 0x00 : (c > 0xff) ? 0xff : c;

			//Set pixel
			r = (r<255)?r:255;				r = (r>0)?r:0;
			g = (g<255)?g:255;				g = (g>0)?g:0;
			b = (b<255)?b:255;				b = (b>0)?b:0;

			pPxDst->r = (ubyte) r;
			pPxDst->g = (ubyte) g;
			pPxDst->b = (ubyte) b;
			pPxDst->a	= pPxSrc->a;
			pPxDst++;
			pPxSrc++;
		}
	}

	return 0;
}


/*void NColorsOp::ToRGB(udword dwHSCB)
{

	if (m_saturation == 0.0) // Grauton, einfacher Fall
	{
		m_color[c_red] = m_color[c_green] = m_color[c_blue] = unsigned char(m_luminance * 255.0);
	}
	else
	{
		float rm1, rm2;

		if (m_luminance <= 0.5f) rm2 = m_luminance + m_luminance * m_saturation;
		else                     rm2 = m_luminance + m_saturation - m_luminance * m_saturation;
		rm1 = 2.0f * m_luminance - rm2;
		m_color[c_red]   = ToRGB1(rm1, rm2, m_hue + 120.0f);
		m_color[c_green] = ToRGB1(rm1, rm2, m_hue);
		m_color[c_blue]  = ToRGB1(rm1, rm2, m_hue - 120.0f);
	}

}

ubyte NColorsOp::ToRGB1(float rm1, float rm2, float rh)
{
	if      (rh > 360.0f) rh -= 360.0f;
	else if (rh <   0.0f) rh += 360.0f;

	if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;
	else if (rh < 180.0f) rm1 = rm2;
	else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;

	return ubyte(rm1 * 255);
}*/


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NLightOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NLightOp, NOperator);

static NVarsBlocDesc blocdescLightOp[] =
{
	VAR(eudword,	true, "Ambiant",		"0",				"NColorProp")	//0
	VAR(eudword,	true, "Diffuse",		"8421504",	"NColorProp")	//1
	VAR(eudword,	true, "Specular",		"-1",				"NColorProp")	//2

	VAR(eubyte,		true, "PosX",				"255",			"NUbyteProp")	//3
	VAR(eubyte,		true, "PosY",				"255",			"NUbyteProp")	//4
	VAR(eubyte,		true, "PosZ",				"127",			"NUbyteProp")	//5

	VAR(eubyte,		true, "Specular power",		"0",			"NUbyteProp")	//6
	VAR(eubyte,		true, "Bump power",				"0",			"NUbyteProp")	//7
};


NLightOp::NLightOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(8, blocdescLightOp);

}

udword NLightOp::Process(float _ftime, NOperator** _pOpsInts)
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
	RGBA Ambiant, Diffuse, Specular;
	ubyte byPosX, byPosY, byPosZ;
	ubyte bySpecPower, byBumpPower;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)Ambiant);
	m_pcvarsBloc->GetValue(1, _ftime, (udword&)Diffuse);
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)Specular);

	m_pcvarsBloc->GetValue(3, _ftime, byPosX);
	m_pcvarsBloc->GetValue(4, _ftime, byPosY);
	m_pcvarsBloc->GetValue(5, _ftime, byPosZ);		//255-near 0-far

	m_pcvarsBloc->GetValue(6, _ftime, bySpecPower);
	m_pcvarsBloc->GetValue(7, _ftime, byBumpPower);

	/////////////////////////////////////////
	//Init
	//sdword nlX	= (sdword)byPosX;
	//sdword nlY = (sdword)byPosY;
	//sdword nlZ = (sdword)byPosZ;
	vec3 light;
	light.x = (float)byPosX - 127.0f;
	light.y = -((float)byPosY - 127.0f);
	light.z = -(127.0f - (float)byPosZ);

	light.normalize();

	float fSpecularPower	= ((float)bySpecPower) / 32.0f;
	float fBumpPower			= ((float)byBumpPower) / 32.0f;

	/////////////////////////////////////////
	// DIRECTIONAL LIGHT TYPE
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

			//compute the dot product between normal and light dir
			float fdot;
			dot(fdot, n, light);
			if (fdot<0.0f)	fdot=0.0f;

			//Add bump on normal
			fdot*=fBumpPower;

			/*float fdotSpec=0.0;
			if (dot > 0.0) {
				//color += diffuse * NdotL;
				//halfV = normalize(halfVector);
				//NdotHV = max(dot(n,halfV),0.0);
				//color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess);
				fdotSpec = pow(fdot, 64);
			}*/

			// Color = ambient + dif*dot + dot^2 * spec
			sdword r	= (sdword) (Ambiant.r + (fdot*Diffuse.r) + (fdot*fdot*Specular.r*fSpecularPower));
			sdword g	= (sdword) (Ambiant.g + (fdot*Diffuse.g) + (fdot*fdot*Specular.g*fSpecularPower));
			sdword b	= (sdword) (Ambiant.b + (fdot*Diffuse.b) + (fdot*fdot*Specular.b*fSpecularPower));

			//sdword r	= pPxSrc->r + (fdot * pPxSrc->r);
			//sdword g	= pPxSrc->g + (fdot * pPxSrc->g);
			//sdword b	= pPxSrc->b + (fdot * pPxSrc->b);

			//Summ
			r = (pPxSrc->r + r) / 2;
			g = (pPxSrc->g + g) / 2;
			b = (pPxSrc->b + b) / 2;

			pPxDst->r = (ubyte) ((r<255)?r:255);
			pPxDst->g = (ubyte) ((g<255)?g:255);
			pPxDst->b = (ubyte) ((b<255)?b:255);
			pPxDst->a = pPxSrc->a;

			pPxSrc++;
			pPxDst++;
			pPxNorm++;
		}
	}


	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NJPEGOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*
IMPLEMENT_CLASS(NJPEGOp, NOperator);

static NVarsBlocDesc blocdescJPEGOp[] =
{
	eudword,	false, "Filename",	"0",	"NFileBrowserProp",	//0
};

NJPEGOp::NJPEGOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescJPEGOp);

	//Creation du bitmap
	m_pObj = new NBitmap();
}


udword NJPEGOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//No Input!
	if (m_byInputs!=0)		return (udword)-1;

	/////////////////////////////////////////
	// Get texture
	NBitmap* pDst = (NBitmap*)m_pObj;
	RGBA* pPxDst = pDst->GetPixels();

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();

	/////////////////////////////////////////
	// Get Variables Values
	char* pszFilename;
	m_pcvarsBloc->GetValue(0, _ftime, pszFilename);

	/////////////////////////////////////////
	// Process operator
	NJPEGLoader l;
	if (l.loadJPG(pszFilename))
	{
		if (l.width==w && l.height==h)
		{
			ubyte* pPxJPEG = l.dataBuffer;
			for (udword y=0; y<w; y++)
			{
				for (udword x=0; x<h; x++)
				{
					pPxDst->r = pPxJPEG[2];
					pPxDst->g = pPxJPEG[1];
					pPxDst->b = pPxJPEG[0];
					pPxDst++;
					pPxJPEG+=3;
				}
			}
		}
	}

	return 0;
}
*/


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NNormalsOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NNormalsOp, NOperator);

static NVarsBlocDesc blocdescNormalsOp[] =
{
	VAR(eudword,	true, "Amplify",	"64", "NUbyteProp")	//0
};


NNormalsOp::NNormalsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescNormalsOp);

}

udword NNormalsOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//////////////////////////////////////////
	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();

	/////////////////////////////////////////
	//Get Variables Values
	ubyte byAmp;
	m_pcvarsBloc->GetValue(0, _ftime, byAmp);

	float fAmp = (float)byAmp / 64.0f; //[0<->4]

	//////////////////////////////////////////
	// Creation des normales
	RGBA* pcurSour = pSrc->GetPixels();
	RGBA* pcurNorm = pDst->GetPixels();

	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			//Y Sobel filter
			float fPix = (float)pcurSour[ (x-1+w) % w + (((y+1) % h)*w) ].r;
			float dY  = fPix / 255.0f * -1.0f;

			fPix = (float)pcurSour[ x % w + (((y+1) % h)*w) ].r;
			dY+= fPix / 255.0f * -2.0f;

			fPix = (float)pcurSour[ (x+1) % w + (((y+1) % h)*w) ].r;
			dY+= fPix / 255.0f * -1.0f;

			fPix = (float)pcurSour[ (x-1+w) % w + (((y-1+h) % h)*w) ].r;
			dY+= fPix / 255.0f * 1.0f;

			fPix = (float)pcurSour[ x % w + (((y-1+h) % h)*w) ].r;
			dY+= fPix / 255.0f * 2.0f;

			fPix = (float)pcurSour[ (x+1) % w + (((y-1+h) % h)*w) ].r;
			dY+= fPix / 255.0f * 1.0f;

			//X Sobel filter
			fPix = (float)pcurSour[ (x-1+w) % w + (((y-1+h) % h)*w) ].r;
			float dX  = fPix / 255.0f * -1.0f;

			fPix = (float)pcurSour[ (x-1+w) % w + ((y % h)*w) ].r;
			dX+= fPix / 255.0f * -2.0f;

			fPix = (float)pcurSour[ (x-1+w) % w + (((y+1) % h)*w) ].r;
			dX+= fPix / 255.0f * -1.0f;

			fPix = (float)pcurSour[ (x+1) % w + (((y-1+h) % h)*w) ].r;
			dX+= fPix / 255.0f * 1.0f;

			fPix = (float)pcurSour[ (x+1) % w + ((y % h)*w) ].r;
			dX+= fPix / 255.0f * 2.0f;

			fPix = (float)pcurSour[ (x+1) % w + (((y+1) % h)*w) ].r;
			dX+= fPix / 255.0f * 1.0f;


			// Compute the cross product of the two vectors
			vec3 norm;
			norm.x = -dX*fAmp;
			norm.y = -dY*fAmp;
			norm.z = 1.0f;

			// Normalize
			norm.normalize();

			// Store
			pcurNorm->x = (ubyte) ((norm.x+1.0f) / 2.0f * 255.0f);	//[-1.0f->1.0f]	[0 -> 255]
			pcurNorm->y = (ubyte) ((norm.y+1.0f) / 2.0f * 255.0f);	//[-1.0f->1.0f]	[0 -> 255]
			pcurNorm->z = (ubyte) ((norm.z+1.0f) / 2.0f * 255.0f);	//[-1.0f->1.0f]	[0 -> 255]
			pcurNorm->a = 255;

			pcurNorm++;
		}
	}

	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCrackOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NCrackOp, NOperator);

static NVarsBlocDesc blocdescCrackOp[] =
{
	VAR(eudword,	true, "Color",			"-1",		"NColorProp")	//0
	VAR(eubyte,		true, "Count",			"100",	"NUbyteProp") //1
	VAR(eubyte,		true, "Variation",	"64",		"NUbyteProp") //2
	VAR(eubyte,		true, "Length",			"255",	"NUbyteProp") //3
	VAR(euword,		true, "Seed",				"5412",	"NUwordProp") //4
};

NCrackOp::NCrackOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(5, blocdescCrackOp);

}

udword NCrackOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst	= (NBitmap*)m_pObj;

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();
	RGBA* pPxDst	= pDst->GetPixels();

	//Copy Source to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), pDst->GetWidth() * pDst->GetHeight() * sizeof(RGBA));

	//Get Variables Values
	RGBA color;
	ubyte byCount, byVariation, byLength;
	uword wSeed;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)color);
	m_pcvarsBloc->GetValue(1, _ftime, byCount);
	m_pcvarsBloc->GetValue(2, _ftime, byVariation);
	m_pcvarsBloc->GetValue(3, _ftime, byLength);
	m_pcvarsBloc->GetValue(4, _ftime, wSeed);

	SetSeedValue(wSeed);

	float fcrackVariation = (float)byVariation / 256.0f;
	float fcrackLength = (float)byLength;

	//Process operator
	ubyte n = 0;
	while( n++ < byCount )
	{
		float x = (float)myfRandom() * float(w);
		float y = (float)myfRandom() * float(h);
		float a = 2.0f*3.141592f*(float)myfRandom();

		sdword count = udword(myfRandom()*fcrackLength);

		while( --count >= 0 )
		{
			udword ix = udword(x)%w;
			udword iy = udword(y)%h;

			pPxDst[ix + (iy*w)].dwCol = color.dwCol;

			x = x + cos(a);
			y = y + sin(a);
			if( x < 0.0 ) x = x + float(w);
			if( y < 0.0 ) y = y + float(h);

			a = a + fcrackVariation*(2.0f*(float)myfRandom()-1.0f);
		}

	}

	return 0;
}


float NCrackOp::myfRandom()
{
	return (float)myRandom() / 65536.0f;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCloudOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NCloudOp, NOperator);

static NVarsBlocDesc blocdescCloudOp[] =
{
	VAR(eudword,	true, "Color 0",		"0",		"NColorProp")	//0
	VAR(eudword,	true, "Color 1",		"-1",		"NColorProp")	//1
	VAR(eubyte,		true, "Amp",				"128",	"NUbyteProp") //2
	VAR(euword,		true, "Seed",				"5412",	"NUwordProp") //3
};

NCloudOp::NCloudOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescCloudOp);
}

udword NCloudOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//No Inputs
	if (m_byInputs!=0)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	// Init
	NBitmap* pDst	= (NBitmap*)m_pObj;

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();
	RGBA* pPxDst	= pDst->GetPixels();

	//Get Variables Values
	RGBA color0, color1;
	uword wSeed;
	ubyte byAmp;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)color0);
	m_pcvarsBloc->GetValue(1, _ftime, (udword&)color1);
	m_pcvarsBloc->GetValue(2, _ftime, byAmp);
	m_pcvarsBloc->GetValue(3, _ftime, wSeed);

	SetSeedValue(wSeed);

	//Create temporary buffer
	m_pbyPxTps	= (sdword*)NMemAlloc(w*h*sizeof(sdword)*2);
	m_pbyPxTps1 = m_pbyPxTps;
	m_pbyPxTps2 = m_pbyPxTps1 + (w*h);
	m_dwWidth		= 2;
	m_dwHeight	= 2;

	memset(m_pbyPxTps1, 0, w*h*sizeof(sdword)*2);

	//Process operator
	Cloud(7, (float)byAmp/256.0f);
	Normalize();

	//Temporary buffeur to Texture
	sdword* pbyPxSrc = m_pbyPxTps1;
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			//Cal color
			udword r = (udword)color0.r + ((*pbyPxSrc * color1.r)>>8);
			udword g = (udword)color0.g + ((*pbyPxSrc * color1.g)>>8);
			udword b = (udword)color0.b + ((*pbyPxSrc * color1.b)>>8);

			//Set pixel
			r = (r<255)?r:255;				r = (r>0)?r:0;
			g = (g<255)?g:255;				g = (g>0)?g:0;
			b = (b<255)?b:255;				b = (b>0)?b:0;

			pPxDst->r = (ubyte) r;
			pPxDst->g = (ubyte) g;
			pPxDst->b = (ubyte) b;
			pPxDst->a = 255;

			pPxDst++;
			pbyPxSrc++;
		}
	}


	//Clean
	NMemFree(m_pbyPxTps);

	return 0;
}


void NCloudOp::Cloud(ubyte byoctaves, float fampMod)
{
	float amp = 1.0f;

	AddRandom(amp);

	sdword dwOctaves = (sdword)byoctaves;
	while( --dwOctaves >= 0 )
	{
		ImageResize(m_dwWidth*2, m_dwHeight*2);
		BoxBlur(3, 3);
		amp *= fampMod;
		AddRandom(amp);
	}

	// blur the image one last time
	BoxBlur(3, 3);

}

void NCloudOp::AddRandom(float fampMod)
{
	sdword amp = (sdword)(fampMod*256.0f);

	sdword* pbypixels = m_pbyPxTps1;
	for (udword x=0; x<m_dwWidth; x++)
	{
		for (udword y=0; y<m_dwHeight; y++)
		{
			sdword r = amp * ((sdword)myRandom() - 32768);	//[-32768<>+32768]
			*pbypixels+= r;
			pbypixels++;
		}
	}
}


void NCloudOp::ImageResize(udword w, udword h)
{
	//Resize image
	sdword* pbyPxSrc = m_pbyPxTps1;
	sdword* pbyPxDst = m_pbyPxTps2;
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			udword px = x * m_dwWidth/w;
			udword py = y * m_dwHeight/h;
			*pbyPxDst = pbyPxSrc[px + py*m_dwWidth];
			pbyPxDst++;
		}
	}

	//Swap buffers
	sdword* pbyswap = m_pbyPxTps1;
	m_pbyPxTps1 = m_pbyPxTps2;
	m_pbyPxTps2 = pbyswap;

	//Save new size
	m_dwWidth=w;
	m_dwHeight=h;
}


void NCloudOp::BoxBlur(udword bw, udword bh)
{
	sdword* pbypixels = m_pbyPxTps1;
	udword w = m_dwWidth;
	udword h = m_dwHeight;

	udword x, y;

	///////////////////////////////////////
	// Smooth x
	sdword* pline = m_pbyPxTps2;

	y = 0;
	while( y < h )
	{
		// Copy original pixels to the line buffer
		x = 0;
		while( x < w )		{	pline[x]=pbypixels[x + y*w];	++x;	}

		// Compute sum of first bw pixels
		x = 0;
		sdword sum = 0;
		while( x < bw )		{ sum+=pline[x]; ++x; }

		// Set blurred pixels
		x = 0;
		while( x < w )
		{
			pbypixels[(x+bw/2)%w + y*w] = sum/(sdword)bw;
			sum -= pline[x];
			sum += pline[(x+bw)%w];
			++x;
		}

		++y;
	}

	///////////////////////////////////////
	// Smooth y
	pline = m_pbyPxTps2;

	x = 0;
	while( x < w )
	{
		// Copy original pixels to the line buffer
		y = 0;
		while( y < h )	{	pline[y]=pbypixels[x + y*w];	++y;	}

		// Compute sum of first bw pixels
		y = 0;
		sdword sum = 0;
		while( y < bh )	{	sum+=pline[y];	++y; }

		// Set blurred pixels
		y = 0;
		while( y < h )
		{
			pbypixels[x + ((y+bh/2)%h)*w] = sum/(sdword)bh;
			sum -= pline[y];
			sum += pline[(y+bh)%h];
			++y;
		}

		++x;
	}


}


void NCloudOp::Normalize()
{
	sdword w = m_dwWidth;
	sdword h = m_dwHeight;

	sdword x, y;

	/////////////////////////////////////////////////////
	// Find the highest and lowest values used
	sdword* pbypixels = m_pbyPxTps1;

	sdword hi = 0, lo = 65536;
	y = 0;
	while( y < h )
	{
		x = 0;
		while( x < w )
		{
			sdword v = *pbypixels;
			if( v < lo ) lo = v;
			if( v > hi ) hi = v;
			pbypixels++;
			++x;
		}
		++y;
	}

	/////////////////////////////////////////////////////
	// Normalize the image
	pbypixels = m_pbyPxTps1;

	y = 0;
	float fhi = 1.0f/(float)(hi-lo);
	while( y < h )
	{
		x = 0;
		while( x < w )
		{
			sdword v = *pbypixels;
			*pbypixels++ = (sdword) (( (float)(v-lo)*fhi ) * 255);
			++x;
		}
		++y;
	}
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

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NSetSizeOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NSetSizeOp, NOperator);

static NVarsBlocDesc blocdescSetSizeOp[] =
{
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048]", "NUbyteComboProp")	//1
};

NSetSizeOp::NSetSizeOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(2, blocdescSetSizeOp);

}

udword NSetSizeOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//One inputs (texture)
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	/////////////////////////////////////////
	//Get input Texture
	NBitmap* pSrc	= (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst	= (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();

	/////////////////////////////////////////
	//Get Variables Values
	ubyte byW, byH;
	m_pcvarsBloc->GetValue(0, _ftime, byW);
	m_pcvarsBloc->GetValue(1, _ftime, byH);
	udword w2=1<<((udword)byW);
	udword h2=1<<((udword)byH);
	pDst->SetSize(w2, h2);

	/////////////////////////////////////////
	//Process operator
	RGBA* pPxSrc	= pSrc->GetPixels();
	RGBA* pPxDst	= pDst->GetPixels();

	for (udword y=0; y<h2; y++)
	{
		for (udword x=0; x<w2; x++)
		{
			udword px = x * w/w2;
			udword py = y * h/h2;
			*pPxDst = pPxSrc[px + py*w];
			pPxDst++;
		}
	}

	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NGradientOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NGradientOp, NOperator);

static NVarsBlocDesc blocdescGradientOp[] =
{
	VAR(eudword,	true, "A",	"16711680", "NColorProp")	//0	blue
	VAR(eudword,	true, "B",	"65280",		"NColorProp")	//1 green
	VAR(eudword,	true, "C",	"255",			"NColorProp")	//2 red
	VAR(eudword,	true, "D",	"16776960", "NColorProp")	//3 blue light
};

NGradientOp::NGradientOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescGradientOp);
}

udword NGradientOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//No Inputs!
	if (m_byInputs!=0)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();

	//Get Variables Values
	RGBA colA, colB, colC, colD;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)colA);
	m_pcvarsBloc->GetValue(1, _ftime, (udword&)colB);
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)colC);
	m_pcvarsBloc->GetValue(3, _ftime, (udword&)colD);

	//Init
	//A B
	//C D
	RGBI col;
	float finv_WH = 1.0f / (float)(w*h);

	//Process operator
	RGBA* pPxDst = pDst->GetPixels();
	for (sdword y=0; y<h; y++)
	{
		for (sdword x=0; x<w; x++)
		{
			//Calc percent A,B,C,D
			float a = (float)((w-x)	* (h-y)) * finv_WH;
			float b = (float)((x)		* (h-y)) * finv_WH;
			float c = (float)((w-x)	* (y))	 * finv_WH;
			float d = (float)((x)		* (y))	 * finv_WH;

			col.r= (colA.r*a) + (colB.r*b) + (colC.r*c) + (colD.r*d);
			col.g= (colA.g*a) + (colB.g*b) + (colC.g*c) + (colD.g*d);
			col.b= (colA.b*a) + (colB.b*b) + (colC.b*c) + (colD.b*d);

			pPxDst->r = (ubyte)col.r;
			pPxDst->g = (ubyte)col.g;
			pPxDst->b = (ubyte)col.b;
			pPxDst->a = 255;
			pPxDst++;
		}
	}

	return 0;
}
