//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PropertyTable.h
//! \brief	FxGen GTK editor - operators' properties processing
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

#ifndef PROPERTYTABLE_H
#define PROPERTYTABLE_H
#include <fxgen_pch.h>

/// Display and store operator's properties
struct PropertyTable : public Gtk::Table
{
		PropertyTable(GtkTable*cobject, const RefPtr<Xml>& refGlade);
		void DisplayOperatorProperties(OperatorWidget*);
		void AdjustProperty(Gtk::Widget* w, int i, OperatorWidget* op);
		void clear();
};

#endif // PROPERTYTABLE_H
