#include "EventsList.h"

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "EventsList.h"
#include "PropertiesCtrl.h"

//#include "..\Src\FxGenLib\Core.h"
//#include "..\Src\FxGenLib\EngineOp.h"

const wxChar wxPropertiesCtrlNameStr[] = wxT("PropertiesCtrl");

BEGIN_EVENT_TABLE(wxPropertiesCtrl, wxControl)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxPropertiesCtrl, wxControl)

wxPropertiesCtrl::wxPropertiesCtrl(wxWindow *parent, wxWindowID id,
               const wxPoint& pos, const wxSize& size, long style,
               const wxValidator &validator,  const wxString& name)
    : wxControl(parent, id, pos, size, style, validator, name)
{
}
