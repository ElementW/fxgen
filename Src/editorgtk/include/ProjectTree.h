//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ProjectTree.h
//! \brief	FxGen GTK editor - a tree model showing file structure
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

#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <fxgen_pch.h>

struct ProjectTree : public Gtk::TreeStore
{
	ProjectTree();
	void DisplayProject(NEngineOp*);
};

#endif // PROJECTTREE_H
