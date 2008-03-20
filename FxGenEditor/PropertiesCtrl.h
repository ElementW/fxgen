#ifndef PROPERTIESCTRL_H_INCLUDED
#define PROPERTIESCTRL_H_INCLUDED

#include <wx/wx.h>
#include <wx/control.h>

extern const wxChar wxPropertiesCtrlNameStr[];

class wxPropertiesCtrl : public wxControl
{
    DECLARE_DYNAMIC_CLASS(wxPropertiesCtrl)

public:
    wxPropertiesCtrl() {}

    wxPropertiesCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxPropertiesCtrlNameStr);


  //FxGen Events
    DECLARE_EVENT_TABLE()
};

#endif // PROPERTIESCTRL_H_INCLUDED
