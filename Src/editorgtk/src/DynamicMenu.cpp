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
