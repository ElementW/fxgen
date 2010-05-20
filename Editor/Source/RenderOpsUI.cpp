//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RenderOpsUI.h
//! \brief	Render Operators Node for user interface
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!
//!	\date		26-02-2010
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
#include "RenderOpsUI.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NFlatOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NFlatOp, NOperatorNode)
	NEWFIELD(eInteger, "width",  NFlatOp, wpow,  0, 12, 1, "8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NComboProp"),
	NEWFIELD(eInteger, "height", NFlatOp, hpow,  0, 12, 1, "8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NComboProp"),
	NEWFIELD(eRgba,		 "color",  NFlatOp, color, 1, -1, 1, "0,0,0,255", "NColorProp"),
FIMPLEMENT_CLASS_END()

NFlatOp::NFlatOp()
{
	wpow=hpow=8;
	color=NRGBA(255,0,0,255);
}

void  NFlatOp_Process(SEngineState* _state, void* _params)
{
	SFlatParams* p = (SFlatParams*)_params;

	SResource* pDst = _state->apResourcesLayers[_state->pcurCall->byDepth];

	//Set Bitmap Size
	udword w=(udword) (((float)(1<<((udword)p->wpow)))* _state->fDetailFactor);
	udword h=(udword) (((float)(1<<((udword)p->hpow)))* _state->fDetailFactor);
	Res_SetBmpSize(pDst,w,h);

	//Process operator
	NRGBA* pPxDst = pDst->pbyPixels;
	for (udword y=0; y<w; y++)
		for (udword x=0; x<h; x++)
			*pPxDst++ = p->color;

}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCloudOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NCloudOp, NOperatorNode)
FIMPLEMENT_CLASS_END()

NCloudOp::NCloudOp()
{
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NGradientOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NGradientOp, NOperatorNode)
FIMPLEMENT_CLASS_END()

NGradientOp::NGradientOp()
{
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCellOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NCellOp, NOperatorNode)
FIMPLEMENT_CLASS_END()

NCellOp::NCellOp()
{
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NNoiseOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NNoiseOp, NOperatorNode)
FIMPLEMENT_CLASS_END()

NNoiseOp::NNoiseOp()
{
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

struct SCloudParams
{
	ubyte wpow;
	ubyte hpow;
	NRGBA color0;
	NRGBA color1;
	ubyte amp;
	uword seed;
};

void NCloudOp_Process(SEngineState* _state, void* _params)
{
	SCloudParams* p = (SCloudParams*)_params;

	//Get Variables Values
	SetSeedValue(p->seed);

	udword w=1<<((udword)p->wpow);
	udword h=1<<((udword)p->hpow);

	w=(udword) ((float)w * _state->fDetailFactor);
	h=(udword) ((float)h * _state->fDetailFactor);

	SResource* pDst = _state->apResourcesLayers[_state->pcurCall->byDepth];
	Res_SetBmpSize(pDst,w,h);

	//Used TPSLAYER as temporary buffer
	Res_SetBmpSize(_state->apResourcesLayers[TPS_LAYER], w,h*2);
	sdword* m_pbyPxTps	= (sdword*)_state->apResourcesLayers[TPS_LAYER]->pbyPixels;
	sdword* m_pbyPxTps1 = m_pbyPxTps;
	sdword* m_pbyPxTps2 = m_pbyPxTps1 + (w*h);

	memset(m_pbyPxTps1, 0, w*h*sizeof(sdword)*2);

	sdword octaves = (sdword)(logf((float)nmin(w,h))/logf(2.0f)-0.5f);

	udword w2	= w >> octaves;
	udword h2	= h >> octaves;

  //Process operator
	sdword* pbyPxSrc = Cloud((ubyte)octaves, (float)p->amp/256.0f, w2, h2, m_pbyPxTps1, m_pbyPxTps2);

	Normalize(w,h,pbyPxSrc);

	//Temporary grey buffeur to color Texture
	NRGBA* pPxDst = pDst->pbyPixels;
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			//Cal color
			udword r = (udword)p->color0.r + ((*pbyPxSrc * (p->color1.r - p->color0.r))>>23);
			udword g = (udword)p->color0.g + ((*pbyPxSrc * (p->color1.g - p->color0.g))>>23);
			udword b = (udword)p->color0.b + ((*pbyPxSrc * (p->color1.b - p->color0.b))>>23);
			udword a = (udword)p->color0.a + ((*pbyPxSrc * (p->color1.a - p->color0.a))>>23);

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
struct SGradientParams
{
	ubyte wpow;
	ubyte hpow;
	NRGBA colA;
	NRGBA colB;
	NRGBA colC;
	NRGBA colD;
};

void  NGradientOp_Process(SEngineState* _state, void* _params)
{
	SGradientParams* p = (SGradientParams*)_params;

	SResource* pDst = _state->apResourcesLayers[_state->pcurCall->byDepth];

	//Set Bitmap Size
	sdword w=(sdword) (((float)(1<<((udword)p->wpow)))* _state->fDetailFactor);
	sdword h=(sdword) (((float)(1<<((udword)p->hpow)))* _state->fDetailFactor);
	Res_SetBmpSize(pDst,w,h);

	//Init
	//A B
	//C D
	NRGBAI color;
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

			color.r= (sdword)((p->colA.r*a) + (p->colB.r*b) + (p->colC.r*c) + (p->colD.r*d));
			color.g= (sdword)((p->colA.g*a) + (p->colB.g*b) + (p->colC.g*c) + (p->colD.g*d));
			color.b= (sdword)((p->colA.b*a) + (p->colB.b*b) + (p->colC.b*c) + (p->colD.b*d));
			color.a= (sdword)((p->colA.a*a) + (p->colB.a*b) + (p->colC.a*c) + (p->colD.a*d));

			pPxDst->r = (ubyte)color.r;
			pPxDst->g = (ubyte)color.g;
			pPxDst->b = (ubyte)color.b;
			pPxDst->a = (ubyte)color.a;
			pPxDst++;
		}
	}

}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NCellOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
struct SCellParams
{
	ubyte wpow;
	ubyte hpow;
	ubyte reg;
	ubyte dens;
	NRGBA color;
	uword seed;
	ubyte mode;
	ubyte pat;
};

void  NCellOp_Process(SEngineState* _state, void* _params)
{
	SCellParams* p = (SCellParams*)_params;
	SResource* pDst = _state->apResourcesLayers[_state->pcurCall->byDepth];

	//Set Bitmap Size
	udword w=(udword) (((float)(1<<((udword)p->wpow)))* _state->fDetailFactor);
	udword h=(udword) (((float)(1<<((udword)p->hpow)))* _state->fDetailFactor);
	Res_SetBmpSize(pDst,w,h);

	SetSeedValue(p->seed);

  udword dens = p->dens>1?p->dens:1;

	NRGBA* pPxDst = pDst->pbyPixels;
	NRGBAArray arrDst(pPxDst,w,h);

	//Init
	const float regularity = p->reg / 255.0f;
	vec3 *cellPoints = (vec3*)NNEWARRAY(vec3,dens*dens);	//###TOFIX### JN: Alloc

	//Render
	for (udword y=0; y<dens; ++y)
	{
		for (udword x=0; x<dens; ++x)
		{
			float rand1 = (float)myRandom() / 65536.0f;
			float rand2 = (float)myRandom() / 65536.0f;
			cellPoints[x+y*dens].x = (x+0.5f+(rand1-0.5f)*(1-regularity))/dens - 1.f/w;
			cellPoints[x+y*dens].y = (y+0.5f+(rand2-0.5f)*(1-regularity))/dens - 1.f/h;
			cellPoints[x+y*dens].z = 0;
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
			int xo = x*dens/w;
			int yo = y*dens/h;
			for (sdword v=-1; v<2; ++v)
			{
				int vo = ((yo+dens+v)%dens)*dens;
				for (sdword u=-1; u<2; ++u)
				{
					vec3 cellPos = cellPoints[((xo+dens+u)%dens) + vo];
					if (u==-1 && x*dens<w) cellPos.x-=1;
					if (v==-1 && y*dens<h) cellPos.y-=1;
					if (u==1 && x*dens>=w*(dens-1)) cellPos.x+=1;
					if (v==1 && y*dens>=h*(dens-1)) cellPos.y+=1;
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

			switch(p->pat)
			{
				case 0:default:
					minDist = (nextMinDist - minDist) * dens;
				break;
				case 1:
					minDist = 2 * nextMinDist * dens - 1;
				break;
				case 2:
					minDist = 1 - minDist * dens;
				break;
			}

			if (minDist<0) minDist = 0;
			if (minDist>1) minDist = 1;

			if(p->mode==1)	//Chessboard
			{
				bool cfc = (xo&1)^(yo&1);
				float coeff = (1 - 2 * cfc) / 2.5f;
				pPxDst->r = (ubyte)((cfc + coeff * minDist)*p->color.r);
				pPxDst->g = (ubyte)((cfc + coeff * minDist)*p->color.g);
				pPxDst->b = (ubyte)((cfc + coeff * minDist)*p->color.b);
			}
			else
			{
				pPxDst->r = (ubyte)(minDist*p->color.r);
				pPxDst->g = (ubyte)(minDist*p->color.g);
				pPxDst->b = (ubyte)(minDist*p->color.b);
			}

			pPxDst->a = (ubyte)(p->color.a);
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
struct SNoiseParams
{
	ubyte wpow;
	ubyte hpow;
	NRGBA color;
	uword seed;
};

void NNoiseOp_Process(SEngineState* _state, void* _params)
{
	SNoiseParams* p = (SNoiseParams*)_params;

	SResource* pDst = _state->apResourcesLayers[_state->pcurCall->byDepth];

	//Set Bitmap Size
	udword w=(udword) (((float)(1<<((udword)p->wpow)))* _state->fDetailFactor);
	udword h=(udword) (((float)(1<<((udword)p->hpow)))* _state->fDetailFactor);
	Res_SetBmpSize(pDst,w,h);

	SetSeedValue(p->seed);

	//Process operator
	NRGBA* pPxDst = pDst->pbyPixels;
	for (udword y=0; y<w; y++)
	{
		for (udword x=0; x<h; x++)
		{
			udword noiseVal = myRandom(1)%256;
			pPxDst->r = (ubyte)((noiseVal*p->color.r)>>8);
			pPxDst->g = (ubyte)((noiseVal*p->color.g)>>8);
			pPxDst->b = (ubyte)((noiseVal*p->color.b)>>8);
			pPxDst->a = p->color.a;
			*pPxDst++;
		}
	}
}


SOpFuncInterface IOpsRender[] = {
	"Flat",			sizeof(SFlatParams),			(fxOPFUNCTION*)&NFlatOp_Process,
	"Gradient",	sizeof(SGradientParams),  (fxOPFUNCTION*)&NGradientOp_Process,
	"Cloud",		sizeof(SCloudParams),			(fxOPFUNCTION*)&NCloudOp_Process,
	"Gradient",	sizeof(SGradientParams),	(fxOPFUNCTION*)&NGradientOp_Process,
	"Cell",			sizeof(SCellParams),			(fxOPFUNCTION*)&NCellOp_Process,
	"Noise",		sizeof(SNoiseParams),			(fxOPFUNCTION*)&NNoiseOp_Process
};

udword dwCountOpsRender = fgCreateOpEngine()->RegisterOpsInterfaces(6, IOpsRender);

