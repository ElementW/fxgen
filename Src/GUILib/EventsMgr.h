//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EventsMgr.h
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
#pragma once

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Core.h"

typedef udword (NObject::*EVENTFNC)(udword dwParam1, udword dwParam2);

#define EVT_REGISTER(id, fnc)											g_pceventsMgr->Register(id, this, (EVENTFNC)fnc);
#define EVT_EXECUTE(id, p1, p2)										g_pceventsMgr->Exec(id, p1, p2);
#define EVT_DECLARE_HANDLER(name)									udword name(udword dwParam1, udword dwParam2);
#define EVT_IMPLEMENT_HANDLER(classname, name)		udword classname::name(udword dwParam1, udword dwParam2)
#define EVT_UNREGISTERALL()												g_pceventsMgr->UnRegisterAllByObject(this);

//-----------------------------------------------------------------
//!	\class	NEventsMgr
//!	\brief	Events manager
//-----------------------------------------------------------------
class NEventsMgr
{
	//Structs
	struct ST_EVENTHANDLERINFO
	{
		NObject*	pObject;
		EVENTFNC	pFnc;
	};

	struct ST_EVENTINFO
	{
		uword wID;
		NArray<ST_EVENTHANDLERINFO>	carrayHandlers;
	};


public:

	//Constructor
	NEventsMgr()
	{
	}

	//Destructor
	~NEventsMgr()
	{
		if (m_carrayEvents.Count())
			TRACE("NEventsMgr warning exiting with events registered !\n");
	}

	//Methods
	void Exec(uword wEventID, udword dwParam1, udword dwParam2)
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

	void Register(uword _wEventID, NObject* _pObject, EVENTFNC _pfnc)
	{
		ST_EVENTHANDLERINFO st_ehi;
		st_ehi.pObject	= _pObject;
		st_ehi.pFnc			= _pfnc;

		ST_EVENTINFO* pei = GetEventFromID(_wEventID);
		if (!pei)
		{
			pei = new ST_EVENTINFO;
			pei->wID = _wEventID;
			m_carrayEvents.AddItem(pei);
		}
		pei->carrayHandlers.AddItem(st_ehi);
	}

	void UnRegisterAllByObject(NObject* _pObject)
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
						delete pei;
					}
					break;
				}
			}
		}

	}

	ST_EVENTINFO* GetEventFromID(uword _wEventID)
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

protected:
	//Datas
	NArray<ST_EVENTINFO*>	m_carrayEvents;

};
