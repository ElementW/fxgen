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

/// Maintain all menus in the application
class DynamicMenu : public Gtk::Menu
{
	public:
		DynamicMenu(MainWindow&);

		template<typename T1> void add(Glib::ustring name, T1 label)
		{
			ag->add( Gtk::Action::create(name, label));
		}

		template<typename T1, typename T2> void add(Glib::ustring name, T1 label, T2 fobj)
		{
			ag->add( Gtk::Action::create(name, label), fobj);
		}

		template<typename T1> void add(Gtk::RadioAction::Group& group, Glib::ustring name, T1 label)
		{
			ag->add( Gtk::RadioAction::create(group, name, label));
		}

		template<typename T1, typename T2> void add(Gtk::RadioAction::Group& group, Glib::ustring name, T1 label, T2 fobj)
		{
			ag->add( Gtk::RadioAction::create(group, name, label), fobj);
		}

		Glib::RefPtr<Gtk::UIManager> uim;
	protected:
		Glib::RefPtr<Gtk::ActionGroup> ag;
		MainWindow* window;
	private:
};

#endif // DYNAMICMENU_H
