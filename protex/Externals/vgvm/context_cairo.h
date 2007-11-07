/*
    Copyright (C) 2007 by Anders Stenberg

    This library is free software; you can redistribute it and/or
    modify it under the terms of the Artistic License version 2.0
    as published by the Free Software Foundation

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
    Artistic License for more details.

    The original version of this library can be located at:
    http://sourceforge.net/projects/vgvm/

*/

#ifndef vgvm_context_cairo_h
#define vgvm_context_cairo_h

#include "vgvm/vgvm.h"
#include <cairomm/context.h>
#include <cairomm/surface.h>

namespace vgvm
{

namespace cairo
{

class CairoContextImpl : public CairoContext
{
public:
                      CairoContextImpl( AllocFunc allocFunc, FreeFunc freeFunc );
  virtual            ~CairoContextImpl();

  virtual void        setOutput( unsigned char* outputBuffer, unsigned int width, unsigned int height, bool alpha );

  virtual Program*    createProgram();
  virtual void        executeProgram( Program* program );

  virtual void        reset();

protected:

  Cairo::RefPtr<Cairo::ImageSurface>    _cairoSurface;
  Cairo::RefPtr<Cairo::Context>         _cairoContext;
  Cairo::RefPtr<Cairo::Gradient>        _gradients[MAX_GRADIENT_COUNT];
  
  int                                   _currentGradient;
};

VGVM_EXPORT CairoContext* createCairoContext( AllocFunc allocFunc, FreeFunc freeFunc );

}

}

#endif vgvm_h