//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		FilterOps.cpp
//! \brief	Filter Operators
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
#include "pch.h"
#include "FilterOps.h"
#include "RectangularArray.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NBlurOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NBlurOp, NOperator);

static NMapVarsBlocDesc mapblocdescBlurOp[] =
{
	MAP(1,	eubyte,		"0",		""	)	//V1 => 0-Width
	MAP(1,	eubyte,		"1",		""	)	//V1 => 1-Height
	MAP(1,	eubyte,		"2",		""	)	//V1 => 2-Amplify
};

static NMapVarsBlocDesc mapblocdescBlurOp2[] =
{
	MAP(2,	eubyte,		"0",		"*0.0039215"	)	//V1 => 0-Width
	MAP(2,	eubyte,		"1",		"*0.0039215"	)	//V1 => 1-Height
	MAP(2,	eubyte,		"2",		""	)	//V1 => 2-Amplify
	MAP(2,	eubyte,		"3",		""	)	//V1 => 3-Type
};

static NVarsBlocDesc blocdescBlurOp[] =
{
	VAR(efloat,		true, "Width",		"0.01",		"NCFloatProp")	//0
	VAR(efloat,		true, "Height",		"0.01",		"NCFloatProp")	//1
	VAR(eubyte,		true, "Amplify",	"16",		"NUbyteProp")	//2
	VAR(eubyte,		false, "Type",	"0,[Box,Gaussian]",	"NUbyteComboProp")	//3
};

NBlurOp::NBlurOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescBlurOp, 3);
	m_pcvarsBloc->SetMapVarBlocDesc(3, mapblocdescBlurOp);
	m_pcvarsBloc->SetMapVarBlocDesc(4, mapblocdescBlurOp2); // can they work together?
}

udword NBlurOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;
	sdword w = pSrc->GetWidth();
	sdword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	ubyte byAmplify, byType;
	float fWidth, fHeight;
	m_pcvarsBloc->GetValue(0, _ftime, fWidth);
	m_pcvarsBloc->GetValue(1, _ftime, fHeight);
	m_pcvarsBloc->GetValue(2, _ftime, byAmplify);
	m_pcvarsBloc->GetValue(3, _ftime, byType);

	// Do three passes if gaussian...
	// Don't change the number of passes if you don't know what you're doing :)
	ubyte byPasses = byType == 1 ? 3 : 1;

	//Radius: 0->0, 2->1, 255->127.5d
	float radiusW= (float)fWidth * 127.5f * _fDetailFactor;
	float radiusH= (float)fHeight * 127.5f * _fDetailFactor;

	//Amplify
	float amplify= (float)byAmplify;
	float amp = powf(floor(amplify*16.0f)/256.0f, 1.0f/byPasses)*256.0f;

	sdword bw = (sdword) (floor(radiusW)*2+1);
	sdword bh = (sdword) (floor(radiusH)*2+1);
	float wamp = amp / bw / 256;
	float hamp = amp / bh / 256;


	if (bw == 0 && bh == 0)
	{
		memcpy(pDst->GetPixels(), pSrc->GetPixels(), w*h*sizeof(RGBA));
		return 0;
	}

	// Allocate a temporary buffer if needed
	RGBA* pPxInter = null;
	if (byPasses > 0 || (bw > 0 && bh > 0))
	{
		pPxInter = (RGBA*)NMemAlloc(w*h*sizeof(RGBA));
	}

	/////////////////////////////////////////////////////////
	// Blur Horizontal

        for (int i=0; i<byPasses; ++i)
        {
          if (bw>0)
          {
            for(sdword y=0;y<h;y++)
            {
              // Make sure to use source, intermediate and destination in a pattern to avoid unnecessary copies
              RGBA* pPxSrc;
              if (i == 0)
              {
                pPxSrc = pSrc->GetPixels() + (y*w);
              } else {
                if (bh == 0)
                {
                  if (i % 2 == byPasses % 2)
                    pPxSrc = pDst->GetPixels() + (y*w);
                  else
                    pPxSrc = pPxInter + (y*w);
                } else {
                  pPxSrc = pDst->GetPixels() + (y*w);
                }
              }
              RGBA* pAccu		= pPxSrc;
              RGBA* pPxDst;
              if (bh == 0)
              {
                if (i % 2 == byPasses % 2)
                  pPxDst = pPxInter + (y*w);
                else
                  pPxDst = pDst->GetPixels() + (y*w);
              } else {
                pPxDst = pPxInter + (y*w);
              }

              // Accumulation precalc
              sdword x = 0;
              RGBAI sum;
              memset(&sum,0, sizeof(RGBAI));
              while (x<bw)
              {
                sum.r+=(sdword)pAccu->r;
                sum.g+=(sdword)pAccu->g;
                sum.b+=(sdword)pAccu->b;
                sum.a+=(sdword)pAccu->a;
                pAccu++;
                ++x;
              }

              pAccu = pPxSrc;

              // Blur
              x=0;
              while (x<w)
              {
                sdword r = (sum.r*wamp);
                sdword g = (sum.g*wamp);
                sdword b = (sum.b*wamp);
                sdword a = (sum.a*wamp);

                RGBA& px = pPxDst[(x+bw/2)%w];
                px.r= (ubyte) ((r<255)?r:255);
                px.g= (ubyte) ((g<255)?g:255);
                px.b= (ubyte) ((b<255)?b:255);
                px.a= (ubyte) ((a<255)?a:255);

                sum.r-=(sdword)pPxSrc->r;
                sum.g-=(sdword)pPxSrc->g;
                sum.b-=(sdword)pPxSrc->b;
				sum.a-=(sdword)pPxSrc->a;
                pPxSrc++;

                RGBA& ac = pAccu[(x+bw)%w];
                sum.r+=(sdword)ac.r;
                sum.g+=(sdword)ac.g;
                sum.b+=(sdword)ac.b;
                sum.a+=(sdword)ac.a;

                ++x;
              }
            }
          }

          /////////////////////////////////////////////////////////
          // Blur Vertical

          if (bh>0)
          {

            for(sdword x=0;x<w;x++)
            {
              // Make sure to use source, intermediate and destination in a pattern to avoid unnecessary copies
              RGBA* pPxSrc;
              if (bw == 0)
              {
                if (i == 0)
                {
                  pPxSrc = pSrc->GetPixels() + x;
                } else {
                  if (i % 2 == byPasses % 2)
                    pPxSrc = pDst->GetPixels() + x;
                  else
                    pPxSrc = pPxInter + x;
                }
              } else {
                pPxSrc = pPxInter + x;
              }
              RGBA* pAccu		= pPxSrc;
              RGBA* pPxDst;
              if (bw == 0)
              {
                if (i % 2 == byPasses % 2)
                  pPxDst = pPxInter + x;
                else
                  pPxDst = pDst->GetPixels() + x;
              } else {
                pPxDst = pDst->GetPixels() + x;
              }

              // Accumulation Precalc
              sdword y = 0;
              RGBAI sum;
              memset(&sum, 0, sizeof(RGBAI));
              while (y<bh)
              {
                sum.r+=(sdword)pAccu->r;
                sum.g+=(sdword)pAccu->g;
                sum.b+=(sdword)pAccu->b;
                sum.a+=(sdword)pAccu->a;
                pAccu+=w;
                ++y;
              }

              pAccu = pPxSrc;

              // Blur
              y=0;
              while (y<h)
              {
                sdword r = (sum.r*hamp);
                sdword g = (sum.g*hamp);
                sdword b = (sum.b*hamp);
                sdword a = (sum.a*hamp);

                RGBA& px = pPxDst[((y+bh/2)%h)*w];
                px.r= (ubyte) ((r<255)?r:255);
                px.g= (ubyte) ((g<255)?g:255);
                px.b= (ubyte) ((b<255)?b:255);
                px.a= (ubyte) ((a<255)?a:255);

                sum.r-=(sdword)pPxSrc->r;
                sum.g-=(sdword)pPxSrc->g;
                sum.b-=(sdword)pPxSrc->b;
				sum.a-=(sdword)pPxSrc->a;
                pPxSrc+=w;

                RGBA& ac = pAccu[((y+bh)%h)*w];
                sum.r+=(sdword)ac.r;
                sum.g+=(sdword)ac.g;
                sum.b+=(sdword)ac.b;
                sum.a+=(sdword)ac.a;

                ++y;
              }
            }
          }
        }

	if (pPxInter)		NMemFree(pPxInter);		//###TOFIX###

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

static NMapVarsBlocDesc mapblocdescColorsOp[] =
{
	MAP(1,	eudword,		"0",		""	)	//V1 => 0-Color Base
	MAP(1,	eudword,		"1",		""	)	//V1 => 1-Color Percent
	MAP(1,	eubyte,			"2",		""	)	//V1 => 2-Brithness
	MAP(1,	eubyte,			"3",		""	)	//V1 => 3-Contrast
};

static NVarsBlocDesc blocdescColorsOp[] =
{
	//eubyte,		false,	"Mode",				"0,[RGB,HLS]",	"NUbyteComboProp",	//0
	VAR(eudword,	true,		"Color Base",			"0",						"NColorProp")				//0
	VAR(eudword,	true,		"Color Percent",	"-1",						"NColorProp")				//1
	VAR(eubyte,		true,		"Brithness",			"127",					"NUbyteProp")				//2
	VAR(eubyte,		true,		"Contrast",				"127",					"NUbyteProp")				//3
	VAR(eubyte,		true,		"Alpha",				"127",					"NUbyteProp")				//3
};


NColorsOp::NColorsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(5, blocdescColorsOp, 2);
	m_pcvarsBloc->SetMapVarBlocDesc(4, mapblocdescColorsOp);
}

udword NColorsOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	ubyte byMode=1;
	RGBA ColorBase, ColorPer;
	ubyte byBrihtness, byContrast, byAlpha;
	//m_pcvarsBloc->GetValue(0, _ftime, byMode);
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)ColorBase);
	m_pcvarsBloc->GetValue(1, _ftime, (udword&)ColorPer);
	m_pcvarsBloc->GetValue(2, _ftime, byBrihtness);
	m_pcvarsBloc->GetValue(3, _ftime, byContrast);
	m_pcvarsBloc->GetValue(4, _ftime, byAlpha);

	//Process operator
	sdword	brithness = (((sdword)byBrihtness)*2) - 256;	//-255 <> +255
	sdword	contrast	= (((sdword)byContrast));						//0 <> 255

	float fconstrast = (float)contrast/128.0f;
//	if (contrast>64)
	fconstrast = fconstrast*fconstrast*fconstrast;
	contrast=(sdword)(fconstrast*256.0f);

	ubyte minalpha = (byAlpha >=127) ? (byAlpha - 127) * 2.f - (byAlpha - 127) / 128.f : 0;
	ubyte maxalpha = (byAlpha <=127) ? byAlpha * 2.f + byAlpha / 127.f : 255;
	float alphamult = (maxalpha - minalpha) / 255.;

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
			pPxDst->a = pPxSrc->a * alphamult + minalpha;
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

udword NLightOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Two inputs (texture, normal)
	if (m_byInputs!=2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	//Get input Texture and Normal
	NBitmap* pSrc	= (NBitmap*)(*_pOpsInts)->m_pObj;
	_pOpsInts++;
	NBitmap* pNorm = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst	= (NBitmap*)m_pObj;


	sdword w = pSrc->GetWidth();
	sdword h = pSrc->GetHeight();

	pDst->SetSize( w, h );
	pNorm->SetSize( w, h );

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

udword NNormalsOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

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

	float fAmp = (float)byAmp*_fDetailFactor / 64.0f; //[0<->4]

	//////////////////////////////////////////
	// Creation des normales
	RGBA* pcurSour = pSrc->GetPixels();
	RGBA* pcurNorm = pDst->GetPixels();

	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			size_t xp = (x-1) % w;
			size_t xn = (x+1) % w;
			size_t yp = ((y-1) % h)*w;
			size_t yn = ((y+1) % h)*w;
			size_t yc = y * w;
			//Y Sobel filter
			float fPix = (float)pcurSour[xp+yn].r;
			float dY  = fPix * -1.0f;

			fPix = (float)pcurSour[x+yn].r;
			dY+= fPix * -2.0f;

			fPix = (float)pcurSour[xn+yn].r;
			dY+= fPix * -1.0f;

			fPix = (float)pcurSour[xp+yp].r;
			dY+= fPix * 1.0f;

			fPix = (float)pcurSour[x+yp].r;
			dY+= fPix * 2.0f;

			fPix = (float)pcurSour[xn+yp].r;
			dY+= fPix * 1.0f;

			//X Sobel filter
			fPix = (float)pcurSour[xp+yp].r;
			float dX  = fPix * -1.0f;

			fPix = (float)pcurSour[xp+yc].r;
			dX+= fPix * -2.0f;

			fPix = (float)pcurSour[xp+yn].r;
			dX+= fPix * -1.0f;

			fPix = (float)pcurSour[xn+yp].r;
			dX+= fPix * 1.0f;

			fPix = (float)pcurSour[xn+yc].r;
			dX+= fPix * 2.0f;

			fPix = (float)pcurSour[xn+yn].r;
			dX+= fPix * 1.0f;


			// Compute the cross product of the two vectors
			vec3 norm;
			norm.x = -dX*fAmp/ 255.0f;
			norm.y = -dY*fAmp/ 255.0f;
			norm.z = 1.0f;

			// Normalize
			norm.normalize();

			// Store
			pcurNorm->x = (ubyte) ((norm.x+1.0f) / 2.0f * 255.0f);	//[-1.0f->1.0f]	[0 -> 255]
			pcurNorm->y = (ubyte) ((norm.y+1.0f) / 2.0f * 255.0f);	//[-1.0f->1.0f]	[0 -> 255]
			pcurNorm->z = (ubyte) ((norm.z+1.0f) / 2.0f * 255.0f);	//[-1.0f->1.0f]	[0 -> 255]
			pcurNorm->a = pcurSour[x+y*w].a;

			pcurNorm++;
		}
	}

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NAbnormalsOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NAbnormalsOp, NOperator);

static NVarsBlocDesc blocdescAbnormalsOp[] =
{
	VAR(eubyte,	false, "Rotation",	"", "")	//0
	VAR(efloat,	true, "w",	"0.0", "NCFloatProp")	//1 "1.0" is full angle
	VAR(efloat,	true, "x",	"0.0", "NFloatProp")	//2
	VAR(efloat,	true, "y",	"0.0", "NFloatProp")	//3
	VAR(efloat,	true, "z",	"1.0", "NFloatProp")	//4
	VAR(eubyte,	false, "Options",	"", "")	//5
	VAR(eubyte,	true, "Sensitivity",	"127", "NUbyteProp")	//6
	VAR(eubyte,	true, "Compensation",	"0,[Normal,Height,Quaternion]", "NUbyteComboProp")	//7
	VAR(eubyte,	true, "Mirror",	"0,[None,X : YZ,Y : XZ,X+Y : Z]", "NUbyteComboProp")	//8
};


NAbnormalsOp::NAbnormalsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(9, blocdescAbnormalsOp, 1);

}

udword NAbnormalsOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//One or two inputs
	if (m_byInputs!=1 && m_byInputs!=2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	//////////////////////////////////////////
	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword width = pSrc->GetWidth();
	udword height = pSrc->GetHeight();
	pDst->SetSize(width,height);

	/////////////////////////////////////////
	//Get Variables Values
	float sensitivity, w,x,y,z;
	ubyte sens, comp, mirror;
	m_pcvarsBloc->GetValue(1, _ftime, w);
	m_pcvarsBloc->GetValue(2, _ftime, x);
	m_pcvarsBloc->GetValue(3, _ftime, y);
	m_pcvarsBloc->GetValue(4, _ftime, z);
	m_pcvarsBloc->GetValue(6, _ftime, sens);
	m_pcvarsBloc->GetValue(7, _ftime, comp);
	m_pcvarsBloc->GetValue(8, _ftime, mirror);
	quat rotation(2 * nv_pi * w, vec3(x, y, z));
	quat rotation0 = rotation;
	sensitivity = sens / 255.f;

	//////////////////////////////////////////
	// Process
	RGBAArray pcurSrc(pSrc->GetPixels(), width);
	RGBAArray pcurDst(pDst->GetPixels(), width);
	RGBAArray pcurQuat;

	if(m_byInputs==2)
	{
		_pOpsInts++;
		NBitmap* pQuat = (NBitmap*)(*_pOpsInts)->m_pObj;

		if(pQuat->GetWidth() < width || pQuat->GetHeight() < height)
			return (udword)-1; // insufficient size

		pcurQuat = RGBAArray(pQuat->GetPixels(), width);
	}


	for (udword y=0; y<height; y++)
		for (udword x=0; x<width; x++)
		{
			quat v(
			(pcurSrc(x,y).r-127.5f)/127.5f,
			(pcurSrc(x,y).g-127.5f)/127.5f,
			(pcurSrc(x,y).b-127.5f)/127.5f, 0);

			if(pcurQuat.width)
			{
				if(comp == 0) // normals
				{
					const RGBA& curQuat = pcurQuat(x,y);
					quat offset(curQuat.r-127.5f,curQuat.g-127.5f,curQuat.b-127.5f,0);
					v *= (1 - sensitivity);
					v += offset * (sensitivity / 127.5f);
				}

				else if(comp == 1) // height
				{
					float sum = pcurQuat(x,y).r+pcurQuat(x,y).g+pcurQuat(x,y).b;
					quat current(2 * nv_pi * sum / 765.f * sensitivity,	vec3(0,0,1));

					rotation = current * rotation0;
				}

				else if(comp == 2) // quaternions
				{
					vec3 q(pcurQuat(x,y).r-127.5f,pcurQuat(x,y).g-127.5f,pcurQuat(x,y).b-127.5f);
					quat current(2 / 255.f * nv_pi * pcurQuat(x,y).a * sensitivity, q);

					rotation = current * rotation0;
				}
			}

			v = rotation * v * rotation.Inverse();
			v.Normalize();

			pcurDst(x,y).r = v.x * 127.5f + 127.5f;
			pcurDst(x,y).g = v.y * 127.5f + 127.5f;
			pcurDst(x,y).b = v.z * 127.5f + 127.5f;
			pcurDst(x,y).a = pcurSrc(x,y).a;

			// mirroring - for broken normal maps
			if(mirror == 1 || mirror == 3)
					pcurDst(x,y).r = 255 - pcurDst(x,y).r;

			if(mirror == 2 || mirror == 3)
					pcurDst(x,y).g = 255 - pcurDst(x,y).g;
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
IMPLEMENT_CLASS(NSlopeMagnitudeOp, NOperator);

static NVarsBlocDesc blocdescNSlopeMagnitudeOp[] =
{
	VAR(eudword,	true, "Amplify",	"64", "NUbyteProp")	//0
};


NSlopeMagnitudeOp::NSlopeMagnitudeOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescNSlopeMagnitudeOp, 1);

}

udword NSlopeMagnitudeOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

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
			norm.x = -dX*4;
			norm.y = -dY*4;
			norm.z = 1.0f;

			norm.normalize();

			// Normalize
			float nm = sqrtf(dX*dX + dY*dY);

			//nm = ;
			// now compute the dot product of the norm vector with (0,0,1)

			norm.x = nm;
			norm.y = nm;
			norm.z = nm;


			//norm.normalize();

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

udword NInvertOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

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
			pPxDst->a = pPxSrc->a;

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

udword NThresholdOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

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
//							NAlphaOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NAlphaOp, NOperator);

NAlphaOp::NAlphaOp(){}

udword NAlphaOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	RGBA*	pPxSrc = pSrc->GetPixels();
	RGBA*	pPxDst = pDst->GetPixels();

	//Process
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			float r = (float)pPxSrc->r * 0.299f;
			float g = (float)pPxSrc->g * 0.587f;
			float b = (float)pPxSrc->b * 0.114f;

			pPxDst->a = (ubyte) (( (udword)pPxDst->a + (udword)(r+g+b) )>>1);
			pPxDst->r=pPxDst->g=pPxDst->b=pPxDst->a;

			pPxDst++;
			pPxSrc++;
		}
	}

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NSegmentOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NSegmentOp, NOperator);

static NVarsBlocDesc blocdescSegmentOp[] =
{
	VAR(eubyte,		true, "Threshold",	"128",	"NUbyteProp")	//0
};

NSegmentOp::NSegmentOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescSegmentOp, 1);
}

udword NSegmentOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Two inputs
	if (m_byInputs!=2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	// Init
	NBitmap* pSrc1 = (NBitmap*)(*_pOpsInts)->m_pObj;
	_pOpsInts++;
	NBitmap* pSrc2 = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst	= (NBitmap*)m_pObj;

	udword w = pSrc1->GetWidth();
	udword h = pSrc1->GetHeight();
	if (pSrc2->GetWidth()!=w || pSrc2->GetHeight()!=h)
		return (udword)-1;
	pDst->SetSize(w,h);

	RGBA* pPxSrc1 = pSrc1->GetPixels();
	RGBA* pPxSrc2 = pSrc2->GetPixels();
	RGBA* pPxDst = pDst->GetPixels();

	//Get Variables Values
	ubyte byThreshold;
	m_pcvarsBloc->GetValue(0, _ftime, byThreshold);

	struct Coord
	{
		uword x;
		uword y;
		Coord(uword x, uword y) : x(x), y(y) {}
	};
	ubyte* pCoverage = (ubyte*)NMemAlloc(w*h);
	Coord* pStack = (Coord*)NMemAlloc(w*h*sizeof(Coord)*4);

	memset(pCoverage, 0, w*h);

	udword index = 0;
	udword stackPtr = 0;
	//Process
	for (udword y=0; y<h; y++)
	{
		for (udword x=0; x<w; x++)
		{
			if (pPxSrc1[index].r+pPxSrc1[index].g+pPxSrc1[index].b > byThreshold*3)
			{
				pStack[stackPtr++] = Coord(x, y);
			}

			RGBA color = pPxSrc2[index];

			while (stackPtr>0)
			{
				Coord current = pStack[--stackPtr];
				udword currentIndex = current.x + current.y*w;
				if (pCoverage[currentIndex] != 0)
				{
					continue;
				}

				if (pPxSrc1[currentIndex].r+pPxSrc1[currentIndex].g+pPxSrc1[currentIndex].b > byThreshold*3)
				{
					pStack[stackPtr++] = Coord(current.x, (current.y+h-1)%h); // Up
					pStack[stackPtr++] = Coord(current.x, (current.y+1)%h); // Down
					pStack[stackPtr++] = Coord((current.x+1)%w, current.y); // Right
					pStack[stackPtr++] = Coord((current.x+w-1)%w, current.y); // Left

					pCoverage[currentIndex] = 1;
					pPxDst[currentIndex] = color;
				}
			}

			if (pCoverage[index] == 0)
			{
				pPxDst[index].r = 0;
				pPxDst[index].g = 0;
				pPxDst[index].b = 0;
				pPxDst[index].a = 255;
			}

			index++;
		}
	}

	NMemFree(pCoverage);
	NMemFree(pStack);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NDilateOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NDilateOp, NOperator);


static NVarsBlocDesc blocdescDilateOp[] =
{
	VAR(eubyte,		true, "Iterations",	"10",	"NUbyteProp")	//0
};

NDilateOp::NDilateOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescDilateOp, 1);
}

udword NDilateOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	// Init
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	ubyte byIterations;
	m_pcvarsBloc->GetValue(0, _ftime, byIterations);

	RGBA* pPxInter = (RGBA*)NMemAlloc(w*h*sizeof(RGBA));

	for (sdword i=0; i<byIterations+1; ++i)
	{
		udword index = 0;
		RGBA* pPxSrc;
		RGBA* pPxDst;

		if (i == 0)
		{
			pPxSrc = pSrc->GetPixels();
		} else
		{
			pPxSrc = (i%2)!=(byIterations%2) ? pDst->GetPixels() : pPxInter;
		}

		pPxDst = (i%2)==(byIterations%2) ? pDst->GetPixels() : pPxInter;

		//Process
		for (udword y=0; y<h; y++)
		{
			for (udword x=0; x<w; x++)
			{
				sdword sum = -1;;
				pPxDst[index] = pPxSrc[index];

				udword neighbourIndex;
				for (sdword v=-1; v<2; ++v)
				{
					for (sdword u=-1; u<2; ++u)
					{
						neighbourIndex = ((x+w+u)%w) + ((y+h+v)%h)*w;
						if (pPxSrc[neighbourIndex].r + pPxSrc[neighbourIndex].g + pPxSrc[neighbourIndex].b > sum)
						{
							sum = pPxSrc[neighbourIndex].r + pPxSrc[neighbourIndex].g + pPxSrc[neighbourIndex].b;
							pPxDst[index] = pPxSrc[neighbourIndex];
						}
					}
				}

				index++;
			}
		}
	}

	NMemFree(pPxInter);


	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NAlphaMaskOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NAlphaMaskOp, NOperator);


static NVarsBlocDesc blocdescAlphaMaskOp[] =
{
	VAR(eubyte,		true, "Color Alpha Mask",			"0,[0 (Off), 1 (On)]",	"NUbyteComboProp")	//7
};


NAlphaMaskOp::NAlphaMaskOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescAlphaMaskOp, 1);
}

udword NAlphaMaskOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//One or two Inputs
	if (m_byInputs !=1 && m_byInputs != 2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	// Init
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;
	NBitmap* pAlpha = null;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	ubyte colormask;
	m_pcvarsBloc->GetValue(0, _ftime, colormask);

	RGBA* pPxSrc = pSrc->GetPixels();
	RGBA* pPxDst = pDst->GetPixels();
	RGBA* pPxAlpha = null;

	if(m_byInputs == 2)
	{
		pAlpha = (NBitmap*)(*++_pOpsInts)->m_pObj;

		if(pAlpha->GetWidth() < w || pAlpha->GetHeight() < h)
			return (udword)-1; // insufficient alpha channel size

		pPxAlpha = pAlpha->GetPixels();
	}

	// Process
	for (udword y=0; y<h; y++)
		for (udword x=0; x<w; x++)
		{
			if(pAlpha)
			{
				*pPxDst = *pPxSrc;

				if(colormask)
				{
					vec3 c1(pPxSrc->r, pPxSrc->g, pPxSrc->b);
					vec3 c2(pPxAlpha->r, pPxAlpha->g, pPxAlpha->b);

					c1.normalize();
					c2.normalize();

					static float correctness = 0;

					if(c1.norm() && c2.norm()) // else - use existing value to avoid hot-spots
						dot(correctness, c1, c2);

					pPxDst->a *= correctness;
				}
				else // monochrome mask
				{
					float alpha = (pPxAlpha->r + pPxAlpha->g + pPxAlpha->b) / 3. / 255.;
					pPxDst->a *= alpha;
				}

				pPxAlpha++;
			}
			else // extract alpha mask
			{
				pPxDst->r = pPxDst->g = pPxDst->b = pPxSrc->a;
				pPxDst->a = 255;
			}

			pPxDst++;
			pPxSrc++;
		}

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NLightOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NLightOp2, NOperator);

static NVarsBlocDesc blocdescLightOp2[] =
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


NLightOp2::NLightOp2()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(8, blocdescLightOp2, 1);

}

udword NLightOp2::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Two inputs (texture, normal)
	if (m_byInputs < 2 || m_byInputs > 4)	return (udword)-1;

	//Get input Texture
	NBitmap* pSrc	= (NBitmap*)(*_pOpsInts)->m_pObj;
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();

	_pOpsInts++;

	NBitmap* pNorm = (NBitmap*)(*_pOpsInts)->m_pObj;
	if (pNorm->GetWidth()!=w || pNorm->GetHeight()!=h)			return (udword)-1;

	_pOpsInts++;

	NBitmap* pSpec = null; // Specular color
	if (m_byInputs>2)
	{
		pSpec = (NBitmap*)(*_pOpsInts)->m_pObj;
		if (pSpec->GetWidth()!=w || pSpec->GetHeight()!=h)			return (udword)-1;
		_pOpsInts++;
	}

	NBitmap* pAmb = null; // Ambient color
	if (m_byInputs>3)
	{
		pAmb = (NBitmap*)(*_pOpsInts)->m_pObj;
		if (pAmb->GetWidth()!=w || pAmb->GetHeight()!=h)			return (udword)-1;
		_pOpsInts++;
	}

	//Set Texture Size
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);
	NBitmap* pDst	= (NBitmap*)m_pObj;
	pDst->SetSize(w,h);

	/////////////////////////////////////////
	//Get Variables Values
	RGBA Ambient, Diffuse, Specular;
	ubyte byPosX, byPosY, byPosZ;
	ubyte bySpecPower, byBumpPower;
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)Ambient);
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

        vec3 halfV = light;
        halfV.z += 1;
        halfV.normalize();

	float fSpecularPower	= 256.0f/(float)bySpecPower;
	float fBumpPower			= ((float)byBumpPower) / 200.0f;

	/////////////////////////////////////////
	// DIRECTIONAL LIGHT TYPE
        RGBA* pPxNorm = pNorm->GetPixels();
	RGBA* pPxSpec = pSpec == null ? null : pSpec->GetPixels();
	RGBA* pPxAmb = pAmb == null ? null : pAmb->GetPixels();
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
			n.z = ((float)pPxNorm->z - 127.0f)/(0.01f+fBumpPower);
			n.normalize();
			pPxNorm++;

			//compute the dot product between normal and light dir
			float fDiffDot;
			dot(fDiffDot, n, light);
			if (fDiffDot<0.0f)	fDiffDot=0.0f;

			//compute the dot product between normal and halfvector
			float fSpecDot;
			dot(fSpecDot, n, halfV);
			if (fSpecDot<0.0f)	fSpecDot=0.0f;
                        fSpecDot = powf(fSpecDot, fSpecularPower+0.000001f);

			//Add bump on normal
			//fdot*=fBumpPower;

			/*float fdotSpec=0.0;
			if (dot > 0.0) {
				//color += diffuse * NdotL;
				//halfV = normalize(halfVector);
				//NdotHV = max(dot(n,halfV),0.0);
				//color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess);
				fdotSpec = pow(fdot, 64);
			}*/

                        RGBA localAmbient;
                        if (pPxAmb != null)
                        {
                          localAmbient.r = Ambient.r + pPxAmb->r;
                          localAmbient.g = Ambient.g + pPxAmb->g;
                          localAmbient.b = Ambient.b + pPxAmb->b;
                          pPxAmb++;
                        } else {
                          localAmbient.r = Ambient.r;
                          localAmbient.g = Ambient.g;
                          localAmbient.b = Ambient.b;
                        }

                        RGBA localSpecular;
                        if (pPxSpec != null)
                        {
                          localSpecular.r = (Specular.r * pPxSpec->r) >> 8;
                          localSpecular.g = (Specular.g * pPxSpec->g) >> 8;
                          localSpecular.b = (Specular.b * pPxSpec->b) >> 8;
                          pPxSpec++;
                        } else {
                          localSpecular.r = Specular.r;
                          localSpecular.g = Specular.g;
                          localSpecular.b = Specular.b;
                        }

			// Color = ambient + dif*dot + dot^2 * spec
			sdword r	= (sdword) ((sdword(pPxSrc->r*(localAmbient.r + fDiffDot*Diffuse.r)) >> 8) + (fSpecDot*localSpecular.r));
			sdword g	= (sdword) ((sdword(pPxSrc->g*(localAmbient.g + fDiffDot*Diffuse.g)) >> 8) + (fSpecDot*localSpecular.g));
			sdword b	= (sdword) ((sdword(pPxSrc->b*(localAmbient.b + fDiffDot*Diffuse.b)) >> 8) + (fSpecDot*localSpecular.b));

			//sdword r	= pPxSrc->r + (fdot * pPxSrc->r);
			//sdword g	= pPxSrc->g + (fdot * pPxSrc->g);
			//sdword b	= pPxSrc->b + (fdot * pPxSrc->b);

			//Summ
			/*r = (pPxSrc->r + r) / 2;
			g = (pPxSrc->g + g) / 2;
			b = (pPxSrc->b + b) / 2;*/

			pPxDst->r = (ubyte) ((r<255)?r:255);
			pPxDst->g = (ubyte) ((g<255)?g:255);
			pPxDst->b = (ubyte) ((b<255)?b:255);
			pPxDst->a = pPxSrc->a;

			pPxSrc++;
			pPxDst++;
		}
	}


	return 0;
}
