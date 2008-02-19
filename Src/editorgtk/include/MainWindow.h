//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		SmartWindow.h
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


#ifndef SMARTWINDOW_H
#define SMARTWINDOW_H
#include "MainMenu.h"

struct CustomFileFilter1: public Gtk::FileFilter
{
	CustomFileFilter1()
	{
		add_pattern("*.prj");
		set_name("FxGen projects");
	}
};

struct CustomFileFilter2: public Gtk::FileFilter
{
	CustomFileFilter2()
	{
		add_pattern("*");
		set_name("All files");
	}
};

class WindowTitleAsterisk
{
	Gtk::Window* window;
	bool value;
public:
	WindowTitleAsterisk(Gtk::Window* w): window(w), value(false) {}
	void operator=(bool b)
	{
		value = b;
		Glib::ustring title = window->get_title();
		if(!value && title.size() && title[title.size()-1] == '*')
			window->set_title(title.substr(0, title.size() - 1));
		if(value && ((title.size() && title[title.size()-1] != '*') || title.empty()))
			window->set_title(title + '*');
	}
	operator bool() {return value;}
};

class MainWindow : public Gtk::Window
{
		CustomFileFilter1 filter_fxgen;
		CustomFileFilter2 filter_all;
		MainMenu* main_menu;
	public:
		MainWindow(GtkWindow*cobject, const RefPtr<Xml>& refGlade);
		bool on_delete_event(GdkEventAny*);
		int ConfirmProjectLoss();
		void ClearProject();
		void LoadProject(string filename, bool append = false);
		void SaveProject(string filename);
		void QuickSaveProject();
		string filename;
		WindowTitleAsterisk project_modified;
//		Glib::Mutex library_mutex; ///<\fixme there should be a single mutex for all users of the library
		~MainWindow();
};

#endif // SMARTWINDOW_H
