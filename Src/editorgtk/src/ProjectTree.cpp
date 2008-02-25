//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ProjectTree.cpp
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

#include "pch.h"
#include "globals.h"

//-----------------------------------------------------------------
//!	\author	Johann Nadalutti (fxgen@free.fr)
//-----------------------------------------------------------------
NObject* _FindNodeFromClassName(NTreeNode* _pParent, char* _pszClassName)
{
	//Sons
	NTreeNode* pnode = _pParent->GetSon();
	while (pnode)
	{
		//Objects array
		NObjectArray& array = pnode->GetObjsArray();
		for (udword i=0; i<array.Count(); i++)
		{
			NObject* pobj = array[i];
			if (strcmp(pobj->GetRTClass()->m_pszClassName, _pszClassName) == 0)
				return pobj;
		}

		//Son Nodes
		NObject* pfind = _FindNodeFromClassName(pnode, _pszClassName);
		if (pfind)
			return pfind;

		//Next brothers nodes
		pnode = pnode->GetBrother();
	}

	return null;
}

ProjectTree::ProjectTree(GtkTreeView*cobject, const RefPtr<Xml>& refGlade)
: Gtk::TreeView(cobject)
{
	model = Gtk::TreeStore::create(columns_pattern);
	set_model(model);
	append_column("Pages", columns_pattern.m_col_name);
	set_headers_visible(false);
	// connect the operators_layout to the selection so that clicking on a page name will display its content
	get_selection()->signal_changed().connect(bind(mem_fun(operators_layout, &OperatorsLayout::display_selected_page), this));
	//ctor
}


void ProjectTree::fill_tree(Gtk::TreeIter iter)
{
	;
}

/// Find all pages in the project and display first of them.
void ProjectTree::DisplayProject(NEngineOp* project)
{
	clear();

	for(NTreeNode* pnode = project->GetRootGroup(); pnode; pnode = pnode->GetSon())
	{
		NObjectArray& arrayObjs = pnode->GetObjsArray();
		for (unsigned idx = 0; idx < arrayObjs.Count(); idx++)
		{
			NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[idx];
			Gtk::TreeModel::iterator row = model->append(); // row for a page
			(*row)[columns_pattern.m_col_name] = ppage->GetName();
			(*row)[columns_pattern.m_col_ptr] = ppage;
		}
	}
	;
	// select first page
	get_selection()->select(model->children().begin());
}

/// Prepare for a new project
void ProjectTree::clear()
{
	operators_layout->clear();
	model->clear();
}
