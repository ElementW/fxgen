//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DynamicMenu.h
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

#ifndef DYNAMICMENU_H
#define DYNAMICMENU_H

class DynamicMenu: public Gtk::Menu
{
	public:
	void add(Glib::ustring group, Glib::ustring name, sigc::slot0<void>);
	Gtk::MenuItem* get_child_by_label(Glib::ustring);
};

#endif // DYNAMICMENU_H
