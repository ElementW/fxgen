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

#ifndef vgvm_interface_h
#define vgvm_interface_h

#include "vgvm/config.h"
#include <stdlib.h>

namespace vgvm
{

const unsigned int MAX_GRADIENT_COUNT = 256;

enum LineCap
{
  LC_BUTT = 0,
  LC_ROUND = 1,
  LC_SQUARE = 2
};

enum LineJoin
{
  LJ_MITER = 0,
  LJ_ROUND = 1,
  LJ_BEVEL = 2
};

enum FillRule
{
  FR_WINDING = 0,
  FR_EVEN_ODD = 1
};

enum Operations
{
  OP_SIZE = 0,
  OP_PUSH_STATE,
  OP_POP_STATE,
  OP_SCALE,
  OP_ROTATE,
  OP_TRANSLATE,
  OP_APPLY_MATRIX,
  OP_SET_MATRIX,
  OP_CREATE_LINEAR_GRADIENT,
  OP_CREATE_RADIAL_GRADIENT,
  OP_ADD_GRADIENT_STOP,
  OP_BEGIN_NEW_PATH,
  OP_BEGIN_NEW_PATH_SEGMENT,
  OP_LINE_WIDTH,
  OP_LINE_CAP,
  OP_LINE_JOIN,
  OP_LINE_MITER,
  OP_FILL_RULE,
  OP_PAINT,
  OP_STROKE,
  OP_FILL,
  OP_SOURCE_COLOR,
  OP_SOURCE_GRADIENT,
  OP_RECTANGLE,
  OP_MOVE_TO,
  OP_LINE_TO,
  OP_CURVE_TO,
  OP_MOVE_TO_RELATIVE,
  OP_LINE_TO_RELATIVE,
  OP_CURVE_TO_RELATIVE,
  OP_CLOSE_PATH,
};

enum ValueType
{
  VT_INT_CONSTANT,
  VT_FLOAT_CONSTANT,
  VT_INT_REGISTER,
  VT_FLOAT_REGISTER
};

struct Value
{
  union 
  {
    float         _float;
    int           _int;
  };

  Value() {}
  Value( float value ) : _float( value ) {}
  Value( int value ) : _int( value ) {}
};

class TypedValue
{
public:
  unsigned char _type;
  Value _value;

  TypedValue() {}
  TypedValue( int value ) : _type( VT_INT_CONSTANT ) { _value._int = value; }
  TypedValue( float value ) : _type( VT_FLOAT_CONSTANT ) { _value._float = value; }
  TypedValue( unsigned char type, float value ) : _type( type ) { _value._float = value; }
  TypedValue( unsigned char type, int value ) : _type( type ) { _value._int = value; }
};




class TypedValueList
{
public:
                    TypedValueList() : _valueCount( 0 ) {}
  
  unsigned int      getValueCount() const { return _valueCount; }
  const TypedValue* getValues() const { return _values; }

  TypedValueList&   operator<<( const TypedValue& value )
  {
    assert( _valueCount<32 );
    _values[_valueCount++] = value;
    
    return *this;
  }

private:
  TypedValue    _values[32];
  unsigned int  _valueCount;
};

typedef void*(*AllocFunc)(size_t);
typedef void(*FreeFunc)(void*);

class VGVM_EXPORT MemoryHandler
{
protected:
  MemoryHandler( AllocFunc allocFunc, FreeFunc freeFunc )
    : _alloc( allocFunc ), _free( freeFunc )
  {
  }

  AllocFunc           _alloc;
  FreeFunc            _free;
};

class VGVM_EXPORT Program : public MemoryHandler
{
public:
                      Program( AllocFunc allocFunc, FreeFunc freeFunc );
  virtual            ~Program();

  unsigned int        getSize() const { return _programSize; }
  unsigned char*      getBuffer() const { return _program; }

  void                addOperation( Operations operation, unsigned char argumentCount = 0, const TypedValue* arguments = 0 );
  void                addOperation( Operations operation, const TypedValueList& arguments );

  void                load( void* buffer, unsigned int size );
  void                clear();

protected:
  void                resize( unsigned int newSize );

  unsigned char*      _program;
  unsigned int        _programSize;
  unsigned int        _reservedSize;
};

class VGVM_EXPORT Context : public MemoryHandler
{
public:
                      Context( AllocFunc allocFunc, FreeFunc freeFunc );
  virtual            ~Context();

  void                setRegisterCount( unsigned int registerCount );
  void                setRegister( unsigned int index, Value value );

  virtual Program*    createProgram() = 0;
  virtual void        executeProgram( Program* program ) = 0;

  virtual void        reset() = 0;

protected:

  int                 getIntValue( const TypedValue& value );
  float               getFloatValue( const TypedValue& value );

  Value*              _registers;
  unsigned int        _registerCount;
};

}

#endif