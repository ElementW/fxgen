#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/clrpicker.h"
#include "wx/tokenzr.h"

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
#include "..\Src\FxGenLib\EngineOp.h"
#include "float.h"

const wxChar wxPropertiesCtrlNameStr[] = wxT("PropertiesCtrl");

BEGIN_EVENT_TABLE(wxPropertiesCtrl, wxControl)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxPropertiesCtrl, wxControl)

wxPropertiesCtrl::wxPropertiesCtrl(wxWindow *parent, wxWindowID id,
               const wxPoint& pos, const wxSize& size, long style,
               const wxValidator &validator,  const wxString& name)
    : wxControl(parent, id, pos, size, style, validator, name)
{
  parent->Connect(wsID_FXGEN_OPSEL_CHANGED, wxEVT_FXGEN_OPERATORSCTRL, wxCommandEventHandler( wxPropertiesCtrl::OnOperatorSelectionChanged ),0,this);

  m_psizer = new wxBoxSizer( wxVERTICAL );
  SetSizer( m_psizer );
}

class DragSpinner : public wxPanel
{
public:
  DragSpinner(wxWindow* _parent)
    : wxPanel( _parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN )
  {
    SetBackgroundStyle(wxBG_STYLE_CUSTOM ); //Added for double buffer

    Connect(wxEVT_PAINT, wxPaintEventHandler(DragSpinner::OnPaint));
    Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseCaptureLostEventHandler(DragSpinner::OnCaptureLost));
    Connect(wxEVT_MOUSE_CAPTURE_CHANGED, wxMouseCaptureChangedEventHandler(DragSpinner::OnCaptureChanged));
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DragSpinner::OnMouse));
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(DragSpinner::OnMouse));
    Connect(wxEVT_MOTION, wxMouseEventHandler(DragSpinner::OnMouse));

    SetMinSize(wxSize(32, 16));
    m_offset = 0;

    m_textColor = *wxBLACK;
  }

  void SetText(const wxString& _text, const wxColor& _color = *wxBLACK)
  {
    m_text = _text;
    m_textColor = _color;
    Refresh();
  }

  void OnMouse(wxMouseEvent& _event)
  {
    if(_event.LeftDown())
    {
      SetCursor(wxCURSOR_SIZEWE);
      if(!HasCapture())
        CaptureMouse();

      m_lastMouseX = _event.GetX();
    } else if(_event.LeftUp())
    {
      if(HasCapture())
        ReleaseMouse();
      SetCursor(wxCURSOR_DEFAULT);
    } else if(_event.Dragging())
    {
      if(HasCapture())
      {
        sdword move = _event.GetX()-m_lastMouseX;
        move *= 1+(abs(move)/5); // Get some acceleration when moving fast
        m_offset += move;
        m_lastMouseX = _event.GetX();
        Refresh();

        wxScrollEvent scrollEvent(wxEVT_SCROLL_CHANGED, GetId(), move);
        scrollEvent.SetEventObject(this);
        wxPostEvent(this, scrollEvent);
      }
    }
  }

  void OnCaptureLost(wxMouseCaptureLostEvent& _event)
  {
    if(HasCapture())
      ReleaseMouse();
  }

  void OnCaptureChanged(wxMouseCaptureChangedEvent& _event)
  {
    if(HasCapture())
      ReleaseMouse();
  }

  void OnPaint(wxPaintEvent& _event)
  {
    wxAutoBufferedPaintDC dc(this);

    sdword w = GetSize().GetWidth();
    sdword h = GetSize().GetHeight();

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW)));
    dc.DrawRectangle(0, 0, w, h);
    dc.SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)));
    for(sdword i=-1; i<(w/16)+1; ++i)
      dc.DrawRectangle(i*16+(m_offset%16), 0, 8, h);

    if(m_text.Length()>0)
    {
      wxSize textExtent = dc.GetTextExtent(m_text);
      dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

      // Shadow
      dc.SetTextForeground(wxColor(m_textColor.Red()/2, m_textColor.Green()/2, m_textColor.Blue()/2));
      dc.DrawText( m_text, (w-textExtent.GetWidth())/2, (h-textExtent.GetHeight())/2 );

      // Text
      dc.SetTextForeground(m_textColor);
      dc.DrawText( m_text, (w-textExtent.GetWidth())/2, (h-textExtent.GetHeight())/2-1 );
    }
  }

private:
  sdword m_lastMouseX;
  sdword m_offset;
  wxString m_text;
  wxColor m_textColor;
};

class PropertyControl : public wxPanel
{
  DECLARE_ABSTRACT_CLASS( PropertyControl );
public:
  virtual void Create( wxWindow* _parent, NOperator* _pop, udword _index ) = 0;
};

template<typename ContainedType, int Variant = 0> class NumericPropControl : public PropertyControl
{
public:
  void Create( wxWindow* _pParent, NOperator* _pop, udword _dwIndex )
  {
    m_pop = _pop;
    m_dwIndex = _dwIndex;

    wxPanel::Create( _pParent );

    m_pSizer = new wxBoxSizer( wxHORIZONTAL );
    SetSizer( m_pSizer );

    //m_psizer->Add( new wxStaticText( this, wxID_ANY, wxString::FromAscii( m_pop->m_pcvarsBloc->GetBlocDesc()[m_index].pszName ) + ":" ), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT, 4 );
    m_pText = new wxTextCtrl( this, wxID_ANY, wxT( "" ), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxTextValidator( wxFILTER_NUMERIC, 0 ) );
    m_pSizer->Add( m_pText, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND );
    m_pSizer->Add( new DragSpinner(this), 0, wxALIGN_CENTER_VERTICAL );

    ContainedType value;
    m_pop->m_pcvarsBloc->GetValue( m_dwIndex, 0.0f, value );
    m_pText->SetValue( wxString() << value );

    m_pSizer->Layout();

    m_pText->Connect( wxEVT_COMMAND_TEXT_ENTER, (wxObjectEventFunction)&NumericPropControl<ContainedType, Variant>::OnText, 0, this );

    Connect(wxEVT_SCROLL_CHANGED, (wxObjectEventFunction)&NumericPropControl<ContainedType, Variant>::OnScroll);
  }

  void OnScroll(wxScrollEvent& _event)
  {
    ContainedType value = 0;
    m_pop->m_pcvarsBloc->GetValue( m_dwIndex, 0.0f, value );
    if(value+_event.GetPosition()*m_valueScale < m_minValue)
      value = m_minValue;
    else if(value+_event.GetPosition()*m_valueScale > m_maxValue)
      value = m_maxValue;
    else
      value += _event.GetPosition()*m_valueScale;
    m_pop->m_pcvarsBloc->SetValue( m_dwIndex, 0.0f, value );
    m_pText->SetValue( wxString() << value );
  }

  void OnText( wxCommandEvent& _event )
  {
    double doublevalue;
    m_pText->GetValue().ToDouble( &doublevalue );
    if(doublevalue < m_minValue)
      doublevalue = m_minValue;
    else if(doublevalue > m_maxValue)
      doublevalue = m_maxValue;
    ContainedType value = (ContainedType)doublevalue;
    m_pop->m_pcvarsBloc->SetValue( m_dwIndex, 0.0f, value );
    m_pText->SetValue( wxString() << value );
  }

protected:
  udword m_dwIndex;
  NOperator* m_pop;

  wxTextCtrl* m_pText;
  wxSizer* m_pSizer;

  static ContainedType m_minValue;
  static ContainedType m_maxValue;
  static ContainedType m_valueScale;
};

class NColorPropControl : public PropertyControl
{
  DECLARE_DYNAMIC_CLASS( NColorPropControl );

public:
  void Create( wxWindow* _pParent, NOperator* _pop, udword _dwIndex )
  {
    m_pop = _pop;
    m_dwIndex = _dwIndex;

    wxPanel::Create( _pParent );

    m_pSizer = new wxBoxSizer( wxHORIZONTAL );
    SetSizer( m_pSizer );

    m_pColorPicker = new wxColourPickerCtrl( this, wxID_ANY );
    m_pSizer->Add( m_pColorPicker, 0, wxALIGN_CENTER_VERTICAL | wxEXPAND );

    m_pSpinners[0] = new DragSpinner( this );
    m_pSpinners[1] = new DragSpinner( this );
    m_pSpinners[2] = new DragSpinner( this );
    m_pSpinners[3] = new DragSpinner( this );

    m_pSizer->Add( m_pSpinners[0], 0, wxALIGN_CENTER_VERTICAL );
    m_pSizer->Add( m_pSpinners[1], 0, wxALIGN_CENTER_VERTICAL );
    m_pSizer->Add( m_pSpinners[2], 0, wxALIGN_CENTER_VERTICAL );
    m_pSizer->Add( m_pSpinners[3], 0, wxALIGN_CENTER_VERTICAL );

    UpdateControls();

    m_pSizer->Layout();

    Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(NColorPropControl::OnScroll));
    Connect(wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler(NColorPropControl::OnColorPicker));
  }

  void OnColorPicker(wxColourPickerEvent& _event)
  {
    wxColor color = m_pColorPicker->GetColour();
    ubyte red = color.Red();
    ubyte green = color.Green();
    ubyte blue = color.Blue();

    udword dwColor;
    m_pop->m_pcvarsBloc->GetValue( m_dwIndex, 0.0f, dwColor );

    dwColor &= 0xFF000000;
    dwColor |= red | (green << 8) | (blue << 16);

    m_pop->m_pcvarsBloc->SetValue( m_dwIndex, 0.0f, dwColor );
    UpdateControls();
  }

  void OnScroll(wxScrollEvent& _event)
  {
    udword dwColor;
    m_pop->m_pcvarsBloc->GetValue( m_dwIndex, 0.0f, dwColor );

    int index = -1;
    for( unsigned int i=0; i<4; ++i )
    {
      if(m_pSpinners[i] == _event.GetEventObject())
      {
        index = i;
        break;
      }
    }

    if(index != -1)
    {
      sdword delta = _event.GetPosition();
      sdword dwComponent = (dwColor >> (index*8)) & 0xFF;
      if(dwComponent+delta < 0)
        dwComponent = 0;
      else if(dwComponent+delta > 255)
        dwComponent = 255;
      else
        dwComponent += delta;

      dwColor &= ~(0xFF<<(index*8));
      dwColor |= dwComponent << (index*8);

      m_pop->m_pcvarsBloc->SetValue( m_dwIndex, 0.0f, dwColor );
      UpdateControls();
    }
  }

  void UpdateControls()
  {
    udword dwColor;
    m_pop->m_pcvarsBloc->GetValue( m_dwIndex, 0.0f, dwColor );
    ubyte red = (dwColor & 0xFF);
    ubyte green = ((dwColor >> 8) & 0xFF);
    ubyte blue = ((dwColor >> 16) & 0xFF);
    ubyte alpha = ((dwColor >> 24) & 0xFF);
    // Different color values is because colors are not equally intense
    m_pSpinners[0]->SetText( wxString() << red, wxColor(192, 0, 0) );
    m_pSpinners[1]->SetText( wxString() << green, wxColor(0, 128, 0) );
    m_pSpinners[2]->SetText( wxString() << blue, wxColor(0, 0, 255) );
    m_pSpinners[3]->SetText( wxString() << alpha, wxColor(128, 128, 128) );

    m_pColorPicker->SetColour( wxColor( red, green, blue, alpha ) );
  }

protected:
  udword m_dwIndex;
  NOperator* m_pop;

  DragSpinner* m_pSpinners[4];
  wxColourPickerCtrl* m_pColorPicker;
  wxSizer* m_pSizer;
};

class NUbyteComboPropControl : public PropertyControl
{
  DECLARE_DYNAMIC_CLASS( NUbyteComboPropControl );

public:
  void Create( wxWindow* _pParent, NOperator* _pop, udword _dwIndex )
  {
    m_pop = _pop;
    m_dwIndex = _dwIndex;

    wxPanel::Create( _pParent );

    m_pSizer = new wxBoxSizer( wxHORIZONTAL );
    SetSizer( m_pSizer );

    m_pComboBox = new wxComboBox( this, wxID_ANY, wxT( "" ), wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY | wxCB_DROPDOWN );
    m_pSizer->Add( m_pComboBox, 0, wxALIGN_CENTER_VERTICAL | wxEXPAND );

    wxStringTokenizer tokenizer( wxString::FromAscii( m_pop->m_pcvarsBloc->GetBlocDesc()[m_dwIndex].pszDefValue ), wxT(",[]") );
    wxString defaultValue = tokenizer.GetNextToken();
    while(tokenizer.HasMoreTokens())
    {
      m_pComboBox->Append(tokenizer.GetNextToken());
    }
    m_pComboBox->Delete(0); // Delete default item

    ubyte bValue;
    m_pop->m_pcvarsBloc->GetValue(m_dwIndex, 0.0f, bValue);
    m_pComboBox->SetSelection(bValue);

    m_pSizer->Layout();

    Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(NUbyteComboPropControl::OnComboBox));
  }

  void OnComboBox(wxCommandEvent& _event)
  {
    ubyte bValue = m_pComboBox->GetSelection();
    m_pop->m_pcvarsBloc->SetValue(m_dwIndex, 0.0f, bValue);
  }

protected:
  udword m_dwIndex;
  NOperator* m_pop;

  wxComboBox* m_pComboBox;
  wxSizer* m_pSizer;
};

class NUbytePropControl : public NumericPropControl<ubyte, 0>
{
  DECLARE_DYNAMIC_CLASS( NUbytePropControl );
};
template<typename ContainedType, int Variant> ubyte NumericPropControl<ubyte, 0>::m_minValue = 0;
template<typename ContainedType, int Variant> ubyte NumericPropControl<ubyte, 0>::m_maxValue = 255;
template<typename ContainedType, int Variant> ubyte NumericPropControl<ubyte, 0>::m_valueScale = 1;

class NUwordPropControl : public NumericPropControl<uword, 0>
{
  DECLARE_DYNAMIC_CLASS( NUwordPropControl );
};
template<typename ContainedType, int Variant> uword NumericPropControl<uword, 0>::m_minValue = 0;
template<typename ContainedType, int Variant> uword NumericPropControl<uword, 0>::m_maxValue = 65535;
template<typename ContainedType, int Variant> uword NumericPropControl<uword, 0>::m_valueScale = 1;

class NUdwordPropControl : public NumericPropControl<udword, 0>
{
  DECLARE_DYNAMIC_CLASS( NUdwordPropControl );
};
template<typename ContainedType, int Variant> udword NumericPropControl<udword, 0>::m_minValue = 0;
template<typename ContainedType, int Variant> udword NumericPropControl<udword, 0>::m_maxValue = 4294967295;
template<typename ContainedType, int Variant> udword NumericPropControl<udword, 0>::m_valueScale = 1;

class NFloatPropControl : public NumericPropControl<float, 0>
{
  DECLARE_DYNAMIC_CLASS( NFloatPropControl );
};
template<typename ContainedType, int Variant> float NumericPropControl<float, 0>::m_minValue = -FLT_MAX;
template<typename ContainedType, int Variant> float NumericPropControl<float, 0>::m_maxValue = FLT_MAX;
template<typename ContainedType, int Variant> float NumericPropControl<float, 0>::m_valueScale = 0.001f;

class NCFloatPropControl : public NumericPropControl<float, 1>
{
  DECLARE_DYNAMIC_CLASS( NCFloatPropControl );
};
template<typename ContainedType, int Variant> float NumericPropControl<float, 1>::m_minValue = 0.0f;
template<typename ContainedType, int Variant> float NumericPropControl<float, 1>::m_maxValue = 1.0f;
template<typename ContainedType, int Variant> float NumericPropControl<float, 1>::m_valueScale = 0.001f;

IMPLEMENT_ABSTRACT_CLASS( PropertyControl, wxPanel );
IMPLEMENT_DYNAMIC_CLASS( NUbytePropControl, PropertyControl );
IMPLEMENT_DYNAMIC_CLASS( NUwordPropControl, PropertyControl );
IMPLEMENT_DYNAMIC_CLASS( NUdwordPropControl, PropertyControl );
IMPLEMENT_DYNAMIC_CLASS( NFloatPropControl, PropertyControl );
IMPLEMENT_DYNAMIC_CLASS( NCFloatPropControl, PropertyControl );
IMPLEMENT_DYNAMIC_CLASS( NColorPropControl, PropertyControl );
IMPLEMENT_DYNAMIC_CLASS( NUbyteComboPropControl, PropertyControl );

//-----------------------------------------------------------------
//!	\brief	Event Operator Selection Changed
//-----------------------------------------------------------------
void wxPropertiesCtrl::OnOperatorSelectionChanged(wxCommandEvent& event)
{
  Update((NOperator*)event.GetClientData());
}

void wxPropertiesCtrl::Update(NOperator* _pop)
{
  m_psizer->Clear( true );

  Freeze();

  int maxWidth = 0;
  if( _pop && _pop->m_pcvarsBloc )
  {
    // Couldn't get wxFlexGridSizer working properly with alignment, hence a weird scheme calculating the max width
    // of the left column
    for( unsigned int i=0; i<_pop->m_pcvarsBloc->Count(); ++i )
    {
      wxBoxSizer* pRowSizer = new wxBoxSizer( wxHORIZONTAL );
      wxStaticText* pText = new wxStaticText( this, wxID_ANY, wxString::FromAscii( _pop->m_pcvarsBloc->GetBlocDesc()[i].pszName ) /*+ ":"*/, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
      if(pText->GetSize().GetWidth() > maxWidth)
        maxWidth = pText->GetSize().GetWidth();
      pRowSizer->Add( pText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT, 4 );
      wxClassInfo* classInfo = wxClassInfo::FindClass( wxString::FromAscii( _pop->m_pcvarsBloc->GetBlocDesc()[i].pszCLASSGUI ) + wxT( "Control" ) );
      wxWindow* pControlWindow;
      if( classInfo )
      {
        PropertyControl* pControl = wxDynamicCast( classInfo->CreateObject(), PropertyControl );
        pControl->Create( this, _pop, i );
        pControlWindow = pControl;
      } else {
        pControlWindow = new wxStaticText( this, 0, wxT( "No control found for type '" ) + wxString::FromAscii( _pop->m_pcvarsBloc->GetBlocDesc()[i].pszCLASSGUI ) /*+ "'"*/ );
      }
      pRowSizer->Add( pControlWindow, 1, wxEXPAND );
      m_psizer->Add( pRowSizer, 0 );
    }

    // Apply max width here, to get an even left column
    for( unsigned int i=0; i<_pop->m_pcvarsBloc->Count(); ++i )
    {
      wxSizer* pRowSizer = m_psizer->GetItem(i)->GetSizer();
      wxWindow* pLabelWindow = pRowSizer->GetItem((unsigned int)0)->GetWindow();
      pLabelWindow->SetMinSize(wxSize(maxWidth, pLabelWindow->GetMinSize().GetWidth()));
    }
  }

  m_psizer->Layout();

  Thaw();
}
