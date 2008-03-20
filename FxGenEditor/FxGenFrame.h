#ifndef FXGENFRAME_H_INCLUDED
#define FXGENFRAME_H_INCLUDED

#include "GUIFrame.h"

class FxGenFrame : public GUIFrame
{
  protected:
		virtual void OnOpen( wxCommandEvent& event );

	public:
		FxGenFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("FxGen Editor alpha 1.0"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 729,376 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~FxGenFrame();
};


#endif // FXGENFRAME_H_INCLUDED
