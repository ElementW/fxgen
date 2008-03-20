#ifndef OPERATORSCTRL_H_INCLUDED
#define OPERATORSCTRL_H_INCLUDED

#include <wx/wx.h>
#include <wx/control.h>

//extern WXDLLEXPORT_DATA(const wxChar) wxOperatorsCtrlNameStr[];
extern const wxChar wxOperatorsCtrlNameStr[];

class wxOperatorsCtrl : public wxControl
{
    DECLARE_DYNAMIC_CLASS(wxOperatorsCtrl)

public:
    wxOperatorsCtrl() {}

    wxOperatorsCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxOperatorsCtrlNameStr);


  //FxGen Events
    DECLARE_EVENT_TABLE()
};

#endif // OPERATORSCTRL_H_INCLUDED
