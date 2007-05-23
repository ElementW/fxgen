//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		CombineOps.cpp
//! \brief	Combine Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
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
#include "pch.h"
#include "CombineOps.h"

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

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);


	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Copy Source to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));

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
	m_pcvarsBloc = AddVarsBloc(5, blocdescRectOp, 1);
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
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	RGBA col;
	ubyte x1,y1,x2,y2;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)col);
	m_pcvarsBloc->GetValue(1, _ftime, x1);
	m_pcvarsBloc->GetValue(2, _ftime, y1);
	m_pcvarsBloc->GetValue(3, _ftime, x2);
	m_pcvarsBloc->GetValue(4, _ftime, y2);

	//Copy Source to this bitmap
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));

	//Process operator
	for (udword y=y1; y<=y2; y++)
	{
		RGBA* pPixelsD = pDst->GetPixels() + (y*w) + x1;
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
	m_pcvarsBloc = AddVarsBloc(3, blocdescPixelsOp, 1);

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
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	RGBA col;
	ubyte byCount;
	uword wSeed;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)col);
	m_pcvarsBloc->GetValue(1, _ftime, wSeed);
	m_pcvarsBloc->GetValue(2, _ftime, byCount);

	//Copy Source to This
	CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(RGBA));

	//Process operator
	SetSeedValue(wSeed);

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
	m_pcvarsBloc = AddVarsBloc(2, blocdescAddOp, 1);
}

udword NAddOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//x Inputs
	if (m_byInputs<1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();

	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w, h);	//Rk. Texture size for AddOp is the first input operator's size


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

			for (udword y=0; y<hh; y++)
			{
				pPxSrc = pSrc->GetPixels() + (y*w2);
				pPxDst = pDst->GetPixels() + (y*w);

				for (udword x=0; x<ww; x++)
				{
					float r = (float)pPxSrc->r * 0.299f;
					float g = (float)pPxSrc->g * 0.587f;
					float b = (float)pPxSrc->b * 0.114f;

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
	m_pcvarsBloc = AddVarsBloc(7, blocdescGlowOp, 1);

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

	sdword w = pSrc->GetWidth();
	sdword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

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
	m_pcvarsBloc = AddVarsBloc(5, blocdescCrackOp, 1);

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

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

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
//							NLerpOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NLerpOp, NOperator);

NLerpOp::NLerpOp()
{
}

udword NLerpOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Three inputs
	if (m_byInputs!=3)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input textures
	NBitmap* pSrc1 = (NBitmap*)(*(_pOpsInts+0))->m_pObj;
	NBitmap* pSrc2 = (NBitmap*)(*(_pOpsInts+1))->m_pObj;
	NBitmap* pBlend = (NBitmap*)(*(_pOpsInts+2))->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	sdword w = pSrc1->GetWidth();
	sdword h = pSrc1->GetHeight();
	pDst->SetSize(w,h);

		//###TODO### multi sources size adjust

	RGBA* pPxSrc1 = pSrc1->GetPixels();
	RGBA* pPxSrc2 = pSrc2->GetPixels();
	RGBA* pPxBlend = pBlend->GetPixels();
	RGBA* pPxDst = pDst->GetPixels();


	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			pPxDst->r = (pPxSrc1->r*(255-pPxBlend->r) + pPxSrc2->r*pPxBlend->r) >> 8;
			pPxDst->g = (pPxSrc1->g*(255-pPxBlend->g) + pPxSrc2->g*pPxBlend->g) >> 8;
			pPxDst->b = (pPxSrc1->b*(255-pPxBlend->b) + pPxSrc2->b*pPxBlend->b) >> 8;
			pPxDst->a = 255;

			pPxSrc1++;
                        pPxSrc2++;
                        pPxBlend++;
			pPxDst++;
		}
	}

	return 0;
}