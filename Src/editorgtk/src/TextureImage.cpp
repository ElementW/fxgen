//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		TextureImage.cpp
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

#include "pch.h"
#include "globals.h"

TextureImage::TextureImage(GtkImage* cobject, const RefPtr<Xml>& refGlade)
: Gtk::Image(cobject)
{
}

/// attach a NBitmap
void TextureImage::SetBitmap(NBitmap* bitmap)
{
	size_t w = bitmap->GetWidth(), h = bitmap->GetHeight();
	size_t rowstride = w * sizeof(RGBA);
	size_t bpp = 8 * sizeof(RGBA) / 4;

	RefPtr<Gdk::Pixbuf> buf = Gdk::Pixbuf::create_from_data(
		reinterpret_cast<const guint8*>(bitmap->GetPixels()), Gdk::COLORSPACE_RGB, true, bpp, w, h, rowstride);

	set(buf);
}

void TextureImage::clear()
{
	RefPtr<Gdk::Pixbuf> buf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, 1, 1);
	set(buf);
}
