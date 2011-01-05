#pragma once
#include "CoreLibPkg.h"

//-----------------------------------------------------------------
//!	\class		NOperatorNode
//!	\brief		operator node contain one operator
//!	\note			TODO base class NOpNodebase for all nodes in OpeperatorCtrl (comment...)
//-----------------------------------------------------------------
class NOperatorNode : public NObject
{
public:
	NOperatorNode();
	virtual ~NOperatorNode();

	virtual NObject* Duplicate();			//!< Duplicate this object (used for copy-paste)

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	//Datas
	sword	m_wPosX, m_wPosY;			//!< Position (grid unit)
	sword	m_wWidth;							//!< Width (grid unit)
	NOperatorFx* m_op;						//!< Operator associed with this node
};

//-----------------------------------------------------------------
//!	\class		NOpGraphModel
//!	\brief		operators node graph model
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
	NOperatorNode*	GetOpFromIdx(udword _idx)	{ return (NOperatorNode*)m_arrayOps[_idx];}

	udword			AddOp(NOperatorNode* _pop);
	void				DeleteAllOps();
	udword			DeleteOp(NOperatorNode* _pop);
	void				MoveOp(NOperatorNode* _pop, sword _x, sword _y);
	void				InvalidateAllOps();

	//Methods search
	void GetOpsFromClassName(const char* _pszClassName, NObjectArray& _carray);

	//Methods for graph compilation
	void ComputeLinks();
	void _ComputeLinks(NOperatorNode* _pop, NOperatorNode* _pprevop, udword _dwCurDepth);

	void GetPrevOperators(NOperatorNode* _pop, NObjectArray& _carrayPrevOpS);
	void GetNextOperators(NOperatorNode* _pop, NObjectArray& _carrayNextOpS);
	NOperatorNode* GetFinalOpFrom(NOperatorNode* _pop);

	//Datas	GUI
	NObjectArray	m_arrayOps;						//!< Operators array

	//Datas for compilation and linkage
	NOperatorNode*		m_pprevOp;
	NObjectArray	m_arrayOpsUnlinked;		//!< Operators unlinked array
};
