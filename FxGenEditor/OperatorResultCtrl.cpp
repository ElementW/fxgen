#include "wx/wx.h"

#include "EventsList.h"

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "EventsList.h"
#include "OperatorResultCtrl.h"

//#include "..\Src\FxGenLib\Core.h"
//#include "..\Src\FxGenLib\EngineOp.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					wxOperatorResultCtrl class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
const wxChar wxOperatorResultCtrlNameStr[] = wxT("OperatorResultCtrl");


BEGIN_EVENT_TABLE(wxOperatorResultCtrl, wxGLCanvas)
 EVT_SIZE(wxOperatorResultCtrl::resized)
 EVT_PAINT    (wxOperatorResultCtrl::render)
END_EVENT_TABLE()

//IMPLEMENT_CLASS(wxOperatorResultCtrl, wxGLCanvas)
//IMPLEMENT_DYNAMIC_CLASS(wxOperatorResultCtrl, wxGLCanvas)

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
wxOperatorResultCtrl::wxOperatorResultCtrl(wxWindow *parent, wxWindowID id,
               const wxPoint& pos, const wxSize& size,
               long style, const wxString& name,
               int* attribList, const wxPalette& palette)
    : wxGLCanvas(parent, id, attribList, pos, size, style, wxT("GLCanvas"), palette)
{
  parent->Connect(wsID_FXGEN_OPSHOW_CHANGED, wxEVT_FXGEN_OPERATORSCTRL, wxCommandEventHandler( wxOperatorResultCtrl::OnFxGenOpShowChanged ),0,this);

  m_pContext = new wxGLContext( this );
}

void wxOperatorResultCtrl::EraseBackground(wxDC& dc, const wxRect& rect)
{

}

//-----------------------------------------------------------------
//!	\brief	FxGen event
//-----------------------------------------------------------------
void wxOperatorResultCtrl::OnFxGenOpShowChanged( wxCommandEvent& event )
{
  wxMessageBox(_("wxOperatorResultCtrl OnFxGenPageChanged"), _("FxGen"));

  //NOperatorsPage* ppage = (NOperatorsPage*)event.GetClientData();
  //DisplayOperatorsPage(ppage);
  //Refresh();

}

void wxOperatorResultCtrl::render( wxPaintEvent& evt )
{
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    if(!IsShown()) return;

    wxGLCanvas::SetCurrent( *m_pContext );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prepare2DViewport(0,0,getWidth()/2, getHeight());
    glLoadIdentity();

    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(getWidth(),0,0);
    glVertex3f(getWidth(),getHeight(),0);
    glVertex3f(0,getHeight(),0);
    glEnd();

    // red square
    glColor4f(1, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(getWidth()/8, getHeight()/3, 0);
    glVertex3f(getWidth()*3/8, getHeight()/3, 0);
    glVertex3f(getWidth()*3/8, getHeight()*2/3, 0);
    glVertex3f(getWidth()/8, getHeight()*2/3, 0);
    glEnd();

    glFlush();
    SwapBuffers();
}

void wxOperatorResultCtrl::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{

    /*
     *  Inits the OpenGL viewport for drawing in 2D
     */

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int wxOperatorResultCtrl::getWidth()
{
    return GetSize().x;
}

int wxOperatorResultCtrl::getHeight()
{
    return GetSize().y;
}

void wxOperatorResultCtrl::resized(wxSizeEvent& evt)
{
    wxGLCanvas::OnSize(evt);

    Refresh();
}
