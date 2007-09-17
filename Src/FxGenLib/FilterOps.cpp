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

static NMapVarsBlocDesc mapblocdescBlurOp[] =
{
	MAP(1,	eubyte,		"0",		""	)	//V1 => 0-Width
	MAP(1,	eubyte,		"1",		""	)	//V1 => 1-Height
	MAP(1,	eubyte,		"2",		""	)	//V1 => 2-Amplify
};

static NVarsBlocDesc blocdescBlurOp[] =
{
	VAR(eubyte,		true, "Width",		"4",		"NUbyteProp")	//0
	VAR(eubyte,		true, "Height",		"4",		"NUbyteProp")	//1
	VAR(eubyte,		true, "Amplify",	"16",		"NUbyteProp")	//2
	VAR(eubyte,		false, "Type",	"0,[Box,Gaussian]",	"NUbyteComboProp")	//3
};

NBlurOp::NBlurOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescBlurOp, 2);
	m_pcvarsBloc->SetMapVarBlocDesc(3, mapblocdescBlurOp);
}

udword NBlurOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
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
	ubyte byWidth, byHeight, byAmplify, byType;
	m_pcvarsBloc->GetValue(0, _ftime, byWidth);
	m_pcvarsBloc->GetValue(1, _ftime, byHeight);
	m_pcvarsBloc->GetValue(2, _ftime, byAmplify);
	m_pcvarsBloc->GetValue(3, _ftime, byType);

	// Do three passes if gaussian...
	// Don't change the number of passes if you don't know what you're doing :)
	ubyte byPasses = byType == 1 ? 3 : 1; 

	//Radius
	float radiusW= (float)byWidth / 2.0f;
	float radiusH= (float)byHeight /2.0f;

	//Amplify
	float amplify= (float)byAmplify;
	sdword amp = sdword(powf(floor(amplify*16.0f)/256.0f, 1.0f/byPasses)*256.0f);

	sdword bw = (sdword) (floor(radiusW)*2+1);
  sdword bh = (sdword) (floor(radiusH)*2+1);


	if (bw == 0 && bh == 0)
	{
		CopyMemory(pDst->GetPixels(), pSrc->GetPixels(), w*h*sizeof(RGBA));
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
              ZeroMemory(&sum, sizeof(RGBAI));
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
                sdword r = (sum.r*amp)/(bw<<8);
                sdword g = (sum.g*amp)/(bw<<8);
                sdword b = (sum.b*amp)/(bw<<8);
                sdword a = (sum.a*amp)/(bw<<8);

                pPxDst[(x+bw/2)%w].r= (ubyte) ((r<255)?r:255);
                pPxDst[(x+bw/2)%w].g= (ubyte) ((g<255)?g:255);
                pPxDst[(x+bw/2)%w].b= (ubyte) ((b<255)?b:255);
                pPxDst[(x+bw/2)%w].a= (ubyte) ((a<255)?a:255);;

                sum.r-=(sdword)pPxSrc->r;
                sum.g-=(sdword)pPxSrc->g;
                sum.b-=(sdword)pPxSrc->b;
	              sum.a-=(sdword)pPxSrc->a;
                pPxSrc++;

                sum.r+=(sdword)pAccu[(x+bw)%w].r;
                sum.g+=(sdword)pAccu[(x+bw)%w].g;
                sum.b+=(sdword)pAccu[(x+bw)%w].b;
                sum.a+=(sdword)pAccu[(x+bw)%w].a;

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
              ZeroMemory(&sum, sizeof(RGBAI));
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
                sdword r = (sum.r*amp)/(bh<<8);
                sdword g = (sum.g*amp)/(bh<<8);
                sdword b = (sum.b*amp)/(bh<<8);
                sdword a = (sum.a*amp)/(bh<<8);

                pPxDst[((y+bh/2)%h)*w].r= (ubyte) ((r<255)?r:255);
                pPxDst[((y+bh/2)%h)*w].g= (ubyte) ((g<255)?g:255);
                pPxDst[((y+bh/2)%h)*w].b= (ubyte) ((b<255)?b:255);
                pPxDst[((y+bh/2)%h)*w].a= (ubyte) ((a<255)?a:255);

                sum.r-=(sdword)pPxSrc->r;
                sum.g-=(sdword)pPxSrc->g;
                sum.b-=(sdword)pPxSrc->b;
								sum.a-=(sdword)pPxSrc->a;
                pPxSrc+=w;

                sum.r+=(sdword)pAccu[((y+bh)%h)*w].r;
                sum.g+=(sdword)pAccu[((y+bh)%h)*w].g;
                sum.b+=(sdword)pAccu[((y+bh)%h)*w].b;
                sum.a+=(sdword)pAccu[((y+bh)%h)*w].a;

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

udword NColorsOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
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
	VAR(eudword,	true, "Ambient",		"0",				"NColorProp")	//0
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
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);
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

udword NInvertOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
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

udword NThresholdOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
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
//							NAlphaOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NAlphaOp, NOperator);

NAlphaOp::NAlphaOp()
{
}

udword NAlphaOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
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
