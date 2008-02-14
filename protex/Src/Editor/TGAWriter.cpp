//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		TGAWriter.cpp
//! \brief	Main frame
//!
//!	\author	Anders Stenberg (anders.stenberg@gmail.com)
//!         Andrew Caudwell (acaudwell@gmail.com)
//!	\date		02-06-2007
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"

struct TGAHeader 
{
  char idlength;
  char colourmaptype;
  char datatypecode;
  short int colourmaporigin;
  short int colourmaplength;
  char colourmapdepth;
  short int x_origin;
  short int y_origin;
  short width;
  short height;
  char bitsperpixel;
  char imagedescriptor;
};

void WriteTGA(NBitmap* _bmp, NString _path, NString _suffix) 
{
	// now calculate the power of two stuff
	int width = (short)_bmp->GetWidth();
	int height = (short)_bmp->GetHeight();

	int div_width_by_128 = width / 128;
	int div_height_by_128 = height / 128;

	if( div_width_by_128 == div_height_by_128 )
	{


		TGAHeader header;
		header.idlength=0;
		header.colourmaptype=0;
		header.datatypecode=2;
		header.colourmaporigin=0;
		header.colourmaplength=0;
		header.colourmapdepth=0;
		header.x_origin=0;
		header.y_origin=0;
		header.width = (short)_bmp->GetWidth();
		header.height = (short)_bmp->GetHeight();
		header.bitsperpixel= 32;
		header.imagedescriptor=0;

		udword length = _path.Length();
		if (length>=4 && _path.Buffer()[length-4] == '.' && 
					   _path.Buffer()[length-3] == 't' && 
					   _path.Buffer()[length-2] == 'g' && 
					   _path.Buffer()[length-1] == 'a')
		{
		_path.SetLength(length-4);
		}

		if (_suffix.Length() > 0)
		{
		_path += "_";
		_path += _suffix.Buffer();
		}

		_path += ".tga";

		FILE* tgaf = fopen(_path.Buffer(),"wb");
		fwrite(&header.idlength, 1, 1, tgaf);
		fwrite(&header.colourmaptype, 1, 1, tgaf);
		fwrite(&header.datatypecode, 1, 1, tgaf);
		fwrite(&header.colourmaporigin, 2, 1, tgaf);
		fwrite(&header.colourmaplength, 2, 1, tgaf);
		fwrite(&header.colourmapdepth, 1, 1, tgaf);
		fwrite(&header.x_origin, 2, 1, tgaf);
		fwrite(&header.y_origin, 2, 1, tgaf);
		fwrite(&header.width, 2, 1, tgaf);
		fwrite(&header.height, 2, 1, tgaf);
		fwrite(&header.bitsperpixel, 1, 1, tgaf);
		fwrite(&header.imagedescriptor, 1, 1, tgaf);

		RGBA* pixels = _bmp->GetPixels();

		char* offset=0;
		for(int y=header.height-1;y>=0;y--) {
		offset = (char*) pixels + y*header.width*4;
		for(int x=0;x<header.width;x++) {
		  fwrite(offset+2, 1, 1, tgaf);
		  fwrite(offset+1, 1, 1, tgaf);
		  fwrite(offset, 1, 1, tgaf);
		  fwrite(offset+3, 1, 1, tgaf);
		  offset += 4;
		}
		}

		fclose(tgaf);

	}
	else 
	{

		int num_tiles = div_height_by_128 * div_height_by_128;
		//for(int Y = div_height_by_128-1; Y>=0; Y-- )
			for(int Y = 0; Y<div_height_by_128; Y++)
		{
			for(int X = 0; X < div_height_by_128; X++ )
			{
				int offsX = X*128;
				int offsY = Y*128;



				TGAHeader header;
				header.idlength=0;
				header.colourmaptype=0;
				header.datatypecode=2;
				header.colourmaporigin=0;
				header.colourmaplength=0;
				header.colourmapdepth=0;
				header.x_origin=offsX;
				header.y_origin=offsY;
				header.width = (short)128;
				header.height = (short)128;
				header.bitsperpixel= 32;
				header.imagedescriptor=0;

				udword length = _path.Length();
				if (length>=4 && _path.Buffer()[length-4] == '.' && 
							   _path.Buffer()[length-3] == 't' && 
							   _path.Buffer()[length-2] == 'g' && 
							   _path.Buffer()[length-1] == 'a')
				{
				_path.SetLength(length-4);
				}

				if (_suffix.Length() > 0)
				{
				_path += "_";
				_path += _suffix.Buffer();

				}


				NString newPath = _path;
				char numBuff[256];
				sprintf(numBuff, "%d", (X*div_height_by_128 + Y));
				newPath += numBuff;
				newPath += ".tga";

				FILE* tgaf = fopen(newPath.Buffer(),"wb");
				fwrite(&header.idlength, 1, 1, tgaf);
				fwrite(&header.colourmaptype, 1, 1, tgaf);
				fwrite(&header.datatypecode, 1, 1, tgaf);
				fwrite(&header.colourmaporigin, 2, 1, tgaf);
				fwrite(&header.colourmaplength, 2, 1, tgaf);
				fwrite(&header.colourmapdepth, 1, 1, tgaf);
				fwrite(&header.x_origin, 2, 1, tgaf);
				fwrite(&header.y_origin, 2, 1, tgaf);
				fwrite(&header.width, 2, 1, tgaf);
				fwrite(&header.height, 2, 1, tgaf);
				fwrite(&header.bitsperpixel, 1, 1, tgaf);
				fwrite(&header.imagedescriptor, 1, 1, tgaf);

				RGBA* pixels = _bmp->GetPixels();

				char* offset=0;
				for(int y=header.width-1;y>=0;y--) {
				offset = (char*) pixels + y*header.width*4;// +(Y*div_height_by_128 + X)*128*4;// + (Y*width)*4;
				for(int x=0;x<header.width;x++) {
				  fwrite(offset+2, 1, 1, tgaf);
				  fwrite(offset+1, 1, 1, tgaf);
				  fwrite(offset, 1, 1, tgaf);
				  fwrite(offset+3, 1, 1, tgaf);
				  offset += 4;
				}
				}

				fclose(tgaf);
			}
		}

	}

}