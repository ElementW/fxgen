#ifndef OPERATORRESULTCTRL_H_INCLUDED
#define OPERATORRESULTCTRL_H_INCLUDED

#include <wx/wx.h>
#include <wx/control.h>

//extern WXDLLEXPORT_DATA(const wxChar) wxOperatorResultCtrlNameStr[];
extern const wxChar wxOperatorResultCtrlNameStr[];

class wxOperatorResultCtrl : public wxControl
{
    DECLARE_DYNAMIC_CLASS(wxOperatorResultCtrl)

public:
    wxOperatorResultCtrl() {}

    wxOperatorResultCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxOperatorResultCtrlNameStr);

  //FxGen Events
    DECLARE_EVENT_TABLE()
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


#endif // OPERATORRESULTCTRL_H_INCLUDED
