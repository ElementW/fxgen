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
	add("load-project", Gtk::Stock::OPEN, bind(bind(mem_fun(window, &MainWindow::LoadProject),false),string()));
	add("append-project", "Append...");
	add("import-data", "Import...");
	add("save-project", Gtk::Stock::SAVE, mem_fun(window, &MainWindow::QuickSaveProject));
	add("save-project-as", Gtk::Stock::SAVE_AS, bind(mem_fun(window, &MainWindow::SaveProject),string()));
	add("export-image", "Export...");
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
	add(radio1, "low-detail", "Low");
	Glib::RefPtr<Gtk::RadioAction>radioaction = Gtk::RadioAction::create(radio1, "normal-detail", "Normal");
	ag->add(radioaction);
	radioaction->set_active();
	add(radio1, "high-detail", "High");
	add(radio1, "fine-detail", "Fine");
	add(radio1, "realistic-detail", "Realistic");
	add(radio1, "ultra-detail", "Ultra");

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

/** Add a new text-only item to a submenu, with a callback connected
\param group Name for the submenu. It will be created if necessary. If empty, items are added to the menu itself.
*/
void DynamicMenu::add(Glib::ustring group, Glib::ustring name, sigc::slot0<void> callback)
{
	DynamicMenu* submenu;
	if(group.size())
	{
		Gtk::MenuItem* submenuitem = get_child_by_label(group);

		if(!submenuitem)
		{
			submenuitem = Gtk::manage(new Gtk::MenuItem(group));
			submenu = Gtk::manage(new DynamicMenu);
			submenuitem->set_submenu(*submenu);
			int pos = get_children().size();
			attach(*submenuitem, 0, 1, pos, pos+1);
		}

		submenu = dynamic_cast<DynamicMenu*>(submenuitem->get_submenu());
	}
	else
		submenu = this;

	if(submenu->get_child_by_label(name))
		return; // nothing to do
	Gtk::MenuItem* item = Gtk::manage(new Gtk::MenuItem(name));
	item->signal_activate().connect(callback);
	int pos = submenu->get_children().size();
	submenu->attach(*item, 0, 1, pos, pos+1);
}


/**
\fixme A relatively dirty hack - there should be no need to look up for children.
*/
Gtk::MenuItem* DynamicMenu::get_child_by_label(Glib::ustring text)
{
	vector<Gtk::Widget*> children = get_children();

	for(unsigned i = 0; i < children.size(); i++)
	{
		Gtk::MenuItem* item = dynamic_cast<Gtk::MenuItem*>(children[i]);
		Gtk::Label* label = dynamic_cast<Gtk::Label*>(item->get_child());
		if(label && label->get_text() == text)
			return item;
	}

	return NULL;
}
