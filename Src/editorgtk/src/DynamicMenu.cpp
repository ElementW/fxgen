//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DynamicMenu.cpp
//! \brief	FxGen GTK editor - contextual menu controller
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
#include "globals.h"

DynamicMenu::DynamicMenu(MainWindow& w): window(&w)
{
	ag = Gtk::ActionGroup::create();
	uim = Gtk::UIManager::create();
	uim->insert_action_group(ag);
    try {
		uim->add_ui_from_file("menu.xml");
    }
    catch(...)
    {
		Gtk::MessageDialog("File menu.xml was not found in current directory.\nNo menus this time.", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true).run();
  	}
	window->add_accel_group(uim->get_accel_group());

	// add actions
	add("FileMenuAction", Gtk::Stock::FILE);
	add("clear-project", Gtk::Stock::NEW, mem_fun(window, &MainWindow::ClearProject));
	add("load-project", Gtk::Stock::OPEN, bind(bind(mem_fun(window, &MainWindow::LoadProject),false),string()));
	add("append-project", "Append...");
	add("import-data", "Import...");
	add("save-project", Gtk::Stock::SAVE, mem_fun(window, &MainWindow::QuickSaveProject));
	add("save-project-as", Gtk::Stock::SAVE_AS, bind(mem_fun(window, &MainWindow::SaveProject),string()));
	add("export-image", "Export...");
	add("exit", Gtk::Stock::QUIT, &Gtk::Main::quit);
	add("EditMenuAction", Gtk::Stock::EDIT);
	add("copy-clipboard", Gtk::Stock::COPY);
	add("cut-clipboard", Gtk::Stock::CUT);
	add("paste-clipboard", Gtk::Stock::PASTE);
	add("edit-delete", Gtk::Stock::DELETE);
	add("InsertMenuAction", "Insert");
	add("ViewMenuAction", "View");

	// detail radio group
	add("DetailMenuAction", "Detail");
	Gtk::RadioAction::Group radio1;
	add(radio1, "low-detail", "Low");
	Glib::RefPtr<Gtk::RadioAction>radioaction = Gtk::RadioAction::create(radio1, "normal-detail", "Normal");
	ag->add(radioaction);
	radioaction->set_active();
	add(radio1, "high-detail", "High");
	add(radio1, "fine-detail", "Fine");
	add(radio1, "realistic-detail", "Realistic");
	add(radio1, "ultra-detail", "Ultra");

	add("HelpMenuAction", Gtk::Stock::HELP);
	add("help-about", Gtk::Stock::ABOUT);

	//it must(?) be here, after all actions were created
	Gtk::Box* box = dynamic_cast<Gtk::Box*>(window->get_child());
	Gtk::MenuBar* mb = (Gtk::MenuBar*)uim->get_widget("/MenuBar");
	if(box&&mb)
	{
		box->pack_start(*mb, Gtk::PACK_SHRINK);
		box->reorder_child(*mb, 0);
	}
}
