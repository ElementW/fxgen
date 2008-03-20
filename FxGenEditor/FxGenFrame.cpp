	#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "FxGenFrame.h"


FxGenFrame::FxGenFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : GUIFrame( parent, id, title, pos, size, style )
{

}

FxGenFrame::~FxGenFrame()
{
}

void FxGenFrame::OnOpen( wxCommandEvent& event )
{
  wxMessageBox(_("Test message"), _("FxGen"));

}
