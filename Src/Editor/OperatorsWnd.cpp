//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorsWnd.cpp
//! \brief	Windows for operators editing
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "OperatorsWnd.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NOperatorsWnd Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NOperatorsWnd::NOperatorsWnd(void)
{
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NOperatorsWnd::~NOperatorsWnd(void)
{
	EVT_UNREGISTERALL();
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NOperatorsWnd::Create(const char* name, const NRect& rect, NWnd* parent)
{
	NOperatorsCtrl::Create(name, rect, parent);

	//Init context Menu
	InitCtxMenu();

	//Register Events
	EVT_REGISTER(EVT_PAGESELECTED, &NOperatorsWnd::OnPageSelected );
	EVT_REGISTER(EVT_RENDER, &NOperatorsWnd::OnRendering );

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Context menu creation
//-----------------------------------------------------------------
void NOperatorsWnd::InitCtxMenu()
{
	//Creation du menu
	m_wndMenu.Create("Operators:", this);
	m_wndMenu.OnItemClick=FDelegate(this, (TDelegate)&NOperatorsWnd::OnMenuItemClick);

	//Create Operators list sorted by category
	NRTClass* prtc = NRTClass::GetFirstClassBySuperClass("NOperator");
	while (prtc)
	{
		//Create operator in order to get operator name and categorie
		NOperator* pop = (NOperator*)prtc->m_pCreateCB();

		//Search if category already exist in menu
		NMenuCtrl* popMenu = null;
		udword count = m_wndMenu.GetItemsCount();
		udword j = 0;
		for (j = 0; j < count; j++)
		{
			NMEItemDesc* pitem = m_wndMenu.GetItemDesc(j);
			if (pitem->strName == NString(pop->GetCategory()))
			{
				popMenu = m_wndMenu.GetPopupMenu(j);
				break;
			}
		}

		//Create category if it doesn't exist
		if (popMenu==null)
		{
			popMenu = m_wndMenu.CreatePopupMenu(pop->GetCategory(), -1);
		}

		//Add new operator
		popMenu->AddItem(pop->GetName(), prtc->CLASSID, 0);

		//Delete operator
		delete pop;

		//Next RTC
		prtc = NRTClass::GetNextClassBySuperClass("NOperator", prtc);
	}

}

//-----------------------------------------------------------------
//!	\brief	Windows Message Right button down
//-----------------------------------------------------------------
void NOperatorsWnd::OnRButtonDown(NPoint pos)
{
	SetFocus();
	ClientToScreen(pos);
	m_wndMenu.TrackPopupMenu(pos);
}

//-----------------------------------------------------------------
//!	\brief	Windows Message Command
//-----------------------------------------------------------------
void NOperatorsWnd::OnMenuItemClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	ID classid = (ID)pmenu->GetClickedCmdID();
	PlaceOperator(classid);
	SetFocus();
	Update();
}

//-----------------------------------------------------------------
//!	\brief	An operator is marked show
//-----------------------------------------------------------------
void NOperatorsWnd::OnMarkShowOperator(NOperator* pop)
{
	NOperatorsCtrl::OnMarkShowOperator(pop);

	NFxGenApp* papp = GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->MarkShowOperator(pop);

}

//-----------------------------------------------------------------
//!	\brief	An operator is deleting
//-----------------------------------------------------------------
void NOperatorsWnd::OnDeletingOperator(NOperator* pop)
{
	NFxGenApp* papp = (NFxGenApp*)GetGUISubSystem();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->DeletingOperator(pop);
}

//-----------------------------------------------------------------
//!	\brief	An operator is deleted
//-----------------------------------------------------------------
void NOperatorsWnd::OnDeletedOperator(NOperator* pop)
{
	NFxGenApp* papp = (NFxGenApp*)GetGUISubSystem();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->DeletedOperator(pop);
}

//-----------------------------------------------------------------
//!	\brief	Return First RTClass from a super class name
//-----------------------------------------------------------------
/*NRTClass* NOperatorsWnd::GetFirstClassBySuperClass(char* _pszSuperClassName)
{
	NRTClass* pcurRTC = NRTClass::m_pFirstRTClass;
	while (pcurRTC!=null)
	{
		if (strcmp(pcurRTC->m_pszSuperClassName, _pszSuperClassName) == 0)
			return pcurRTC;
		pcurRTC = pcurRTC->m_pNextRTC;
	}

	return null;
}*/

//-----------------------------------------------------------------
//!	\brief	Return Next RTClass from a super class name
//-----------------------------------------------------------------
/*NRTClass* NOperatorsWnd::GetNextClassBySuperClass(char* _pszSuperClassName, NRTClass* _prtclass)
{
	_prtclass = _prtclass->m_pNextRTC;

	while (_prtclass!=null)
	{
		if (strcmp(_prtclass->m_pszSuperClassName, _pszSuperClassName) == 0)
			return _prtclass;

		_prtclass = _prtclass->m_pNextRTC;
	}

	return null;
}*/



//-----------------------------------------------------------------
//!	\brief	Event a page is selected
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NOperatorsWnd, OnPageSelected)
{
	NOperatorsPage* popsPage = (NOperatorsPage*)dwParam1;
	DisplayOperatorsPage(popsPage);
	Update();

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	Event rendering asked
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NOperatorsWnd, OnRendering)
{
	float* ftime = (float*)dwParam2;
	Update(*ftime);
	return 0;
}
