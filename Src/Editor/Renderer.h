//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Renderer.h
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
#include "Core.h"

#include <windows.h>

#ifdef __WINE__
#define WINGDIAPI /* linux GL/gl.h has this thing, whatever it is */
#endif
#include <GL/gl.h>				// Header File For The OpenGL32 Library
#include <GL/glext.h>			// Header File For The OpenGL32 Library extensions

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------

/*
//BlendingFactorDest
GL_ZERO
GL_ONE
GL_SRC_COLOR
GL_ONE_MINUS_SRC_COLOR
GL_SRC_ALPHA
GL_ONE_MINUS_SRC_ALPHA
GL_DST_ALPHA
GL_ONE_MINUS_DST_ALPHA
*/

/*
//BlendingFactorSrc
GL_ZERO
GL_ONE
GL_DST_COLOR
GL_ONE_MINUS_DST_COLOR
GL_SRC_ALPHA_SATURATE
GL_SRC_ALPHA
GL_ONE_MINUS_SRC_ALPHA
GL_DST_ALPHA
GL_ONE_MINUS_DST_ALPHA
*/

//-----------------------------------------------------------------
//!	\class	NGLRenderer
//!	\brief	OpenGL renderer
//-----------------------------------------------------------------
class NGLRenderer
{
public:
	//Constructor-Destructor
					NGLRenderer();
	virtual	~NGLRenderer();

	//Methods
	bool	Init(HWND hwnd, bool bFullScreen);
	void	Exit();

	void	Resize(int _nWidth, int _nHeight);

	HWND	GetHWnd()		{ return m_hWnd;	}
	HDC		GetHDC()		{ return m_hDC;		}

	void Clear (udword _dwFlags); //Flags for clearing
	void Update(); //Blitting(Wnd) or flipping(FS) surfaces

	//Texture
	udword	CreateTexture(udword w, udword h);
	void		DeleteTexture(udword _dwTextID);
	void		CopyPixelsToTexture(udword _dwtexid, udword _w, udword _h, RGBA* _pPixels);

	//Primitives methods
/*
	void	SetVertexBuffer	(udword dwNumVertices, void* pVertices, VERTEXFORMAT vf)	{}
	void	DrawTriangles		(uword wNumIndices, uword* pwIndices)											{}
	void	DrawLine(vec3 p1, vec3 p2);
	void	DrawQuad(vec3 p1, vec3 p2);
*/

	//Matrices
	void	SetProjectMatrix(const mat4& m);
	void	SetViewMatrix		(const mat4& m);
	void	SetWorldMatrix	(const mat4& m);

	//Datas
  HDC						m_hDC;              // Private GDI Device Context
  HGLRC					m_hRC;              // Permanent Rendering Context
  HWND					m_hWnd;             // Holds Our Window Handle
	bool					m_bFullScreen;
	bool				m_bHasMipmapGeneration;
	bool				m_bHasAnisotropicFiltering;

	mat4					m_mtxProj, m_mtxView, m_mtxWorld;
};

//-----------------------------------------------------------------
// Externs
//-----------------------------------------------------------------
extern NGLRenderer* gGetRenderer();
