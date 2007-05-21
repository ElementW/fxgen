//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RenderOps.cpp
//! \brief	Render Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
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

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "RenderOps.h"

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
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eudword,	true, "Color",	"4278190080", "NColorProp")	//2
};

NFlatOp::NFlatOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(3, blocdescFlatOp);
}

udword NFlatOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//No Inputs!
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;

	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);

	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	RGBA col;
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)col);

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	RGBA* pPxDst = pDst->GetPixels();

	//Process operator
	for (udword y=0; y<w; y++)
		for (udword x=0; x<h; x++)
			*pPxDst++ = col;

	return 0;
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
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eudword,	true, "Color 0",		"0",		"NColorProp")	//2
	VAR(eudword,	true, "Color 1",		"-1",		"NColorProp")	//3
	VAR(eubyte,		true, "Amp",				"128",	"NUbyteProp") //4
	VAR(euword,		true, "Seed",				"5412",	"NUwordProp") //5
};

NCloudOp::NCloudOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescCloudOp);
}

udword NCloudOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//No Inputs
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	RGBA color0, color1;
	uword wSeed;
	ubyte byAmp;
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)color0);
	m_pcvarsBloc->GetValue(3, _ftime, (udword&)color1);
	m_pcvarsBloc->GetValue(4, _ftime, byAmp);
	m_pcvarsBloc->GetValue(5, _ftime, wSeed);

	SetSeedValue(wSeed);

	ubyte byVal;
	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);
	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	NBitmap* pDst	= (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	RGBA* pPxDst	= pDst->GetPixels();

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
//							NGradientOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NGradientOp, NOperator);

static NVarsBlocDesc blocdescGradientOp[] =
{
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eudword,	true, "A",	"16711680", "NColorProp")	//2	blue
	VAR(eudword,	true, "B",	"65280",		"NColorProp")	//3 green
	VAR(eudword,	true, "C",	"255",			"NColorProp")	//4 red
	VAR(eudword,	true, "D",	"16776960", "NColorProp")	//5 blue light
};

NGradientOp::NGradientOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescGradientOp);
}

udword NGradientOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//No Inputs!
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;
	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);
	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	RGBA colA, colB, colC, colD;
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)colA);
	m_pcvarsBloc->GetValue(3, _ftime, (udword&)colB);
	m_pcvarsBloc->GetValue(4, _ftime, (udword&)colC);
	m_pcvarsBloc->GetValue(5, _ftime, (udword&)colD);

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);
	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	RGBA* pPxDst = pDst->GetPixels();

	//Init
	//A B
	//C D
	RGBI col;
	float finv_WH = 1.0f / (float)(w*h);

	//Process operator
	for (sdword y=0; y<h; y++)
	{
		for (sdword x=0; x<w; x++)
		{
			//Calc percent A,B,C,D
			float a = (float)((w-x)	* (h-y)) * finv_WH;
			float b = (float)((x)		* (h-y)) * finv_WH;
			float c = (float)((w-x)	* (y))	 * finv_WH;
			float d = (float)((x)		* (y))	 * finv_WH;

			col.r= (sdword)((colA.r*a) + (colB.r*b) + (colC.r*c) + (colD.r*d));
			col.g= (sdword)((colA.g*a) + (colB.g*b) + (colC.g*c) + (colD.g*d));
			col.b= (sdword)((colA.b*a) + (colB.b*b) + (colC.b*c) + (colD.b*d));

			pPxDst->r = (ubyte)col.r;
			pPxDst->g = (ubyte)col.g;
			pPxDst->b = (ubyte)col.b;
			pPxDst->a = 255;
			pPxDst++;
		}
	}

	return 0;
}


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
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eubyte,		true, "Regularity",	"128",	"NUbyteProp")	//2
	VAR(eubyte,		true, "Density",		"8",		"NUbyteProp")	//3
	VAR(eudword,	true, "Color",			"-1",		"NColorProp")	//4
	VAR(euword,		true, "Seed",				"5412",	"NUwordProp") //5
};

NCellOp::NCellOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescCellOp);
}

udword NCellOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//No Inputs
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;
	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);
	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	ubyte byRegularity;
	ubyte byDensity;
	uword wSeed;
	RGBA col;
	m_pcvarsBloc->GetValue(2, _ftime, byRegularity);
	m_pcvarsBloc->GetValue(3, _ftime, byDensity);
	m_pcvarsBloc->GetValue(4, _ftime, (udword&)col);
	m_pcvarsBloc->GetValue(5, _ftime, wSeed);

  byDensity = byDensity>1?byDensity:1;

	SetSeedValue(wSeed);

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);
	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	RGBA* pPxDst = pDst->GetPixels();

	//Init
	const float regularity = byRegularity / 255.0f;
	vec3 *cellPoints = (vec3*)NMemAlloc(sizeof(vec3)*byDensity*byDensity);	//###TOFIX### JN: MemAlloc

	//Render
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
