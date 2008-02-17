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
MainMenu* main_menu;
NamedPointerColumns columns_pattern;

int main(int argc, char *argv[])
{
	Glib::thread_init();
	bool check = NEngineOp::GetEngine()->m_bEngineLock; // will refuse to start if the engine is not thread-safe
    Gtk::Main app(argc, argv);
    RefPtr<Xml> refXml;
	refXml = Xml::create_from_buffer(editor_glade, strlen(editor_glade));

	// create global objects
    refXml->get_widget_derived("window1", window);
    refXml->get_widget_derived("image1", image);
    refXml->get_widget_derived("fixed1", operators_layout);
    refXml->get_widget_derived("treeview1", project_tree);
    refXml->get_widget_derived("table1", property_table);
	main_menu = new MainMenu(*window);
	window->ClearProject();

	if(argc > 1 && Glib::file_test(argv[1], Glib::FILE_TEST_EXISTS))
		window->LoadProject(argv[1]);

    app.run(*window);

    return 0;
}
