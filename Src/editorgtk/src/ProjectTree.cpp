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
//!	\brief	Select first page found from a root node
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

ProjectTree::ProjectTree()
{
	//ctor
}

void ProjectTree::DisplayProject(NEngineOp* project)
{
	operators_layout->clear();
	NTreeNode* root_group = project->GetRootGroup();
	NOperatorsPage* page = dynamic_cast<NOperatorsPage*>(_FindNodeFromClassName(root_group, "NOperatorsPage"));
	if(page)
	{
		operators_layout->page = page;
		operators_layout->clear();
		NObjectArray& array = page->m_arrayOps;
		for(unsigned i = 0; i < array.Count(); i++)
		{
			NOperator* op = dynamic_cast<NOperator*>(array[i]);
			if(op)
			{
				int x = op->m_wPosX;
				int y = op->m_wPosY;
				int w = op->m_wWidth;
				operators_layout->add(op, x, y, w);
			}
		}
	}
}
