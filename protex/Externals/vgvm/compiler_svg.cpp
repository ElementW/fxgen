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
#include <math.h>
#include <assert.h>

#include <cairo.h>
#include <tinyxml.h>

#include "vgvm/vgvm.h"

namespace vgvm
{

namespace svg
{

const float PI = float(3.14159265358979323846);

struct RGBAColor
{
  union
  {
    struct { unsigned char r, g, b, a; };
    unsigned int color;
  };

};

struct ElementIdPair
{
  const TiXmlElement *element;
  const char *id;
};

class ElementIndexer : public TiXmlVisitor
{
public:
  ElementIdPair *elements;
  int &elementCount;

  ElementIndexer(ElementIdPair *elements, int &elementCount) : elements(elements), elementCount(elementCount) {}

	  virtual bool VisitEnter( const TiXmlElement& element, const TiXmlAttribute* /*firstAttribute*/ )
  { 
    const char *id = element.Attribute("id");
    if (id != 0)
    {
      elements[elementCount].element = &element;
      elements[elementCount].id = id;
      elementCount++;
    }
    return true; 
  }
};

class StopParser : public TiXmlVisitor
{
public:
  Program* _output;

  StopParser( Program* output ) : _output( output ) {}

  void executeGradientStop( float offset, const char *style )
  {
    RGBAColor stopColor;
    stopColor.color = 0;
    float stopOpacity; stopOpacity = 1;

    if (style != 0)
    {
      size_t styleLength = strlen(style)+1;
      char *styleCopy = new char[styleLength];
      memcpy(styleCopy, style, styleLength);

      const char *token = strtok(styleCopy, ";");
      while (token != 0)
      {
        char *value = (char*)strchr(token, ':');
        if (value == 0)
          continue;
        value[0] = 0;
        value++;
        char *attribute = (char*)token;

        if (strcmp(attribute, "stop-color") == 0)
        {
          if (value[0] == '#')
          {
            RGBAColor color;
            sscanf(value+1, "%x", &color);
            stopColor.r = color.b;
            stopColor.g = color.g;
            stopColor.b = color.r;
          }
        }
        else if (strcmp(attribute, "stop-opacity") == 0)
        {
          stopOpacity = (float)atof(value);
        }

        token = strtok(0, ";");
      }
      
      delete[] styleCopy;
    }

    _output->addOperation( OP_ADD_GRADIENT_STOP, TypedValueList() << TypedValue( offset ) << 
                                                                     TypedValue( stopColor.r/255.0f ) << 
                                                                     TypedValue( stopColor.g/255.0f ) << 
                                                                     TypedValue( stopColor.b/255.0f ) << 
                                                                     TypedValue( stopOpacity ) ); 
  }

  virtual bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* /*firstAttribute*/ )	
  { 
    if (strcmp(element.Value(), "stop") == 0)
    {
      double offset = 0.0f;
      element.Attribute("offset", &offset);
      executeGradientStop( (float)offset, element.Attribute("style") );
    }
    return true; 
  }
};

class SVGConverter : public TiXmlVisitor
{
public:

  Program* _output;

  bool _fillStack[1000];
  bool _strokeStack[1000];
  int _stackPointer;

  struct GradientMatrixPair
  {
    const char *id;
    cairo_matrix_t matrix;
  } _gradients[100];
  int _gradientCount;

  ElementIdPair *_elements;
  int _elementCount;

  SVGConverter( Program* output, ElementIdPair *elements, int elementCount ) 
    : _output( output ), _stackPointer( 0 ), _gradientCount( 0 ), _elements( elements ), _elementCount( elementCount )
  {
    _fillStack[0] = false;
    _strokeStack[0] = true;
  }

  void executeStyle(const char *style, bool styleOnly = false)
  {
    bool stroke = _strokeStack[_stackPointer];
    RGBAColor strokeColor; strokeColor.color = 0;
    float strokeOpacity = 1;
    float strokeWidth = 1.0f;
    float strokeMiter = 4.0f;
    LineCap strokeCap = LC_BUTT;
    LineJoin strokeJoin = LJ_MITER;
    int strokeGradient = -1;

    bool fill = _fillStack[_stackPointer];
    RGBAColor fillColor; fillColor.color = 0;
    float fillOpacity = 1.0f;
    FillRule fillRule = FR_WINDING;
    int fillGradient = -1;
    
    float opacity = 1;

    if (style != 0)
    {
      size_t styleLength = strlen(style)+1;
      char *styleCopy = new char[styleLength];
      memcpy(styleCopy, style, styleLength);

      const char *token = strtok(styleCopy, ";");
      while (token != 0)
      {
        char *value = (char*)strchr(token, ':');
        if (value == 0)
          continue;
        value[0] = 0;
        value++;
        char *attribute = (char*)token;

        if( strcmp(attribute, "stroke") == 0 )
        {
          if( value[0] == '#' )
          {
            stroke = true;
            RGBAColor color;
            sscanf(value+1, "%x", &color);
            strokeColor.r = color.b;
            strokeColor.g = color.g;
            strokeColor.b = color.r;
          } else if( value[0] == 'u' && value[1] == 'r' && value[2] == 'l' )
          {
            value[strlen(value)-1] = 0;
            for( int i=0; i<_gradientCount; ++i )
            {
              if( strcmp( _gradients[i].id, value+5 ) == 0 )
              {
                strokeGradient = i;
                break;
              }
            }
            stroke = true;
          } else if (strcmp(value, "none") == 0)
          {
            stroke = false;
          }
        }
        else if (strcmp(attribute, "fill") == 0)
        {
          if (value[0] == '#')
          {
            fill = true;
            RGBAColor color;
            sscanf(value+1, "%x", &color);
            fillColor.r = color.b;
            fillColor.g = color.g;
            fillColor.b = color.r;
          } else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l')
          {
            value[strlen(value)-1] = 0;
            for( int i=0; i<_gradientCount; ++i )
            {
              if( strcmp( _gradients[i].id, value+5 ) == 0 )
              {
                fillGradient = i;
                break;
              }
            }
            fill = true;
          } else if (strcmp(value, "none") == 0)
          {
            fill = false;
          }
        }
        else if (strcmp(attribute, "stroke-width") == 0)
        {
          strokeWidth = (float)atof(value);
        }
        else if (strcmp(attribute, "stroke-miter") == 0)
        {
          strokeMiter = (float)atof(value);
        }
        else if (strcmp(attribute, "stroke-opacity") == 0)
        {
          strokeOpacity = (float)atof(value);
        }
        else if (strcmp(attribute, "stroke-linecap") == 0)
        {
          if (strcmp(value, "butt") == 0)
            strokeCap = LC_BUTT;
          else if (strcmp(value, "square") == 0)
            strokeCap = LC_SQUARE;
          else if (strcmp(value, "round") == 0)
            strokeCap = LC_ROUND;
        }
        else if (strcmp(attribute, "stroke-linejoin") == 0)
        {
          if (strcmp(value, "miter") == 0)
            strokeJoin = LJ_MITER;
          else if (strcmp(value, "round") == 0)
            strokeJoin = LJ_ROUND;
          else if (strcmp(value, "bevel") == 0)
            strokeJoin = LJ_BEVEL;
        }
        else if (strcmp(attribute, "fill-opacity") == 0)
        {
          fillOpacity = (float)atof(value);
        }
        else if (strcmp(attribute, "fill-rule") == 0)
        {
          if (strcmp(value, "nonzero") == 0)
            fillRule = FR_WINDING;
          else if (strcmp(value, "evenodd") == 0)
            fillRule = FR_EVEN_ODD;
        }
        else if (strcmp(attribute, "opacity") == 0)
        {
          opacity = (float)atof(value);
        }

        token = strtok(0, ";");
      }
      
      delete[] styleCopy;
    }

    if(fill)
    {
      if (fillGradient == -1)
      {
        _output->addOperation( OP_SOURCE_COLOR, TypedValueList() << TypedValue( fillColor.r/255.0f) << 
                                                                    TypedValue( fillColor.g/255.0f) << 
                                                                    TypedValue( fillColor.b/255.0f) << 
                                                                    TypedValue( fillOpacity * opacity ) );
      }
      else
      {
        assert( _gradients[fillGradient].id != 0 );
        _output->addOperation( OP_SOURCE_GRADIENT, TypedValueList() << TypedValue( fillGradient ) <<
                                                                       TypedValue( (float)_gradients[fillGradient].matrix.xx ) <<
                                                                       TypedValue( (float)_gradients[fillGradient].matrix.xy ) <<
                                                                       TypedValue( (float)_gradients[fillGradient].matrix.x0 ) <<
                                                                       TypedValue( (float)_gradients[fillGradient].matrix.yx ) <<
                                                                       TypedValue( (float)_gradients[fillGradient].matrix.yy ) <<
                                                                       TypedValue( (float)_gradients[fillGradient].matrix.y0 ) );
      }
      _output->addOperation( OP_FILL_RULE, TypedValueList() << TypedValue( (int)fillRule ) );
      if( !styleOnly ) _output->addOperation( OP_FILL );
    }
    _fillStack[_stackPointer+1] = fill;

    if(stroke)
    {
      if (strokeGradient == -1)
      {
        _output->addOperation( OP_SOURCE_COLOR, TypedValueList() << TypedValue( strokeColor.r/255.0f) << 
                                                                    TypedValue( strokeColor.g/255.0f) << 
                                                                    TypedValue( strokeColor.b/255.0f) << 
                                                                    TypedValue( strokeOpacity * opacity ) );
      }
      else
      {
        assert( _gradients[fillGradient].id != 0 );
        _output->addOperation( OP_SOURCE_GRADIENT, TypedValueList() << TypedValue( fillGradient ) <<
                                                                       TypedValue( (float)_gradients[strokeGradient].matrix.xx ) <<
                                                                       TypedValue( (float)_gradients[strokeGradient].matrix.xy ) <<
                                                                       TypedValue( (float)_gradients[strokeGradient].matrix.x0 ) <<
                                                                       TypedValue( (float)_gradients[strokeGradient].matrix.yx ) <<
                                                                       TypedValue( (float)_gradients[strokeGradient].matrix.yy ) <<
                                                                       TypedValue( (float)_gradients[strokeGradient].matrix.y0 ) );
      }
      _output->addOperation( OP_LINE_WIDTH, TypedValueList() << TypedValue( strokeWidth ) );
      _output->addOperation( OP_LINE_CAP, TypedValueList() << TypedValue( (int)strokeCap ) );
      _output->addOperation( OP_LINE_JOIN, TypedValueList() << TypedValue( (int)strokeJoin ) );
      _output->addOperation( OP_LINE_MITER, TypedValueList() << TypedValue( strokeMiter ) );
      if( !styleOnly ) _output->addOperation( OP_STROKE );
    }
    _fillStack[_stackPointer+1] = stroke;

    _stackPointer++;
  }

  cairo_matrix_t executeTransform(const char *transform, bool apply = true)
  {
    cairo_matrix_t resultMatrix;
    cairo_matrix_init_identity(&resultMatrix);

    if (transform != 0)
    {
      size_t transformLength = strlen(transform)+1;
      char *transformCopy = new char[transformLength];
      memcpy(transformCopy, transform, transformLength);

      const char *token = strtok(transformCopy, "()");
      while (token != 0)
      {
        const char *operation = token+strspn(token, " ");
        const char *arguments = strtok(0, "()");
        if (arguments == 0)
          break;

        if (strcmp(operation, "translate") == 0)
        {
          float x = 0, y = 0;
          if (sscanf(arguments, "%f, %f", &x, &y) == 2)
          {
            cairo_matrix_translate(&resultMatrix, x, y);
          }
        }
        else if (strcmp(operation, "rotate") == 0)
        {
          float x = 0;
          if (sscanf(arguments, "%f", &x) == 1)
          {
            x *= (float)(PI / 180.0f) ;
            cairo_matrix_rotate(&resultMatrix, x);
          }
        }
        else if (strcmp(operation, "scale") == 0)
        {
          float x = 0, y = 0;
          if (sscanf(arguments, "%f, %f", &x, &y) == 2)
          {
            cairo_matrix_scale(&resultMatrix, x, y);
          }
        }
        else if( strcmp( operation, "matrix" ) == 0 )
        {
          float xx = 0, xy = 0, x0 = 0, yx = 0, yy = 0, y0 = 0;
          if( sscanf( arguments, "%f, %f, %f, %f, %f, %f", &xx, &yx, &xy, &yy, &x0, &y0) == 6 )
          {
            cairo_matrix_t matrix;
            cairo_matrix_init( &matrix, xx, yx, xy, yy, x0, y0 );
            cairo_matrix_multiply( &resultMatrix, &resultMatrix, &matrix );
          }
        }

        token = strtok( 0, "()" );
      }

      delete[] transformCopy;
    }

    if (apply)
      _output->addOperation( OP_APPLY_MATRIX, TypedValueList() << TypedValue( (float)resultMatrix.xx ) <<
                                                                  TypedValue( (float)resultMatrix.xy ) <<
                                                                  TypedValue( (float)resultMatrix.x0 ) <<
                                                                  TypedValue( (float)resultMatrix.yx ) <<
                                                                  TypedValue( (float)resultMatrix.yy ) <<
                                                                  TypedValue( (float)resultMatrix.y0 ) );

    return resultMatrix;
  }

  virtual bool VisitEnter( const TiXmlElement& element, const TiXmlAttribute* firstAttribute )
  { 
    const char *value = element.Value();
    if( strcmp( element.Value(), "svg" ) == 0 )
    {
      double width = 256, height = 256;
      element.Attribute( "width", &width );
      element.Attribute( "height", &height );
      _output->addOperation( OP_SIZE, TypedValueList() << TypedValue( (float)width ) << TypedValue( (float)height ) );
    } else if( strcmp( element.Value(), "g" ) == 0 )
    {
      _output->addOperation( OP_PUSH_STATE );
      executeTransform( element.Attribute( "transform" ) );
      executeStyle( element.Attribute("style"), true );
    } else if (strcmp(element.Value(), "rect") == 0)
    {
      double x = 0, y = 0, w = 0, h = 0;
      element.Attribute("x", &x);
      element.Attribute("y", &y);
      element.Attribute("width", &w);
      element.Attribute("height", &h);
      
      _output->addOperation( OP_PUSH_STATE );
      executeTransform(element.Attribute("transform"));
      _output->addOperation( OP_RECTANGLE, TypedValueList() << TypedValue( (float)x ) << TypedValue( (float)y ) << TypedValue( (float)w ) << TypedValue( (float)h ) );
      executeStyle(element.Attribute("style"));
      _output->addOperation( OP_BEGIN_NEW_PATH );
      _output->addOperation( OP_POP_STATE );
      _stackPointer--;
    } else if (strcmp(element.Value(), "path") == 0)
    {
      const char *pathDefinition = element.Attribute("d");
      if (pathDefinition != 0)
      {
        _output->addOperation( OP_PUSH_STATE );
        executeTransform(element.Attribute("transform"));

        size_t pathLength = strlen(pathDefinition)+1;
        char *tokenCopy = new char[pathLength];
        memcpy(tokenCopy, pathDefinition, pathLength);
        const char *token = strtok(tokenCopy, " ,");
        bool grabNext = false;
        while (token != 0)
        {
          if (grabNext)
            token = strtok(0, " ,");
          if (token == 0)
            break;
          grabNext = true;
          if (token[0] == 'Z' || token[0] == 'z')
          {
            _output->addOperation( OP_CLOSE_PATH );
          } else if (token[0] == 'C' || token[0] == 'c')
          {
            Operations operation = token[0] == 'C' ? OP_CURVE_TO : OP_CURVE_TO_RELATIVE;

            float v[6];
            while(true)
            {
              int i;
              for (i=0; i<6; ++i)
              {
                token = strtok(0, " ,");
                if (token == 0 || sscanf(token, "%f", &v[i]) != 1) break;
              }
              if (i == 6)
              {
                _output->addOperation( operation, TypedValueList() << TypedValue( (float)v[0] ) << TypedValue( (float)v[1] ) << TypedValue( (float)v[2] ) << TypedValue( (float)v[3] ) << TypedValue( (float)v[4] ) << TypedValue( (float)v[5] ) );
              } else break;
            }

            grabNext = false; 
          } else if (token[0] == 'M' || token[0] == 'm')
          {
            Operations operation = token[0] == 'M' ? OP_MOVE_TO : OP_MOVE_TO_RELATIVE;

            float v[2];
            int i;
            for (i=0; i<2; ++i)
            {
              token = strtok(0, " ,");
              if (token == 0 || sscanf(token, "%f", &v[i]) != 1) break;
            }
            if (i == 2)
            {
              _output->addOperation( operation, TypedValueList() << TypedValue( (float)v[0] ) << TypedValue( (float)v[1] ) );
            } else grabNext = false;
          } else if (token[0] == 'L' || token[0] == 'l')
          {
            Operations operation = token[0] == 'L' ? OP_LINE_TO : OP_LINE_TO_RELATIVE;

            float v[2];
            int i;
            for (i=0; i<2; ++i)
            {
              token = strtok(0, " ,");
              if (token == 0 || sscanf(token, "%f", &v[i]) != 1) break;
            }
            if (i == 2)
            {
              _output->addOperation( operation, TypedValueList() << TypedValue( (float)v[0] ) << TypedValue( (float)v[1] ) );
            } else grabNext = false;
          }
        }

        executeStyle(element.Attribute("style"));
        _output->addOperation( OP_BEGIN_NEW_PATH );
        _output->addOperation( OP_POP_STATE );
        _stackPointer--;

        delete[] tokenCopy;
      }
    } else if (strcmp(element.Value(), "linearGradient") == 0)
    {
      double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
      element.Attribute("x1", &x1);
      element.Attribute("y1", &y1);
      element.Attribute("x2", &x2);
      element.Attribute("y2", &y2);

      _gradients[_gradientCount].id = element.Attribute( "id" );
      if( _gradients[_gradientCount].id != 0 )
      {
        _output->addOperation( OP_CREATE_LINEAR_GRADIENT, TypedValueList() << TypedValue( _gradientCount ) << TypedValue( (float)x1 ) << TypedValue( (float)y1 ) << TypedValue( (float)x2 ) << TypedValue( (float)y2 ) );

        _gradients[_gradientCount].matrix = executeTransform(element.Attribute("gradientTransform"), false);
        cairo_matrix_invert(&_gradients[_gradientCount].matrix);
        
        StopParser stopParser( _output );
        element.Accept( &stopParser ); 

        const char *xlink = element.Attribute("xlink:href");
        if (xlink != 0)
        {
          for( int i=0; i<_elementCount; ++i )
          {
            if( strcmp( _elements[i].id, xlink+1 ) == 0 )
            {
              _elements[i].element->Accept( &stopParser );
              break;
            }
          }
        }
        
        _gradientCount++;
      }

      return false;
    } else if (strcmp(element.Value(), "radialGradient") == 0)
    {
      double cx = 0, cy = 0, fx = 0, fy = 0, r;
      element.Attribute("cx", &cx);
      element.Attribute("cy", &cy);
      element.Attribute("fx", &fx);
      element.Attribute("fy", &fy);
      element.Attribute("r", &r);

      _gradients[_gradientCount].id = element.Attribute("id");
      if( _gradients[_gradientCount].id != 0 )
      {
        _output->addOperation( OP_CREATE_RADIAL_GRADIENT, TypedValueList() << TypedValue( _gradientCount ) << 
                                                                              TypedValue( (float)fx ) << 
                                                                              TypedValue( (float)fy ) << 
                                                                              TypedValue( 0.0f ) << 
                                                                              TypedValue( (float)cx ) << 
                                                                              TypedValue( (float)cy ) << 
                                                                              TypedValue( (float)r ) );

        _gradients[_gradientCount].matrix = executeTransform(element.Attribute("gradientTransform"), false);
        cairo_matrix_invert( &_gradients[_gradientCount].matrix );

        StopParser stopParser( _output );
        element.Accept( &stopParser );

        const char *xlink = element.Attribute("xlink:href");
        if (xlink != 0)
        {
          for (int i=0; i<_elementCount; ++i)
          {
            if (strcmp(_elements[i].id, xlink+1) == 0)
            {
              _elements[i].element->Accept(&stopParser);
              break;
            }
          }
        }
        
        _gradientCount++;
      }

      return false;
    }

    return true; 
  }
	  
  virtual bool VisitExit(const TiXmlElement& element)		
  { 
    const char *value = element.Value();
    if (strcmp(element.Value(), "g") == 0)
    {
      _output->addOperation( OP_POP_STATE );
      _stackPointer--;
    }
    return true; 
  }
};


bool compileSVG( const TiXmlDocument* svgDocument, Program* program )
{
  ElementIdPair elements[1000];
  int elementCount = 0;

  program->addOperation( OP_PUSH_STATE );
  ElementIndexer indexer( elements, elementCount );
  svgDocument->Accept(&indexer);
  
  SVGConverter converter( program, elements, elementCount );
  svgDocument->Accept(&converter);
  program->addOperation( OP_POP_STATE );

  return true;
}

}

}