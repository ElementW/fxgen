#pragma once
#include "CoreLibPkg.h"


//-----------------------------------------------------------------
//!	\class		NOpGraphModel
//!	\brief		operators graph model
//-----------------------------------------------------------------
class  NOpGraphModel : public NObject
{
public:
	FDECLARE_CLASS();

	NOpGraphModel();
	virtual ~NOpGraphModel();

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	udword			GetOpsCount()							{ return m_arrayOps.Count();					}
	NOperator*	GetOpFromIdx(udword _idx)	{ return (NOperator*)m_arrayOps[_idx];}

	udword			AddOp(NOperator* _pop);
	void				DeleteAllOps();
	udword			DeleteOp(NOperator* _pop);
	void				MoveOp(NOperator* _pop, sword _x, sword _y);
	void				InvalidateAllOps();

	//Methodes de recherche
	void GetOpsFromClassName(const char* _pszClassName, NObjectArray& _carray);

	//Methodes pour la generation des liens entre les ops
	void ComputeLinks();
	void _ComputeLinks(NOperator* _pop, NOperator* _pprevop, udword _dwCurDepth);

	void GetPrevOperators(NOperator* _pop, NObjectArray& _carrayPrevOpS);
	void GetNextOperators(NOperator* _pop, NObjectArray& _carrayNextOpS);
	NOperator* GetFinalOpFrom(NOperator* _pop);

	//Datas	GUI
	NObjectArray	m_arrayOps;						//!< Operators array

	//Datas for compilation and linkage
	NOperator*		m_pprevOp;
	NObjectArray	m_arrayOpsUnlinked;		//!< Operators unlinked array
};
