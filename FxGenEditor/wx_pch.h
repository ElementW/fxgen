/***************************************************************
 * Name:      wx_pch.h
 * Purpose:   Header to create Pre-Compiled Header (PCH)
 * Author:     ()
 * Created:   2008-03-15
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED



// basic wxWidgets headers
#include <wx/wxprec.h>
#include <wx/dcbuffer.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//#if !wxUSE_GLCANVAS
//    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
//#endif

#ifdef WX_PRECOMP
//  #include "..\Src\FxGenLib\Core.h"
//  #include "..\Src\FxGenLib\EngineOp.h"
#endif // WX_PRECOMP




#endif // WX_PCH_H_INCLUDED
