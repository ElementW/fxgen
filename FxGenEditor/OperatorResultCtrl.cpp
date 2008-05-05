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

const wxChar wxOperatorResultCtrlNameStr[] = wxT("OperatorResultCtrl");
//const wxChar wxOperatorResultCtrlNameStr[1] = { _("OperatorResultCtrl") };

BEGIN_EVENT_TABLE(wxOperatorResultCtrl, wxControl)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxOperatorResultCtrl, wxControl)

wxOperatorResultCtrl::wxOperatorResultCtrl(wxWindow *parent, wxWindowID id,
               const wxPoint& pos, const wxSize& size, long style,
               const wxValidator &validator,  const wxString& name)
    : wxControl(parent, id, pos, size, style, validator, name)
{
}

