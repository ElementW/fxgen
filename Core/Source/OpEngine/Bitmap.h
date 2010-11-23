//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Bitmap.h
//! \brief	bitmap resource type
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef BITMAP_H
#define BITMAP_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "../../Include/CoreLib.h"
#include "../Types.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//!	\class	NBitmap
//!	\brief	Bitmap resource type
//-----------------------------------------------------------------
class CORELIB_API NBitmap : public NObject
{
public:
	//Constructor-Destructor
					NBitmap();
	virtual	~NBitmap();

	FDECLARE_CLASS();

	//Methods
	void SetSize(udword _w, udword _h);

	//Members Access
	udword				GetWidth()			{ return m_dwWidth;			}
	udword				GetHeight()			{ return m_dwHeight;		}
	NRGBA*				GetPixels()			{ return m_pbyPixels;		}

//	virtual void					saveBMP(char *filename);

protected:
	//Datas
	NRGBA*		m_pbyPixels;
	udword	m_dwWidth, m_dwHeight;
};

#endif //BITMAP_H

