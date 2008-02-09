//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ViewportsWnd.h
//! \brief	Window for viewport
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
//                   Includes
//-----------------------------------------------------------------
#include "ViewportsCtrl.h"
#include "Editor.h"
#include "Renderer.h"

//-----------------------------------------------------------------
//!	\class		NViewportsWnd
//!	\brief		Window for viewport
//-----------------------------------------------------------------
class NViewportsWnd :	public NViewportsCtrl
{
public:
	NViewportsWnd(void);
	virtual ~NViewportsWnd(void);

	//Methods
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent);

protected:
	//Messages
	virtual	void	OnMouseWheel	(udword flags, sword zDelta, NPoint point);
	virtual void	OnMouseMove		(udword flags, NPoint pos);
	virtual void	OnMButtonDown	(udword flags, NPoint pos);
	virtual void	OnMButtonUp		(udword flags, NPoint pos);
	virtual void	OnLeftButtonDown(udword flags, NPoint pos);
	virtual void	OnLeftButtonUp(udword flags, NPoint pos);
	virtual void	OnRightButtonDown(udword flags, NPoint pos);
	virtual	void	OnCommand(udword id);

	//Events
	EVT_DECLARE_HANDLER(OnOPDeleting);
	EVT_DECLARE_HANDLER(OnRender);

	//Methods
	void DisplayTexture(NObject* pobj);

	//Datas
	HFONT				m_hfontNormal;
	NGLRenderer m_renderer;			//!< OpenGL Renderer
	sdword			m_dwTextureID;	//!< OpenGL Texture ID
	udword			m_dwTexWidth, m_dwTexHeight;

	NMenuCtrl		m_wndMenu;

	enum {
		NONE,
		PANNING,
		ROTATING
	} m_eDragMode;
	float				m_fScale;
	bool				m_bPanning;
	bool				m_bRotating;
	vec3				m_vecTrans;
	vec3				m_vecRot;
	NPoint				m_ptStartDrag;

	bool				m_bTiling;
	bool				m_bFiltering;
	bool				m_bOrtho;


	NObject*		m_pcurObject;		//!< Objet in visualisation

public:
	NObject* GetCurrentObject(){ return m_pcurObject; }
};