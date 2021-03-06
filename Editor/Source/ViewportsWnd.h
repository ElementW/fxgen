//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ViewportsWnd.h
//! \brief	Window for viewport
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);

protected:
	//Messages
	virtual	void	OnPaint(N2DPainter *_ppainter);
	virtual	void	OnMouseWheel	(sword zDelta, NPoint point);
	virtual void	OnMouseMove		(NPoint pos);
	virtual void	OnMButtonDown	(NPoint pos);
	virtual void	OnMButtonUp		(NPoint pos);
	virtual void	OnLButtonDown(NPoint pos);
	virtual void	OnLButtonUp(NPoint pos);
	virtual void	OnRButtonDown(NPoint pos);


	//Events
	EVT_DECLARE_HANDLER(OnOPDeleting);
	EVT_DECLARE_HANDLER(OnRender);

	//Methods
	void DisplayTexture(NObject* pobj);
	udword CreateTexture(udword _w, udword _h);
	void DeleteTexture(udword _dwTextID);
	void CopyPixelsToTexture(udword _dwtexid, udword _w, udword _h, NRGBA* _ppixels);

	// Messages Notify
	void OnMenuItemClick(NObject* _psender);

	//Datas
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
	NPoint			m_ptStartDrag;

	bool				m_bTiling;
	bool				m_bFiltering;
	bool				m_bOrtho;

	NObject*		m_pcurObject;		//!< Objet in visualisation
};
