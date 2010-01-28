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
//#include "CoreLib.h"
#include "CoreLibPkg.h"
//#include "../Source/GUI.h"
//#include "../Source/Layout.h"
//#include "../Source/MenuCtrl.h"

//-----------------------------------------------------------------
//		Prototypes
//-----------------------------------------------------------------
class NOperatorsWnd;
class NProjectWnd;
class NStoredWnd;
class NOperatorNode;
class NOperatorsProject;

//-----------------------------------------------------------------
//!	\class		NEditorGUI
//!	\brief		Main frame
//-----------------------------------------------------------------
class NEditorGUI :	public NGUILayout
{
public:
	NEditorGUI(void);
	virtual ~NEditorGUI(void);

	virtual	bool Create(char* name, const NRect& rect);

	//Methods
	void MarkShowOperator(NOperatorNode* pop);	//!< Set final Operator to mark final result
	void DeletingOperator(NOperatorNode* pop);	//!< Notif deleting operator
	void DeletedOperator(NOperatorNode* pop);		//!< Notif deleted operator
	void EmitPropertiesChanged(NOperatorNode* pop);

	NOperatorNode* Execute(float _ftime);

	void LoadProject(NString path = NString());
	void SaveProject(NString path = NString());

	NOperatorsProject* GetProject() { return m_pcurProject; }

protected:
	//Methods
	void	OnNewProject();
	void	OnOpenProject();
	void	OnSaveProjectAs();

	void OnKeyDown(udword dwchar);

	bool AnnoyUserProjectMayBeLost();

	// Messages Notify
	void OnFile(NObject* _psender);
	void OnOption(NObject* _psender);
	void OnFileMenuClick(NObject* _psender);
	void OnOptionMenuClick(NObject* _psender);

	//Datas
	NOperatorsProject*	m_pcurProject;		//!< Current project
	bool							m_bExecuteLocked;	//!< Lock-Unlock operators execution
	NOperatorsWnd*		m_opswnd;					//!< Operators Window
	NProjectWnd*			m_pprojectwnd;		//!< Pages Window
	NStoredWnd*				m_pstoredwnd;			//!< Stored Window
	NOperatorNode*		m_popMarkedShow;	//!< Operator marked show for final result
	NString						projectname;
	float							m_fDetailFactor;
	NMenuCtrl					m_wndFileMenu;
	NMenuCtrl					m_wndOptionMenu;

};


#endif //MAINFRM_H
