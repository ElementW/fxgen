#ifndef OPERATORRESULTCTRL_H_INCLUDED
#define OPERATORRESULTCTRL_H_INCLUDED

#include <wx/wx.h>
#include <wx/control.h>
#include <wx/glcanvas.h>
//#include <wx/palette.h>


extern const wxChar wxOperatorResultCtrlNameStr[];

class wxOperatorResultCtrl : public wxGLCanvas
{
    //DECLARE_DYNAMIC_CLASS(wxOperatorResultCtrl)

public:
    //wxOperatorResultCtrl() {}

    wxOperatorResultCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxOperatorResultCtrlNameStr,
               int* attribList = 0,
                const wxPalette& palette = wxNullPalette);

// Constructeur
/*GL_Window(wxWindow* parent, wxWindowID id, const wxPoint& pos,
const wxSize& size, long style=0,
const wxString& name="GLCanvas", int* attribList = 0,
const wxPalette& palette = wxNullPalette)
: wxGLCanvas(parent, id, pos, size, style,
name, attribList, palette) {};*/


  //Methods
	int getWidth();
	int getHeight();

	//void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

  //Events
  DECLARE_EVENT_TABLE()
  void render(wxPaintEvent& event);
  void resized(wxSizeEvent& evt);

  //FxGen Events
  virtual void OnFxGenOpShowChanged( wxCommandEvent& event );

    // erase part of the control
    virtual void EraseBackground(wxDC& dc, const wxRect& rect);

    wxGLContext *m_pContext;
};


#endif // OPERATORRESULTCTRL_H_INCLUDED
