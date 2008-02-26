//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MainMenu.cpp
//! \brief	FxGen GTK editor - main menu initialization
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
#include "MainMenu.h"
#include "DynamicMenu.h"
#include "Operators.h"
#include "globals.h"

extern char* menu_xml;

void set_current_detail(int value)
{
	if(image)
		image->clear();
	OperatorWidget::detail = (1<<(value + 2)) / 8.0;
	NEngineOp::GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
	NEngineOp::GetEngine()->InvalidateAllOps();
	if(OperatorWidget::preview_op)
		OperatorWidget::preview_op->UpdateImage();
}

MainMenu::MainMenu(MainWindow& w): window(&w)
{
	ag = Gtk::ActionGroup::create();
	uim = Gtk::UIManager::create();
	uim->insert_action_group(ag);
	uim->add_ui_from_string(menu_xml);
	window->add_accel_group(uim->get_accel_group());

	// add actions
	add("FileMenuAction", "File");
	add("clear-project", Gtk::Stock::NEW, mem_fun(window, &MainWindow::ClearProject));
	add("load-project", Gtk::Stock::OPEN, bind(bind(mem_fun(window, &MainWindow::LoadProject),false),""));
	add("append-project", "Append...");
	add("import-data", "Import...");
	add("save-project", Gtk::Stock::SAVE, mem_fun(window, &MainWindow::QuickSaveProject));
	add("save-project-as", Gtk::Stock::SAVE_AS, bind(mem_fun(window, &MainWindow::SaveProject),""));
	add("export-image", "Export...", mem_fun(window, &MainWindow::ExportImage));
	add("exit", Gtk::Stock::QUIT, &Gtk::Main::quit);
	add("EditMenuAction", "Edit");
	add("copy-clipboard", Gtk::Stock::COPY);
	add("cut-clipboard", Gtk::Stock::CUT);
	add("paste-clipboard", Gtk::Stock::PASTE);
#ifndef _WIN32
	add("edit-delete", Gtk::Stock::DELETE);
#endif
	add("InsertMenuAction", "Insert");
	add("ViewMenuAction", "View");

	// detail radio group
	add("DetailMenuAction", "Detail");
	Gtk::RadioAction::Group radio1;
	add(radio1, "micro-detail", "Micro (1/8)", bind(&set_current_detail, -2));
	add(radio1, "tiny-detail", "Tiny (1/4)", bind(&set_current_detail, -1));
	add(radio1, "low-detail", "Low (1/2)", bind(&set_current_detail, 0));
	Glib::RefPtr<Gtk::RadioAction>radioaction = Gtk::RadioAction::create(radio1, "normal-detail", "Normal");
	ag->add(radioaction, bind(&set_current_detail, 1));
	add(radio1, "high-detail", "High (2x)", bind(&set_current_detail, 2));
	add(radio1, "fine-detail", "Fine (4x)", bind(&set_current_detail, 3));
	add(radio1, "realistic-detail", "Realistic (8x)", bind(&set_current_detail, 4));
	add(radio1, "ultra-detail", "Ultra (16x)", bind(&set_current_detail, 5));
	radioaction->set_active();

	add("HelpMenuAction", "Help");
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

