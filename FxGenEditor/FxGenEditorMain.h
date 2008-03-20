/***************************************************************
 * Name:      FxGenEditorMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2008-03-15
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef FXGENEDITORMAIN_H
#define FXGENEDITORMAIN_H

#include <wx/wx.h>
#include "wx/aui/aui.h"
#include <wx/treectrl.h>

#include "FxGenEditorApp.h"


#include "GUIFrame.h"

class FxGenEditorFrame: public GUIFrame
{
  public:
    FxGenEditorFrame(wxFrame *frame);
    ~FxGenEditorFrame();
  private:
  //Events
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnQuit(wxCommandEvent& event);
    virtual void OnAbout(wxCommandEvent& event);
    virtual void OnNew( wxCommandEvent& event );
    virtual void OnOpen( wxCommandEvent& event );

//###TEST###
    virtual void OnFxGenProjectNew( wxCommandEvent& event );

  //Docking windows
  wxAuiManager m_mgr;

};

#endif // FXGENEDITORMAIN_H
