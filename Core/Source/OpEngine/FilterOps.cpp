//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		FilterOps.cpp
//! \brief	Filter Operators
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
#include "../../Include/CoreLib.h"
#include "FilterOps.h"
#include "RectangularArray.h"
#include "Bitmap.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NBlurOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NBlurOp, NOperatorFx);

static NVarsBlocDesc blocdescBlurOp[] =
{
	VAR(efloat,		true, "Width",		"0.01",		"NFloatProp", 0.0f, 100.0f, 0.001f)	//0
	VAR(efloat,		true, "Height",		"0.01",		"NFloatProp", 0.0f, 100.0f, 0.001f)	//1
	VAR(eubyte,		true, "Amplify",	"16",		"NUbyteProp", 0.0f, 255.0f, 1.0f)	//2
	VAR(eubyte,		false, "Type",	"0,[Box,Gaussian]",	"NUbyteComboProp", 0.0f, 0.0f, 0.0f)	//3
};

NBlurOp::NBlurOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescBlurOp, 1);
}

udword NBlurOp::Process(float _ftime, SEngineState& _state)
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
	float radiusW= (float)fWidth * 127.5f * _state.fDetailFactor;
	float radiusH= (float)fHeight * 127.5f * _state.fDetailFactor;

	//Amplify
	float amplify= (float)byAmplify;
	float amp = powf(floor(amplify*16.0f)/256.0f, 1.0f/byPasses)*256.0f;

	sdword bw = (sdword) (floor(radiusW)*2+1);
	sdword bh = (sdword) (floor(radiusH)*2+1);
	float wamp = amp / bw / 256;
	float hamp = amp / bh / 256;


	if (bw == 0 && bh == 0)
	{
		memcpy(pDst->GetPixels(), pSrc->GetPixels(), w*h*sizeof(NRGBA));
		return 0;
	}

	// Allocate a temporary buffer if needed
	NRGBA* pPxInter = null;
	if (byPasses > 0 || (bw > 0 && bh > 0))
	{
		pPxInter = (NRGBA*)NMemAlloc(w*h*sizeof(NRGBA));
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
              NRGBA* pPxSrc;
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
              NRGBA* pAccu		= pPxSrc;
              NRGBA* pPxDst;
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
              NRGBAI sum(0,0,0,0);
              while (x<bw)
              {
                sum.r+=pAccu->r;
                sum.g+=pAccu->g;
                sum.b+=pAccu->b;
                sum.a+=pAccu->a;
                pAccu++;
                ++x;
              }

              pAccu = pPxSrc;

              // Blur
              x=0;
              while (x<w)
              {
                sdword r = sdword(sum.r*wamp);
                sdword g = sdword(sum.g*wamp);
                sdword b = sdword(sum.b*wamp);
                sdword a = sdword(sum.a*wamp);

                NRGBA& px = pPxDst[(x+bw/2)%w];
                px.r= (ubyte) ((r<255)?r:255);
                px.g= (ubyte) ((g<255)?g:255);
                px.b= (ubyte) ((b<255)?b:255);
                px.a= (ubyte) ((a<255)?a:255);

                sum.r-=pPxSrc->r;
                sum.g-=pPxSrc->g;
                sum.b-=pPxSrc->b;
								sum.a-=pPxSrc->a;
                pPxSrc++;

                NRGBA ac = pAccu[(x+bw)%w];
                sum.r+=ac.r;
                sum.g+=ac.g;
                sum.b+=ac.b;
                sum.a+=ac.a;

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
              NRGBA* pPxSrc;
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
              NRGBA* pAccu		= pPxSrc;
              NRGBA* pPxDst;
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
              NRGBAI sum(0,0,0,0);
              while (y<bh)
              {
                sum.r+=pAccu->r;
                sum.g+=pAccu->g;
                sum.b+=pAccu->b;
                sum.a+=pAccu->a;
                pAccu+=w;
                ++y;
              }

              pAccu = pPxSrc;

              // Blur
              y=0;
              while (y<h)
              {
                sdword r = sdword(sum.r*hamp);
                sdword g = sdword(sum.g*hamp);
                sdword b = sdword(sum.b*hamp);
                sdword a = sdword(sum.a*hamp);

                NRGBA& px = pPxDst[((y+bh/2)%h)*w];
                px.r= (ubyte) ((r<255)?r:255);
                px.g= (ubyte) ((g<255)?g:255);
                px.b= (ubyte) ((b<255)?b:255);
                px.a= (ubyte) ((a<255)?a:255);

                sum.r-=pPxSrc->r;
                sum.g-=pPxSrc->g;
                sum.b-=pPxSrc->b;
				sum.a-=pPxSrc->a;
                pPxSrc+=w;

                NRGBA ac = pAccu[((y+bh)%h)*w];
                sum.r+=ac.r;
                sum.g+=ac.g;
                sum.b+=ac.b;
                sum.a+=ac.a;

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
FIMPLEMENT_CLASS(NColorsOp, NOperatorFx);

static NVarsBlocDesc blocdescColorsOp[] =
{
	VAR(eudword,	true,		"Color Base",			"0",					"NColorProp", 0.0f, 0.0f, 0.0f)				//0
	VAR(eudword,	true,		"Color Percent",		"-1",					"NColorProp", 0.0f, 0.0f, 0.0f)				//1
	VAR(eubyte,		true,		"Brithness",			"127",					"NUbyteProp", 0.0f, 255.0f, 1.0f)				//2
	VAR(eubyte,		true,		"Contrast",				"127",					"NUbyteProp", 0.0f, 255.0f, 1.0f)				//3
	VAR(eubyte,		true,		"Saturation",			"127",					"NUbyteProp", 0.0f, 255.0f, 1.0f)				//4
	VAR(eubyte,		true,		"Alpha",				"127",					"NUbyteProp", 0.0f, 255.0f, 1.0f)				//5
};


NColorsOp::NColorsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescColorsOp, 1);
}

udword NColorsOp::Process(float _ftime, SEngineState& _state)
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
//	ubyte byMode=1;
	NRGBA ColorBase, ColorPer;
	ubyte byBrihtness, byContrast, bySaturation, byAlpha;
	//m_pcvarsBloc->GetValue(0, _ftime, byMode);
	m_pcvarsBloc->GetValue(0, _ftime, (udword&)ColorBase);
	m_pcvarsBloc->GetValue(1, _ftime, (udword&)ColorPer);
	m_pcvarsBloc->GetValue(2, _ftime, byBrihtness);
	m_pcvarsBloc->GetValue(3, _ftime, byContrast);
	m_pcvarsBloc->GetValue(4, _ftime, bySaturation);
	m_pcvarsBloc->GetValue(5, _ftime, byAlpha);

	//Process operator
	sdword	brithness = (((sdword)byBrihtness)*2) - 256;	//-255 <> +255
	sdword	contrast	= (((sdword)byContrast));						//0 <> 255

	float fconstrast = (float)contrast/128.0f;
//	if (contrast>64)
	fconstrast = fconstrast*fconstrast*fconstrast;
	contrast=(sdword)(fconstrast*256.0f);

	ubyte minalpha = (byAlpha >=127) ? ubyte((byAlpha - 127) * 2.f - (byAlpha - 127) / 128.f) : 0;
	ubyte maxalpha = (byAlpha <=127) ? ubyte(byAlpha * 2.f + byAlpha / 127.f) : 255;
	float alphamult = (maxalpha - minalpha) / 255.f;

	NRGBA* pPxSrc = pSrc->GetPixels();
	NRGBA* pPxDst = pDst->GetPixels();

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


			//Saturation
			if(/*for efficiency*/bySaturation != 127)
			{
				sdword l = r + g + b;
				sdword u = (3 * r - l) * bySaturation / 127;
				sdword v = (3 * b - l) * bySaturation / 127;
				r = (u + l) / 3;
				g = (l - (u + v)) / 3;
				b = (v + l) / 3;
			}

			//Set pixel
			r = (r<255)?r:255;				r = (r>0)?r:0;
			g = (g<255)?g:255;				g = (g>0)?g:0;
			b = (b<255)?b:255;				b = (b>0)?b:0;

			pPxDst->r = (ubyte) r;
			pPxDst->g = (ubyte) g;
			pPxDst->b = (ubyte) b;
			pPxDst->a = ubyte(pPxSrc->a * alphamult + minalpha);
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
FIMPLEMENT_CLASS(NLightOp, NOperatorFx);

static NVarsBlocDesc blocdescLightOp[] =
{
	VAR(eudword,	true, "Ambiant",		"0",				"NColorProp", 0.0f, 0.0f, 0.0f)	//0
	VAR(eudword,	true, "Diffuse",		"8421504",	"NColorProp", 0.0f, 0.0f, 0.0f)	//1
	VAR(eudword,	true, "Specular",		"-1",				"NColorProp", 0.0f, 0.0f, 0.0f)	//2

	VAR(eubyte,		true, "PosX",				"255",			"NUbyteProp", 0.0f, 255.0f, 1.0f)	//3
	VAR(eubyte,		true, "PosY",				"255",			"NUbyteProp", 0.0f, 255.0f, 1.0f)	//4
	VAR(eubyte,		true, "PosZ",				"127",			"NUbyteProp", 0.0f, 255.0f, 1.0f)	//5

	VAR(eubyte,		true, "Specular power",		"0",			"NUbyteProp", 0.0f, 255.0f, 1.0f)	//6
	VAR(eubyte,		true, "Bump power",				"0",			"NUbyteProp", 0.0f, 255.0f, 1.0f)	//7
};


NLightOp::NLightOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(8, blocdescLightOp, 1);

}

udword NLightOp::Process(float _ftime, SEngineState& _state)
{
	//Two inputs (texture, normal)
	if (m_byInputs!=2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get input Texture and Normal
	N2DBitmap* pSrc	= (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pNorm = (N2DBitmap*)_state.apInputs[1];
	N2DBitmap* pDst	= (N2DBitmap*)m_pObj;


	sdword w = pSrc->GetWidth();
	sdword h = pSrc->GetHeight();

	pDst->SetSize( w, h );
	pNorm->SetSize( w, h );

	/////////////////////////////////////////
	//Get Variables Values
	NRGBA Ambiant, Diffuse, Specular;
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
	NRGBA* pPxNorm = pNorm->GetPixels();
	NRGBA* pPxSrc	= pSrc->GetPixels();
	NRGBA* pPxDst	= pDst->GetPixels();

	for (sdword y=0; y<h; y++)
	{
		for (sdword x=0; x<w; x++)
		{
			//Normalized normal map
			vec3 n;
			n.x = ((float)pPxNorm->x - 127.0f);
			n.y = ((float)pPxNorm->y - 127.0f);
			n.z = ((float)pPxNorm->z - 127.0f);

			n.normalize();

			//compute the dot product between normal and light dir
			float fdot, fdot2;
			dot(fdot, n, light);
			if (fdot<0.0f)	fdot=0.0f;

			//Add bump on normal
			fdot*=fBumpPower;
			fdot2 = fdot*fdot;

			/*float fdotSpec=0.0;
			if (dot > 0.0) {
				//color += diffuse * NdotL;
				//halfV = normalize(halfVector);
				//NdotHV = max(dot(n,halfV),0.0);
				//color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess);
				fdotSpec = pow(fdot, 64);
			}*/

			// Color = ambient + dif*dot + dot^2 * spec
			sdword r	= (sdword) (Ambiant.r + (fdot*Diffuse.r) + (fdot2*Specular.r*fSpecularPower));
			sdword g	= (sdword) (Ambiant.g + (fdot*Diffuse.g) + (fdot2*Specular.g*fSpecularPower));
			sdword b	= (sdword) (Ambiant.b + (fdot*Diffuse.b) + (fdot2*Specular.b*fSpecularPower));

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
FIMPLEMENT_CLASS(NNormalsOp, NOperatorFx);

static NVarsBlocDesc blocdescNormalsOp[] =
{
	VAR(eudword,	true, "Amplify",	"64", "NUbyteProp", 0.0f, 255.0f, 1.0f)	//0
};


NNormalsOp::NNormalsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescNormalsOp, 1);

}

udword NNormalsOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//////////////////////////////////////////
	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);


	/////////////////////////////////////////
	//Get Variables Values
	ubyte byAmp;
	m_pcvarsBloc->GetValue(0, _ftime, byAmp);

	float fAmp = (float)byAmp*_state.fDetailFactor / 64.0f; //[0<->4]

	//////////////////////////////////////////
	// Creation des normales
	NRGBA* pcurSour = pSrc->GetPixels();
	NRGBA* pcurNorm = pDst->GetPixels();

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
FIMPLEMENT_CLASS(NAbnormalsOp, NOperatorFx);

static NVarsBlocDesc blocdescAbnormalsOp[] =
{
	VAR(eubyte,	false, "Rotation",	"0.0", "NUbyteProp", 0.0f, 255.0f, 1.0f)	//0
	VAR(efloat,	true, "w",	"0.0", "NFloatProp", 0.0f, 1.0f, 0.001f)	//1 "1.0" is full angle
	VAR(efloat,	true, "x",	"0.0", "NFloatProp", -1.0f, 1.0f, 0.001f)	//2
	VAR(efloat,	true, "y",	"0.0", "NFloatProp", -1.0f, 1.0f, 0.001f)	//3
	VAR(efloat,	true, "z",	"1.0", "NFloatProp", -1.0f, 1.0f, 0.001f)	//4
	VAR(eubyte,	false, "Options",	"0.0", "NUbyteProp",0.0f, 255.0f, 1.0f)	//5
	VAR(eubyte,	true, "Sensitivity",	"127", "NUbyteProp",0.0f, 255.0f, 1.0f)	//6
	VAR(eubyte,	true, "Compensation",	"0,[Normal,Height,Quaternion]", "NUbyteComboProp",0.0f, 0.0f, 0.0f)	//7
	VAR(eubyte,	true, "Mirror",	"0,[None,X : YZ,Y : XZ,X+Y : Z]", "NUbyteComboProp",0.0f, 0.0f, 0.0f)	//8
};


NAbnormalsOp::NAbnormalsOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(9, blocdescAbnormalsOp, 1);

}

udword NAbnormalsOp::Process(float _ftime, SEngineState& _state)
{
	//One or two inputs
	if (m_byInputs!=1 && m_byInputs!=2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//////////////////////////////////////////
	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

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
	NRGBAArray pcurSrc(pSrc->GetPixels(), width);
	NRGBAArray pcurDst(pDst->GetPixels(), width);
	NRGBAArray pcurQuat;

	if(m_byInputs==2)
	{
		N2DBitmap* pQuat = (N2DBitmap*)_state.apInputs[1];

		if(pQuat->GetWidth() < width || pQuat->GetHeight() < height)
			return (udword)-1; // insufficient size

		pcurQuat = NRGBAArray(pQuat->GetPixels(), width);
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
					const NRGBA& curQuat = pcurQuat(x,y);
					quat offset(curQuat.r-127.5f,curQuat.g-127.5f,curQuat.b-127.5f,0);
					v *= (1 - sensitivity);
					v += offset * (sensitivity / 127.5f);
				}

				else if(comp == 1) // height
				{
					float sum = pcurQuat(x,y).grey();
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

			pcurDst(x,y).r = ubyte(v.x * 127.5f + 127.5f);
			pcurDst(x,y).g = ubyte(v.y * 127.5f + 127.5f);
			pcurDst(x,y).b = ubyte(v.z * 127.5f + 127.5f);
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
FIMPLEMENT_CLASS(NSlopeMagnitudeOp, NOperatorFx);

static NVarsBlocDesc blocdescNSlopeMagnitudeOp[] =
{
	VAR(eudword,	true, "Amplify",	"64", "NUbyteProp",0.0f, 255.0f, 1.0f)	//0
};


NSlopeMagnitudeOp::NSlopeMagnitudeOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescNSlopeMagnitudeOp, 1);

}

udword NSlopeMagnitudeOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//////////////////////////////////////////
	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);


	/////////////////////////////////////////
	//Get Variables Values
	ubyte byAmp;
	m_pcvarsBloc->GetValue(0, _ftime, byAmp);

//	float fAmp = (float)byAmp / 64.0f; //[0<->4]

	//////////////////////////////////////////
	// Creation des normales
	NRGBA* pcurSour = pSrc->GetPixels();
	NRGBA* pcurNorm = pDst->GetPixels();

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
FIMPLEMENT_CLASS(NInvertOp, NOperatorFx);

udword NInvertOp::Process(float _ftime, SEngineState& _state)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	// Init
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst	= (N2DBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	NRGBA* pPxSrc = pSrc->GetPixels();
	NRGBA* pPxDst = pDst->GetPixels();

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
FIMPLEMENT_CLASS(NThresholdOp, NOperatorFx);

static NVarsBlocDesc blocdescThresholdOp[] =
{
	VAR(eubyte,		true, "Threshold",	"128",	"NUbyteProp",0.0f, 255.0f, 1.0f)	//0
	VAR(eubyte,		true, "Ratio",	"128",		"NUbyteProp",0.0f, 255.0f, 1.0f)	//1
	VAR(eubyte,		false, "Mode",				"0,[Expand Downwards, Expand Upwards, Compress Below, Compress Above]", "NUbyteComboProp",0.0f, 0.0f, 0.0f)	//2
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
    sword newIntensity = threshold - sword((threshold-intensity)*ratio);
    return newIntensity<0 ? 0 : newIntensity;
  } else {
    return intensity;
  }
}

static inline ubyte compressIntensity(ubyte intensity, float ratio, ubyte threshold)
{
  if (intensity<threshold)
  {
    sword newIntensity = threshold - sword((threshold-intensity)/ratio);
    return newIntensity<0 ? 0 : newIntensity;
  } else {
    return intensity;
  }
}

udword NThresholdOp::Process(float _ftime, SEngineState& _state)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	// Init
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst	= (N2DBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	NRGBA* pPxSrc = pSrc->GetPixels();
	NRGBA* pPxDst = pDst->GetPixels();

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
FIMPLEMENT_CLASS(NAlphaOp, NOperatorFx);

NAlphaOp::NAlphaOp(){}

udword NAlphaOp::Process(float _ftime, SEngineState& _state)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	NRGBA*	pPxSrc = pSrc->GetPixels();
	NRGBA*	pPxDst = pDst->GetPixels();

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
FIMPLEMENT_CLASS(NSegmentOp, NOperatorFx);

static NVarsBlocDesc blocdescSegmentOp[] =
{
	VAR(eubyte,		true, "Threshold",	"128",	"NUbyteProp",0.0f, 255.0f, 1.0f)	//0
};

NSegmentOp::NSegmentOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescSegmentOp, 1);
}

udword NSegmentOp::Process(float _ftime, SEngineState& _state)
{
	//Two inputs
	if (m_byInputs!=2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	// Init
	N2DBitmap* pSrc1 = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pSrc2 = (N2DBitmap*)_state.apInputs[1];
	N2DBitmap* pDst	= (N2DBitmap*)m_pObj;

	udword w = pSrc1->GetWidth();
	udword h = pSrc1->GetHeight();
	if (pSrc2->GetWidth()!=w || pSrc2->GetHeight()!=h)
		return (udword)-1;
	pDst->SetSize(w,h);

	NRGBA* pPxSrc1 = pSrc1->GetPixels();
	NRGBA* pPxSrc2 = pSrc2->GetPixels();
	NRGBA* pPxDst = pDst->GetPixels();

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

			NRGBA color = pPxSrc2[index];

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
FIMPLEMENT_CLASS(NDilateOp, NOperatorFx);


static NVarsBlocDesc blocdescDilateOp[] =
{
	VAR(eubyte,		true, "Iterations",	"10",	"NUbyteProp",0.0f, 255.0f, 1.0f)	//0
};

NDilateOp::NDilateOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescDilateOp, 1);
}

udword NDilateOp::Process(float _ftime, SEngineState& _state)
{
	//One input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	// Init
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	ubyte byIterations;
	m_pcvarsBloc->GetValue(0, _ftime, byIterations);

	NRGBA* pPxInter = (NRGBA*)NMemAlloc(w*h*sizeof(NRGBA));

	for (sdword i=0; i<byIterations+1; ++i)
	{
		udword index = 0;
		NRGBA* pPxSrc;
		NRGBA* pPxDst;

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
FIMPLEMENT_CLASS(NAlphaMaskOp, NOperatorFx);


static NVarsBlocDesc blocdescAlphaMaskOp[] =
{
	VAR(eubyte,		true, "Color Alpha Mask",			"0,[0 (Off), 1 (On)]",	"NUbyteComboProp",0.0f, 0.0f, 0.0f)	//7
};


NAlphaMaskOp::NAlphaMaskOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescAlphaMaskOp, 1);
}

udword NAlphaMaskOp::Process(float _ftime, SEngineState& _state)
{
	//One or two Inputs
	if (m_byInputs !=1 && m_byInputs != 2)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	// Init
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;
	N2DBitmap* pAlpha = null;

	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	ubyte colormask;
	m_pcvarsBloc->GetValue(0, _ftime, colormask);

	NRGBA* pPxSrc = pSrc->GetPixels();
	NRGBA* pPxDst = pDst->GetPixels();
	NRGBA* pPxAlpha = null;

	if(m_byInputs == 2)
	{
		pAlpha = (N2DBitmap*)_state.apInputs[1];

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

					pPxDst->a = ubyte(pPxDst->a * correctness);
				}
				else // monochrome mask
				{
					float alpha = (pPxAlpha->r + pPxAlpha->g + pPxAlpha->b) / 3.f / 255.f;
					pPxDst->a = ubyte(pPxDst->a * alpha);
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
