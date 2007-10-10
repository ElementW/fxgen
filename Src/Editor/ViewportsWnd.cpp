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
#include "pch.h"
#include "ViewportsWnd.h"
#include "TGAWriter.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define ID_RESET			100
#define ID_TILEONOFF	101
#define ID_FILTERONOFF	102
#define ID_EXPORT	103

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
	m_hfontNormal	= null;
	m_pcurObject	= null;
	m_dwTextureID = 0;
	m_dwTexWidth=m_dwTexHeight=0;
	m_fScale			= 1.0f;
	m_bPanning		= false;
	m_vecTrans.x=m_vecTrans.y=m_vecTrans.z=0.0f;
	m_bTiling			= false;
	m_bFiltering			= true;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NViewportsWnd::~NViewportsWnd(void)
{
	if (m_hfontNormal)	::DeleteObject(m_hfontNormal);
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NViewportsWnd::Create(const char* name, const NRect& rect, NWnd* parent)
{
	NViewportsCtrl::Create(name, rect, parent);

	//Create Normal Font
	m_hfontNormal = ::CreateFont(
		10, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Lucida console");

	//Initialize Renderer
	m_renderer.Init(m_W32HWnd, false);
	m_renderer.Resize(rect.right-rect.left, rect.bottom-rect.top);
	m_renderer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_renderer.Update();

	m_dwTextureID = m_renderer.CreateTexture(256,256);

	//Create context menu
	m_wndMenu.Create("Viewport:", this);
	m_wndMenu.AddItem("Reset",			ID_RESET,				0);
	m_wndMenu.AddItem("Tiling",			ID_TILEONOFF,		ME_ITEMSTYLE_CHECKBOX);
	m_wndMenu.AddItem("Filtering",	ID_FILTERONOFF,	ME_ITEMSTYLE_CHECKBOX);
	m_wndMenu.AddItem("Export TGA",	ID_EXPORT,			0);

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
	NOperator* pop = (NOperator*)dwParam1;
	if (pop->m_pObj==m_pcurObject)
	{
		m_pcurObject=null;
	}

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	Event rendering asked
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NViewportsWnd, OnRender)
{
	//Texte
	NRect rc = GetClientRect();
	NGraphics dc(this);
	dc.FillSolidRect(rc, RGB(115,115,115));
	dc.SetFont(m_hfontNormal);
	dc.SetTextColor(RGB(200,255,200));

	//Check operator
	NOperator* pop = (NOperator*)dwParam1;
	if (pop==null || pop->m_pObj==null || (pop!=null && pop->m_bError))
	{
		m_pcurObject = null;

		if (pop==null)
			dc.DrawText("Select an operator by double clicking on it", rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		else if (pop && pop->m_bError)
			dc.DrawText("Invalid links !", rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		return 0;
	}

	NString str;
	str.Format("Texture Size w%d h%d"
	dc.DrawText(str, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	//Get resource
	m_pcurObject = pop->m_pObj;

	//Display Operator result
	if (m_pcurObject!=null)
	{
		if (strcmp(m_pcurObject->GetRTClass()->m_pszClassName, "NBitmap") == 0)
			DisplayTexture(m_pcurObject);

	//Display nothing
	} else {
		m_renderer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_renderer.Update();
	}

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	Display a texture from NObject*
//!	\param	pobj	Bitmap pointeur (NBitmap*)
//-----------------------------------------------------------------
void NViewportsWnd::DisplayTexture(NObject* pobj)
{
	if (m_dwTextureID==0)		return;

	//Resize Viewport Renderer
	NRect rect = GetClientRect();
	m_renderer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_renderer.Resize(rect.right-rect.left, rect.bottom-rect.top);

	//Copy Pixels to Texture
	NBitmap* ptex = (NBitmap*)pobj;

	if (ptex->GetWidth()!=m_dwTexWidth || ptex->GetHeight()!=m_dwTexHeight)
	{
		m_dwTexWidth	= ptex->GetWidth();
		m_dwTexHeight = ptex->GetHeight();

		m_renderer.DeleteTexture(m_dwTextureID);
		m_dwTextureID = m_renderer.CreateTexture(m_dwTexWidth, m_dwTexHeight);
	}

	m_renderer.CopyPixelsToTexture(m_dwTextureID, m_dwTexWidth, m_dwTexHeight, ptex->GetPixels() );

	//Calc Aspect
	NRect rc = GetClientRect();
	int w = rc.right-rc.left;
	int h = rc.bottom-rc.top;
	float fAspect = 1.0f;//(float)w/(float)h;

	//Unit1
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);	// Disable Texture Mapping

	//Unit0
	//glActiveTextureARB(GL_TEXTURE0_ARB);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);											// Enable Blending       (disable alpha testing)

	glMatrixMode(GL_PROJECTION);
	mat4 matProj;
	ortho2D(matProj, -0.5f*fAspect, 0.5f*fAspect, 0.5f, -0.5f);
	glLoadMatrixf((GLfloat*)&matProj.mat_array);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(m_vecTrans.x, m_vecTrans.y, m_vecTrans.z);
	glScalef(m_fScale, m_fScale, m_fScale);


	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_dwTextureID);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0f);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
        if (m_bFiltering)
        {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,		GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,		GL_LINEAR);
        } else {
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,		GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,		GL_NEAREST);
        }
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		GL_REPEAT);

	glBegin(GL_TRIANGLE_STRIP);
	//glTexCoord2f(0.0f, 0.0f);		glVertex3f(-0.5f, 0.0f, 0.0f);
	//glTexCoord2f(0.0f, 1.0f);		glVertex3f(-0.5f, 1.0f, 0.0f);
	//glTexCoord2f(1.0f, 0.0f);		glVertex3f(0.5f, 0.0f, 0.0f);
	//glTexCoord2f(1.0f, 1.0f);		glVertex3f(0.5f, 1.0f, 0.0f);

	float f = 1.0f;	//No Tiling
	if (m_bTiling)	f=3.0f;	//Tiling

	float tx = (float)m_dwTexWidth	/ (float)w;
	float ty = (float)m_dwTexHeight / (float)h;

	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-(tx/2.0f)*f, -(ty/2.0f)*f, 0.0f);
	glTexCoord2f(0.0f, 1.0f*f);		glVertex3f(-(tx/2.0f)*f, +(ty/2.0f)*f, 0.0f);
	glTexCoord2f(1.0f*f, 0.0f);		glVertex3f(+(tx/2.0f)*f, -(ty/2.0f)*f, 0.0f);
	glTexCoord2f(1.0f*f, 1.0f*f);	glVertex3f(+(tx/2.0f)*f, +(ty/2.0f)*f, 0.0f);

	glEnd();

	//Updating viewport
	m_renderer.Update();
}


//-----------------------------------------------------------------
//!	\brief	Mouse wheel message
//-----------------------------------------------------------------
void NViewportsWnd::OnMouseWheel(udword flags, sword zDelta, NPoint point)
{
	if (zDelta>0) m_fScale*=2.0f;
	else					m_fScale/=2.0f;

        if (m_fScale>0.9f && m_fScale<1.1f) m_fScale = 1.0f; // Re-"normalize" when at 1, to avoid accumulating numerical errors

	if (m_fScale<0.0625f)	m_fScale=0.0625f;
}

//-----------------------------------------------------------------
//!	\brief	Mouse move message
//-----------------------------------------------------------------
void NViewportsWnd::OnMouseMove(udword flags, NPoint pos)
{
	if (m_bPanning)
	{
			NPoint ptOffset = pos-m_ptStartPan;
			m_vecTrans.x+=(float)ptOffset.x / 300.0f;
			m_vecTrans.y+=(float)ptOffset.y / 300.0f;
			m_ptStartPan=pos;
	}

}

//-----------------------------------------------------------------
//!	\brief	Mouse Middle button message
//-----------------------------------------------------------------
void NViewportsWnd::OnMButtonDown(udword flags, NPoint pos)
{
	SetFocus();
	m_ptStartPan	= pos;
	m_bPanning		= true;
	SetCapture();
}

//-----------------------------------------------------------------
//!	\brief	Mouse Middle button message
//-----------------------------------------------------------------
void NViewportsWnd::OnMButtonUp(udword flags, NPoint pos)
{
	m_bPanning		= false;
	ReleaseCapture();
}

//-----------------------------------------------------------------
//!	\brief	Mouse Left button message
//-----------------------------------------------------------------
void NViewportsWnd::OnLeftButtonUp(udword flags, NPoint pos)
{
	SetFocus();
}

//-----------------------------------------------------------------
//!	\brief	Mouse Right button message
//-----------------------------------------------------------------
void NViewportsWnd::OnRightButtonDown(udword flags, NPoint pos)
{
	SetFocus();

	POINT pt;	::GetCursorPos(&pt);	//Cursor position even with keyboard 'Context key'

	NPoint pT(pt.x, pt.y);
	m_wndMenu.TrackPopupMenu(pT);

}

//-----------------------------------------------------------------
//!	\brief	Command message
//!	\param	_id	Command ID
//-----------------------------------------------------------------
void NViewportsWnd::OnCommand(udword _id)
{
	switch (_id)
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

                case ID_EXPORT:
                {
                        if (m_pcurObject != null &&
                            strcmp(m_pcurObject->GetRTClass()->m_pszClassName, "NBitmap") == 0)
                        {
	                  //Save File Dialog
	                  NFileDialog dlg;
	                  dlg.Create("Export TGA...", this, false);
	                  if (dlg.DoModal())
	                  {
		                  NString str = dlg.GetPathName();
                                  WriteTGA((NBitmap*)m_pcurObject, str);
	                  }
                        }
                        break;
                }
        }

}
