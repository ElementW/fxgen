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
#include "operatorswnd.h"

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
	m_hMenu=null;
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NOperatorsWnd::~NOperatorsWnd(void)
{
	if (m_hMenu)				::DestroyMenu(m_hMenu);

	EVT_UNREGISTERALL();
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NOperatorsWnd::Create(char* name, NRect& rect, NWnd* parent)
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
	m_hMenu = CreatePopupMenu();

	//Create Operators list sorted by category
	NRTClass* prtc = GetFirstClassBySuperClass("NOperator");
	while (prtc)
	{
		//Create operator in order to get operator name and categorie
		NOperator* pop = (NOperator*)prtc->m_pCreateCB();

		//Search if category already exist in menu
		HMENU popMenu =null;
		sdword count = ::GetMenuItemCount(m_hMenu);
		for (sdword j = 0; j < count; j++)
		{
			char szBuf[256];
			if (::GetMenuString(m_hMenu, j, szBuf, sizeof(szBuf), MF_BYPOSITION) && (strcmp(szBuf, pop->GetCategory()) == 0))
			{
				popMenu = ::GetSubMenu(m_hMenu, j);
				break;
			}
		}

		//Create category if it doesn't exist
		if (popMenu==null)
		{
			popMenu = CreateMenu();
			::AppendMenu(m_hMenu, MF_POPUP, (UINT)popMenu, pop->GetCategory() );
		}

		//Add new operator
		::AppendMenu(popMenu, MF_STRING, (DWORD)prtc->CLASSID, pop->GetName() );	//+1 for class prefix 'N'

		//Delete operator
		delete pop;

		//Next RTC
		prtc = GetNextClassBySuperClass("NOperator", prtc);
	}

}

//-----------------------------------------------------------------
//!	\brief	Windows Message Right button down
//-----------------------------------------------------------------
void NOperatorsWnd::OnRightButtonDown(udword flags, NPoint pos)
{
	if (m_hMenu && m_popsPage)
	{
		POINT pt;
		::GetCursorPos(&pt);	//Cursor position even with keyboard 'Context key'
		::TrackPopupMenu(m_hMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON, pt.x, pt.y, null, m_W32HWnd, null);
	}

}

//-----------------------------------------------------------------
//!	\brief	Windows Message Command
//-----------------------------------------------------------------
void NOperatorsWnd::OnCommand(udword id)
{
	ID classid = (ID)id;
	//TRACE("NOperatorsWnd::OnCommand CLASSID %d\n", *classid);
	PlaceOperator(classid);
	Update();
}

//-----------------------------------------------------------------
//!	\brief	An operator is marked show
//-----------------------------------------------------------------
void NOperatorsWnd::OnMarkShowOperator(NOperator* pop)
{
	NOperatorsCtrl::OnMarkShowOperator(pop);

	NFxGenApp* papp = (NFxGenApp*)GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->MarkShowOperator(pop);

}

//-----------------------------------------------------------------
//!	\brief	An operator is deleting
//-----------------------------------------------------------------
void NOperatorsWnd::OnDeletingOperator(NOperator* pop)
{
	NFxGenApp* papp = (NFxGenApp*)GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->DeletingOperator(pop);
}

//-----------------------------------------------------------------
//!	\brief	An operator is deleted
//-----------------------------------------------------------------
void NOperatorsWnd::OnDeletedOperator(NOperator* pop)
{
	NFxGenApp* papp = (NFxGenApp*)GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->DeletedOperator(pop);
}

//-----------------------------------------------------------------
//!	\brief	Return First RTClass from a super class name
//-----------------------------------------------------------------
NRTClass* NOperatorsWnd::GetFirstClassBySuperClass(char* _pszSuperClassName)
{
	NRTClass* pcurRTC = NRTClass::m_pFirstRTClass;
	while (pcurRTC!=null)
	{
		if (strcmp(pcurRTC->m_pszSuperClassName, _pszSuperClassName) == 0)
			return pcurRTC;
		pcurRTC = pcurRTC->m_pNextRTC;
	}

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Return Next RTClass from a super class name
//-----------------------------------------------------------------
NRTClass* NOperatorsWnd::GetNextClassBySuperClass(char* _pszSuperClassName, NRTClass* _prtclass)
{
	_prtclass = _prtclass->m_pNextRTC;

	while (_prtclass!=null)
	{
		if (strcmp(_prtclass->m_pszSuperClassName, _pszSuperClassName) == 0)
			return _prtclass;

		_prtclass = _prtclass->m_pNextRTC;
	}

	return null;
}



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
