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


#ifndef vgvm_config_h
#define vgvm_config_h

#if _WIN32

# if BUILDING_VGVM_DLL
#   define VGVM_EXPORT __declspec(dllexport)
# elif USING_VGVM_DLL
#   define VGVM_EXPORT __declspec(dllimport)
# else
#   define VGVM_EXPORT
# endif
#else
# define VGVM_EXPORT
#endif

#endif
