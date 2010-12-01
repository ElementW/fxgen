//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EditorGUI.cpp
//! \brief	Editor GUI
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		23-10-2009
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef MAINFRM_H
#define MAINFRM_H

//-----------------------------------------------------------------
//		Includes
//-----------------------------------------------------------------
#include "CoreLibPkg.h"
#include "LayoutDock.h"

//-----------------------------------------------------------------
//		Prototypes
//-----------------------------------------------------------------
class NOperatorsWnd;
class NAssetWnd;
class NAssetsWnd;

//class NOperator;
class NAssetModel;

//-----------------------------------------------------------------
//!	\class		NEditorGUI
//!	\brief		Main frame
//-----------------------------------------------------------------
class NEditorGUI :	public NLayoutDock
{
public:
	NEditorGUI(void);
	virtual ~NEditorGUI(void);

	virtual	bool Create(char* name, const NRect& rect);

	//Get unique Instance
	static	NEditorGUI* GetInstance();

	//Methods
	void MarkShowOperator(NOperator* pop);	//!< Set final Operator to mark final result
	void DeletingOperator(NOperator* pop);	//!< Notif deleting operator
	void DeletedOperator(NOperator* pop);		//!< Notif deleted operator
	void EmitPropertiesChanged(NOperator* pop);

	NOperator* Execute(float _ftime);

	void LoadAsset(NString path);
	void SaveAsset(NString path = "");

	NAssetModel* GetAsset() { return m_pcurAsset; }

protected:
	//Methods
	void	OnNewAsset();
	void	OnOpenAsset();
	void	OnSaveAssetAs();

	void OnKeyDown(udword dwchar);

	bool AnnoyUserAssetMayBeLost();

	// Messages Notify
	void OnFile(NObject* _psender);
	void OnOption(NObject* _psender);
	void OnFileMenuClick(NObject* _psender);
	void OnOptionMenuClick(NObject* _psender);

	//Datas
	NAssetModel*			m_pcurAsset;		//!< Current project
	bool							m_bExecuteLocked;	//!< Lock-Unlock operators execution
	NOperatorsWnd*		m_opswnd;					//!< Operators Window
	NAssetWnd*				m_passetswnd;			//!< Assets window
	NOperator*				m_popMarkedShow;	//!< Operator marked show for final result
	NString						projectname;
	float							m_fDetailFactor;
	NMenuCtrl					m_wndFileMenu;
	NMenuCtrl					m_wndOptionMenu;

};


#endif //MAINFRM_H
