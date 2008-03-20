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

wxProjectTreeCtrl::wxProjectTreeCtrl(wxWindow *parent, wxWindowID id,
               const wxPoint& pos, const wxSize& size, long style,
               const wxValidator &validator,  const wxString& name)
    : wxTreeCtrl(parent, id, pos, size, style, validator, name)
{
     //wxMessageBox(_("Constructor02"), _("FxGen"));
  this->Connect(wsID_FXGEN_PRJ_LOADED, wxEVT_FXGEN_PROJECT, wxCommandEventHandler( wxProjectTreeCtrl::OnFxGenProjectLoaded ));
  this->Connect(wsID_FXGEN_PRJ_NEW, wxEVT_FXGEN_PROJECT, wxCommandEventHandler( wxProjectTreeCtrl::OnFxGenProjectNew ));
}

void wxProjectTreeCtrl::OnFxGenProjectLoaded( wxCommandEvent& event )
{
  wxMessageBox(_("Project Loaded hi"), _("FxGen"));
}

void wxProjectTreeCtrl::OnFxGenProjectNew( wxCommandEvent& event )
{
  wxMessageBox(_("Project Loaded new"), _("FxGen"));
}


/*
//-----------------------------------------------------------------
//!	\brief	Slot for FxGenProjectLoading
//-----------------------------------------------------------------
void QProjectsTreeWidget::FxGenProjectLoaded()
{
	//QMessageBox::warning(this, tr("Application"), tr("QProjectsTreeWidget::FxGenProjectLoaded"));

	//Get Project RootNode
	NEngineOp* peng = NEngineOp::GetEngine();
	NTreeNode* pRoot = peng->GetRootGroup();

	//Display Tree
	DisplayFxGenProject(pRoot);

}

//-----------------------------------------------------------------
//!	\brief	Display a FxGenEngine Project
//!	\param	_prootNode	Root node
//-----------------------------------------------------------------
void QProjectsTreeWidget::DisplayFxGenProject(NTreeNode* _prootNode)
{
	//Clear-Init
	this->clear();
	setColumnCount(1);
	this->

	m_prootNode = _prootNode;

	//Make Items From TreeNodes
	NTreeNode* pnodeRoot = _prootNode;
	QTreeWidgetItem* pitemRoot = new QTreeWidgetItem((QTreeWidget*)0);
	pitemRoot->setText(0, "Root");
	_AddItemsFromTreeNodes(pnodeRoot, pitemRoot);

	//Display Items
	addTopLevelItem((QTreeWidgetItem*)pitemRoot);


}

void QProjectsTreeWidget::_AddItemsFromTreeNodes(NTreeNode* _pparentNode, QTreeWidgetItem* _pparentItem)
{
	//Sons
	NTreeNode* pnode = _pparentNode->GetSon();
	while (pnode)
	{
		//Parent
		QTreeWidgetItem* pitem = new QTreeWidgetItem(_pparentItem);
		pitem->setText(0, pnode->GetName());

		//Objects array
		NObjectArray& array = pnode->GetObjsArray();
		for (udword i=0; i<array.Count(); i++)
		{
			NObject* pobj = array[i];
			QTreeWidgetItem* pitemObs = new QTreeWidgetItem(pitem);
			pitemObs->setText(0, pobj->GetName());
		}

		//Son Nodes
		_AddItemsFromTreeNodes(pnode, pitem);

		//Next brothers nodes
		pnode = pnode->GetBrother();
	}

}

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
