//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DistordOps.cpp
//! \brief	Distord Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
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
#include "DistordOps.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRotoZoomOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NRotoZoomOp, NOperator);

static NMapVarsBlocDesc mapblocdescRotoZoomOp[] =
{
	MAP(1, eubyte,		"2",		"*0.00390625"	)	//V1 => 0-CenterX
	MAP(1, eubyte,		"3",		"*0.00390625"	)	//V1 => 1-CenterY
	MAP(1, eubyte,		"4",		"*0.00390625"	)	//V1 => 2-Rotate
	MAP(1, eubyte,		"5,6",	"*0.0078125"	)	//V1 => 3-Zoom
};


static NVarsBlocDesc blocdescRotoZoomOp[] =
{
	VAR(eubyte,	false, "Set Width",		"0,[0 (Default),1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Set Height",	"0,[0 (Default),1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(efloat,		true, "CenterX",	"0.5",		"NFloatProp")	//2
	VAR(efloat,		true, "CenterY",	"0.5",		"NFloatProp")	//3
	VAR(efloat,		true, "Rotate",		"0.0",		"NFloatProp")	//4
	VAR(efloat,		true, "ZoomX",		"1.0",		"NFloatProp")	//5
	VAR(efloat,		true, "ZoomY",		"1.0",		"NFloatProp")	//6
	VAR(eubyte,		true, "Wrap",			"1,[0 (Off), 1 (On)]",	"NUbyteComboProp")	//7
};

NRotoZoomOp::NRotoZoomOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(8, blocdescRotoZoomOp, 2);
	//To Keep compatibility with oldier blocs versions (will be removed after alpha)
	m_pcvarsBloc->SetMapVarBlocDesc(4, mapblocdescRotoZoomOp);

}

NRotoZoomOp::~NRotoZoomOp()
{
}

udword NRotoZoomOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Get input texture
	NBitmap* pSrc = (NBitmap*)(*_pOpsInts)->m_pObj;
	NBitmap* pDst = (NBitmap*)m_pObj;

	udword tw = pSrc->GetWidth();
	udword th = pSrc->GetHeight();

	udword w = tw;
	udword h = th;

	//Get Variables Values
	ubyte byVal;

	m_pcvarsBloc->GetValue(0, 0, byVal);
	if (byVal!=0)		w=1<<((udword)(byVal-1));

	m_pcvarsBloc->GetValue(1, 0, byVal);
	if (byVal!=0)		h=1<<((udword)(byVal-1));

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
	fZoomX=0.5f - (fZoomX/2.0f);
	fZoomX=exp(fZoomX*6.0f);

	fZoomY=0.5f - (fZoomY/2.0f);
	fZoomY=exp(fZoomY*6.0f);

	//Process RotoZoom
	RGBA* pPxlSrc = pSrc->GetPixels();
	RGBA* pPxlDst = pDst->GetPixels();

	float fCoefX = ((float)tw / (float)w) * fZoomX;
	float fCoefY = ((float)th / (float)h) * fZoomY;

	float	c = (float) (cos(fRotate));
	float	s = (float) (sin(fRotate));

	float tw2 = (float)w / 2.0f;
	float th2 = (float)h / 2.0f;

	float	ys = s * -th2;
	float	yc = c * -th2;

	RGBAI Color;

	for (udword y=0; y<h; y++)
	{
		float	u = (((c * -tw2) - ys) * fCoefX) + (fCenterX*(float)tw);		// x' = cos(x)-sin(y) + Center X;
		float	v = (((s * -tw2) + yc) * fCoefY) + (fCenterY*(float)th);		// y' = sin(x)+cos(y) + Center Y;

		for (udword x=0; x<w; x++)
		{
			float uf = fabs(u - (sdword)u);	//Fraction
			float vf = fabs(v - (sdword)v);	//Fraction

			udword ut = (udword)u;
			udword vt = (udword)v;

			//Texels
			// 1 | 2
			//-------
			// 3 | 4

			//Texel1
			float WeightFactors = (1.0f-uf) * (1.0f-vf);						
			RGBA* ptexel = pPxlSrc + ((vt%th)*tw) + (ut%tw);

			Color.r = ((sdword)ptexel->r	* WeightFactors);
			Color.g = ((sdword)ptexel->g	* WeightFactors);
			Color.b = ((sdword)ptexel->b	* WeightFactors);
			Color.a = ((sdword)ptexel->a	* WeightFactors);
			
			//Texel2			
			WeightFactors = uf * (1.0f-vf);
			ptexel = pPxlSrc + ((vt%th)*tw) + ((ut+1)%tw);
			Color.r+= ((sdword)ptexel->r	* WeightFactors);
			Color.g+= ((sdword)ptexel->g	* WeightFactors);
			Color.b+= ((sdword)ptexel->b	* WeightFactors);
			Color.a+= ((sdword)ptexel->a	* WeightFactors);

			//Texel3
			WeightFactors = (1.0f-uf) * vf;			
			ptexel = pPxlSrc + (((vt+1)%th)*tw) + (ut%tw);
			Color.r+= ((sdword)ptexel->r	* WeightFactors);
			Color.g+= ((sdword)ptexel->g	* WeightFactors);
			Color.b+= ((sdword)ptexel->b	* WeightFactors);
			Color.a+= ((sdword)ptexel->a	* WeightFactors);

			//Texel4
			WeightFactors = uf * vf;			
			ptexel = pPxlSrc + (((vt+1)%th)*tw) + ((ut+1)%tw);
			Color.r+= ((sdword)ptexel->r	* WeightFactors);
			Color.g+= ((sdword)ptexel->g	* WeightFactors);
			Color.b+= ((sdword)ptexel->b	* WeightFactors);
			Color.a+= ((sdword)ptexel->a	* WeightFactors);

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
IMPLEMENT_CLASS(NDistortOp, NOperator);

static NVarsBlocDesc blocdescDistortOp[] =
{
	VAR(eudword,	true, "Power",	"0", "NUbyteProp")	//0
};

NDistortOp::NDistortOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescDistortOp, 1);

}

udword NDistortOp::Process(float _ftime, NOperator** _pOpsInts)
{
	//Two inputs (texture, normal)
	if (m_byInputs!=2)		return (udword)-1;

	//Get input Texture and Normal
	NBitmap* pSrc		= (NBitmap*)(*(_pOpsInts+0))->m_pObj;
	NBitmap* pNorm	= (NBitmap*)(*(_pOpsInts+1))->m_pObj;

	// Same inputs W and H sizes
	udword w = pSrc->GetWidth();
	udword h = pSrc->GetHeight();
	if (w!=pNorm->GetWidth() )		return (udword)-1;
	if (h!=pNorm->GetHeight())		return (udword)-1;

	//Bitmap instance
	gNFxGen_GetEngine()->GetBitmap(&m_pObj);

	//Set Texture size
	NBitmap* pDst	= (NBitmap*)m_pObj;
	pDst->SetSize(w, h);
	

	/////////////////////////////////////////
	//Get Variables Values
	ubyte byPower;
	m_pcvarsBloc->GetValue(0, _ftime, byPower);

	float fPower = (float)byPower;	//0.0<->8.0

	/////////////////////////////////////////
	//Process operator
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

			//Get texel
			udword u = ((sdword)x + (sdword)(n.x*fPower)) % w;
			udword v = ((sdword)y + (sdword)(n.y*fPower)) % h;

			*pPxDst++ = pPxSrc[u + (v*w)];

			pPxNorm++;
		}
	}

	return 0;
}

