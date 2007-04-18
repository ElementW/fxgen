//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DentoidOps.cpp
//! \brief	Operators contributed by Anders 'Dentoid' Stenberg
//!
//!	\author	Anders Stenberg (anders.stenberg@gmail.com)
//!	\date		12-04-2007
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
#include "DentoidOps.h"


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCellOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NCellOp, NOperator);

static NVarsBlocDesc blocdescCellOp[] =
{
	VAR(eubyte,		true, "Regularity",	"128",	"NUbyteProp")	//0
	VAR(eubyte,		true, "Density",		"8",		"NUbyteProp")	//1
	VAR(eudword,	true, "Color",			"-1",		"NColorProp")	//2
	VAR(euword,		true, "Seed",				"5412",	"NUwordProp") //3
};

NCellOp::NCellOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescCellOp);
}

udword NCellOp::Process(float _ftime, NOperator** _pOpsInts)
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
	ubyte byRegularity;
	ubyte byDensity;
	uword wSeed;
	RGBA col;
	m_pcvarsBloc->GetValue(0, _ftime, byRegularity);
	m_pcvarsBloc->GetValue(1, _ftime, byDensity);
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)col);
	m_pcvarsBloc->GetValue(3, _ftime, wSeed);

  byDensity = byDensity>1?byDensity:1;
  //byDensity = byDensity<32?byDensity:32;

	SetSeedValue(wSeed);

	const float regularity = byRegularity / 255.0f;
	vec3 *cellPoints = (vec3*)NMemAlloc(sizeof(vec3)*byDensity*byDensity);

	for (udword y=0; y<byDensity; ++y)
	{
		for (udword x=0; x<byDensity; ++x)
		{
			float rand1 = (float)myRandom() / 65536.0f;
			float rand2 = (float)myRandom() / 65536.0f;
			cellPoints[x+y*byDensity].x = (x+0.5f+(rand1-0.5f)*(1-regularity))/byDensity;
			cellPoints[x+y*byDensity].y = (y+0.5f+(rand2-0.5f)*(1-regularity))/byDensity;
			cellPoints[x+y*byDensity].z = 0;
		}
	}

	//Temporary buffeur to Texture
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
      vec3 pixelPos;
      pixelPos.x = x/(float)w,
      pixelPos.y = y/(float)h;
      pixelPos.z = 0;

      float minDist = 10;
      float nextMinDist = minDist;
      int xo = x*byDensity/w;
      int yo = y*byDensity/h;
      for (sdword v=-1; v<2; ++v)
      {
        int vo = ((yo+byDensity+v)%byDensity)*byDensity;
        for (sdword u=-1; u<2; ++u)
        {
          vec3 cellPos = cellPoints[((xo+byDensity+u)%byDensity) + vo];
          if (u==-1 && x*byDensity<w) cellPos.x-=1;
          if (v==-1 && y*byDensity<h) cellPos.y-=1;
          if (u==1 && x*byDensity>=w*(byDensity-1)) cellPos.x+=1;
          if (v==1 && y*byDensity>=h*(byDensity-1)) cellPos.y+=1;
          vec3 tmp;
          float dist = sub(tmp, pixelPos, cellPos).norm ();
          if (dist<minDist) 
          {
            nextMinDist = minDist;
            minDist = dist;
          } else if (dist<nextMinDist)
          {
            nextMinDist = dist;
          }
        }
      }

      minDist = (nextMinDist-minDist)*byDensity;
      if (minDist<0) minDist = 0;
      if (minDist>1) minDist = 1;

			pPxDst->r = (ubyte)(minDist*col.r);
			pPxDst->g = (ubyte)(minDist*col.g);
			pPxDst->b = (ubyte)(minDist*col.b);
			pPxDst->a = 255;

			pPxDst++;
		}
	}

	NMemFree(cellPoints);

	return 0;
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NInvertOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NInvertOp, NOperator);

udword NInvertOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	// Init
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst	= (NBitmap*)m_pObj;

	udword w = pDst->GetWidth();
	udword h = pDst->GetHeight();
	RGBA* pPxSrc = pSrc->GetPixels();
	RGBA* pPxDst = pDst->GetPixels();

	//Temporary buffeur to Texture
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			pPxDst->r = 255-pPxSrc->r;
			pPxDst->g = 255-pPxSrc->g;
			pPxDst->b = 255-pPxSrc->b;
			pPxDst->a = 255;

			pPxSrc++;
			pPxDst++;
		}
	}

	return 0;
}
