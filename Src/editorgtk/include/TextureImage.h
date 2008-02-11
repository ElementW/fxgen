//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		TextureImage.h
//! \brief	FxGen GTK editor - an image widget that can display FxGen NBitmaps
//!
//!	\author	Sebastian Olter (qduaty@gmail.com)
//!	\date		07-02-2008
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

#ifndef TEXTUREIMAGE_H
#define TEXTUREIMAGE_H

//#include <pch.h>
#include "fxgen_pch.h"

class TextureImage : public Gtk::Image
{
	public:
		TextureImage(GtkImage* cobject, const RefPtr<Xml>& refGlade);
		void SetBitmap(NBitmap*);
	protected:
	private:
};

#endif // TEXTUREIMAGE_H
