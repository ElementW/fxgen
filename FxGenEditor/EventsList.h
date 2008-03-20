#ifndef EVENTSLIST_H_INCLUDED
#define EVENTSLIST_H_INCLUDED

//Events Identifiers
#define   wsID_FXGEN_PRJ_NEW         wxID_HIGHEST + 1
#define   wsID_FXGEN_PRJ_LOADED      wxID_HIGHEST + 2

//Events Types
BEGIN_DECLARE_EVENT_TYPES()
  DECLARE_EVENT_TYPE(wxEVT_FXGEN_PROJECT, wxID_ANY)
END_DECLARE_EVENT_TYPES()

//Externals
extern const wxEventType wxEVT_FXGEN_PROJECT;



#endif // EVENTSLIST_H_INCLUDED
