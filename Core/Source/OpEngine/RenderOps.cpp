//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RenderOps.cpp
//! \brief	Render Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
//!						Sebastian Olter (qduaty@gmail.com)
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
#include "../../Include/CoreLib.h"
#include "RenderOps.h"
#include "RectangularArray.h"
#include "Bitmap.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NFlatOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NFlatOp, NOperator);

static NVarsBlocDesc blocdescFlatOp[] =
{
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eudword,	true, "Color",	"4278190080", "NColorProp")	//2
};


NFlatOp::NFlatOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(3, blocdescFlatOp, 2);
}

udword NFlatOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//No Inputs!
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;

	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);

	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	w=(udword) ((float)w*_fDetailFactor);
	h=(udword) ((float)h*_fDetailFactor);

	NRGBA col;
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)col);

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	NRGBA* pPxDst = pDst->GetPixels();

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
FIMPLEMENT_CLASS(NCloudOp, NOperator);

static NVarsBlocDesc blocdescCloudOp[] =
{
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eudword,	true, "Color 0",		"4278190080",		"NColorProp")	//2
	VAR(eudword,	true, "Color 1",		"-1",		"NColorProp")	//3
	VAR(eubyte,		true, "Amp",				"128",	"NUbyteProp") //4	//###TEST###
	VAR(euword,		true, "Seed",				"5412",	"NUwordProp") //5
};

NCloudOp::NCloudOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescCloudOp, 2);
}

udword NCloudOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//No Inputs
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	NRGBA color0, color1;
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

	w=(udword) ((float)w*_fDetailFactor);
	h=(udword) ((float)h*_fDetailFactor);

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	NBitmap* pDst	= (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	NRGBA* pPxDst	= pDst->GetPixels();

	//Create temporary buffer
	m_pbyPxTps	= (sdword*)NMemAlloc(w*h*sizeof(sdword)*2);
	m_pbyPxTps1 = m_pbyPxTps;
	m_pbyPxTps2 = m_pbyPxTps1 + (w*h);

	memset(m_pbyPxTps1, 0, w*h*sizeof(sdword)*2);

	sdword octaves = (sdword)(logf((float)min(w,h))/logf(2.0f)-0.5f);

	m_dwWidth	= w >> octaves;
	m_dwHeight	= h >> octaves;

  //Process operator
	Cloud((ubyte)octaves, (float)byAmp/256.0f);
	Normalize();

	//Temporary buffeur to Texture
	sdword* pbyPxSrc = m_pbyPxTps1;
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			//Cal color
			udword r = (udword)color0.r + ((*pbyPxSrc * (color1.r - color0.r))>>23);
			udword g = (udword)color0.g + ((*pbyPxSrc * (color1.g - color0.g))>>23);
			udword b = (udword)color0.b + ((*pbyPxSrc * (color1.b - color0.b))>>23);
			udword a = (udword)color0.a + ((*pbyPxSrc * (color1.a - color0.a))>>23);

			//Set pixel
			r = (r<255)?r:255;				r = (r>0)?r:0;
			g = (g<255)?g:255;				g = (g>0)?g:0;
			b = (b<255)?b:255;				b = (b>0)?b:0;
			a = (a<255)?a:255;				a = (a>0)?a:0;

			pPxDst->r = (ubyte) r;
			pPxDst->g = (ubyte) g;
			pPxDst->b = (ubyte) b;
			pPxDst->a = (ubyte) a;

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
	sdword amp = (sdword)(fampMod*256.0f*16.0f);

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
		// Compute sum of first bw pixels
		x = 0;
		sdword sum = 0;
		while( x < bw )		{ sum+=pbypixels[x]; ++x; }

		// Set blurred pixels
		x = 0;
		while( x < w )
		{
			pbypixels[(x+bw/2)%w] = sum/(sdword)bw;
			sum -= pbypixels[x];
			sum += pbypixels[(x+bw)%w];
			++x;
		}

		++y;
		pbypixels+=w;
	}

	pbypixels = m_pbyPxTps1;
	///////////////////////////////////////
	// Smooth y

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

	sdword hi = 0, lo = 65536*32767;
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
	float fhi = 1.0f/(float)(hi-lo) * 256*128;
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
FIMPLEMENT_CLASS(NGradientOp, NOperator);

static NVarsBlocDesc blocdescGradientOp[] =
{
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eudword,	true, "A",	"4294901760", "NColorProp")	//2	blue
	VAR(eudword,	true, "B",	"4278255360",		"NColorProp")	//3 green
	VAR(eudword,	true, "C",	"4278190335",			"NColorProp")	//4 red
	VAR(eudword,	true, "D",	"4294967040", "NColorProp")	//5 blue light
};

NGradientOp::NGradientOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescGradientOp, 2);
}

udword NGradientOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//No Inputs!
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;
	m_pcvarsBloc->GetValue(0, 0, byVal);
	sdword w=1<<((udword)byVal);
	m_pcvarsBloc->GetValue(1, 0, byVal);
	sdword h=1<<((udword)byVal);

	w=(sdword) ((float)w*_fDetailFactor);
	h=(sdword) ((float)h*_fDetailFactor);

	NRGBA colA, colB, colC, colD;
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)colA);
	m_pcvarsBloc->GetValue(3, _ftime, (udword&)colB);
	m_pcvarsBloc->GetValue(4, _ftime, (udword&)colC);
	m_pcvarsBloc->GetValue(5, _ftime, (udword&)colD);

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);
	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	NRGBA* pPxDst = pDst->GetPixels();

	//Init
	//A B
	//C D
	NRGBAI col;
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
			col.a= (sdword)((colA.a*a) + (colB.a*b) + (colC.a*c) + (colD.a*d));

			pPxDst->r = (ubyte)col.r;
			pPxDst->g = (ubyte)col.g;
			pPxDst->b = (ubyte)col.b;
			pPxDst->a = (ubyte)col.a;
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
FIMPLEMENT_CLASS(NCellOp, NOperator);

static NVarsBlocDesc blocdescCellOp[] =
{
	VAR(eubyte,	false, "Width",			"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eubyte,	true, 	"Regularity",	"128",	"NUbyteProp")	//2
	VAR(eubyte,	true, 	"Density",		"8",		"NUbyteProp")	//3
	VAR(eudword,true, 	"Color",		"-1",		"NColorProp")	//4
	VAR(euword,	true, 	"Seed",			"5412",	"NUwordProp") //5
	VAR(eubyte,	true, 	"Mode",			"0,[Grid,Chessboard]", "NUbyteComboProp")	//6
	VAR(eubyte,	true, 	"Pattern",			"0,[Both,Cross,Cone]", "NUbyteComboProp")	//6
};

NCellOp::NCellOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(8, blocdescCellOp, 3);
}

udword NCellOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//No Inputs
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;
	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);
	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	w=(udword) ((float)w*_fDetailFactor);
	h=(udword) ((float)h*_fDetailFactor);

	ubyte byRegularity, byDensity, chessboard, pattern;
	uword wSeed;
	NRGBA col;
	m_pcvarsBloc->GetValue(2, _ftime, byRegularity);
	m_pcvarsBloc->GetValue(3, _ftime, byDensity);
	m_pcvarsBloc->GetValue(4, _ftime, (udword&)col);
	m_pcvarsBloc->GetValue(5, _ftime, wSeed);
	m_pcvarsBloc->GetValue(6, _ftime, chessboard);
	m_pcvarsBloc->GetValue(7, _ftime, pattern);

  byDensity = byDensity>1?byDensity:1;

	SetSeedValue(wSeed);

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);
	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	NRGBA* pPxDst = pDst->GetPixels();
	NRGBAArray arrDst(pPxDst,w,h);

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
			cellPoints[x+y*byDensity].x = (x+0.5f+(rand1-0.5f)*(1-regularity))/byDensity - 1.f/w;
			cellPoints[x+y*byDensity].y = (y+0.5f+(rand2-0.5f)*(1-regularity))/byDensity - 1.f/h;
			cellPoints[x+y*byDensity].z = 0;
		}
	}

	//Temporary buffeur to Texture
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
//			bool cfc = false;// chessboard field color
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
//						cfc = ((xo%2)^(yo%2)) ^ (!u ^ !v);
						nextMinDist = minDist;
						minDist = dist;
					} else if (dist<nextMinDist)
					{
						nextMinDist = dist;
					}
				}
			}

			switch(pattern)
			{
				case 0:default:
					minDist = (nextMinDist - minDist) * byDensity;
				break;
				case 1:
					minDist = 2 * nextMinDist * byDensity - 1;
				break;
				case 2:
					minDist = 1 - minDist * byDensity;
				break;
			}

			if (minDist<0) minDist = 0;
			if (minDist>1) minDist = 1;

			if(chessboard)
			{
				bool cfc = ((xo&1)^(yo&1))!=0;
				float coeff = (1 - 2 * cfc) / 2.5f;
				pPxDst->r = (ubyte)((cfc + coeff * minDist)*col.r);
				pPxDst->g = (ubyte)((cfc + coeff * minDist)*col.g);
				pPxDst->b = (ubyte)((cfc + coeff * minDist)*col.b);
			}
			else
			{
				pPxDst->r = (ubyte)(minDist*col.r);
				pPxDst->g = (ubyte)(minDist*col.g);
				pPxDst->b = (ubyte)(minDist*col.b);
			}

			pPxDst->a = (ubyte)(col.a);
			pPxDst++;
		}
	}

	NMemFree(cellPoints);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NNoiseOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NNoiseOp, NOperator);

static NVarsBlocDesc blocdescNoiseOp[] =
{
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eudword,	true, "Color",	"-1", "NColorProp")	//2
	VAR(euword,		true, "Seed",				"5412",	"NUwordProp") //3
};


NNoiseOp::NNoiseOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescNoiseOp, 1);
}

udword NNoiseOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//No Inputs!
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;

	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);

	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	w=(udword) ((float)w*_fDetailFactor);
	h=(udword) ((float)h*_fDetailFactor);

	NRGBA col;
	m_pcvarsBloc->GetValue(2, _ftime, (udword&)col);

	uword wSeed;
	m_pcvarsBloc->GetValue(3, _ftime, wSeed);
	SetSeedValue(wSeed);

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	NRGBA* pPxDst = pDst->GetPixels();

	//Process operator
	for (udword y=0; y<w; y++)
		for (udword x=0; x<h; x++)
		{
			udword noiseVal = myRandom(1)%256;
			pPxDst->r = (ubyte)((noiseVal*col.r)>>8);
			pPxDst->g = (ubyte)((noiseVal*col.g)>>8);
			pPxDst->b = (ubyte)((noiseVal*col.b)>>8);
			pPxDst->a = col.a;
			*pPxDst++;
		}

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NVectorOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*FIMPLEMENT_CLASS(NVectorOp, NOperator);

static void* vgvmAlloc( size_t size )
{
  return NMemAlloc( size );
}

static void vgvmFree( void* memory )
{
  return NMemFree( memory );
}

static NVarsBlocDesc blocdescVectorOp[] =
{
	VAR(eubyte,	false, "Width",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",	"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(eubyte,	true,  "VectorData",    "0",	                                           "NVectorDataProp")	//2
};


NVectorOp::NVectorOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(3, blocdescVectorOp, 1);

	m_pContext = vgvm::cairo::createCairoContext(vgvmAlloc, vgvmFree);
	m_pProgram = m_pContext->createProgram();
}

NVectorOp::~NVectorOp()
{
	delete m_pProgram;
	delete m_pContext;
}

void NVectorOp::SetVectorData(ubyte* _data, udword _length)
{
	m_pProgram->load(_data, _length);
}

bool NVectorOp::Save(NArchive* _s)
{
	if(!NOperator::Save(_s))
		return false;
	*_s << (udword)m_pProgram->getSize();
	_s->PutData(m_pProgram->getBuffer(), m_pProgram->getSize());
	return true;
}

bool NVectorOp::Load(NArchive* _l)
{
	NOperator::Load(_l);
	udword size;
	*_l >> size;
	if(size > 0)
	{
		ubyte* data = (ubyte*)NMemAlloc(size);
		_l->GetData(data, size);
		m_pProgram->load(data, size);
		NMemFree(data);
	}
        return true;
}

udword NVectorOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//No Inputs!
	if (m_byInputs!=0)		return (udword)-1;

	//Get Variables Values
	ubyte byVal;

	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1<<((udword)byVal);

	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1<<((udword)byVal);

	w=(udword) ((float)w*_fDetailFactor);
	h=(udword) ((float)h*_fDetailFactor);

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	NRGBA* pPxDst = pDst->GetPixels();

	memset( pPxDst, 0, w*h*4 );

	m_pContext->setOutput((unsigned char*)pPxDst, w, h, true);
	m_pContext->reset();
	m_pContext->setRegisterCount( 1 );
	m_pContext->setRegister( 0, vgvm::Value( 256.0f ) );
	m_pContext->executeProgram( m_pProgram );

	// Switch from BGR to RGB
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			NRGBA color = *pPxDst;
			pPxDst->r = color.b;
			pPxDst->g = color.g;
			pPxDst->b = color.r;

			pPxDst++;
		}
	}

	return 0;
}
*/
