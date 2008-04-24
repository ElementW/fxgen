///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 20 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* m_menuItemNew;
	m_menuItemNew = new wxMenuItem( fileMenu, wxID_NEW, wxString( wxT("New") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( m_menuItemNew );

	wxMenuItem* m_menuItemOpen;
	m_menuItemOpen = new wxMenuItem( fileMenu, wxID_OPEN, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( m_menuItemOpen );

	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( fileMenu, wxID_SAVE, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( m_menuItemSave );

	wxMenuItem* m_menuItemSaveAs;
	m_menuItemSaveAs = new wxMenuItem( fileMenu, wxID_SAVEAS, wxString( wxT("Save as...") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( m_menuItemSaveAs );

	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );

	mbar->Append( fileMenu, wxT("&File") );

	m_menuOptions = new wxMenu();
	m_menuDetails = new wxMenu();
	wxMenuItem* m_menuItemLow;
	m_menuItemLow = new wxMenuItem( m_menuDetails, wxID_DETAILLOW, wxString( wxT("Low") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuDetails->Append( m_menuItemLow );

	wxMenuItem* m_menuItemNormal;
	m_menuItemNormal = new wxMenuItem( m_menuDetails, wxID_DETAILNORMAL, wxString( wxT("Normal") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuDetails->Append( m_menuItemNormal );

	wxMenuItem* m_menuItemHigh;
	m_menuItemHigh = new wxMenuItem( m_menuDetails, wxID_DETAILHIGH, wxString( wxT("High") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuDetails->Append( m_menuItemHigh );

	m_menuOptions->Append( -1, wxT("Texture Details"), m_menuDetails );

	wxMenuItem* m_menuItemPlugins;
	m_menuItemPlugins = new wxMenuItem( m_menuOptions, wxID_PLUGINS, wxString( wxT("Plugins...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuOptions->Append( m_menuItemPlugins );

	mbar->Append( m_menuOptions, wxT("Options") );

	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );

	mbar->Append( helpMenu, wxT("&Help") );

	this->SetMenuBar( mbar );

	m_toolBarMain = this->CreateToolBar( wxTB_DOCKABLE|wxTB_HORIZONTAL, wxID_TOOLBARMAIN );
	m_toolBarMain->SetToolBitmapSize( wxSize( 32,32 ) );
	m_toolBarMain->AddTool( wxID_NEW, wxT("New"), wxBitmap( wxT("Images\\new.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBarMain->AddTool( wxID_OPEN, wxT("Open"), wxBitmap( wxT("Images\\open.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBarMain->AddTool( wxID_SAVE, wxT("Save"), wxBitmap( wxT("Images\\save.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	m_toolBarMain->Realize();

	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Connect( m_menuItemNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnNew ) );
	this->Connect( m_menuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnOpen ) );
	this->Connect( m_menuItemSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnSave ) );
	this->Connect( m_menuItemSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnSaveAs ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	this->Connect( wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnOpen ) );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnNew ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnSave ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnSaveAs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	this->Disconnect( wxID_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnOpen ) );
}
