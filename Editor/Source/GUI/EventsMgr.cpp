//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EventsMgr.cpp
//! \brief	Events manager
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "..\pch.h"
#include "GUI.h"
#include "EventsMgr.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NDialog class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
NEventsMgr::NEventsMgr()
{
}

NEventsMgr::~NEventsMgr()
{
  if (m_carrayEvents.Count())
    TRACE("NEventsMgr warning exiting with events registered !\n");
}

void NEventsMgr::Exec(uword wEventID, udword dwParam1, udword dwParam2)
{
	ST_EVENTINFO* pei = GetEventFromID(wEventID);
	if (pei)
	{
		for (udword i=0; i<pei->carrayHandlers.Count(); i++)
		{
			ST_EVENTHANDLERINFO pehi = pei->carrayHandlers[i];
			(pehi.pObject->*pehi.pFnc)(dwParam1, dwParam2);
		}
	}
}

void NEventsMgr::Register(uword _wEventID, NObject* _pObject, EVENTFNC _pfnc)
{
	ST_EVENTHANDLERINFO st_ehi;
	st_ehi.pObject	= _pObject;
	st_ehi.pFnc			= _pfnc;

	ST_EVENTINFO* pei = GetEventFromID(_wEventID);
	if (!pei)
	{
		pei = NNEW(ST_EVENTINFO);
		pei->wID = _wEventID;
		m_carrayEvents.AddItem(pei);
	}
	pei->carrayHandlers.AddItem(st_ehi);
}

void NEventsMgr::UnRegisterAllByObject(NObject* _pObject)
{
	TRACE("NEventsMgr::UnRegisterAllByObject\n");

	for (udword i=0; i<m_carrayEvents.Count(); i++)
	{
		ST_EVENTINFO* pei = m_carrayEvents[i];
		for (udword j=0; j<pei->carrayHandlers.Count(); j++)
		{
			ST_EVENTHANDLERINFO* pehi = &pei->carrayHandlers[j];
			if (pehi->pObject == _pObject)
			{
				pei->carrayHandlers.RemoveItem(j);
				if (pei->carrayHandlers.Count()==0)
				{
					m_carrayEvents.RemoveItem(i);
					i--;
					NDELETE(pei, ST_EVENTINFO);
				}
				break;
			}
		}
	}

}

NEventsMgr::ST_EVENTINFO* NEventsMgr::GetEventFromID(uword _wEventID)
{
	for (udword i=0; i<m_carrayEvents.Count(); i++)
	{
		if (m_carrayEvents[i]->wID == _wEventID)
		{
			return m_carrayEvents[i];
		}
	}
	return null;
}
