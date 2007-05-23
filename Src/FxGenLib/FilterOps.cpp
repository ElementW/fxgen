//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		FilterOps.cpp
//! \brief	Filter Operators
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
#include "FilterOps.h"

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
	m_pcvarsBloc = AddVarsBloc(3, blocdescBlurOp, 1);
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
	sdword w = pSrc->GetWidth();
	sdword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

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
	m_pcvarsBloc = AddVarsBloc(4, blocdescColorsOp, 1);
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

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

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
	m_pcvarsBloc = AddVarsBloc(8, blocdescLightOp, 1);

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

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

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
	m_pcvarsBloc = AddVarsBloc(1, blocdescNormalsOp, 1);

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

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);


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

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

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


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NThresholdOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NThresholdOp, NOperator);

static NVarsBlocDesc blocdescThresholdOp[] =
{
	VAR(eubyte,		true, "Threshold",	"128",	"NUbyteProp")	//0
	VAR(eubyte,		true, "Ratio",	"128",		"NUbyteProp")	//1
	VAR(eubyte,		false, "Mode",				"0,[Expand Downwards, Expand Upwards, Compress Below, Compress Above]", "NUbyteComboProp")	//2
};

NThresholdOp::NThresholdOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(3, blocdescThresholdOp, 1);
}

static inline ubyte expandIntensity(ubyte intensity, float ratio, ubyte threshold)
{
  if (intensity<threshold)
  {
    sword newIntensity = threshold - (threshold-intensity)*ratio;
    return newIntensity<0 ? 0 : newIntensity;
  } else {
    return intensity;
  }
}

static inline ubyte compressIntensity(ubyte intensity, float ratio, ubyte threshold)
{
  if (intensity<threshold)
  {
    sword newIntensity = threshold - (threshold-intensity)/ratio;
    return newIntensity<0 ? 0 : newIntensity;
  } else {
    return intensity;
  }
}

udword NThresholdOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	// Init
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst	= (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	RGBA* pPxSrc = pSrc->GetPixels();
	RGBA* pPxDst = pDst->GetPixels();

	//Get Variables Values
	ubyte byThreshold;
	ubyte byRatio;
	ubyte byMode;
	m_pcvarsBloc->GetValue(0, _ftime, byThreshold);
	m_pcvarsBloc->GetValue(1, _ftime, byRatio);
	m_pcvarsBloc->GetValue(2, _ftime, byMode);
        //if (byRatio<1) byRatio = 1;
        if (byMode == 0)
        {
          float ratio = 1+byRatio*0.1f;
	  for (udword y=0; y<h; y++)
	  {
		  for (udword x=0; x<w; x++)
		  {
			pPxDst->r = expandIntensity(pPxSrc->r, ratio, byThreshold);
			pPxDst->g = expandIntensity(pPxSrc->g, ratio, byThreshold);
			pPxDst->b = expandIntensity(pPxSrc->b, ratio, byThreshold);
			pPxDst->a = 255;

			  pPxSrc++;
			  pPxDst++;
		  }
	  }
        } else if (byMode == 1) {
          float ratio = 1+byRatio*0.1f;
	  for (udword y=0; y<h; y++)
	  {
		  for (udword x=0; x<w; x++)
		  {
			pPxDst->r = 255-expandIntensity(255-pPxSrc->r, ratio, 255-byThreshold);
			pPxDst->g = 255-expandIntensity(255-pPxSrc->g, ratio, 255-byThreshold);
			pPxDst->b = 255-expandIntensity(255-pPxSrc->b, ratio, 255-byThreshold);
			pPxDst->a = 255;

			  pPxSrc++;
			  pPxDst++;
		  }
	  }
        } else if (byMode == 2) {
          float ratio = 1+byRatio*0.05f;
	  for (udword y=0; y<h; y++)
	  {
		  for (udword x=0; x<w; x++)
		  {
			pPxDst->r = compressIntensity(pPxSrc->r, ratio, byThreshold);
			pPxDst->g = compressIntensity(pPxSrc->g, ratio, byThreshold);
			pPxDst->b = compressIntensity(pPxSrc->b, ratio, byThreshold);
			pPxDst->a = 255;

			  pPxSrc++;
			  pPxDst++;
		  }
	  }
        } else if (byMode == 3) {
          float ratio = 1+byRatio*0.05f;
	  for (udword y=0; y<h; y++)
	  {
		  for (udword x=0; x<w; x++)
		  {
			pPxDst->r = 255-compressIntensity(255-pPxSrc->r, ratio, 255-byThreshold);
			pPxDst->g = 255-compressIntensity(255-pPxSrc->g, ratio, 255-byThreshold);
			pPxDst->b = 255-compressIntensity(255-pPxSrc->b, ratio, 255-byThreshold);
			pPxDst->a = 255;

			  pPxSrc++;
			  pPxDst++;
		  }
	  }
        }

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NGaussianOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NGaussianOp, NOperator);

static NVarsBlocDesc blocdescGaussianOp[] =
{
	VAR(eubyte,		true, "Size",		"16",		"NUbyteProp")	//0
	VAR(eubyte,		true, "Amplify",	"16",		"NUbyteProp")	//1
};

NGaussianOp::NGaussianOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(2, blocdescGaussianOp, 1);
}

udword NGaussianOp::Process(float _ftime, NOperator** _pOpsInts)
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
	ubyte bySize, byAmplify;
	m_pcvarsBloc->GetValue(0, _ftime, bySize);
	m_pcvarsBloc->GetValue(1, _ftime, byAmplify);

	//Radius
	float radiusW= (float)bySize / 2.0f;
	float radiusH= (float)bySize /2.0f;

	//Amplify
	float amplify= (float)byAmplify;
	sdword amp = sdword(floor(amplify*16.0f));

        if (bySize < 2)
        {
          // No blur, so just copy
	  CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w*h*sizeof(RGBA));
        } else {
	  /////////////////////////////////////////////////////////
	  // Allocate an intermediate buffer
	  RGBA* pPxInter = (RGBA*)NMemAlloc(w*h*sizeof(RGBA));    //###TOFIX### ?

	  sdword bw = (sdword) (floor(radiusW)*2);
	  sdword bh = (sdword) (floor(radiusH)*2);

          for (int i=0; i<3; ++i)
          {
	          /////////////////////////////////////////////////////////
	          // Blur Horizontal
		  for(sdword y=0;y<h;y++)
		  {
			  RGBA* pPxSrc;
			  RGBA* pAccu;
			  RGBA* pPxDst	= pPxInter + (y*w);

                          if (i == 0)
                          {
                            pPxSrc = pSrc->GetPixels() + (y*w);
                          } else {
                            pPxSrc = pDst->GetPixels() + (y*w);
                          }

                          pAccu = pPxSrc;

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

	          /////////////////////////////////////////////////////////
	          // Blur Vertical

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
        }

	return 0;
}

