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
#ifndef EVENTSMGR_H
#define EVENTSMGR_H

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
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
class  NEventsMgr
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
		GArray<ST_EVENTHANDLERINFO>	carrayHandlers;
	};


public:

	//Constructor-Destructor
	NEventsMgr();
	~NEventsMgr();

	//Methods
	void Exec(uword wEventID, udword dwParam1, udword dwParam2);
	void Register(uword _wEventID, NObject* _pObject, EVENTFNC _pfnc);
	void UnRegisterAllByObject(NObject* _pObject);
	ST_EVENTINFO* GetEventFromID(uword _wEventID);

protected:
	//Datas
	GArray<ST_EVENTINFO*>	m_carrayEvents;

};

#endif //EVENTSMGR

