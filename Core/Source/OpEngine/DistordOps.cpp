//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DistordOps.cpp
//! \brief	Distord Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	        Anders Stenberg (anders.stenberg@gmail.com)
//!         Andrew Caudwell (acaudwell@gmail.com)
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
#include "DistordOps.h"
#include "RectangularArray.h"
#include "Bitmap.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRotoZoomOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NRotoZoomOp, NOperatorFx);

static NVarsBlocDesc blocdescRotoZoomOp[] =
{
	VAR(eubyte,	false, "Set Width",		"0,[0 (Default),1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp",0.0f, 0.0f, 0.0f)	//0
	VAR(eubyte,	false, "Set Height",	"0,[0 (Default),1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp",0.0f, 0.0f, 0.0f)	//1
	VAR(efloat,		true, "CenterX",	"0.5",		"NFloatProp",-1.0f, 1.0f, 0.001f)	//2
	VAR(efloat,		true, "CenterY",	"0.5",		"NFloatProp",-1.0f, 1.0f, 0.001f)	//3
	VAR(efloat,		true, "Rotate",		"0.0",		"NFloatProp",-1.0f, 1.0f, 0.001f)	//4
	VAR(efloat,		true, "ZoomX",		"1.0",		"NFloatProp",-1.0f, 1.0f, 0.001f)	//5
	VAR(efloat,		true, "ZoomY",		"1.0",		"NFloatProp",-1.0f, 1.0f, 0.001f)	//6
	VAR(eubyte,		true, "Wrap",			"1,[0 (Off), 1 (On)]",	"NUbyteComboProp",0.0f, 0.0f, 0.0f)	//7
};

NRotoZoomOp::NRotoZoomOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(8, blocdescRotoZoomOp, 1);

}

NRotoZoomOp::~NRotoZoomOp()
{
}

udword NRotoZoomOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

	udword tw = pSrc->GetWidth();
	udword th = pSrc->GetHeight();

	udword w = tw;
	udword h = th;

	//Get Variables Values
	ubyte byVal;

	m_pcvarsBloc->GetValue(0, 0, byVal);
	if (byVal!=0)
	{
		w=1<<((udword)(byVal-1));
		w=(udword) ((float)w*_state.fDetailFactor);
	}

	m_pcvarsBloc->GetValue(1, 0, byVal);
	if (byVal!=0)
	{
		h=1<<((udword)(byVal-1));
		h=(udword) ((float)h*_state.fDetailFactor);
	}

	pDst->SetSize(w, h);

	float fCenterX, fCenterY, fRotate, fZoomX, fZoomY;
	m_pcvarsBloc->GetValue(2, _ftime, fCenterX);	//-0.5 <> +0.1
	m_pcvarsBloc->GetValue(3, _ftime, fCenterY);	//-0.5 <> +0.1
	m_pcvarsBloc->GetValue(4, _ftime, fRotate);		//0.0 <> 1.0 => 0 => 2xPi
	m_pcvarsBloc->GetValue(5, _ftime, fZoomX);
	m_pcvarsBloc->GetValue(6, _ftime, fZoomY);

	//Rotate
	fRotate = fRotate * nv_two_pi;

	//Zoom
	//fZoomX=0.5f - (fZoomX/2.0f);
	fZoomX=pow(.5f,fZoomX-1);

	//fZoomY=0.5f - (fZoomY/2.0f);
	fZoomY=pow(.5f,fZoomY-1);

	//Process RotoZoom
	NRGBA* pPxlSrc = pSrc->GetPixels();
	NRGBA* pPxlDst = pDst->GetPixels();

	float fCoefX = /*((float)tw / (float)w) * */fZoomX;
	float fCoefY = /*((float)th / (float)h) * */fZoomY;

	float	c = (float) (cosf(fRotate));
	float	s = (float) (sinf(fRotate));

	float tw2 = (float)w / 2.0f;
	float th2 = (float)h / 2.0f;

	float	ys = s * -th2;
	float	yc = c * -th2;

	NRGBAI Color;

	for (udword y=0; y<h; y++)
	{
		float	u = (((c * -tw2) - ys) * fCoefX) + (fCenterX*(float)tw);		// x' = cos(x)-sin(y) + Center X;
		float	v = (((s * -tw2) + yc) * fCoefY) + (fCenterY*(float)th);		// y' = sin(x)+cos(y) + Center Y;

		for (udword x=0; x<w; x++)
		{
      float uf = u>=0 ? (u - (sdword)u) : 1+(u - (sdword)u);	//Fraction
      float vf = v>=0 ? (v - (sdword)v) : 1+(v - (sdword)v);	//Fraction

      udword ut = u>=0 ? (udword)u : (udword)u - 1;
			udword vt = v>=0 ? (udword)v : (udword)v - 1;

			//Texels
			// 1 | 2
			//-------
			// 3 | 4

			//Texel1
			float WeightFactors = (1.0f-uf) * (1.0f-vf);
			NRGBA* ptexel = pPxlSrc + ((vt%th)*tw) + (ut%tw);

			Color.r = (sdword)(ptexel->r	* WeightFactors);
			Color.g = (sdword)(ptexel->g	* WeightFactors);
			Color.b = (sdword)(ptexel->b	* WeightFactors);
			Color.a = (sdword)(ptexel->a	* WeightFactors);

			//Texel2
			WeightFactors = uf * (1.0f-vf);
			ptexel = pPxlSrc + ((vt%th)*tw) + ((ut+1)%tw);
			Color.r+= (sdword)(ptexel->r	* WeightFactors);
			Color.g+= (sdword)(ptexel->g	* WeightFactors);
			Color.b+= (sdword)(ptexel->b	* WeightFactors);
			Color.a+= (sdword)(ptexel->a	* WeightFactors);

			//Texel3
			WeightFactors = (1.0f-uf) * vf;
			ptexel = pPxlSrc + (((vt+1)%th)*tw) + (ut%tw);
			Color.r+= (sdword)(ptexel->r	* WeightFactors);
			Color.g+= (sdword)(ptexel->g	* WeightFactors);
			Color.b+= (sdword)(ptexel->b	* WeightFactors);
			Color.a+= (sdword)(ptexel->a	* WeightFactors);

			//Texel4
			WeightFactors = uf * vf;
			ptexel = pPxlSrc + (((vt+1)%th)*tw) + ((ut+1)%tw);
			Color.r+= (sdword)(ptexel->r	* WeightFactors);
			Color.g+= (sdword)(ptexel->g	* WeightFactors);
			Color.b+= (sdword)(ptexel->b	* WeightFactors);
			Color.a+= (sdword)(ptexel->a	* WeightFactors);

			//Pixel
			pPxlDst->r = (ubyte)Color.r;
			pPxlDst->g = (ubyte)Color.g;
			pPxlDst->b = (ubyte)Color.b;
			pPxlDst->a = (ubyte)Color.a;
			pPxlDst++;

			//Vectors
			u+=c*fCoefX;	v+=s*fCoefY;
		};

		//Vectors
		ys+=s; yc+=c;
	};


	return 1;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NDistortOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NDistortOp, NOperatorFx);

static NVarsBlocDesc blocdescDistortOp[] =
{
	VAR(eudword,	true, "Power",	"0", "NUbyteProp", 0.0f, 255.0f, 1.0f)	//0
};

NDistortOp::NDistortOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescDistortOp, 1);

}

udword NDistortOp::Process(float _ftime, SEngineState& _state)
{
	//Two inputs (texture, normal)
	if (m_byInputs!=2)		return (udword)-1;

	//Get input Texture and Normal
	N2DBitmap* pSrc		= (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pNorm	= (N2DBitmap*)_state.apInputs[1];

	// Same inputs W and H sizes
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	if (w!=pNorm->GetWidth() )		return (udword)-1;
	if (h!=pNorm->GetHeight())		return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Set Texture size
	N2DBitmap* pDst	= (N2DBitmap*)m_pObj;
	pDst->SetSize(w, h);


	/////////////////////////////////////////
	//Get Variables Values
	ubyte byPower;
	m_pcvarsBloc->GetValue(0, _ftime, byPower);

	float fPower = (float)byPower;	//0.0<->8.0

	/////////////////////////////////////////
	//Process operator
	NRGBA* pPxNorm = pNorm->GetPixels();
	NRGBA* pPxSrc	= pSrc->GetPixels();
	NRGBA* pPxDst	= pDst->GetPixels();

	NRGBA Color;

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

			//Get texel
			float u = fmodf((x + (n.x*fPower)), w);
			float v = fmodf((y + (n.y*fPower)), h);

            float uf = u>=0 ? (u - (sdword)u) : 1+(u - (sdword)u);	//Fraction
            float vf = v>=0 ? (v - (sdword)v) : 1+(v - (sdword)v);	//Fraction

            udword ut = u>=0 ? (udword)u : (udword)u - 1;
			udword vt = v>=0 ? (udword)v : (udword)v - 1;


			//Texels
			// 1 | 2
			//-------
			// 3 | 4

			//Texel1
			float WeightFactors = (1.0f-uf) * (1.0f-vf);
			NRGBA* ptexel = pPxSrc + ((vt%h)*w) + (ut%w);

			Color.r = (ubyte)(ptexel->r	* WeightFactors);
			Color.g = (ubyte)(ptexel->g	* WeightFactors);
			Color.b = (ubyte)(ptexel->b	* WeightFactors);
			Color.a = (ubyte)(ptexel->a	* WeightFactors);

			//Texel2
			WeightFactors = uf * (1.0f-vf);
			ptexel = pPxSrc + ((vt%h)*w) + ((ut+1)%w);
			Color.r+= (ubyte)(ptexel->r	* WeightFactors);
			Color.g+= (ubyte)(ptexel->g	* WeightFactors);
			Color.b+= (ubyte)(ptexel->b	* WeightFactors);
			Color.a+= (ubyte)(ptexel->a	* WeightFactors);

			//Texel3
			WeightFactors = (1.0f-uf) * vf;
			ptexel = pPxSrc + (((vt+1)%h)*w) + (ut%w);
			Color.r+= (ubyte)(ptexel->r	* WeightFactors);
			Color.g+= (ubyte)(ptexel->g	* WeightFactors);
			Color.b+= (ubyte)(ptexel->b	* WeightFactors);
			Color.a+= (ubyte)(ptexel->a	* WeightFactors);

			//Texel4
			WeightFactors = uf * vf;
			ptexel = pPxSrc + (((vt+1)%h)*w) + ((ut+1)%w);
			Color.r+= (ubyte)(ptexel->r	* WeightFactors);
			Color.g+= (ubyte)(ptexel->g	* WeightFactors);
			Color.b+= (ubyte)(ptexel->b	* WeightFactors);
			Color.a+= (ubyte)(ptexel->a	* WeightFactors);

			*pPxDst++ = Color;

			pPxNorm++;
		}
	}

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//                                                      NVortexOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NVortexOp, NOperatorFx);

static NVarsBlocDesc blocdescVortexOp[] =
{
	VAR(efloat,  true, "CenterX",  "0.5",  "NFloatProp", -1.0f, 1.0f, 0.001f)   //0
	VAR(efloat,  true, "CenterY",  "0.5",  "NFloatProp", -1.0f, 1.0f, 0.001f)   //1
	VAR(efloat,  true, "RayX",     "0.5",  "NFloatProp", -1.0f, 1.0f, 0.001f)   //2
	VAR(efloat,  true, "RayY",     "0.5",  "NFloatProp", -1.0f, 1.0f, 0.001f)   //3
	VAR(efloat,  true, "Twist",    "1.28", "NFloatProp", -100.0f, 100.0f, 0.001f)   //4
};


NVortexOp::NVortexOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(5, blocdescVortexOp, 1);

}

udword NVortexOp::Process(float _ftime, SEngineState& _state)
{
	//Only one Input
	if (m_byInputs!=1)              return (udword)-1;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Get input texture
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;

	sdword w = pSrc->GetWidth();
	sdword h = pSrc->GetHeight();
	pDst->SetSize(w,h);

	//Get Variables Values
	float byCenterX, byCenterY, byRayX, byRayY;
	float byTwist;
	m_pcvarsBloc->GetValue(0, _ftime, byCenterX);
	m_pcvarsBloc->GetValue(1, _ftime, byCenterY);
	m_pcvarsBloc->GetValue(2, _ftime, byRayX);
	m_pcvarsBloc->GetValue(3, _ftime, byRayY);
	m_pcvarsBloc->GetValue(4, _ftime, byTwist);

	//Process operator
	sdword  dwCenterX       = (sdword)(byCenterX*w);
	sdword  dwCenterY       = (sdword)(byCenterY*h);
	sdword  dwRadiusX       = (sdword)(byRayX*w);
	sdword  dwRadiusY       = (sdword)(byRayY*h);

	float   f1_RadiusX = 1.0f/(float)dwRadiusX;
	float   f1_RadiusY = 1.0f/(float)dwRadiusY;

	float radians = byTwist * 100.0f * nv_to_rad;

	NRGBA* pPxSrc = pSrc->GetPixels();
	NRGBA* baseSrc = pPxSrc;
	NRGBA* pPxDst = pDst->GetPixels();

//	NRGBA* src;
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

			if(d>1.0f) {
				//not inside radius, just copy it
				pPxDst->r = pPxSrc->r;
				pPxDst->g = pPxSrc->g;
				pPxDst->b = pPxSrc->b;
				pPxDst->a = pPxSrc->a;
			} else {
				//twist more as closer to the middle
				//d = sinf(d * nv_half_pi);
				//this seems to give a good enough approximation of sinf
				d = ffast_cos(d * nv_half_pi - nv_half_pi);
				d=1.0f - d;

				//rotate around middle
				float nx = x - dwCenterX;
				float ny = y - dwCenterY;

				float rad = radians*d;

				//todo: optimize this. cosf/sinf are slow
				float bx = nx;
				nx = bx*cosf(rad) - ny*sinf(rad) + dwCenterX;
				ny = bx*sinf(rad) + ny*cosf(rad) + dwCenterY;

				if(nx>=w) nx = nx - w;
				if(ny>=h) ny = ny - h;
				if(nx<0) nx = w + nx;
				if(ny<0) ny = h + ny;

				//bilinear sample nearest 4 pixels at rotated pos
				int ix,iy;
				ix = (int) nx;
				iy = (int) ny;

				float fracX = nx - ix;
				float fracY = ny - iy;

				float   ul = (1.0f - fracX) * (1.0f - fracY);
				float   ll = (1.0f - fracX) * fracY;
				float   ur = fracX * (1.0f - fracY);
				float   lr = fracX * fracY;

				int wrapx = (ix+1)%w;
				int wrapy = (iy+1)%h;
				NRGBA* texelUL = baseSrc + ix + iy*w;
				NRGBA* texelUR = baseSrc + wrapx + iy*w;
				NRGBA* texelLL = baseSrc + ix + wrapy*w;
				NRGBA* texelLR = baseSrc + wrapx + wrapy*w;

				pPxDst->r = (int)(ul * texelUL->r + ll * texelLL->r + ur * texelUR->r + lr * texelLR->r);
				pPxDst->g = (int)(ul * texelUL->g + ll * texelLL->g + ur * texelUR->g + lr * texelLR->g);
				pPxDst->b = (int)(ul * texelUL->b + ll * texelLL->b + ur * texelUR->b + lr * texelLR->b);
				pPxDst->a = (int)(ul * texelUL->a + ll * texelLL->a + ur * texelUR->a + lr * texelLR->a);

			}
			pPxSrc++;
			pPxDst++;
		}
	}

	return 1;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//                                                      NLookupOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NLookupOp, NOperatorFx);

NLookupOp::NLookupOp()
{
}

udword NLookupOp::Process(float _ftime, SEngineState& _state)
{
	//Two inputs (texture, texcoords)
	if (m_byInputs!=2) return (udword)-1;

	//Get input Texture and TexCoords
	N2DBitmap* pSrc = (N2DBitmap*)_state.apInputs[0];
	N2DBitmap* pTexCoords = (N2DBitmap*)_state.apInputs[2];

	udword w = pTexCoords->GetWidth();
	udword h = pTexCoords->GetHeight();
	udword tw = pSrc->GetWidth();
	udword th = pSrc->GetHeight();

	float scaleW = tw / 256.0f;
	float scaleH = th / 256.0f;

	//Bitmap instance
	NEngineOp::GetInstance()->GetBitmap(&m_pObj);

	//Set Texture size
	N2DBitmap* pDst = (N2DBitmap*)m_pObj;
	pDst->SetSize(w, h);

	/////////////////////////////////////////
	//Process operator
	NRGBA* pPxTexCoords = pTexCoords->GetPixels();
	NRGBA* pPxSrc = pSrc->GetPixels();
	NRGBA* pPxDst = pDst->GetPixels();

	for (udword y=0; y < h; y++)
	{
		for (udword x=0; x < w; x++)
		{
			udword u = (udword)(pPxTexCoords->r * scaleW);
			udword v = (udword)(pPxTexCoords->g * scaleH);
			pPxTexCoords++;

			*pPxDst++ = pPxSrc[(v*tw) + u];
		}
	}
	return 0;
}





