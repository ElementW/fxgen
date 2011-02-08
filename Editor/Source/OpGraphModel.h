#pragma once
#include "CoreLibPkg.h"

//-----------------------------------------------------------------
//	Prototypes
//-----------------------------------------------------------------
class NAssetModel;

//-----------------------------------------------------------------
//!	\class		NOperatorNode
//!	\brief		operator node contain one operator
//!	\note			TODO base class NOpNodebase for all nodes in OpeperatorCtrl (comment...)
//-----------------------------------------------------------------
class NOperatorNode : public NObject
{
public:
	FDECLARE_CLASS();

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

	NOperatorNode* m_pprevOpToProcess;	//!< Nodes Links
	NOperatorNode* m_pnextOpToProcess;	//!< Nodes Links
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

	//Methods
	void AttachToAsset(NAssetModel* _passet);

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	udword			GetOpsCount()							{ return m_arrayNodes.Count();					}
	NOperatorNode*	GetOpFromIdx(udword _idx)	{ return (NOperatorNode*)m_arrayNodes[_idx];}

	udword			AddOp(NOperatorNode* _pop);
	void				DeleteAllOps();
	udword			DeleteOp(NOperatorNode* _pop);
	void				MoveOp(NOperatorNode* _pop, sword _x, sword _y);
	void				InvalidateAllOps();

	//Methods search
	//void GetOpsFromClassName(const char* _pszClassName, NObjectArray& _carray);

	//Methods for graph compilation
	void ComputeLinks();
	void _ComputeLinks(NOperatorNode* _popStart, NOperatorNode* _pop, NOperatorNode* _pprevop, udword _dwCurDepth);

	void GetPrevOperators(NOperatorNode* _pop, NObjectArray& _carrayPrevOpS);
	void GetNextOperators(NOperatorNode* _pop, NObjectArray& _carrayNextOpS);

	NOperatorNode* GetStartOpFrom(NOperatorNode* _pop);
	NOperatorNode* GetFinalOpFrom(NOperatorNode* _pop);

	//Datas	GUI
	NObjectArray	m_arrayNodes;						//!< Operators nodes array

	//Datas for compilation and linkage
	NOperatorNode*		m_pprevOp;
	NObjectArray	m_arrayOpsUnlinked;		//!< Operators unlinked array
	NAssetModel* m_passet;
};
