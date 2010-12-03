#pragma once
#include "CoreLibPkg.h"

//-----------------------------------------------------------------
//!	\class		NAssetModel
//!	\brief		Asset model
//-----------------------------------------------------------------
class NAssetModel :	public NObject
{
public:
	NAssetModel(void);
	virtual ~NAssetModel(void);

	NTreeNode*			GetRootGroup()				{ return m_pRootGroup;		}

	//Serialization
	//virtual	bool Save(NArchive* _s);	//!< Save object
	//virtual	bool Load(NArchive* _l);	//!< Load object


protected:
	NTreeNode* m_pRootGroup;							//!< Root	Groups
};
