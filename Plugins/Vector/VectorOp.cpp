//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		VectorOp.cpp
//! \brief	Vector Render Operator
//!
//!	\author Anders Stenberg (anders.stenberg@gmail.com)
//!
//!	\date		10-02-2008
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
#include "VectorOp.h"
//#include "vgvm/vgvm.h"
//#include "vgvm/contexts/cairo.h"
//#include "RectangularArray.h"


const char* GetModuleName()  { return "Vector"; }

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NVectorOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVectorOp, NOperator);

/*static void* vgvmAlloc( size_t size )
{
  return NMemAlloc( size );
}

static void vgvmFree( void* memory )
{
  return NMemFree( memory );
}*/

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

//	m_pContext = vgvm::cairo::createCairoContext(vgvmAlloc, vgvmFree);
//	m_pProgram = m_pContext->createProgram();
}

NVectorOp::~NVectorOp()
{
//	delete m_pProgram;
//	delete m_pContext;
}

/*void NVectorOp::SetVectorData(ubyte* _data, udword _length)
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
*/

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
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w,h);
	RGBA* pPxDst = pDst->GetPixels();

	memset( pPxDst, 0, w*h*4 );

/*
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
			RGBA color = *pPxDst;
			pPxDst->r = color.b;
			pPxDst->g = color.g;
			pPxDst->b = color.r;

			pPxDst++;
		}
	}
*/
	return 0;
}
