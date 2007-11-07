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

#ifndef vgvm_cairo_interface_h
#define vgvm_cairo_interface_h

#include "vgvm/vgvm.h"

namespace vgvm
{

namespace cairo
{

class VGVM_EXPORT CairoContext : public Context
{
public:
  
  virtual void setOutput( unsigned char* outputBuffer, unsigned int width, unsigned int height, bool alpha ) = 0;
protected:

  CairoContext( AllocFunc allocFunc, FreeFunc freeFunc );
};

VGVM_EXPORT CairoContext* createCairoContext( AllocFunc allocFunc = malloc, FreeFunc freeFunc = free );

}

}

#endif