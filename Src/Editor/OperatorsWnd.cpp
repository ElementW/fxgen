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

void NOperatorsWnd::InitCtxMenu()
{
	//Creation du menu
	m_hMenu = CreatePopupMenu();

	//Creation de la liste des Blocs en fct de leurs categorie
	NRTClass* prtc = GetFirstClassBySuperClass("NOperator");
	while (prtc)
	{
		//Recherche si la categorie existe deja
		HMENU popMenu =null;
		sdword count = ::GetMenuItemCount(m_hMenu);
		for (sdword j = 0; j < count; j++)
		{
			char szBuf[256];
			if (::GetMenuString(m_hMenu, j, szBuf, sizeof(szBuf), MF_BYPOSITION) && (strcmp(szBuf, prtc->m_pszSuperClassName) == 0))
			{
				popMenu = ::GetSubMenu(m_hMenu, j);
				break;
			}
		}

		//on cree la categorie si elle n'existe pas deja
		if (popMenu==null)
		{
			popMenu = CreateMenu();
			::AppendMenu(m_hMenu, MF_POPUP, (UINT)popMenu, prtc->m_pszSuperClassName);
		}

		//Insertion du bloc
		::AppendMenu(popMenu, MF_STRING, (DWORD)prtc->CLASSID, prtc->m_pszClassName+1);	//+1 for class prefix 'N'

		//Next RTC
		prtc = GetNextClassBySuperClass("NOperator", prtc);
	}

}


void NOperatorsWnd::OnRightButtonDown(udword flags, NPoint pos)
{
	if (m_hMenu && m_popsPage)
	{
		POINT pt;
		::GetCursorPos(&pt);	//Cursor position even with keyboard 'Context key'
		::TrackPopupMenu(m_hMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON, pt.x, pt.y, null, m_W32HWnd, null);
	}

}

void NOperatorsWnd::OnCommand(udword id)
{
	ID classid = (ID)id;
	//TRACE("NOperatorsWnd::OnCommand CLASSID %d\n", *classid);
	PlaceOperator(classid);
	Update();
}


void NOperatorsWnd::OnMarkShowOperator(NOperator* pop)
{
	NOperatorsCtrl::OnMarkShowOperator(pop);

	NFxGenApp* papp = (NFxGenApp*)GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->MarkShowOperator(pop);

}

void NOperatorsWnd::OnDeletingOperator(NOperator* pop)
{
	NFxGenApp* papp = (NFxGenApp*)GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->DeletingOperator(pop);
}


void NOperatorsWnd::OnDeletedOperator(NOperator* pop)
{
	NFxGenApp* papp = (NFxGenApp*)GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();
	pfrm->DeletedOperator(pop);
}


EVT_IMPLEMENT_HANDLER(NOperatorsWnd, OnPageSelected)
{
	NOperatorsPage* popsPage = (NOperatorsPage*)dwParam1;
	DisplayOperatorsPage(popsPage);
	Update();

	return 0;
}

EVT_IMPLEMENT_HANDLER(NOperatorsWnd, OnRendering)
{
	float* ftime = (float*)dwParam2;
	Update(*ftime);
	return 0;
}

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
