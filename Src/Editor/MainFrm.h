//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MainFrame.h
//! \brief	Main Frame
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
#pragma once

//-----------------------------------------------------------------
//		Prototypes
//-----------------------------------------------------------------
class NOperatorsWnd;
class NProjectWnd;
class NStoredWnd;

//-----------------------------------------------------------------
//!	\class		NMainFrm
//!	\brief		Main frame
//-----------------------------------------------------------------
class NMainFrm :	public NFrmWnd
{
public:
	NMainFrm(void);
	virtual ~NMainFrm(void);

	virtual	bool Create(char* name, const NRect& rect);
	virtual	void	OnCommand(udword id);

	//Methods
	void MarkShowOperator(NOperator* pop);	//!< Set final Operator to mark final result
	void DeletingOperator(NOperator* pop);	//!< Notif deleting operator
	void DeletedOperator(NOperator* pop);		//!< Notif deleted operator

	NOperator* Execute(float _ftime);

	void LoadProject(NString path = NString());
	void SaveProject(NString path = NString());

protected:
	//Methods
	void	OnNewProject();
	void	OnOpenProject();
	void	OnSaveProjectAs();

	void OnKeyDown(udword dwchar);
	//Datas
	bool						m_bExecuteLocked;	//!< Lock-Unlock operators execution
	NOperatorsWnd*	m_opswnd;					//!< Operators Window
	NProjectWnd*		m_pprojectwnd;		//!< Pages Window
	NStoredWnd*			m_pstoredwnd;			//!< Stored Window
	NOperator*			m_popMarkedShow;	//!< Operator marked show for final result
	NString projectname;
	float						m_fDetailFactor;
};

bool AnnoyUserProjectMayBeLost();
