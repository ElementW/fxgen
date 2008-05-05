#include "wx/wx.h"
#include "EventsList.h"

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__


#include "EventsList.h"
#include "ProjectTreeCtrl.h"

//#include "..\Src\FxGenLib\Core.h"
//#include "..\Src\FxGenLib\EngineOp.h"

BEGIN_EVENT_TABLE(wxProjectTreeCtrl, wxTreeCtrl)
//EVT_COMMAND  (wsID_FXGEN_PRJ_NEW, wxEVT_FXGEN_PROJECT, wxProjectTreeCtrl::OnFxGenProjectNew)
  EVT_TREE_SEL_CHANGED(-1, wxProjectTreeCtrl::OnTreeSelChanged)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxProjectTreeCtrl, wxTreeCtrl)

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
wxProjectTreeCtrl::wxProjectTreeCtrl(wxWindow *parent, wxWindowID id,
               const wxPoint& pos, const wxSize& size, long style,
               const wxValidator &validator,  const wxString& name)
    : wxTreeCtrl(parent, id, pos, size, style, validator, name)
{
     //wxMessageBox(_("Constructor02"), _("FxGen"));
  parent->Connect(wsID_FXGEN_PRJ_LOADED, wxEVT_FXGEN_PROJECT, wxCommandEventHandler( wxProjectTreeCtrl::OnFxGenProjectLoaded ),0,this);
  parent->Connect(wsID_FXGEN_PRJ_NEW, wxEVT_FXGEN_PROJECT, wxCommandEventHandler( wxProjectTreeCtrl::OnFxGenProjectNew ),0,this);
}

//-----------------------------------------------------------------
//!	\brief	FxGen event
//-----------------------------------------------------------------
void wxProjectTreeCtrl::OnFxGenProjectLoaded( wxCommandEvent& event )
{
  //wxMessageBox(_("wxProjectTreeCtrl Project Loaded"), _("FxGen"));

	//Get Project RootNode
	NEngineOp* peng = NEngineOp::GetEngine();
	NTreeNode* pRoot = peng->GetRootGroup();

	//Display Tree
	DisplayFxGenProject(pRoot);

}

//-----------------------------------------------------------------
//!	\brief	FxGen event
//-----------------------------------------------------------------
void wxProjectTreeCtrl::OnFxGenProjectNew( wxCommandEvent& event )
{
  DeleteAllItems();
  //wxMessageBox(_("wxProjectTreeCtrl Project New"), _("FxGen"));
}


//-----------------------------------------------------------------
//!	\brief	Display a FxGenEngine Project
//!	\param	_prootNode	Root node
//-----------------------------------------------------------------
void wxProjectTreeCtrl::DisplayFxGenProject(NTreeNode* _prootNode)
{
  //Clear-Init
  DeleteAllItems();
	m_prootNode = _prootNode;

	//Make Items From TreeNodes
	wxFxGenItemData* pitemData = new wxFxGenItemData(NULL);
	wxTreeItemId pitemRoot = AddRoot(_T(".Root"),-1,-1,pitemData);
	_AddItemsFromTreeNodes(m_prootNode, pitemRoot);

}

void wxProjectTreeCtrl::_AddItemsFromTreeNodes(NTreeNode* _pparentNode, wxTreeItemId _pparentItem)
{
	//Sons
	NTreeNode* pnode = _pparentNode->GetSon();
	while (pnode)
	{
		//Parent
    wxFxGenItemData* pitemData = new wxFxGenItemData(pnode);
		wxTreeItemId pitem = AppendItem(_pparentItem, wxString::FromAscii(pnode->GetName()),-1,-1, pitemData );

		//Objects array
		NObjectArray& array = pnode->GetObjsArray();
		for (udword i=0; i<array.Count(); i++)
		{
			NObject* pobj = array[i];
	    wxFxGenItemData* pitemData = new wxFxGenItemData(pobj);
			wxTreeItemId pitemObs = AppendItem(pitem, wxString::FromAscii(pobj->GetName()),-1,-1, pitemData );
		}

		//Son Nodes
		_AddItemsFromTreeNodes(pnode, pitem);

		//Next brothers nodes
		pnode = pnode->GetBrother();
	}

}


//-----------------------------------------------------------------
//!	\brief	Return Selected Operator Page
//-----------------------------------------------------------------
NOperatorsPage* wxProjectTreeCtrl::GetSelectedPage()
{
	wxTreeItemId itemSel = GetSelection();

	//Get Selected Object
	NObject* pobj = ((wxFxGenItemData*)GetItemData(itemSel))->m_pobj;

	//Check if selected item is a NOperatorsPage
	if (pobj!=null)
	{
		if (strcmp(pobj->GetRTClass()->m_pszClassName, "NOperatorsPage")==0 )
			return (NOperatorsPage*)pobj;
	}

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Event Tree Item Sel Changed
//-----------------------------------------------------------------
void wxProjectTreeCtrl::OnTreeSelChanged(wxTreeEvent& event)
{
  NOperatorsPage* ppage = GetSelectedPage();
  if (ppage)
  {
    wxCommandEvent  fxevent( wxEVT_FXGEN_PROJECT, wsID_FXGEN_PAGE_CHANGED);
    fxevent.SetClientData(ppage);
    fxevent.SetEventObject( this );
    wxPostEvent( GetEventHandler(), fxevent );
  }

}
