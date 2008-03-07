//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ImageOp.cpp
//! \brief	Experimental Image operator plug-in for FxGen
//!
//!	\author	Sebastian Olter (qduaty@gmail.com)
//!	\date		25-02-2008
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

#include <fxgen_pch.h>
#include <pch.h>

//-----------------------------------------------------------------
//!	\class		NImageOp
//!	\brief		Import an image file
//-----------------------------------------------------------------
class FXGEN_API NImageOp : public NOperator
{
public:
	DECLARE_CLASS();

	NImageOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Image"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

IMPLEMENT_CLASS(NImageOp, NOperator);

static NVarsBlocDesc blocdescImageOp[] =
{
	VAR(eubyte,	false, "Width",			"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//0
	VAR(eubyte,	false, "Height",		"8,[1,2,4,8,16,32,64,128,256,512,1024,2048,4096]", "NUbyteComboProp")	//1
	VAR(estring,	true, 	"File",			"",	"NFileBrowserProp")	//2
};

NImageOp::NImageOp()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(3, blocdescImageOp, 1);
}


udword NImageOp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//No Inputs
	if (m_byInputs != 0)
		return (udword)-1;

	//Get Variables Values
	ubyte byVal;
	m_pcvarsBloc->GetValue(0, 0, byVal);
	udword w=1 << (udword)byVal;
	m_pcvarsBloc->GetValue(1, 0, byVal);
	udword h=1 << (udword)byVal;

	w = (udword)(w * _fDetailFactor);
	h = (udword)(h * _fDetailFactor);

	char* filename = NULL;
	m_pcvarsBloc->GetValue(2, _ftime, filename);

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);
	NBitmap* pDst = (NBitmap*)m_pObj;
	pDst->SetSize(w, h);

	//Temporary buffeur to Texture
	RGBA black(0,0,0,255);
	RGBA gray(127,127,127,255);
	RGBA white(255,255,255,255);
	RGBA red(255,0,0,255);

	if(!Glib::file_test(filename, Glib::FILE_TEST_EXISTS))
		return (udword)-1; // no image file

	RefPtr<Gdk::Pixbuf> bufin = Gdk::Pixbuf::create_from_file(filename);
	if(!(bufin->get_width() && bufin->get_height()))
		return (udword)-1; // invalid image file - no further processing

	size_t rowstride = w * sizeof(RGBA);
	size_t bpp = 8 * sizeof(RGBA) / 4;

	RefPtr<Gdk::Pixbuf> bufout = Gdk::Pixbuf::create_from_data(
		reinterpret_cast<const guint8*>(pDst->GetPixels()), Gdk::COLORSPACE_RGB, true, bpp, w, h, rowstride);
	bufin->scale(bufout, 0, 0, w, h, 0, 0, w * 1. / bufin->get_width(), h * 1. / bufin->get_height(), Gdk::INTERP_HYPER);

	return 0;
}

