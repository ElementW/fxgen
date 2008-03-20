#include "EventsList.h"

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "EventsList.h"
#include "OperatorsCtrl.h"

//#include "..\Src\FxGenLib\Core.h"
//#include "..\Src\FxGenLib\EngineOp.h"

const wxChar wxOperatorsCtrlNameStr[] = wxT("OperatorsCtrl");
//const wxChar wxControlNameStr[] = wxT("control");

BEGIN_EVENT_TABLE(wxOperatorsCtrl, wxControl)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxOperatorsCtrl, wxControl)

wxOperatorsCtrl::wxOperatorsCtrl(wxWindow *parent, wxWindowID id,
               const wxPoint& pos, const wxSize& size, long style,
               const wxValidator &validator,  const wxString& name)
    : wxControl(parent, id, pos, size, style, validator, name)
{
}
