//	The Ultimate BMP Extractor v. 1.0
//	(fxgen API test)
//	Copyright (C) 2007 Sebastian Olter (qduaty@gmail.com)
//	This file is provided for free in the terms of GNU General Public License (GPL) v.2
//	Link it with fxgenlib

#include <math.h>
#include <string>

//#ifdef __GNUC__ // in case gcc is used
//#define sqrtf sqrt
//#include "gcccompat/gcccompat.h"
//#endif

float DETAIL = 8.0;

using namespace std;

#include "../../Src/FxGenLib/EngineOp.h"
#include "../../Src/FxGenLib/MainOps.h"

// application stuff

struct BMP // Windows Bitmap header
{
	BMP(int w, int h)
			: signature(0x4D42), reserved(0), offset(sizeof(BMP)),
			size_of_BITMAPINFOHEADER_structure(40),	width(w), height(h),
			number_of_planes(1), bpp(24), compression_type(0),
			horizontal_resolution(4000), vertical_resolution(4000),
			number_of_colors(0), number_of_important_colors(0)
	{
		size_of_image_data = width * height * bpp / 8;
		filesize = (int)(sizeof(BMP) + size_of_image_data);
	}

	void write(string filename, const RGBA* data)
	{
		printf("%d",(int)sizeof(RGBA));
		FILE* f = fopen(filename.c_str(), "wb");
		/* put header */
		fwrite((char*)this + 2, sizeof(BMP), 1, f);

		/* put data */
		for (int i = 0; i < width * height; i++)
		{
			// flip vertically
			int linein = i / width; // round down
			int lineout = height - linein - 1;
			int j = lineout * width + (i % width);

			fputc(data[j].b, f);
			fputc(data[j].g, f);
			fputc(data[j].r, f);
		}

		fclose(f);
	}

int padding: 16; // we pad here, instead gcc would do it for us :P

// rest of layout is from http://www.fastgraph.com/help/bmp_header_format.html
int signature: 16;
int filesize: 32;
int reserved: 32;
int offset:	32; // to start of image data in bytes
int size_of_BITMAPINFOHEADER_structure:	32;
int width: 32;
int height:	32;
int number_of_planes: 16;
int bpp: 16; //(1, 4, 8, or 24)
int compression_type: 32; // (0=none, 1=RLE-8, 2=RLE-4)
int size_of_image_data:	32; //in bytes (including padding)
int horizontal_resolution: 32; //in pixels per meter (unreliable)
int vertical_resolution: 32;
int number_of_colors: 32; // or zero
int number_of_important_colors:	32; //, or zero

};

void showProgress(float howmuch)
{
	printf("\r");

	for (int i = 0; i < 79 * howmuch; i++)
		printf("#");
}

float progress = 0.0f, frac = 1.0f;

//void GlobalProgress(udword current, udword total)
//{
//	frac = 1. / total;
//}

void LocalProgress(udword current, udword total)
{
	progress += frac / total;
	showProgress(progress);
}


// FxGen use case

int main(int argc, char *argv[])
{
	printf("The Ultimate BMP Extractor v. 1.0\n");

	if(argc == 1)
	{
		printf("Usage:\n%s <file.prj> [quality]\n", argv[0]);
		return 1;
	}

	if(argc == 3)
	{
		DETAIL = atof(argv[2]);
	}

	NEngineOp* peng = NEngineOp::GetEngine();

	if (peng->LoadProject(argv[1]))
	{
		//Get All Finals result Operators List
		NObjectArray carrayOps;
		peng->GetFinalsResultsList(carrayOps, NULL, true);

		frac = 1.0f / (float)carrayOps.Count();

		//Process Operators...
		for (udword i=0; i<carrayOps.Count(); i++)
		{
			NStoreResultOp* pFinalResultOp = (NStoreResultOp*)carrayOps[i];
			peng->ProcessFinalResult(pFinalResultOp, 0.0f, DETAIL, LocalProgress);
		}

		//Free Memory
		// Keep just final result bitmaps in memory
		peng->CompactMemory(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED);

		//Now use generated Textures :-)
		for (udword i=0; i<carrayOps.Count(); i++)
		{
			NStoreResultOp* pFinalResultOp = (NStoreResultOp*)carrayOps[i];

			const char* pszName = pFinalResultOp->GetUserName();
			NBitmap* pbmp				= pFinalResultOp->GetBitmap();

			printf("\nWriting result: %s", pszName);
			BMP bmp(pbmp->GetWidth(), pbmp->GetHeight());
			bmp.write(string(pszName) + ".bmp", pbmp->GetPixels());
		}

		/*peng->ProcessOperators(0.0f, 1.f, GlobalProgress, LocalProgress);
		peng->CompactMemory();	//!< Keep just final result bitmaps in memory

		for (int i = 0; i < peng->GetFinalResultCount(); i++)
		{
			NBitmap* pbmp =	peng->GetFinalResultBitmapByIdx(i);
			printf("\nWriting result: %s", pbmp->GetName());

			BMP bmp(pbmp->GetWidth(), pbmp->GetHeight());

			bmp.write(string(pbmp->GetName()) + ".bmp", pbmp->GetPixels());
		}*/
	}
	else
		printf("Project file \"%s\" is broken or unreadable.\n", argv[1]);

	return 0;
}
