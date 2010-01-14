//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ViewportsWnd.cpp
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

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "ViewportsWnd.h"
//#include "TGAWriter.h"
#include "FileChooserDialog.h"
#include "Operator.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define ID_RESET				100
#define ID_TILEONOFF		101
#define ID_FILTERONOFF	102
#define ID_2D3D					103
#define ID_EXPORT				104

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NViewportsWnd Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NViewportsWnd::NViewportsWnd(void)
{
	m_pcurOp	= null;
	m_dwTextureID = 0;
	m_dwTexWidth=m_dwTexHeight=0;
	m_fScale			= 1.0f;
	m_eDragMode = NONE;
	m_vecTrans.x=m_vecTrans.y=m_vecTrans.z=0.0f;
	m_vecRot.x=m_vecRot.y=m_vecRot.z=0.0f;
	m_bTiling			= false;
	m_bFiltering	= false;
	m_bOrtho = true;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NViewportsWnd::~NViewportsWnd(void)
{
	EVT_UNREGISTERALL();
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NViewportsWnd::Create(const char* name, const NRect& rect, NGUIWnd* parent)
{
	NViewportsCtrl::Create(name, rect, parent);

	//Create context menu
	m_wndMenu.Create("Viewport:", this);
	m_wndMenu.OnItemClick=FDelegate(this, (TDelegate)&NViewportsWnd::OnMenuItemClick);

	m_wndMenu.AddItem("Reset",				ID_RESET,				0);
	m_wndMenu.AddItem("Tiling",				ID_TILEONOFF,		ME_ITEMSTYLE_CHECKBOX);
	m_wndMenu.AddItem("Filtering",		ID_FILTERONOFF,	ME_ITEMSTYLE_CHECKBOX);
	m_wndMenu.AddItem("Toggle 2D/3D",	ID_2D3D,				ME_ITEMSTYLE_CHECKBOX);
	//m_wndMenu.AddItem("Export TGA",		ID_EXPORT,			null);

	//Register Events
	EVT_REGISTER(EVT_OPDELETING,	(EVENTFNC)&NViewportsWnd::OnOPDeleting	);
	EVT_REGISTER(EVT_RENDER,			(EVENTFNC)&NViewportsWnd::OnRender			);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Event an opearator will been deleted
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NViewportsWnd, OnOPDeleting)
{
	NOperatorNode* pop = (NOperatorNode*)dwParam1;
	if (pop==m_pcurOp)
	{
		m_pcurOp=null;
	}

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	Event rendering asked
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NViewportsWnd, OnRender)
{
	NOperatorNode* pop = (NOperatorNode*)dwParam1;
	if (pop==null || pop->m_pObj==null || (pop!=null && pop->m_bError))
	{
		m_pcurOp = null;
	} else {
		m_pcurOp = pop;
	}

	RedrawWindow();

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	Display a texture from NObject*
//!	\param	pobj	Bitmap pointeur (NBitmap*)
//-----------------------------------------------------------------
void NViewportsWnd::DisplayTexture(NObject* pobj)
{
	//if (m_dwTextureID==0)		return;
/*
	//Copy Pixels to Texture
	NBitmap* ptex = (NBitmap*)pobj;

	if (ptex->GetWidth()!=m_dwTexWidth || ptex->GetHeight()!=m_dwTexHeight)
	{
		m_dwTexWidth	= ptex->GetWidth();
		m_dwTexHeight = ptex->GetHeight();

		DeleteTexture(m_dwTextureID);
		m_dwTextureID = CreateTexture(m_dwTexWidth, m_dwTexHeight);
	}

	if (!m_dwTextureID)
		return;

	CopyPixelsToTexture(m_dwTextureID, m_dwTexWidth, m_dwTexHeight, ptex->GetPixels() );

	//Calc Aspect
	NRect rc = GetClientRect();
	int w = rc.right-rc.left;
	int h = rc.bottom-rc.top;
	float fAspect = (float)w/(float)h;


	//////////////////////////////////////////////////////
	// Init

	//Unit1
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);	// Disable Texture Mapping

	//Unit0
	//glActiveTextureARB(GL_TEXTURE0_ARB);

	glPushAttrib(GL_ALL_ATTRIB_BITS); 

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
//	glEnable(GL_BLEND);											// Enable Blending       (disable alpha testing)

	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);

	//////////////////////////////////////////////////////
	// Matrix

	//Projection matrix
	glMatrixMode(GL_PROJECTION);
	mat4 matProj;
	if (m_bOrtho)
		ortho2D(matProj, -0.5f*fAspect, 0.5f*fAspect, 0.5f, -0.5f);
	else
		perspective(matProj, sinf(nv_half_pi), fAspect, 0.1f, 100.0f);
	glLoadMatrixf((GLfloat*)&matProj.mat_array);

	//Model View matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (m_bOrtho)
	{
		glTranslatef(m_vecTrans.x, m_vecTrans.y, m_vecTrans.z);
		glScalef(m_fScale, m_fScale, m_fScale);
	} else {
		glTranslatef(m_vecTrans.x/m_fScale, -m_vecTrans.y/m_fScale, -1/m_fScale);
		glRotatef(m_vecRot.x, 0, 1, 0);
		glRotatef(m_vecRot.y, 1, 0, 0);
	}

	//Texture matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);

	//////////////////////////////////////////////////////
	// Render States

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_dwTextureID);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0f);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
	if (m_bFiltering)
	{
		if (m_bHasMipmapGeneration && !m_bOrtho)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,		GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,		GL_LINEAR_MIPMAP_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,		GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,		GL_LINEAR);
		}
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,		GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,		GL_NEAREST);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		GL_REPEAT);


	//////////////////////////////////////////////////////
	//Draw texture
	glBegin(GL_TRIANGLE_STRIP);

		float f = 1.0f;	//No Tiling
		if (m_bTiling)	f=3.0f;	//Tiling

		float ty = (float)m_dwTexHeight / (float)h;
		float tx = (float)m_dwTexWidth / (float)h;
		if (!m_bOrtho)
			ty = -ty;

		glTexCoord2f(0.0f, 0.0f);			glVertex3f(-(tx/2.0f)*f, -(ty/2.0f)*f, 0.0f);
		glTexCoord2f(0.0f, 1.0f*f);		glVertex3f(-(tx/2.0f)*f, +(ty/2.0f)*f, 0.0f);
		glTexCoord2f(1.0f*f, 0.0f);		glVertex3f(+(tx/2.0f)*f, -(ty/2.0f)*f, 0.0f);
		glTexCoord2f(1.0f*f, 1.0f*f);	glVertex3f(+(tx/2.0f)*f, +(ty/2.0f)*f, 0.0f);

	glEnd();

	//////////////////////////////////////////////////////
	//Restore Attributs
	glPopAttrib();

	//Updating viewport
	//m_renderer.Update();

	//Texte
	rc.top = rc.bottom-32;
	N2DPainter dc(this);
	//dc.FillSolidRect(rc, RGBA(115,115,115));
	dc.SetTextColor(RGBA(200,255,200));
	NString str;
	str.Format("Texture Size %dx%d", m_dwTexWidth, m_dwTexHeight);
	dc.DrawText(str.Buffer(), rc, NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE);
*/
}


//-----------------------------------------------------------------
//!	\brief	Mouse wheel message
//-----------------------------------------------------------------
void NViewportsWnd::OnMouseWheel(NPoint pos, sdword zDelta)
{
	if (zDelta>0) m_fScale*=2.0f;
	else					m_fScale/=2.0f;

	if (m_fScale>0.9f && m_fScale<1.1f) m_fScale = 1.0f; // Re-"normalize" when at 1, to avoid accumulating numerical errors

	if (m_fScale<0.0625f)	m_fScale=0.0625f;
}

//-----------------------------------------------------------------
//!	\brief	Mouse move message
//-----------------------------------------------------------------
void NViewportsWnd::OnMouseMove(NPoint pos)
{
	if (m_eDragMode == PANNING)
	{
		NPoint ptOffset = pos-m_ptStartDrag;
		NRect rc= GetClientRect();
		m_vecTrans.x+=(float)ptOffset.x / rc.Height();//Width();
		m_vecTrans.y+=(float)ptOffset.y / rc.Height();
		m_ptStartDrag=pos;
	}
	else if (m_eDragMode == ROTATING)
	{
		NPoint ptOffset = pos-m_ptStartDrag;
		m_vecRot.x+=(float)ptOffset.x / 3.0f;
		m_vecRot.y+=(float)ptOffset.y / 3.0f;
		m_ptStartDrag=pos;
	}
}

//-----------------------------------------------------------------
//!	\brief	Mouse Middle button message
//-----------------------------------------------------------------
void NViewportsWnd::OnMButtonDown(NPoint pos)
{
	SetFocus();
	if (m_eDragMode == NONE)
	{
		m_ptStartDrag	= pos;
		m_eDragMode	= PANNING;
		SetCapture();
	}
}

//-----------------------------------------------------------------
//!	\brief	Mouse Middle button message
//-----------------------------------------------------------------
void NViewportsWnd::OnMButtonUp(NPoint pos)
{
	SetFocus();
	if (m_eDragMode == PANNING)
	{
		m_eDragMode	= NONE;
		ReleaseCapture();
	}
}

//-----------------------------------------------------------------
//!	\brief	Mouse Left button message
//-----------------------------------------------------------------
void NViewportsWnd::OnLButtonDown(NPoint pos)
{
	SetFocus();
	if (!m_bOrtho && m_eDragMode == NONE)
	{
		m_ptStartDrag	= pos;
		m_eDragMode	= ROTATING;
		SetCapture();
	}
}

//-----------------------------------------------------------------
//!	\brief	Mouse Left button message
//-----------------------------------------------------------------
void NViewportsWnd::OnLButtonUp(NPoint pos)
{
	SetFocus();
	if (m_eDragMode == ROTATING)
	{
		m_eDragMode	= NONE;
		ReleaseCapture();
	}
}

//-----------------------------------------------------------------
//!	\brief	Mouse Right button message
//-----------------------------------------------------------------
void NViewportsWnd::OnRButtonDown(NPoint pos)
{
	SetFocus();

	ClientToScreen(pos);
	m_wndMenu.TrackPopupMenu(pos);

}


void NViewportsWnd::OnPaint()
{
	NRect rc = GetClientRect();

	N2DPainter dc(this);
	dc.FillSolidRect(rc, RGBA(115,115,115,255));

	////////////////////////////////////////
	//No operator valid
	if (m_pcurOp==null || m_pcurOp->m_pObj==null || m_pcurOp->m_bError)
	{
		//Texte
		//dc.FillSolidRect(rc, RGBA(255,115,115,115));
    if (m_pcurOp==null)
      dc.DrawString("Select an operator by double clicking on it", rc, NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE, RGBA(200,255,200,255) );
		else
      dc.DrawString("Invalid links !", rc, NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE, RGBA(200,255,200,255) );

	////////////////////////////////////////
	//Display operator
	} else {
		DisplayTexture(m_pcurOp->m_pObj);
	}
	

}


//-----------------------------------------------------------------
//!	\brief	Create a GL texture
//!	\param	_w	Width
//!	\param	_h	Height
//!	\return	OpenGL texture ID
//-----------------------------------------------------------------
udword NViewportsWnd::CreateTexture(udword _w, udword _h)
{
	sdword dwTexnameID;

	//Creation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, (GLuint*)&dwTexnameID);
	if (dwTexnameID>=0)
	{
		glBindTexture(GL_TEXTURE_2D, dwTexnameID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, null);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,			GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,			GL_REPEAT);
		/*if (m_bHasMipmapGeneration)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		}*/
		/*if (m_bHasAnisotropicFiltering)
		{
			GLfloat maxAnisotropy = 0;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			// Removing this for now... It seems like it has to be turned of for GL_NEAREST filtering to work
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy); 
		}*/
	}

	return dwTexnameID;
}

//-----------------------------------------------------------------
//!	\brief	Delete a GL texture
//!	\param	_dwTextID	OpenGL Texture ID
//-----------------------------------------------------------------
void NViewportsWnd::DeleteTexture(udword _dwTextID)
{
	glDeleteTextures(1, (GLuint*)&_dwTextID);
}


//-----------------------------------------------------------------
//!	\brief	Copy pixels buffer to a GL texture
//!	\param	_dwtexid	OpenGL Texture ID
//!	\param	_w				Width
//!	\param	_h				Height
//!	\param	_ppixels	Pixels
//-----------------------------------------------------------------
/*void NViewportsWnd::CopyPixelsToTexture(udword _dwtexid, udword _w, udword _h, NRGBA* _ppixels)
{
	glBindTexture(GL_TEXTURE_2D, _dwtexid);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)_ppixels);
	//GLenum e = glGetError();
	//GL_INVALID_ENUM
}*/

//-----------------------------------------------------------------
//!	\brief	Windows Message Command
//-----------------------------------------------------------------
void NViewportsWnd::OnMenuItemClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;

	switch (pmenu->GetClickedCmdID())
	{
		case ID_RESET:
		{
			m_fScale = 1.0f;
			m_vecTrans.x=m_vecTrans.y=m_vecTrans.z=0.0f;
			break;
		}

		case ID_TILEONOFF:
		{
			m_bTiling=!m_bTiling;
			break;
		}

		case ID_FILTERONOFF:
		{
			m_bFiltering=!m_bFiltering;
			break;
		}

		case ID_2D3D:
		{
			m_bOrtho=!m_bOrtho;
			break;
		}

		/*case ID_EXPORT:
		{
			if (m_pcurOp != null && !m_pcurOp->m_bError)
			{
				//Save File Dialog
				NFileChooserDialog dlg;
				dlg.Create("Export TGA...", this, false);
				if (dlg.DoModal())
				{
					NString str = dlg.GetPathName();
					WriteTGA((NBitmap*)m_pcurOp->m_pObj, str);
				}
			}
			break;
		}*/
	}

}
