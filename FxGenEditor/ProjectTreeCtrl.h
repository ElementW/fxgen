#ifndef PROJECTTREEWIDGET_H_INCLUDED
#define PROJECTTREEWIDGET_H_INCLUDED

#include <wx/wx.h>
#include <wx/treectrl.h>

#include "..\Src\FxGenLib\Core.h"
#include "..\Src\FxGenLib\EngineOp.h"


class wxFxGenItemData : public wxTreeItemData
{
  public:
    wxFxGenItemData(NObject* _pobj) : wxTreeItemData()
    {
      m_pobj = _pobj;
    }

    NObject* m_pobj;
};

class wxProjectTreeCtrl : public wxTreeCtrl
{
    DECLARE_DYNAMIC_CLASS(wxProjectTreeCtrl)

public:
    wxProjectTreeCtrl() {}

    wxProjectTreeCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxTreeCtrlNameStr);

  //Events
  DECLARE_EVENT_TABLE()
  void OnTreeSelChanged(wxTreeEvent& event);

  //FxGen Events
  virtual void OnFxGenProjectLoaded( wxCommandEvent& event );
  virtual void OnFxGenProjectNew( wxCommandEvent& event );

  //Methods
  void DisplayFxGenProject(NTreeNode* _prootNode);
  void _AddItemsFromTreeNodes(NTreeNode* _pparentNode, wxTreeItemId _pparentItem);
	NOperatorsPage* GetSelectedPage();

  //Datas
  NTreeNode *m_prootNode;
};

#endif // PROJECTTREEWIDGET_H_INCLUDED
