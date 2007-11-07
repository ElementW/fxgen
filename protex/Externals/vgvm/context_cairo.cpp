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


#include <assert.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include "vgvm/vgvm.h"
#include "vgvm/contexts/cairo.h"
#include "context_cairo.h"

namespace vgvm
{

namespace cairo
{

CairoContext* createCairoContext( AllocFunc allocFunc, FreeFunc freeFunc )
{
  return new CairoContextImpl( allocFunc, freeFunc );
}

CairoContext::CairoContext( AllocFunc allocFunc, FreeFunc freeFunc ) 
  : Context( allocFunc, freeFunc ) 
{
}


CairoContextImpl::CairoContextImpl( AllocFunc allocFunc, FreeFunc freeFunc )
  : CairoContext( allocFunc, freeFunc ), _currentGradient( 0 )
{
  reset();
}

CairoContextImpl::~CairoContextImpl()
{
}

void CairoContextImpl::setOutput( unsigned char* outputBuffer, unsigned int width, unsigned int height, bool alpha )
{
  _cairoSurface = Cairo::ImageSurface::create( outputBuffer, alpha ? Cairo::FORMAT_ARGB32 : Cairo::FORMAT_RGB24, width, height, width * (alpha ? 4 : 3) );
  _cairoContext = Cairo::Context::create( _cairoSurface );
}

void CairoContextImpl::reset()
{
  if( _cairoSurface )
  {
    _cairoContext = Cairo::Context::create( _cairoSurface );
  } else {
    _cairoContext.clear();
  }

  for( unsigned int i=0; i<MAX_GRADIENT_COUNT; ++i )
  {
    _gradients[i].clear();
  }
}

Program* CairoContextImpl::createProgram()
{
  return new Program( _alloc, _free );
}

#pragma pack(push, 1)
struct Operation
{
  unsigned char _type;
  unsigned char _argumentCount;
  TypedValue _arguments;
};
#pragma pack(pop)

void CairoContextImpl::executeProgram( Program* program )
{
  if( !_cairoSurface || !_cairoContext )
  {
    return;
  }

  unsigned char* programBuffer = program->getBuffer();
  unsigned int ip = 0;
  while( ip < program->getSize() )
  {
    Operation &operation = *(Operation*)(programBuffer + ip);
    ip += 2 + operation._argumentCount * sizeof( TypedValue );

    switch( operation._type )
    {
    case OP_SIZE:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        float y = getFloatValue( (&operation._arguments)[1] );
        _cairoContext->scale( _cairoSurface->get_width()/x, _cairoSurface->get_height()/y );
      }
      break;
    case OP_PUSH_STATE:
      _cairoContext->save();
      break;
    case OP_POP_STATE:
      _cairoContext->restore();
      break;
    case OP_BEGIN_NEW_PATH:
      _cairoContext->begin_new_path();
      break;
    case OP_BEGIN_NEW_PATH_SEGMENT:
      _cairoContext->begin_new_sub_path();
      break;
    case OP_SCALE:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        float y = getFloatValue( (&operation._arguments)[1] );
        _cairoContext->scale( x, y );
      }
      break;
    case OP_ROTATE:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        _cairoContext->rotate( x );
      }
      break;
    case OP_TRANSLATE:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        float y = getFloatValue( (&operation._arguments)[1] );
        _cairoContext->translate( x, y );
      }
      break;
    case OP_APPLY_MATRIX:
      {
        Cairo::Matrix matrix;
        matrix.xx = getFloatValue( (&operation._arguments)[0] );
        matrix.xy = getFloatValue( (&operation._arguments)[1] );
        matrix.x0 = getFloatValue( (&operation._arguments)[2] );
        matrix.yx = getFloatValue( (&operation._arguments)[3] );
        matrix.yy = getFloatValue( (&operation._arguments)[4] );
        matrix.y0 = getFloatValue( (&operation._arguments)[5] );
        _cairoContext->transform( matrix );
      }
      break;
    case OP_SET_MATRIX:
      {
        Cairo::Matrix matrix;
        matrix.xx = getFloatValue( (&operation._arguments)[0] );
        matrix.xy = getFloatValue( (&operation._arguments)[1] );
        matrix.x0 = getFloatValue( (&operation._arguments)[2] );
        matrix.yx = getFloatValue( (&operation._arguments)[3] );
        matrix.yy = getFloatValue( (&operation._arguments)[4] );
        matrix.y0 = getFloatValue( (&operation._arguments)[5] );
        _cairoContext->set_matrix( matrix );
      }
      break;
    case OP_LINE_WIDTH:
      {
        float w = getFloatValue( (&operation._arguments)[0] );
        _cairoContext->set_line_width( w );
      }
      break;
    case OP_LINE_MITER:
      {
        float w = getFloatValue( (&operation._arguments)[0] );
        _cairoContext->set_miter_limit( w );
      }
      break;
    case OP_LINE_CAP:
      {
        int v = getIntValue( (&operation._arguments)[0] );
        if( v == LC_BUTT ) _cairoContext->set_line_cap( Cairo::LINE_CAP_BUTT );
        else if( v == LC_SQUARE ) _cairoContext->set_line_cap( Cairo::LINE_CAP_SQUARE );
        else if( v == LC_ROUND ) _cairoContext->set_line_cap( Cairo::LINE_CAP_ROUND );
      }
      break;
    case OP_LINE_JOIN:
      {
        int v = getIntValue( (&operation._arguments)[0] );
        if( v == LJ_MITER ) _cairoContext->set_line_join( Cairo::LINE_JOIN_MITER );
        else if( v == LJ_ROUND ) _cairoContext->set_line_join( Cairo::LINE_JOIN_ROUND );
        else if( v == LJ_BEVEL ) _cairoContext->set_line_join( Cairo::LINE_JOIN_BEVEL );
      }
      break;
    case OP_FILL_RULE:
      {
        int v = getIntValue( (&operation._arguments)[0] );
        if( v == FR_WINDING ) _cairoContext->set_fill_rule( Cairo::FILL_RULE_WINDING );
        else if( v == FR_EVEN_ODD ) _cairoContext->set_fill_rule( Cairo::FILL_RULE_EVEN_ODD );
      }
      break;
    case OP_CREATE_LINEAR_GRADIENT:
      {
        int gradientId = getIntValue( (&operation._arguments)[0] );
        float x1 = getFloatValue( (&operation._arguments)[1] );
        float y1 = getFloatValue( (&operation._arguments)[2] );
        float x2 = getFloatValue( (&operation._arguments)[3] );
        float y2 = getFloatValue( (&operation._arguments)[4] );
        _currentGradient = gradientId;
        assert( _gradients[_currentGradient] == 0 );

        _gradients[_currentGradient] = Cairo::LinearGradient::create( x1, y1, x2, y2 );
      }
      break;
    case OP_CREATE_RADIAL_GRADIENT:
      {
        int gradientId = getIntValue( (&operation._arguments)[0] );
        float x1 = getFloatValue( (&operation._arguments)[1] );
        float y1 = getFloatValue( (&operation._arguments)[2] );
        float r1 = getFloatValue( (&operation._arguments)[3] );
        float x2 = getFloatValue( (&operation._arguments)[4] );
        float y2 = getFloatValue( (&operation._arguments)[5] );
        float r2 = getFloatValue( (&operation._arguments)[6] );
        _currentGradient = gradientId;
        assert( _gradients[_currentGradient] == 0 );

        _gradients[_currentGradient] = Cairo::RadialGradient::create( x1, y1, r1, x2, y2, r2 );
        cairo_pattern_set_extend( _gradients[_currentGradient]->cobj(), CAIRO_EXTEND_PAD );
      }
      break;
    case OP_ADD_GRADIENT_STOP:
      {
        assert( _gradients[_currentGradient] != 0 );

        float x = getFloatValue( (&operation._arguments)[0] );
        float r = getFloatValue( (&operation._arguments)[1] );
        float g = getFloatValue( (&operation._arguments)[2] );
        float b = getFloatValue( (&operation._arguments)[3] );
        float a = getFloatValue( (&operation._arguments)[4] );

        _gradients[_currentGradient]->add_color_stop_rgba( x, r, g, b, a );
      }
      break;
    case OP_SOURCE_COLOR:
      {
        float r = getFloatValue( (&operation._arguments)[0] );
        float g = getFloatValue( (&operation._arguments)[1] );
        float b = getFloatValue( (&operation._arguments)[2] );
        float a = getFloatValue( (&operation._arguments)[3] );
        _cairoContext->set_source_rgba( r, g, b, a );
      }
      break;
    case OP_SOURCE_GRADIENT:
      {
        Cairo::Matrix matrix;
        int gradientId = getIntValue( (&operation._arguments)[0] );
        matrix.xx = getFloatValue( (&operation._arguments)[1] );
        matrix.xy = getFloatValue( (&operation._arguments)[2] );
        matrix.x0 = getFloatValue( (&operation._arguments)[3] );
        matrix.yx = getFloatValue( (&operation._arguments)[4] );
        matrix.yy = getFloatValue( (&operation._arguments)[5] );
        matrix.y0 = getFloatValue( (&operation._arguments)[6] );

        assert( _gradients[gradientId] != 0 );
        _gradients[gradientId]->set_matrix( matrix );
        _cairoContext->set_source( _gradients[gradientId] );
      }
      break;
    case OP_RECTANGLE:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        float y = getFloatValue( (&operation._arguments)[1] );
        float w = getFloatValue( (&operation._arguments)[2] );
        float h = getFloatValue( (&operation._arguments)[3] );
        _cairoContext->rectangle( x, y, w, h );
      }
      break;
    case OP_MOVE_TO:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        float y = getFloatValue( (&operation._arguments)[1] );
        _cairoContext->move_to( x, y );
      }
      break;
    case OP_LINE_TO:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        float y = getFloatValue( (&operation._arguments)[1] );
        _cairoContext->line_to( x, y );
      }
      break;
    case OP_CURVE_TO:
      {
        float x1 = getFloatValue( (&operation._arguments)[0] );
        float y1 = getFloatValue( (&operation._arguments)[1] );
        float x2 = getFloatValue( (&operation._arguments)[2] );
        float y2 = getFloatValue( (&operation._arguments)[3] );
        float x3 = getFloatValue( (&operation._arguments)[4] );
        float y3 = getFloatValue( (&operation._arguments)[5] );
        _cairoContext->curve_to( x1, y1, x2, y2, x3, y3 );
      }
      break;
    case OP_MOVE_TO_RELATIVE:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        float y = getFloatValue( (&operation._arguments)[1] );
        _cairoContext->rel_move_to( x, y );
      }
      break;
    case OP_LINE_TO_RELATIVE:
      {
        float x = getFloatValue( (&operation._arguments)[0] );
        float y = getFloatValue( (&operation._arguments)[1] );
        _cairoContext->rel_line_to( x, y );
      }
      break;
    case OP_CURVE_TO_RELATIVE:
      {
        float x1 = getFloatValue( (&operation._arguments)[0] );
        float y1 = getFloatValue( (&operation._arguments)[1] );
        float x2 = getFloatValue( (&operation._arguments)[2] );
        float y2 = getFloatValue( (&operation._arguments)[3] );
        float x3 = getFloatValue( (&operation._arguments)[4] );
        float y3 = getFloatValue( (&operation._arguments)[5] );
        _cairoContext->rel_curve_to(x1, y1, x2, y2, x3, y3);
      }
      break;
    case OP_CLOSE_PATH:
      _cairoContext->close_path();
      break;
    case OP_PAINT:
      _cairoContext->paint();
      break;
    case OP_STROKE:
      _cairoContext->stroke_preserve();
      break;
    case OP_FILL:
      _cairoContext->fill_preserve();
      break;
    default:
      break;
    }
  }
}

}

}