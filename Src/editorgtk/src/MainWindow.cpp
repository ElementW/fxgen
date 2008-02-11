//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		SmartWindow.cpp
//! \brief	FxGen GTK editor - application main window
//!
//!	\author	Sebastian Olter (qduaty@gmail.com)
//!	\date		07-02-2008
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

#include "pch.h"
#include "MainWindow.h"
#include "fxgen_pch.h"
#include "globals.h"

MainWindow::MainWindow(GtkWindow* cobject, const RefPtr<Xml>& refGlade)
:Gtk::Window(cobject)
{
	//ctor
}

MainWindow::~MainWindow()
{
	//dtor
}

bool MainWindow::on_delete_event(GdkEventAny*)
{
	if(project_modified && ConfirmProjectLoss() != Gtk::RESPONSE_OK)
		return true;
	return false;
}

int MainWindow::ConfirmProjectLoss()
{
	Gtk::MessageDialog dlg("Your project contains unsaved data.", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL, true);
	dlg.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_APPLY);
	int response = dlg.run();
	switch(response)
	{
		case Gtk::RESPONSE_APPLY:
			SaveProject(filename);
			return Gtk::RESPONSE_OK;
		break;
		default:
			return response;
	}
	return Gtk::RESPONSE_CANCEL;
}

void MainWindow::QuickSaveProject()
{
	SaveProject(filename);
}


void MainWindow::SaveProject(string _filename)
{
	if(_filename.empty())
	{
		Gtk::FileChooserDialog dlg(*(Gtk::Window*)this, "Save", Gtk::FILE_CHOOSER_ACTION_SAVE);
		dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dlg.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
		dlg.add_filter(filter_fxgen);
		dlg.add_filter(filter_all);
		int response = dlg.run();
		if(response == Gtk::RESPONSE_OK)
			_filename = dlg.get_filename();
		else return;
	}

    NEngineOp* engine = NEngineOp::GetEngine();
    if (engine->SaveProject(_filename.c_str()))
    {
    	filename = _filename;
		project_modified = false;
		set_title(Glib::filename_display_basename(filename));
    }
	else
		Gtk::MessageDialog dlg("Problem saving file.", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
}

void MainWindow::ClearProject()
{
	if(project_modified)
	{
		if(window->ConfirmProjectLoss() == Gtk::RESPONSE_CANCEL)
			return;
	}

	window->filename.clear();

    NEngineOp* engine = NEngineOp::GetEngine();
	engine->Clear();
	project_tree->clear();
	;
}

void MainWindow::LoadProject(string _filename, bool append)
{
	if(project_modified)
	{
		if(window->ConfirmProjectLoss() == Gtk::RESPONSE_CANCEL)
			return;
	}

	if(_filename.empty() || !Glib::file_test(_filename, Glib::FILE_TEST_EXISTS))
	{
		Gtk::FileChooserDialog dlg(*window, "Open", Gtk::FILE_CHOOSER_ACTION_OPEN);
		dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dlg.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
		dlg.add_filter(filter_fxgen);
		dlg.add_filter(filter_all);
		int response = dlg.run();
		if(response == Gtk::RESPONSE_OK)
			_filename = dlg.get_filename();
		else return;
	}

    NEngineOp* engine = NEngineOp::GetEngine();
    if (engine->LoadProject(_filename.c_str()))
    {
    	filename = _filename;
		window->set_title(Glib::filename_display_basename(filename));
		project_modified = false;
		project_tree->DisplayProject(NEngineOp::GetEngine());
	}
	else
		ClearProject();
}
