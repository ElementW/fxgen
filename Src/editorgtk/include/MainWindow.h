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

class MainWindow : public Gtk::Window
{
	public:
		MainWindow(GtkWindow*cobject, const RefPtr<Xml>& refGlade);
		bool on_delete_event(GdkEventAny*);
		int ConfirmProjectLoss();
		void ClearProject();
		void LoadProject(string filename, bool append = false);
		void SaveProject(string filename);
		void QuickSaveProject();
		string filename;
		~MainWindow();
	protected:
		CustomFileFilter1 filter_fxgen;
		CustomFileFilter2 filter_all;
	private:
};

#endif // SMARTWINDOW_H
