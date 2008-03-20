#ifndef PROJECTTREEWIDGET_H_INCLUDED
#define PROJECTTREEWIDGET_H_INCLUDED

#include <wx/wx.h>
#include <wx/treectrl.h>

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

  //FxGen Events
    DECLARE_EVENT_TABLE()
    virtual void OnFxGenProjectLoaded( wxCommandEvent& event );
    virtual void OnFxGenProjectNew( wxCommandEvent& event );
};


/*
class QProjectsTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    QProjectsTreeWidget(QWidget *parent = 0);

//Send Signals
signals:
	void FxGenProjectSelPageChanged(NOperatorsPage* _ppage);

//Received signals
public slots:
	void FxGenProjectLoaded();

protected:
	//Methods
	void DisplayFxGenProject(NTreeNode* _prootNode);
	void _AddItemsFromTreeNodes(NTreeNode* _pparentNode, QTreeWidgetItem* _pparentItem);
	NOperatorsPage* GetSelectedPage();

	virtual void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );


	//Datas
	NTreeNode* m_prootNode;	//!< Root Node Ptr
};
*/


#endif // PROJECTTREEWIDGET_H_INCLUDED
