#ifndef FONT_H
#define FONT_H

#include <GL/gl.h>	/* Header File For The OpenGL Library */

// FreeType Headers
#include "ft2build.h"
#include "freetype\freetype.h"
#include "freetype\ftglyph.h"
#include "freetype\ftoutln.h"
#include "freetype\fttrigon.h"


// This Holds All Of The Information Related To Any FreeType Font That We Want To Create.
class NGUIFont
{
public:
	float m_h; // Holds The Height Of The Font.
	GLuint *textures;	// Holds The Texture Id's
	GLuint list_base;		// Holds The First Display List Id

	// The Init Function Will Create A Font With the Height h From The File fname.
	void init(const char * fname, unsigned int h);
	void print(float x, float y, const char *fmt, ...);
	void make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base );

	// Free All The Resources Associated With The Font.
  void clean();
};

#endif //FONT_H
