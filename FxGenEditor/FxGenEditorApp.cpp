/***************************************************************
 * Name:      FxGenEditorApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2008-03-15
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "FxGenEditorApp.h"
#include "FxGenEditorMain.h"

IMPLEMENT_APP(FxGenEditorApp);

bool FxGenEditorApp::OnInit()
{
    FxGenEditorFrame* frame = new FxGenEditorFrame(0L);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();

    return true;
}
