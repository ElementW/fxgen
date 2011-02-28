//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		CombineOps.cpp
//! \brief	Combine Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
//!						Sebastian Olter (qduaty@gmail.com)
//!
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
#include "../../Include/FxGenLib.h"
#include "CombineOps.h"
#include "RectangularArray.h"
#include "Bitmap.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NNopOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NNopOp,	NOperatorFx);

udword NNopOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

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
//							NRectOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NRectOp, NOperatorFx);

static NVarsBlocDesc blocdescRectOp[] =
{
	VAR(eudword,	true, "Color",	"-1",		"NColorProp", 0.0f, 0.0f, 0.0f)	//0
	VAR(efloat,		true, "x1",			"0.0",	"NFloatProp", -1.0f, 1.0f, 0.001f)	//1
	VAR(efloat,		true, "y1",			"0.0",	"NFloatProp", -1.0f, 1.0f, 0.001f)	//2
	VAR(efloat,		true, "x2",			"0.5",	"NFloatProp", -1.0f, 1.0f, 0.001f)	//3
	VAR(efloat,		true, "y2",			"0.5",	"NFloatProp", -1.0f, 1.0f, 0.001f)	//4
};

NRectOp::NRectOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(5, blocdescRectOp, 1);
}

udword NRectOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	NRGBA col = m_pcvarsBloc->GetUDwordValue(0, _ftime);
	float fx1 = m_pcvarsBloc->GetFloatValue(1, _ftime);
	float fy1 = m_pcvarsBloc->GetFloatValue(2, _ftime);
	float fx2 = m_pcvarsBloc->GetFloatValue(3, _ftime);
	float fy2 = m_pcvarsBloc->GetFloatValue(4, _ftime);

	//Init
	udword x1, x2, y1, y2;
	x1 = max(0, fx1 * w);
	x2 = min(w, fx2 * w);
	y1 = max(0, fy1 * h);
	y2 = min(h, fy2 * h);

	//Copy Source to this bitmap
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));

	//Process operator
	for (udword y=y1; y<y2; y++)
	{
		NRGBA* pPixelsD = pDst->GetPixels() + (y*w) + x1;
		for (udword x=x1; x<x2; x++)
		{
			float alpha = col.a / 255.f;
			pPixelsD->r = ubyte(pPixelsD->r * (1 - alpha) + col.r * alpha);
			pPixelsD->g = ubyte(pPixelsD->g * (1 - alpha) + col.g * alpha);
			pPixelsD->b = ubyte(pPixelsD->b * (1 - alpha) + col.b * alpha);
			// input alpha is preserved
			++pPixelsD;
		}
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
FIMPLEMENT_CLASS(NPixelsOp,NOperatorFx);

static NVarsBlocDesc blocdescPixelsOp[] =
{
	VAR(eudword,	true, "Color",	"-1",			"NColorProp", 0.0f, 0.0f, 0.0f )	//0
	VAR(euword,		true, "Seed",		"5412",		"NUwordProp", 0.0f, 65535.0f, 1.0f)	//1
	VAR(eubyte,		true, "Count",	"128",		"NUbyteProp", 0.0f, 255.0f, 1.0f)	//2
};

NPixelsOp::NPixelsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(3, blocdescPixelsOp, 1);

}

udword NPixelsOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	NRGBA col = m_pcvarsBloc->GetUDwordValue(0, _ftime);
	uword wSeed = m_pcvarsBloc->GetUWordValue(1, _ftime);
	ubyte byCount = m_pcvarsBloc->GetUByteValue(2, _ftime);

	//Copy Source to This
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));

	//Process operator
	SetSeedValue(wSeed);

	udword dwCount = 2<<(byCount>>4);
	NRGBA* pPxDst = pDst->GetPixels();
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
FIMPLEMENT_CLASS(NAddOp, NOperatorFx);

static NVarsBlocDesc blocdescAddOp[] =
{
	VAR(eubyte,		false, "Mode",				"8,[Add Clamp,Add Wrap,Sub Clamp,Sub Wrap,Multiply,Multiply x2,Blend,Alpha,Layer]", "NUbyteComboProp", 0.0f, 0.0f, 0.0f)	//0
	VAR(eudword,	true,	 "RGB Percent",	"-1" , "NColorProp", 0.0f, 0.0f, 0.0f)			//1
};

NAddOp::NAddOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(2, blocdescAddOp, 1);
}

udword NAddOp::Process(float _ftime, SEngineState& _state)
{
	//x Inputs
	if (m_byInputs<1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();

	N2DBitmap* pDst = (N2DBitmap*)m_pObj;
	pDst->SetSize(w, h);	//Rk. Texture size for AddOp is the first input operator's size


	//Copy Source 1 to This
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));

	//Get Variables Values
	ubyte byMode = m_pcvarsBloc->GetUByteValue(0, _ftime);
	NRGBA colPercent = m_pcvarsBloc->GetUDwordValue(1, _ftime);

	udword rcolPercent = colPercent.r;
	udword gcolPercent = colPercent.g;
	udword bcolPercent = colPercent.b;
//	udword acolPercent = 255;

	///////////////////////////////////////////////////////////
	//Add n Sources
	for (udword i=1; i<(udword)m_byInputs; i++)
	{
		pSrc = (N2DBitmap*)_state.apInputs[i];
		if (pSrc==null)		break;

		NRGBA* pPxSrc = pSrc->GetPixels();
		NRGBA* pPxDst = pDst->GetPixels();

		udword w2 = pSrc->GetWidth();
		udword h2 = pSrc->GetHeight();

		udword ww=min(w,w2);
		udword hh=min(h,h2);

		//////////////////////////////////////////////
		//Add Clamp
		if (byMode==0)
		{
			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword r = (udword)pPxDst->r + (((udword)pPxSrc->r * rcolPercent)>>8);
					udword g = (udword)pPxDst->g + (((udword)pPxSrc->g * gcolPercent)>>8);
					udword b = (udword)pPxDst->b + (((udword)pPxSrc->b * bcolPercent)>>8);
					pPxDst->r = (ubyte) ((r<255)?r:255);
					pPxDst->g = (ubyte) ((g<255)?g:255);
					pPxDst->b = (ubyte) ((b<255)?b:255);
					pPxDst++; pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Add Wrap
		} else if (byMode==1)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
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

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
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

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
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

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
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

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
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

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword r = (udword)pPxDst->r + (((udword)pPxSrc->r * rcolPercent)>>8);
					udword g = (udword)pPxDst->g + (((udword)pPxSrc->g * gcolPercent)>>8);
					udword b = (udword)pPxDst->b + (((udword)pPxSrc->b * bcolPercent)>>8);

					pPxDst->r = (ubyte) (r >> 1);
					pPxDst->g = (ubyte) (g >> 1);
					pPxDst->b = (ubyte) (b >> 1);

					pPxDst++;
					pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Alpha
		}	else if (byMode==7)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword a = (udword)pPxDst->a + (((udword)pPxSrc->a * bcolPercent)>>8);
					pPxDst->a = (ubyte) (a >> 1);
					pPxDst++;
					pPxSrc++;
				}
			}

		//////////////////////////////////////////////
		//Layer
		} else if (byMode==8) {

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
				{
					float alpha = pPxSrc->a / 255.f;
					// 1-(1-a1)(1-a2) = 1 - (1-a1-a2+a1a2) = a1+a2-a1a2
					pPxDst->a = ubyte((pPxDst->a - pPxDst->a * alpha) + pPxSrc->a);
					pPxDst->r = ubyte(pPxSrc->r * alpha + pPxDst->r * (1 - alpha));
					pPxDst->g = ubyte(pPxSrc->g * alpha + pPxDst->g * (1 - alpha));
					pPxDst->b = ubyte(pPxSrc->b * alpha + pPxDst->b * (1 - alpha));
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
FIMPLEMENT_CLASS(NGlowOp, NOperatorFx);

static NVarsBlocDesc blocdescGlowOp[] =
{
	VAR(eudword,	true, "Color",		"-1",			"NColorProp", 0.0f, 0.0f, 0.0f)	//0
	VAR(efloat,		true, "CenterX",	"0.5",		"NFloatProp", -1.0f, 1.0f, 0.001f)	//1
	VAR(efloat,		true, "CenterY",	"0.5",		"NFloatProp", -1.0f, 1.0f, 0.001f)	//2
	VAR(efloat,		true, "RayX",			"0.5",		"NFloatProp", -1.0f, 1.0f, 0.001f)	//3
	VAR(efloat,		true, "RayY",			"0.5",		"NFloatProp", -1.0f, 1.0f, 0.001f)	//4
	VAR(efloat,		true, "Alpha",		"1.0",		"NFloatProp", 0.0f, 1.0f, 0.001f)	//5
	VAR(efloat,		true, "Gamma",		"1.0",		"NFloatProp", 0.0f, 1.0f, 0.001f)	//6
};


NGlowOp::NGlowOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescGlowOp, 1);
}

udword NGlowOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

	sdword w = pSrc->GetWidth();
	sdword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	NRGBA col = m_pcvarsBloc->GetUDwordValue(0, _ftime);
	float fCenterX	= m_pcvarsBloc->GetFloatValue(1, _ftime);
	float fCenterY	= m_pcvarsBloc->GetFloatValue(2, _ftime);
	float fRayX			= m_pcvarsBloc->GetFloatValue(3, _ftime);
	float fRayY			= m_pcvarsBloc->GetFloatValue(4, _ftime);
	float fAlpha		= m_pcvarsBloc->GetFloatValue(5, _ftime);
	float fGamma		= m_pcvarsBloc->GetFloatValue(6, _ftime);

	//Process operator
	sdword	dwCenterX	= sdword(fCenterX*w);
	sdword  dwCenterY	= sdword(fCenterY*h);
	sdword  dwRadiusX	= sdword(fRayX*w);
	sdword  dwRadiusY	= sdword(fRayY*h);

	float fRed	= col.r;
	float fGreen= col.g;
	float fBlue	= col.b;

	float	f1_RadiusX = 1.0f/(float)dwRadiusX;
	float	f1_RadiusY = 1.0f/(float)dwRadiusY;

	NRGBA* pPxSrc = pSrc->GetPixels();
	NRGBA* pPxDst = pDst->GetPixels();

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
			pPxDst->a = pPxSrc->a;

			pPxSrc++;
			pPxDst++;
		}
	}

	return 1;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCrackOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NCrackOp, NOperatorFx);

static NVarsBlocDesc blocdescCrackOp[] =
{
	VAR(eudword,	true, "Color",			"-1",		"NColorProp", 0.0f, 0.0f, 0.0f)	//0
	VAR(euword,		true, "Count",			"100",	"NUwordProp", 0.0f, 65535.0f, 1.0f) //1
	VAR(eubyte,		true, "Variation",	"64",		"NUbyteProp", 0.0f, 255.0f, 1.0f) //2
	VAR(eubyte,		true, "Length",			"255",	"NUbyteProp", 0.0f, 255.0f, 1.0f) //3
	VAR(euword,		true, "Seed",				"5412",	"NUwordProp", 0.0f, 65535.0f, 1.0f) //4
	VAR(eubyte,		true, "Length decision", "0,[Random,Constant,Normal based]",	"NUbyteComboProp", 0.0f, 0.0f, 0.0f) //5
	VAR(eubyte,		true, "High quality", "0,[Off,Alpha,Subpixel]",	"NUbyteComboProp", 0.0f, 0.0f, 0.0f) //6
};

NCrackOp::NCrackOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescCrackOp, 1);
}

udword NCrackOp::Process(float _ftime, SEngineState& _state)
{
	//One or two Inputs
	if (m_byInputs!=1 && m_byInputs!=2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst	= (N2DBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	NRGBAArray normals, dest(pDst->GetPixels(), w, h);

	if(m_byInputs==2)
	{
		N2DBitmap* pNorm = (N2DBitmap*)_state.apInputs[1];

		if(pNorm->GetWidth() < w || pNorm->GetHeight() < h)
			return (udword)-1; // insufficient size

		normals = NRGBAArray(pNorm->GetPixels(), w, h);
	}

	//Copy Source to This
	memcpy(pDst->GetPixels(), pSrc->GetPixels(), pDst->GetWidth() * pDst->GetHeight() * sizeof(NRGBA));

	//Get Variables Values
	NRGBA color = m_pcvarsBloc->GetUDwordValue(0, _ftime);
	uword wCount = m_pcvarsBloc->GetUWordValue(1, _ftime);
	ubyte byVariation = m_pcvarsBloc->GetUByteValue(2, _ftime);
	ubyte byLength = m_pcvarsBloc->GetUByteValue(3, _ftime);
	uword wSeed = m_pcvarsBloc->GetUWordValue(4, _ftime);
	ubyte byMode = m_pcvarsBloc->GetUByteValue(5, _ftime);
	ubyte byHQ = m_pcvarsBloc->GetUByteValue(6, _ftime);

	SetSeedValue(wSeed);

	//Process operator
	uword n = 0;
	wCount = uword(wCount * _state.fDetailFactor);
	while( n++ < wCount )
	{
		// double gives better resolution
		double x = myfRandom() * w;
		double y = myfRandom() * h;
		double a = 2.0f*3.141592f*myfRandom();

		// determine line length
		sdword count = (sdword)(byLength * _state.fDetailFactor);

		if(normals.width && byMode == 2)
		{
			NRGBA &N = normals(x,y);
			vec3 normal(N.r - 127.f, N.g - 127.f, 0.f);
			count = (sdword)(count * normal.norm() * normal.norm() / 8) /* adjusted value */;
			count = min(count, byLength * _state.fDetailFactor);
		}

		if(byMode == 0)
			count = (sdword)(count * myfRandom() * 2); // E=1

		// draw a line
		while( --count >= 0 )
		{
			a += byVariation / 256.0f *(2.0f*myfRandom()-1.0f);

			x = x + cos(a);
			y = y + sin(a);

			if(normals.width)
			{
				NRGBA &N = normals(x,y);
				vec3 normal(127.f - N.r, N.g-127.f, 0/*n.b*/); // x flip
				a = normal.azimuth() + .5f * nv_pi; // rotate 90 degrees ccw
				// alpha-based placement decision
				if(normal.norm() < (255 - N.a) / 4.f)
//				if(normal.elevation() > n.a * nv_pi * 2.f / 255.f)
					continue;
			}

			if(byHQ == 2)
				dest.set(x, y, color);
			else if(byHQ == 1)
				dest(x, y) += color;
			else
				dest(x, y) = color;
		}
	}

	return 0;
}

float NCrackOp::myfRandom()
{
	return (float)myRandom(1) / 65536.0f;
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NLerpOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NLerpOp, NOperatorFx);

NLerpOp::NLerpOp()
{
}

udword NLerpOp::Process(float _ftime, SEngineState& _state)
{
	//Three inputs
	if (m_byInputs!=3)		return (udword)-1;

	//Get input textures
	N2DBitmap* pSrc1 = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pSrc2 = (N2DBitmap*)_state.apInputs[1];
	N2DBitmap* pBlend = (N2DBitmap*)_state.apInputs[2];

	// Same inputs W and H sizes
	udword w = pSrc1->GetWidth();
	udword h = pSrc1->GetHeight();
	if (w!=pSrc2->GetWidth()	|| w!=pBlend->GetWidth()	)		return (udword)-1;
	if (h!=pSrc2->GetHeight() || h!=pBlend->GetHeight() )		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Set Bitmap Size
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;
	pDst->SetSize(w,h);

	//Process
	NRGBA* pPxSrc1 = pSrc1->GetPixels();
	NRGBA* pPxSrc2 = pSrc2->GetPixels();
	NRGBA* pPxBlend = pBlend->GetPixels();
	NRGBA* pPxDst = pDst->GetPixels();


	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			pPxDst->r = (pPxSrc1->r*(255-pPxBlend->r) + pPxSrc2->r*pPxBlend->r) >> 8;
			pPxDst->g = (pPxSrc1->g*(255-pPxBlend->g) + pPxSrc2->g*pPxBlend->g) >> 8;
			pPxDst->b = (pPxSrc1->b*(255-pPxBlend->b) + pPxSrc2->b*pPxBlend->b) >> 8;
			pPxDst->a = (pPxSrc1->a*(255-pPxBlend->a) + pPxSrc2->a*pPxBlend->a) >> 8;

			pPxSrc1++;
			pPxSrc2++;
			pPxBlend++;
			pPxDst++;
		}
	}

	return 0;
}
