//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RenderOpsFunc.cpp
//! \brief	Render Operators Functions
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
//!						Sebastian Olter (qduaty@gmail.com)
//!
//!	\date		26-05-2009
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
//							NFlatOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void  NFlatOp_Process(SEngineState* _state, ubyte _wpow, ubyte _hpow, NRGBA _color)
{
	SResource* pDst = _state->apLayers[_state->pcurCall->byDepth];

	//Set Bitmap Size
	udword w=(udword) (((float)(1<<((udword)_wpow)))* _state->fDetailFactor);
	udword h=(udword) (((float)(1<<((udword)_hpow)))* _state->fDetailFactor);
	Res_SetBmpSize(pDst,w,h);

	//Process operator
	NRGBA* pPxDst = pDst->pbyPixels;
	for (udword y=0; y<w; y++)
		for (udword x=0; x<h; x++)
			*pPxDst++ = _color;

}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NCloudOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void AddRandom(float fampMod, udword w, udword h, sdword* pxls)
{
	sdword amp = (sdword)(fampMod*256.0f*16.0f);

	sdword* pbypixels = pxls;//m_pbyPxTps1;
	for (udword x=0; x<w; x++)
	{
		for (udword y=0; y<h; y++)
		{
			sdword r = amp * ((sdword)myRandom() - 32768);	//[-32768<>+32768]
			*pbypixels+= r;
			pbypixels++;
		}
	}
}


void ImageResize(udword w, udword h, sdword *pxls, udword w2, udword h2, sdword* pxls2)
{
	//Resize image
	sdword* pbyPxSrc = pxls;
	sdword* pbyPxDst = pxls2;
	for (udword y=0; y<h2; y++)
	{
		for (udword x=0; x<w2; x++)
		{
			udword px = x * w/w2;
			udword py = y * h/h2;
			*pbyPxDst = pbyPxSrc[px + py*w];
			pbyPxDst++;
		}
	}
}

void BoxBlur(udword w, udword h, sdword *pxls, udword bw, udword bh, sdword* pxlsTps)
{
	sdword* pbypixels = pxls;
	//udword w = pbmp->GetWidth();
	//udword h = pbmp->GetHeight();

	udword x, y;

	///////////////////////////////////////
	// Smooth x
	sdword* pline = pxlsTps;

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

	pbypixels = pxls;
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

sdword* Cloud(ubyte byoctaves, float fampMod, udword w, udword h, sdword* pxls,sdword* pxlsTps)
{
	float amp = 1.0f;

	AddRandom(amp,w,h,pxls);

	sdword dwOctaves = (sdword)byoctaves;
	while( --dwOctaves >= 0 )
	{
		ImageResize(w, h, pxls, w*2, h*2, pxlsTps);
		w*=2;	h*=2;
		//Swap buffers
		sdword* pbyswap = pxls;
		pxls = pxlsTps;
		pxlsTps = pbyswap;
		
		BoxBlur(w,h,pxls,3, 3, pxlsTps);
		amp *= fampMod;
		AddRandom(amp, w,h, pxls);
	}

	// blur the image one last time
	BoxBlur(w,h,pxls,3, 3, pxlsTps);

	return pxls;
}



void Normalize(udword w, udword h, sdword *pxls)
{
	sdword x, y;

	/////////////////////////////////////////////////////
	// Find the highest and lowest values used
	sdword* pbypixels = pxls;

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
	pbypixels = pxls;

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

void  NCloudOp_Process(SEngineState* _state, ubyte _wpow, ubyte _hpow, NRGBA _color0, NRGBA _color1, ubyte _amp, uword _seed)
{
	//Get Variables Values
	SetSeedValue(_seed);

	udword w=1<<((udword)_wpow);
	udword h=1<<((udword)_hpow);

	w=(udword) ((float)w * _state->fDetailFactor);
	h=(udword) ((float)h * _state->fDetailFactor);

	SResource* pDst = _state->apResourcesLayers[_state->pcurCall->byDepth];
	Res_SetBmpSize(pDst,w,h);

	//Used TPSLAYER as temporary buffer
	Res_SetBmpSize(_state->apLayers[TPS_LAYER], w,h*2);
	sdword* m_pbyPxTps	= (sdword*)_state->apLayers[TPS_LAYER]->pbyPixels;
	sdword* m_pbyPxTps1 = m_pbyPxTps;
	sdword* m_pbyPxTps2 = m_pbyPxTps1 + (w*h);

	memset(m_pbyPxTps1, 0, w*h*sizeof(sdword)*2);

	sdword octaves = (sdword)(logf((float)nmin(w,h))/logf(2.0f)-0.5f);

	udword w2	= w >> octaves;
	udword h2	= h >> octaves;

  //Process operator
	sdword* pbyPxSrc = Cloud((ubyte)octaves, (float)_amp/256.0f, w2, h2, m_pbyPxTps1, m_pbyPxTps2);

	Normalize(w,h,pbyPxSrc);

	//Temporary grey buffeur to color Texture
	NRGBA* pPxDst = pDst->pbyPixels;
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			//Cal color
			udword r = (udword)_color0.r + ((*pbyPxSrc * (_color1.r - _color0.r))>>23);
			udword g = (udword)_color0.g + ((*pbyPxSrc * (_color1.g - _color0.g))>>23);
			udword b = (udword)_color0.b + ((*pbyPxSrc * (_color1.b - _color0.b))>>23);
			udword a = (udword)_color0.a + ((*pbyPxSrc * (_color1.a - _color0.a))>>23);

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

}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NGradientOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void  NGradientOp_Process(SEngineState* _state, ubyte _wpow, ubyte _hpow, NRGBA _colA, NRGBA _colB, NRGBA _colC, NRGBA _colD)
{
	SResource* pDst = _state->apLayers[_state->pcurCall->byDepth];

	//Set Bitmap Size
	sdword w=(sdword) (((float)(1<<((udword)_wpow)))* _state->fDetailFactor);
	sdword h=(sdword) (((float)(1<<((udword)_hpow)))* _state->fDetailFactor);
	Res_SetBmpSize(pDst,w,h);

	//Init
	//A B
	//C D
	NRGBAI _color;
	float finv_WH = 1.0f / (float)(w*h);

	//Process operator
	NRGBA* pPxDst = pDst->pbyPixels;
	for (sdword y=0; y<h; y++)
	{
		for (sdword x=0; x<w; x++)
		{
			//Calc percent A,B,C,D
			float a = (float)((w-x)	* (h-y)) * finv_WH;
			float b = (float)((x)		* (h-y)) * finv_WH;
			float c = (float)((w-x)	* (y))	 * finv_WH;
			float d = (float)((x)		* (y))	 * finv_WH;

			_color.r= (sdword)((_colA.r*a) + (_colB.r*b) + (_colC.r*c) + (_colD.r*d));
			_color.g= (sdword)((_colA.g*a) + (_colB.g*b) + (_colC.g*c) + (_colD.g*d));
			_color.b= (sdword)((_colA.b*a) + (_colB.b*b) + (_colC.b*c) + (_colD.b*d));
			_color.a= (sdword)((_colA.a*a) + (_colB.a*b) + (_colC.a*c) + (_colD.a*d));

			pPxDst->r = (ubyte)_color.r;
			pPxDst->g = (ubyte)_color.g;
			pPxDst->b = (ubyte)_color.b;
			pPxDst->a = (ubyte)_color.a;
			pPxDst++;
		}
	}

}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NCellOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void  NCellOp_Process(SEngineState* _state, ubyte _wpow, ubyte _hpow, ubyte _reg, ubyte _dens, NRGBA _color, uword _seed, ubyte _mode, ubyte _pat)
{
	SResource* pDst = _state->apLayers[_state->pcurCall->byDepth];

	//Set Bitmap Size
	udword w=(udword) (((float)(1<<((udword)_wpow)))* _state->fDetailFactor);
	udword h=(udword) (((float)(1<<((udword)_hpow)))* _state->fDetailFactor);
	Res_SetBmpSize(pDst,w,h);

	SetSeedValue(_seed);

  _dens = _dens>1?_dens:1;

	NRGBA* pPxDst = pDst->pbyPixels;
	NRGBAArray arrDst(pPxDst,w,h);

	//Init
	const float regularity = _reg / 255.0f;
	vec3 *cellPoints = (vec3*)NNEWARRAY(vec3,_dens*_dens);	//###TOFIX### JN: Alloc

	//Render
	for (udword y=0; y<_dens; ++y)
	{
		for (udword x=0; x<_dens; ++x)
		{
			float rand1 = (float)myRandom() / 65536.0f;
			float rand2 = (float)myRandom() / 65536.0f;
			cellPoints[x+y*_dens].x = (x+0.5f+(rand1-0.5f)*(1-regularity))/_dens - 1.f/w;
			cellPoints[x+y*_dens].y = (y+0.5f+(rand2-0.5f)*(1-regularity))/_dens - 1.f/h;
			cellPoints[x+y*_dens].z = 0;
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
			int xo = x*_dens/w;
			int yo = y*_dens/h;
			for (sdword v=-1; v<2; ++v)
			{
				int vo = ((yo+_dens+v)%_dens)*_dens;
				for (sdword u=-1; u<2; ++u)
				{
					vec3 cellPos = cellPoints[((xo+_dens+u)%_dens) + vo];
					if (u==-1 && x*_dens<w) cellPos.x-=1;
					if (v==-1 && y*_dens<h) cellPos.y-=1;
					if (u==1 && x*_dens>=w*(_dens-1)) cellPos.x+=1;
					if (v==1 && y*_dens>=h*(_dens-1)) cellPos.y+=1;
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

			switch(_pat)
			{
				case 0:default:
					minDist = (nextMinDist - minDist) * _dens;
				break;
				case 1:
					minDist = 2 * nextMinDist * _dens - 1;
				break;
				case 2:
					minDist = 1 - minDist * _dens;
				break;
			}

			if (minDist<0) minDist = 0;
			if (minDist>1) minDist = 1;

			if(_mode==1)	//Chessboard
			{
				bool cfc = (xo&1)^(yo&1);
				float coeff = (1 - 2 * cfc) / 2.5f;
				pPxDst->r = (ubyte)((cfc + coeff * minDist)*_color.r);
				pPxDst->g = (ubyte)((cfc + coeff * minDist)*_color.g);
				pPxDst->b = (ubyte)((cfc + coeff * minDist)*_color.b);
			}
			else
			{
				pPxDst->r = (ubyte)(minDist*_color.r);
				pPxDst->g = (ubyte)(minDist*_color.g);
				pPxDst->b = (ubyte)(minDist*_color.b);
			}

			pPxDst->a = (ubyte)(_color.a);
			pPxDst++;
		}
	}

	NDELETEARRAY(cellPoints);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NNoiseOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void NNoiseOp_Process(SEngineState* _state, ubyte _wpow, ubyte _hpow, NRGBA _color, uword _seed)
{
	SResource* pDst = _state->apLayers[_state->pcurCall->byDepth];

	//Set Bitmap Size
	udword w=(udword) (((float)(1<<((udword)_wpow)))* _state->fDetailFactor);
	udword h=(udword) (((float)(1<<((udword)_hpow)))* _state->fDetailFactor);
	Res_SetBmpSize(pDst,w,h);

	SetSeedValue(_seed);

	//Process operator
	NRGBA* pPxDst = pDst->pbyPixels;
	for (udword y=0; y<w; y++)
	{
		for (udword x=0; x<h; x++)
		{
			udword noiseVal = myRandom(1)%256;
			pPxDst->r = (ubyte)((noiseVal*_color.r)>>8);
			pPxDst->g = (ubyte)((noiseVal*_color.g)>>8);
			pPxDst->b = (ubyte)((noiseVal*_color.b)>>8);
			pPxDst->a = _color.a;
			*pPxDst++;
		}
	}
}


SOpFuncInterface IOpsRender[] = {
	"Flat",			3*4,  (fxOPFUNCTION*)&NFlatOp_Process,
	"Gradient",	8*4,  (fxOPFUNCTION*)&NGradientOp_Process,
	"Cloud",		6*4,  (fxOPFUNCTION*)&NCloudOp_Process
};

udword dwCountOpsRender = fgCreateOpEngine()->RegisterOpsInterfaces(3, IOpsRender);

