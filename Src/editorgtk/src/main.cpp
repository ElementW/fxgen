//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		main.cpp
//! \brief	FxGen GTK editor
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
#include "fxgen_pch.h"

// globals
MainWindow* window;
TextureImage* image;
OperatorsLayout* operators_layout;
bool project_modified  = false;
ProjectTree* project_tree;
PropertyTable* property_table;
DynamicMenu* dynamic_menu;


int main(int argc, char *argv[])
{
    Gtk::Main app(argc, argv);
    RefPtr<Xml> refXml;
    try {
		refXml = Xml::create("editor.glade");
    }
    catch(...)
    {
		Gtk::MessageDialog("File editor.glade was not found in current directory.\nProgram will quit.", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true).run();
    	exit(1);
	}

	// create global objects
    refXml->get_widget_derived("window1", window);
    refXml->get_widget_derived("image1", image);
    refXml->get_widget_derived("eventbox1", operators_layout);
    project_tree = new ProjectTree;
    refXml->get_widget_derived("table1", property_table);
	dynamic_menu = new DynamicMenu(*window);
// load image (temporary)
    window->LoadProject("test.prj");

    app.run(*window);

    return 0;
}
