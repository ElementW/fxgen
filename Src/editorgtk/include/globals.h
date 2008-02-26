//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		globals.h
//! \brief	FxGen GTK editor - globally visible objects
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

#include "MainWindow.h"
#include "ProjectTree.h"
#include "TextureImage.h"
#include "Operators.h"
#include "PropertyTable.h"

extern MainWindow* window;
extern TextureImage* image;
extern ProjectTree* project_tree;
extern OperatorsLayout* operators_layout;
extern PropertyTable* property_table;
extern char* editor_glade;

struct NamedPointerColumns: public Gtk::TreeModelColumnRecord
{
	NamedPointerColumns() { add(m_col_name); add(m_col_ptr); }
	Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	Gtk::TreeModelColumn<void*> m_col_ptr;
};

extern NamedPointerColumns columns_pattern;
vector<string> parse_fxgen_combo_string(string csv);
Glib::ustring hex_color(int number);


