//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Renderer.cpp
//! \brief	OpenGL renderer
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
//									Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Renderer.h"

//-----------------------------------------------------------------
//									Defines
//-----------------------------------------------------------------

#define GL_GENERATE_MIPMAP_SGIS			0x8191
#define GL_TEXTURE_MAX_ANISOTROPY_EXT		0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT	0x84FF

//-----------------------------------------------------------------
//									Variables
//-----------------------------------------------------------------
NGLRenderer* g_pcGLRenderer = NULL;
NGLRenderer*	gGetRenderer()						{	return g_pcGLRenderer; }


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NGLRenderer Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NGLRenderer :: NGLRenderer()
{
	g_pcGLRenderer	= this;

	m_hDC						= NULL;        // Private GDI Device Context
	m_hRC						= NULL;        // Permanent Rendering Context
	m_hWnd					= NULL;        // Holds Our Window Handle
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NGLRenderer::~NGLRenderer()
{
	g_pcGLRenderer	= NULL;
}


//-----------------------------------------------------------------
//!	\brief	control Creation
//!	\param	hwnd 				win32 windows handle
//!	\param	bFullScreen true is fullscreen mode
//!	\return	true if success
//-----------------------------------------------------------------
bool NGLRenderer::Init(HWND hwnd, bool bFullScreen)
{
	m_hWnd=hwnd;
	m_bFullScreen=bFullScreen;

	//Video Mode
	if (bFullScreen)
	{
		DEVMODE dmScreenSettings;
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= 640;
		dmScreenSettings.dmPelsHeight	= 480;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
	}

	//Pixel Format
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.dwLayerMask = PFD_MAIN_PLANE;

	// Did We Get A Device Context?
	if (!(m_hDC = GetDC(m_hWnd)))
	{
		Exit();		return false;
	}

	// Did Windows Find A Matching Pixel Format?
	GLuint   PixelFormat;
	if (!(PixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
	{
		Exit();		return false;
	}

	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(m_hDC, PixelFormat, &pfd))
	{
		Exit(); 	return false;
	}

	// Are We Able To Get A Rendering Context?
	if (!(m_hRC=wglCreateContext(m_hDC)))
	{
		Exit();		return false;
	}

	// Try To Activate The Rendering Context
	if(!wglMakeCurrent(m_hDC, m_hRC))
	{
		Exit();		return false;
	}

	// Initialisation de l'etat de rendu
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	const char* extensionString = (const char*)glGetString(GL_EXTENSIONS);
	m_bHasMipmapGeneration = extensionString != 0 && strstr(extensionString, "GL_SGIS_generate_mipmap") != 0;
	m_bHasAnisotropicFiltering = extensionString != 0 && strstr(extensionString, "GL_EXT_texture_filter_anisotropic") != 0;

	return true; // Success
}


//-----------------------------------------------------------------
//!	\brief	Exit renderer
//-----------------------------------------------------------------
void NGLRenderer::Exit()
{
  if (m_hRC)
	{
		if (m_bFullScreen)
			ChangeDisplaySettings(NULL, 0);

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hRC);
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
		m_hRC = NULL;
	}

}

//-----------------------------------------------------------------
//!	\brief	Resize renderer
//!	\param	_nWidth 	viewport width
//!	\param	_nHeight	viewport height
//-----------------------------------------------------------------
void NGLRenderer::Resize(int _nWidth, int _nHeight)
{
	// Prevent A Divide By Zero By
	if (_nHeight == 0)		_nHeight = 1;

	glViewport(0, 0, _nWidth, _nHeight); // Reset The Current Viewport

//	glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
//	glLoadIdentity();                // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
//	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
//	glLoadIdentity();           // Reset The Modelview Matrix
}

//-----------------------------------------------------------------
//!	\brief	Clear viewport
//!	\param	_dwFlags 	clear flags (see glClear())
//-----------------------------------------------------------------
void NGLRenderer::Clear (udword _dwFlags)
{
	glClearColor(0.45f, 0.45f, 0.45f, 0.0f);				// Background
	glClear(_dwFlags);
}

//-----------------------------------------------------------------
//!	\brief	Update viewport
//-----------------------------------------------------------------
void NGLRenderer::Update()
{
	//Update
	SwapBuffers(m_hDC);
}


//-----------------------------------------------------------------
//!	\brief	Set projection matrix
//!	\param	m		matrix
//-----------------------------------------------------------------
void NGLRenderer::SetProjectMatrix(const mat4& m)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)&m.mat_array);
	m_mtxProj = m;

	//glLoadIdentity();
	//gluPerspective(nv_to_deg * m_fFov,(GLfloat)(rect.right-rect.left)/(GLfloat)(rect.bottom-rect.top), m_fNearClip, m_fFarClip);
	//GLfloat glmtx[16];
	//glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&glmtx);

}


//-----------------------------------------------------------------
//!	\brief	Set view matrix
//!	\param	m		matrix
//-----------------------------------------------------------------
void NGLRenderer::SetViewMatrix(const mat4& m)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)&m.mat_array);
	m_mtxView = m;
}


//-----------------------------------------------------------------
//!	\brief	Set world matrix
//!	\param	m		matrix
//-----------------------------------------------------------------
void NGLRenderer::SetWorldMatrix(const mat4& m)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)&m_mtxView.mat_array);
	glMultMatrixf((GLfloat*)&m.mat_array);

	m_mtxWorld = m;
}


//-----------------------------------------------------------------
//!	\brief	Create a GL texture
//!	\param	_w	Width
//!	\param	_h	Height
//!	\return	OpenGL texture ID
//-----------------------------------------------------------------
udword NGLRenderer::CreateTexture(udword _w, udword _h)
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
		if (m_bHasMipmapGeneration)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		}
		if (m_bHasAnisotropicFiltering)
		{
			GLfloat maxAnisotropy = 0;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			// Removing this for now... It seems like it has to be turned of for GL_NEAREST filtering to work
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy); 
		}
	}

	return dwTexnameID;
}

//-----------------------------------------------------------------
//!	\brief	Delete a GL texture
//!	\param	_dwTextID	OpenGL Texture ID
//-----------------------------------------------------------------
void NGLRenderer::DeleteTexture(udword _dwTextID)
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
void NGLRenderer::CopyPixelsToTexture(udword _dwtexid, udword _w, udword _h, RGBA* _ppixels)
{
	glBindTexture(GL_TEXTURE_2D, _dwtexid);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)_ppixels);
	//GLenum e = glGetError();
	//GL_INVALID_ENUM
}