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


#include "vgvm/config.h"
#include <assert.h>
#include <memory.h>
#include "vgvm/vgvm.h"

namespace vgvm
{

Program::Program( AllocFunc allocFunc, FreeFunc freeFunc )
  : MemoryHandler( allocFunc, freeFunc ),
    _program( 0 ),
    _programSize( 0 ),
    _reservedSize( 0 )
{
}

Program::~Program()
{
  clear();
}

void Program::clear()
{
  if( _program != 0 )
  {
    _free( _program );
    _program = 0;
  }

  _programSize = 0;
  _reservedSize = 0;
};

void Program::load( void* buffer, unsigned int size )
{
  clear();

  _program = (unsigned char*)_alloc( size );
  _reservedSize = size;
  _programSize = size;

  memcpy( _program, buffer, size );
}

void Program::resize( unsigned int newSize )
{
  if( newSize == _reservedSize )
  {
    return;
  }

  unsigned char* newProgram = (unsigned char*)_alloc( newSize );

  if( _program != 0 )
  {
    memcpy( newProgram, _program, _reservedSize < newSize ? _reservedSize : newSize );
    _free( _program );
  }

  _program = newProgram;
  _reservedSize = newSize;
}

void Program::addOperation( Operations operation, unsigned char argumentCount, const TypedValue* arguments )
{
  unsigned int operationSize = 2 + argumentCount * sizeof(TypedValue);
  unsigned int requiredSize = _programSize + operationSize;
  if( _reservedSize < requiredSize )
  {
    resize( requiredSize * 2 );
  }

  _program[_programSize + 0] = operation;
  _program[_programSize + 1] = argumentCount;
  for( unsigned int i=0; i<argumentCount; ++i )
  {
    *(TypedValue*)&_program[_programSize + 2 + i*sizeof( TypedValue )] = arguments[i];
  }

  _programSize += operationSize;
}

void Program::addOperation( Operations operation, const TypedValueList& arguments )
{
  addOperation( operation, arguments.getValueCount(), arguments.getValues() );
}


Context::Context( AllocFunc allocFunc, FreeFunc freeFunc )
  : MemoryHandler( allocFunc, freeFunc ),
    _registerCount( 0 ),
    _registers( 0 )
{
}

Context::~Context()
{
  _free( _registers );
}

void Context::setRegisterCount( unsigned int registerCount )
{
  if( registerCount == _registerCount )
  {
    return;
  }

  Value* newRegisters = (Value*)_alloc( sizeof(Value) * registerCount );

  if( _registers != 0 )
  {
    memcpy( newRegisters, _registers, sizeof( Value ) * (_registerCount < registerCount ? _registerCount : registerCount) );
    _free( _registers );
  }

  _registers = newRegisters;
  _registerCount = registerCount;
}

void Context::setRegister( unsigned int index, Value value )
{
  assert( index < _registerCount );

  _registers[index] = value;
}

int Context::getIntValue( const TypedValue& value )
{
  assert( value._type == VT_INT_CONSTANT || value._type == VT_INT_REGISTER );

  if( value._type == VT_INT_CONSTANT )
  {
    return value._value._int;
  } else if( value._type == VT_INT_REGISTER )
  {
    assert( (unsigned int)value._value._int < _registerCount );
    return _registers[value._value._int]._int;
  }

  return 0;
}

float Context::getFloatValue( const TypedValue& value )
{
  assert( value._type == VT_FLOAT_CONSTANT || value._type == VT_FLOAT_REGISTER );

  if( value._type == VT_FLOAT_CONSTANT )
  {
    return value._value._float;
  } else if( value._type == VT_FLOAT_REGISTER )
  {
    assert( (unsigned int)value._value._int < _registerCount );
    return _registers[value._value._int]._float;
  }

  return 0;
}

}
