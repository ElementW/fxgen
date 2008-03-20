/***************************************************************
 * Name:      FxGenEditorMain.cpp
 * Purpose:   Code for Application Frame
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

#include "FxGenEditorMain.h"
#include "EventsList.h"

#include "ProjectTreeCtrl.h"
#include "OperatorsCtrl.h"
#include "OperatorResultCtrl.h"
#include "PropertiesCtrl.h"

#include "..\Src\FxGenLib\Core.h"
#include "..\Src\FxGenLib\EngineOp.h"

//Events Types
DEFINE_EVENT_TYPE(wxEVT_FXGEN_PROJECT)

//helper functions
enum wxbuildinfoformat {short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}


FxGenEditorFrame::FxGenEditorFrame(wxFrame *frame) : GUIFrame(frame)
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("Hello Code::Blocks user!"), 0);
    statusBar->SetStatusText(wxbuildinfo(short_f), 1);
#endif

  // notify wxAUI which frame to use
  m_mgr.SetManagedWindow(this);

  // create several text controls
  wxProjectTreeCtrl*    pprojectCtrl = new wxProjectTreeCtrl(this, -1, wxDefaultPosition, wxSize(200,150), wxNO_BORDER);
  wxOperatorResultCtrl* presultCtrl = new wxOperatorResultCtrl(this, -1, wxDefaultPosition, wxSize(200,150), wxNO_BORDER);
  wxOperatorsCtrl*      poperatorCtrl = new wxOperatorsCtrl(this, -1, wxDefaultPosition, wxSize(200,150), wxNO_BORDER);
  wxPropertiesCtrl*     ppropertiesCtrl = new wxPropertiesCtrl(this, -1, wxDefaultPosition, wxSize(200,150), wxNO_BORDER);

  // add the panes to the manager
  m_mgr.AddPane(presultCtrl, wxTOP, wxT("Operator result"));
  m_mgr.AddPane(pprojectCtrl, wxLEFT, wxT("Project"));
  m_mgr.AddPane(poperatorCtrl, wxCENTER, wxT("Operators"));
  m_mgr.AddPane(ppropertiesCtrl, wxRIGHT, wxT("Properties"));

  // tell the manager to "commit" all the changes just made
  m_mgr.Update();

  //###Test###
	//this->Connect(wsID_FXGEN_PRJ_LOADED, wxEVT_FXGEN_PROJECT, wxEventHandler( FxGenEditorFrame::OnFxGenProjectLoaded ) );
	//this->Connect(wsID_FXGEN_PRJ_NEW, wxEVT_FXGEN_PROJECT, wxEventHandler( FxGenEditorFrame::OnFxGenProjectNew ) );
	//this->Connect(wsID_FXGEN_PRJ_NEW, wxEVT_FXGEN_PROJECT, wxCommandEventHandler( FxGenEditorFrame::OnFxGenProjectNew ));

}

FxGenEditorFrame::~FxGenEditorFrame()
{
  // deinitialize the frame manager
  m_mgr.UnInit();
}

void FxGenEditorFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void FxGenEditorFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void FxGenEditorFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

//-----------------------------------------------------------------
//!	\brief	New FxGen Project
//-----------------------------------------------------------------
void FxGenEditorFrame::OnNew( wxCommandEvent& event )
{
  //wxEvent(int winid = 0, wxEventType commandType = wxEVT_NULL );
  {
    wxMessageBox(_("Send New Event"), _("FxGen"));

    wxCommandEvent  fxevent( wxEVT_FXGEN_PROJECT, -1);
    fxevent.SetEventObject( this );
    GetEventHandler()->ProcessEvent( fxevent );

  }

}

//-----------------------------------------------------------------
//!	\brief	Open an FxGen Project
//-----------------------------------------------------------------
void FxGenEditorFrame::OnOpen( wxCommandEvent& event )
{
  wxString filename = wxFileSelector(_("Choose a FxGen file to open"));
  if ( !filename.empty() )
  {
    //Clear Old FxGen Project
    NEngineOp::GetEngine()->Clear();

    //Emit Event if project loaded
    if ( NEngineOp::GetEngine()->LoadProject((const char*)filename.c_str()))
    {
      //wxEvent(int winid = 0, wxEventType commandType = wxEVT_NULL );
      wxCommandEvent  fxevent( wxEVT_FXGEN_PROJECT, wsID_FXGEN_PRJ_LOADED);
      fxevent.SetEventObject( this );
      GetEventHandler()->ProcessEvent( fxevent );
    }
  }

}

void FxGenEditorFrame::OnFxGenProjectNew( wxCommandEvent& event )
{
  wxMessageBox(_("FxGenEditorFrame Project Loaded new"), _("FxGen"));
}

