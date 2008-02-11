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
#include "DynamicMenu.h"
#include "ProjectTree.h"
#include "TextureImage.h"
#include "Operators.h"
#include "PropertyTable.h"

extern MainWindow* window;
extern TextureImage* image;
extern bool project_modified;
extern ProjectTree* project_tree;
extern OperatorsLayout* operators_layout;
extern PropertyTable* property_table;
Glib::ustring hex_color(int number);
extern DynamicMenu* dynamic_menu;
