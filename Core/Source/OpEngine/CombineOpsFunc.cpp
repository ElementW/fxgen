//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		CombineOpsFunc.cpp
//! \brief	Combine Operators Functions
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
//!					Sebastian Olter (qduaty@gmail.com)
//!
//!	\date		06-05-2009
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
#include "EngineOp.h"
#include "RectangularArray.h"
#include "Bitmap.h"
#include "Maths.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NNopOp Implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*void NopOp_Process(SEngineState* _state)
{
	NBitmap* pSrc = _state->apInputs[0];
	NBitmap* pDst = _state->pcurCall->pbmp;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	NMemCopy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));
}*/


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NRectOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
struct SRectParams
{
  NRGBA color;
  vec4  rect;
};

void  NRectOp_Process(SEngineState* _state, SRectParams* _p)
{
	SResource* pRes = _state->apResourcesLayers[_state->pcurCall->byDepth];
	udword w= pRes->dwWidth;
	udword h= pRes->dwHeight;

	//Init
	uword x1, x2, y1, y2;
  x1 = nmax(0, nmin(w, nmin(_p->rect.x1, _p->rect.x2) * w));
  x2 = nmax(0, nmin(w, nmax(_p->rect.x1, _p->rect.x2) * w));
  y1 = nmax(0, nmin(h, nmin(_p->rect.y1, _p->rect.y2) * h));
  y2 = nmax(0, nmin(h, nmax(_p->rect.y1, _p->rect.y2) * h));

  float alpha = _p->color.a / 255.f;

	//Process operator
	for (udword y=y1; y<y2; y++)
	{
		NRGBA* pPixelsD = pRes->pbyPixels + (y*w) + x1;
		for (udword x=x1; x<x2; x++)
		{
      pPixelsD->r = ubyte(pPixelsD->r * (1.0 - alpha) + _p->color.r * alpha);
      pPixelsD->g = ubyte(pPixelsD->g * (1.0 - alpha) + _p->color.g * alpha);
      pPixelsD->b = ubyte(pPixelsD->b * (1.0 - alpha) + _p->color.b * alpha);
			// input alpha is preserved
			++pPixelsD;
		}
	}

}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NPixelsOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*void NPixelsOp_Process(SEngineState* _state, NRGBA _color, uword _seed, ubyte _count)
{
	//Get input texture
	NBitmap* pSrc = _state->apInputs[0];
	NBitmap* pDst = _state->pcurCall->pbmp;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Copy Source to This
	NMemCopy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));

	//Process operator
	SetSeedValue(_seed);

	udword dwCount = 2<<(_count>>4);
	NRGBA* pPxDst = pDst->GetPixels();
	while (--dwCount)
	{
		udword x=myRandom() % w;
		udword y=myRandom() % h;
		pPxDst[x + y*w] = _color;
	}

}*/

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NAddOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
struct SAddParams
{
  ubyte bymode;
  NRGBA colPercent;
};

void  NAddOp_Process(SEngineState* _state, SAddParams* _p)
{
	SResource* pRes1 = _state->apResourcesLayers[_state->pcurCall->byDepth];
	udword w= pRes1->dwWidth;
	udword h= pRes1->dwHeight;
	NRGBA *pPxSrc1, *pPxSrc2;

	////Get input texture
	//NBitmap* pSrc = _state->apInputs[0];
	//NBitmap* pDst = _state->pcurCall->pbmp;

	//udword w = pSrc->GetWidth();
	//udword h = pSrc->GetHeight();
	//pDst->SetSize(w,h);

	////Copy Source 1 to This
	//NMemCopy(pDst->GetPixels(), pSrc->GetPixels(), w * h * sizeof(NRGBA));

	//Get Variables Values
  udword rcolPercent = _p->colPercent.r;
  udword gcolPercent = _p->colPercent.g;
  udword bcolPercent = _p->colPercent.b;
//	udword acolPercent = 255;

	///////////////////////////////////////////////////////////
	//Add n Sources
	for (udword i=_state->pcurCall->byDepth+1; i<(udword)_state->pcurCall->byDepth+(udword)_state->pcurCall->byInputsCount; i++)
	{
		SResource* pRes2 = _state->apResourcesLayers[i];
		if (pRes2==null)		break;

		//NRGBA* pPxSrc2 = pRes2->pbyPixels;

		udword w2 = pRes2->dwWidth;
		udword h2 = pRes2->dwHeight;

		udword ww=nmin(w,w2);
		udword hh=nmin(h,h2);

		//////////////////////////////////////////////
		//Add Clamp
    if (_p->bymode==0)
		{
			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword r = (udword)pPxSrc2->r + (((udword)pPxSrc1->r * rcolPercent)>>8);
					udword g = (udword)pPxSrc2->g + (((udword)pPxSrc1->g * gcolPercent)>>8);
					udword b = (udword)pPxSrc2->b + (((udword)pPxSrc1->b * bcolPercent)>>8);
					pPxSrc2->r = (ubyte) ((r<255)?r:255);
					pPxSrc2->g = (ubyte) ((g<255)?g:255);
					pPxSrc2->b = (ubyte) ((b<255)?b:255);
					pPxSrc2++; pPxSrc1++;
				}
			}

		//////////////////////////////////////////////
		//Add Wrap
    } else if (_p->bymode==1)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword r = (udword)pPxSrc2->r + (((udword)pPxSrc1->r * rcolPercent)>>8);
					udword g = (udword)pPxSrc2->g + (((udword)pPxSrc1->g * gcolPercent)>>8);
					udword b = (udword)pPxSrc2->b + (((udword)pPxSrc1->b * bcolPercent)>>8);
					pPxSrc2->r = (ubyte) (r%255);
					pPxSrc2->g = (ubyte) (g%255);
					pPxSrc2->b = (ubyte) (b%255);
					pPxSrc2++;	pPxSrc1++;
				}
			}

		//////////////////////////////////////////////
		//Sub Clamp
    } else if (_p->bymode==2)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					sdword r = (sdword)pPxSrc2->r - (((sdword)pPxSrc1->r * rcolPercent)>>8);
					sdword g = (sdword)pPxSrc2->g - (((sdword)pPxSrc1->g * gcolPercent)>>8);
					sdword b = (sdword)pPxSrc2->b - (((sdword)pPxSrc1->b * bcolPercent)>>8);
					pPxSrc2->r = (ubyte) ((r>0)?r:0);
					pPxSrc2->g = (ubyte) ((g>0)?g:0);
					pPxSrc2->b = (ubyte) ((b>0)?b:0);
					pPxSrc2++;	pPxSrc1++;
				}
			}

		//////////////////////////////////////////////
		//Sub Wrap
    } else if (_p->bymode==3)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword r = (udword)pPxSrc2->r - (((udword)pPxSrc1->r * rcolPercent)>>8);
					udword g = (udword)pPxSrc2->g - (((udword)pPxSrc1->g * gcolPercent)>>8);
					udword b = (udword)pPxSrc2->b - (((udword)pPxSrc1->b * bcolPercent)>>8);
					pPxSrc2->r = (ubyte) (r%255);
					pPxSrc2->g = (ubyte) (g%255);
					pPxSrc2->b = (ubyte) (b%255);
					pPxSrc2++;	pPxSrc1++;
				}
			}

		//////////////////////////////////////////////
		//Multiply
    } else if (_p->bymode==4)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword r = (udword)pPxSrc2->r * (((udword)pPxSrc1->r * rcolPercent)>>8);
					udword g = (udword)pPxSrc2->g * (((udword)pPxSrc1->g * gcolPercent)>>8);
					udword b = (udword)pPxSrc2->b * (((udword)pPxSrc1->b * bcolPercent)>>8);

					pPxSrc2->r = (ubyte) (r>>8);
					pPxSrc2->g = (ubyte) (g>>8);
					pPxSrc2->b = (ubyte) (b>>8);

					pPxSrc2++;		pPxSrc1++;
				}
			}

		//////////////////////////////////////////////
		//Multiply X2
    } else if (_p->bymode==5)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword r = (udword)pPxSrc2->r * (((udword)pPxSrc1->r * rcolPercent)>>8);
					udword g = (udword)pPxSrc2->g * (((udword)pPxSrc1->g * gcolPercent)>>8);
					udword b = (udword)pPxSrc2->b * (((udword)pPxSrc1->b * bcolPercent)>>8);
					r>>=7;	g>>=7;	b>>=7;

					pPxSrc2->r = (ubyte) ((r<255)?r:255);
					pPxSrc2->g = (ubyte) ((g<255)?g:255);
					pPxSrc2->b = (ubyte) ((b<255)?b:255);

					pPxSrc2++;		pPxSrc1++;
				}
			}

		//////////////////////////////////////////////
		//Blend
    }	else if (_p->bymode==6)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword r = (udword)pPxSrc2->r + (((udword)pPxSrc1->r * rcolPercent)>>8);
					udword g = (udword)pPxSrc2->g + (((udword)pPxSrc1->g * gcolPercent)>>8);
					udword b = (udword)pPxSrc2->b + (((udword)pPxSrc1->b * bcolPercent)>>8);

					pPxSrc2->r = (ubyte) (r >> 1);
					pPxSrc2->g = (ubyte) (g >> 1);
					pPxSrc2->b = (ubyte) (b >> 1);

					pPxSrc2++;
					pPxSrc1++;
				}
			}

		//////////////////////////////////////////////
		//Alpha
    }	else if (_p->bymode==7)	{

			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					udword a = (udword)pPxSrc2->a + (((udword)pPxSrc1->a * bcolPercent)>>8);
					pPxSrc2->a = (ubyte) (a >> 1);
					pPxSrc2++;
					pPxSrc1++;
				}
			}

		//////////////////////////////////////////////
		//Layer
    } else if (_p->bymode==8) {

			for (udword y=0; y<hh; y++)
			{
				pPxSrc2 = pRes1->pbyPixels + (y*w2);
				pPxSrc1 = pRes2->pbyPixels + (y*w);

				for (udword x=0; x<ww; x++)
				{
					float alpha = pPxSrc1->a / 255.f;
					// 1-(1-a1)(1-a2) = 1 - (1-a1-a2+a1a2) = a1+a2-a1a2
					pPxSrc2->a = ubyte((pPxSrc2->a - pPxSrc2->a * alpha) + pPxSrc1->a);
					pPxSrc2->r = ubyte(pPxSrc1->r * alpha + pPxSrc2->r * (1 - alpha));
					pPxSrc2->g = ubyte(pPxSrc1->g * alpha + pPxSrc2->g * (1 - alpha));
					pPxSrc2->b = ubyte(pPxSrc1->b * alpha + pPxSrc2->b * (1 - alpha));
					pPxSrc2++;
					pPxSrc1++;
				}
			}
		}

	}
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NGlowOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*void NGlowOp_Process(SEngineState* _state, NRGBA _color, vec2 _center, vec2 _ray, float _alpha, float _gama)
{
	//Get input texture
	NBitmap* pSrc = _state->apInputs[0];
	NBitmap* pDst = _state->pcurCall->pbmp;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Process operator
	sdword	dwCenterX	= sdword(_center.x*w);
	sdword  dwCenterY	= sdword(_center.y*h);
	sdword  dwRadiusX	= sdword(_ray.x*w);
	sdword  dwRadiusY	= sdword(_ray.y*h);

	float fRed	= _color.r;
	float fGreen= _color.g;
	float fBlue	= _color.b;

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
			udword r = (udword) (((float)pPxSrc->r) + ( (_gama * d * fRed	) * _alpha ));
			udword g = (udword) (((float)pPxSrc->g) + ( (_gama * d * fGreen) * _alpha ));
			udword b = (udword) (((float)pPxSrc->b) + ( (_gama * d * fBlue ) * _alpha ));

			//Affectation couleur
			pPxDst->r = (ubyte) ((r<255)?r:255);
			pPxDst->g = (ubyte) ((g<255)?g:255);
			pPxDst->b = (ubyte) ((b<255)?b:255);
			pPxDst->a = pPxSrc->a;

			pPxSrc++;
			pPxDst++;
		}
	}

}
*/
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NCrackOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*void NCrackOp_Process(SEngineState* _state, NRGBA _color, uword _count, ubyte _variation, ubyte _len, uword _lenDecision, ubyte _quality, ubyte _norm)
{
	//Get input texture
	NBitmap* pSrc = _state->apInputs[0];
	NBitmap* pDst = _state->pcurCall->pbmp;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	NRGBAArray normals, dest(pDst->GetPixels(), w, h);

	if(m_byInputs==2)
	{
		_pOpsInts++;
		NBitmap* pNorm = (NBitmap*)(*_pOpsInts)->m_pObj;

		if(pNorm->GetWidth() < w || pNorm->GetHeight() < h)
			return (udword)-1; // insufficient size

		normals = NRGBAArray(pNorm->GetPixels(), w, h);
	}

	//Copy Source to This
	NMemCopy(pDst->GetPixels(), pSrc->GetPixels(), pDst->GetWidth() * pDst->GetHeight() * sizeof(NRGBA));

	//Get Variables Values
	NRGBA color;
	ubyte byVariation, byLength, byMode, byHQ, byNormalMode;
	uword wSeed, wCount;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)color);
	m_pcvarsBloc->GetValue(1, _ftime, wCount);
	m_pcvarsBloc->GetValue(2, _ftime, byVariation);
	m_pcvarsBloc->GetValue(3, _ftime, byLength);
	m_pcvarsBloc->GetValue(4, _ftime, wSeed);
	m_pcvarsBloc->GetValue(5, _ftime, byMode);
	m_pcvarsBloc->GetValue(6, _ftime, byHQ);
	m_pcvarsBloc->GetValue(7, _ftime, byNormalMode);

	SetSeedValue(wSeed);

	//Process operator
	uword n = 0;
	wCount = uword(wCount * _fDetailFactor);
	while( n++ < wCount )
	{
		// double gives better resolution
		double x = myfRandom() * w;
		double y = myfRandom() * h;
		double a = 2.0f*3.141592f*myfRandom();

		// determine line length
		sdword count = (sdword)(byLength * _fDetailFactor);

		if(normals.width && byMode == 2)
		{
			NRGBA &N = normals(x,y);
			vec3 normal(N.r - 127.f, N.g - 127.f, 0.f);
			count = (sdword)(count * normal.norm() * normal.norm() / 8) // adjusted value
			count = nmin(count, byLength * _fDetailFactor);
		}

		if(byMode == 0)
			count = (sdword)(count * myfRandom() * 2); // E=1

		bool first = true;
		// draw a line
		while( --count >= 0 )
		{
			a += byVariation / 256.0f *(2.0f*myfRandom()-1.0f);

			x = x + cos(a);
			y = y + sin(a);

			if(normals.width && (first || byNormalMode == 0))
			{
				first = false;

				NRGBA &N = normals(x,y);
				vec3 normal(127.f - N.r, N.g-127.f, 0); // x flip
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

}

float NCrackOp::myfRandom()
{
	return (float)myRandom(1) / 65536.0f;
}

*/


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NLerpOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*
void NLerpOp_Process(SEngineState* _state)
{
	//Get inputs textures
	NBitmap* pSrc1  = _state->apInputs[0];
	NBitmap* pSrc2  = _state->apInputs[1];
	NBitmap* pBlend = _state->apInputs[2];

	// Same inputs W and H sizes
	udword w = pSrc1->GetWidth();
	udword h = pSrc1->GetHeight();
	//if (w!=pSrc2->GetWidth()	|| w!=pBlend->GetWidth()	)		return (udword)-1;
	//if (h!=pSrc2->GetHeight() || h!=pBlend->GetHeight() )		return (udword)-1;

	//Set Bitmap Size
	NBitmap* pDst = _state->pcurCall->pbmp;
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

}
*/


SOpFuncInterface IOpsCombine[] = {
  "Rect",		sizeof(SRectParams),  (fxOPFUNCTION*)&NRectOp_Process,
  "Add",		sizeof(SAddParams),   (fxOPFUNCTION*)&NAddOp_Process
};

udword dwCountOpsCombine = fgCreateOpEngine()->RegisterOpsInterfaces(2, IOpsCombine);
