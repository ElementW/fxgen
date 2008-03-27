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
	wxTreeItemId pitemRoot = AddRoot(_T("Root"));
	_AddItemsFromTreeNodes(m_prootNode, pitemRoot);

}

void wxProjectTreeCtrl::_AddItemsFromTreeNodes(NTreeNode* _pparentNode, wxTreeItemId _pparentItem)
{
	//Sons
	NTreeNode* pnode = _pparentNode->GetSon();
	while (pnode)
	{
		//Parent
		wxTreeItemId pitem = AppendItem(_pparentItem, wxString::FromAscii(pnode->GetName()) );//new QTreeWidgetItem(_pparentItem);

		//Objects array
		NObjectArray& array = pnode->GetObjsArray();
		for (udword i=0; i<array.Count(); i++)
		{
			NObject* pobj = array[i];
			wxTreeItemId pitemObs = AppendItem(pitem, wxString::FromAscii(pobj->GetName()) );//new QTreeWidgetItem(pitem);
		}

		//Son Nodes
		_AddItemsFromTreeNodes(pnode, pitem);

		//Next brothers nodes
		pnode = pnode->GetBrother();
	}

}

/*
void QProjectsTreeWidget::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{
	//QMessageBox::warning(this, tr("Application"), tr("QProjectsTreeWidget::currentItemChanged"));

	//GetFxGen Page
	NOperatorsPage* ppage = GetSelectedPage();

	///Emit Signal
	emit FxGenProjectSelPageChanged(ppage);
}


NOperatorsPage* QProjectsTreeWidget::GetSelectedPage()
{
	QTreeWidgetItem* pitem = currentItem();


	//Get Selected Object
	NObject* pobj = GetSelectedItemObj();

	//Check if selected item is a TreeNode
	if (pobj!=null)
	{
		if (strcmp(pobj->GetRTClass()->m_pszClassName, "NOperatorsPage")==0 )
			return (NOperatorsPage*)pobj;
	}

	return null;
}
*/
