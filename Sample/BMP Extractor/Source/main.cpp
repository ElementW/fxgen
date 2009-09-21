//	The Ultimate BMP Extractor v. 1.0
//	(fxgen API test)
//	Copyright (C) 2007 Sebastian Olter (qduaty@gmail.com)
//										 Johann Nadalutti (fxgen@free.fr)
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

#include "..\..\..\FxGenLib\Include\FxGenLib.h"

#include "..\..\..\FxGenLib\Source\MemoryMgr.h"
#include "..\..\..\FxGenLib\Source\Maths.h"
#include "..\..\..\FxGenLib\Source\Bitmap.h"
#include "..\..\..\FxGenLib\Source\CompilerOp.h"
//#include "..\..\..\FxGenLib\Source\EngineOp.h"
#include "stream.h"

class NEngineOp;

class NDebugMemoryMgr gMemMgr;



//-----------------------------------------------------------------
//!	\struct		SFx2OperatorDesc
//!	\brief		Operators desc for export to fxGen 0.7a (Fx2 Export)
//-----------------------------------------------------------------
struct SFx2OperatorDesc
{
	udword	x, y, w;					//!< Position x and y in the graph and width
	char		szGroup[32];			//!< Group name used for incremental compilation (could be texture name)
	char		szIFnc[32];				//!< Operator fonction interface name
	char		szToLoad[32];			//!< Operator to Load name
	char		szStoredName[32];	//!< Operator store name
	udword	adwParams[32];		//!< Operator parameters
	ubyte		byParamsCount;
};

//-----------------------------------------------------------------
//! \struct BMP
//! \brief	
//-----------------------------------------------------------------
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

	void write(string filename, const NRGBA* data)
	{
		printf("%d",(int)sizeof(NRGBA));
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


void CompilerTest();
void CompilerTest2();
extern udword ImportFxGen2(const char* _pszfilename, SFx2OperatorDesc** _oapopsDesc);



// FxGen use case

int main(int argc, char *argv[])
{
	/*printf("The Ultimate BMP Extractor v. 1.0\n");

	if(argc == 1)
	{
		printf("Usage:\n%s <file.prj> [quality]\n", argv[0]);
		return 1;
	}

	if(argc == 3)
	{
		DETAIL = atof(argv[2]);
	}*/

//	CompilerTest();
	CompilerTest2();

		//Free Memory
		// Keep just final result bitmaps in memory
/*		peng->CompactMemory(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED); //TOFIX

		NBitmap* pbmp = (NBitmap*)popsSequence->popEnd->m_pObj;

		printf("\nWriting result: %s", stComp.pfirstOpsSeq->szName);
			BMP bmp(pbmp->GetWidth(), pbmp->GetHeight());
			bmp.write(string(stComp.pfirstOpsSeq->szName) + ".bmp", pbmp->GetPixels());
		//}
*/
/*	}
	else
	{
		printf("Project file \"%s\" is broken or unreadable.\n", argv[1]);
	}*/

	return 0;
}


void CompilerTest()
{
	udword i;
	NEngineOp* peng = fgCreateOpEngine();

	////////////////////////////////////////////////////
	//Functions Interfaces registered
	udword dwOpsCount = peng->GetOpsInterfacesCount();
	for (i=0; i<dwOpsCount; i++)
	{
		SOpFuncInterface* pfi = peng->GetOpsInterfaceFromIdx(i);
		printf("Op Interface : %s P<%d>\n", pfi->szName, pfi->dwParamsSize);
	}

	////////////////////////////////////////////////////
	//Make a graph at hand :-(
	SOperatorDesc uiops[16];
	NMemFill(uiops, 0, sizeof(uiops));

//--Seq01
	uiops[0].pszGroup	= "Page01";
	uiops[0].x				= 1;
	uiops[0].y				= 1;
	uiops[0].w				= 2;
	uiops[0].pszIFnc	= "Flat";
	udword* ppar = uiops[0].adwParams;
	ppar[0] = 2;//_wpow
	ppar[1] = 7;//_hpow
	ppar[2]	= 0xFF001100;//_color

	uiops[1].pszGroup	= "Page01";
	uiops[1].x				= 1;
	uiops[1].y				= 2;
	uiops[1].w				= 2;
	uiops[1].pszIFnc	= "Color";

	uiops[2].pszGroup	= "Page01";
	uiops[2].x				= 1;
	uiops[2].y				= 3;
	uiops[2].w				= 2;
	uiops[2].pszIFnc	= "Store";
	uiops[2].pszStoredName = ".Seq01";

//--Seq02
	uiops[3].pszGroup	= "Page01";
	uiops[3].x				= 10;
	uiops[3].y				= 1;
	uiops[3].w				= 2;
	uiops[3].pszIFnc	= "Load";
	uiops[3].pszToLoad= ".Seq01";

	uiops[4].pszGroup	= "Page01";
	uiops[4].x				= 10;
	uiops[4].y				= 2;
	uiops[4].w				= 2;
	uiops[4].pszIFnc	= "Rect";

	uiops[5].pszGroup	= "Page01";
	uiops[5].x				= 10;
	uiops[5].y				= 3;
	uiops[5].w				= 2;
	uiops[5].pszIFnc	= "Store";
	uiops[5].pszStoredName = ".Seq02";

	////////////////////////////////////////////////////
	//Compile graph
	SCompiledOp stComp;
	NCompilerOp	compiler;
	compiler.Compile(&stComp, uiops, 6, true);

	////////////////////////////////////////////////////

	//Compiling sequences from operators graph
	printf("Total OpsCount<%d>\n\n", stComp.dwTotalOpsCount);

	SOpsSequence *popsSequence = stComp.pfirstOpsSeq;
	while (popsSequence)
	{
		printf("Sequence <%s> OpsCount<%d>\n", popsSequence->szName, popsSequence->dwOpsCount);
		//printf("\tFirst Ope <%s:%s>\n", popsSequence->popStart->GetName(), popsSequence->popStart->GetUserName());
		//printf("\tLast Ope <%s:%s>\n", popsSequence->popEnd->GetName(), popsSequence->popEnd->GetUserName());

		printf("\tToLoadSeq Count<%d>\n", popsSequence->dwSeqsToLoadCount);
		for (udword i=0; i<popsSequence->dwSeqsToLoadCount; i++)
		{
			printf("\t\tSeqtoLoad<%s>\n", popsSequence->apSeqsToLoad[i]->szName);
		}

		popsSequence = popsSequence->pnextOpsSeq;
	};

	//Sequences are compiled, so now process
	popsSequence = stComp.pfirstOpsSeq;
	peng->ProcessSequence(popsSequence, 0.0f, 1.0f);

}


udword ImportFxGen2(const char* _pszfilename, SFx2OperatorDesc** _oapopsDesc)
{
	//Open FxGen2 File
	NFileStream fs;
	if (!fs.Open(_pszfilename))
		return 0;
	
	//Read OperatorsDesc Count
	udword dwCount;
	fs>>dwCount;

	*_oapopsDesc = (SFx2OperatorDesc*)malloc(dwCount*sizeof(SFx2OperatorDesc));

	//Read OperatorsDesc
	for (udword i=0; i<dwCount; i++)
	{
		fs.GetData(&(*_oapopsDesc)[i], sizeof(SFx2OperatorDesc));
	}

	//Close
	fs.Close();

	return dwCount;
}


void CompilerTest2()
{
	SFx2OperatorDesc* paopDesc = null;
	//udword dwCount = ImportFxGen2("C:\\Datas\\Projects\\FXGen_v0_6a\\FXGen_v0_6a\\Datas\\test", &paopDesc);
	udword dwCount = ImportFxGen2("C:\\Cle\\Projects\\FXGen_v0_6a\\FXGen_v0_6a\\Datas\\test", &paopDesc);

	if (dwCount)
	{
		//Make OperatorDesc for compiler
		SOperatorDesc* aops = (SOperatorDesc*)malloc(dwCount * sizeof(SOperatorDesc));
		for (udword i=0; i<dwCount; i++)
		{
			aops[i].x = paopDesc[i].x;
			aops[i].y = paopDesc[i].y;
			aops[i].w = paopDesc[i].w;
			aops[i].pszGroup			= paopDesc[i].szGroup;
			aops[i].pszIFnc				= paopDesc[i].szIFnc;
			aops[i].pszToLoad			= paopDesc[i].szToLoad;
			aops[i].pszStoredName = paopDesc[i].szStoredName;
			aops[i].byParamsCount	= paopDesc[i].byParamsCount;
			memcpy(aops[i].adwParams, paopDesc[i].adwParams, sizeof(aops[i].adwParams));
		}

		//Compile
		SCompiledOp stComp;
		NCompilerOp	compiler;
		bool bCompiled = compiler.Compile(&stComp,	aops, dwCount, true);

		free(aops);

		//Compiling sequences from operators graph
		printf("Total OpsCount<%d>\n\n", stComp.dwTotalOpsCount);

		SOpsSequence *popsSequence = stComp.pfirstOpsSeq;
		while (popsSequence)
		{
			printf("Sequence <%s> OpsCount<%d>\n", popsSequence->szName, popsSequence->dwOpsCount);
			//printf("\tFirst Ope <%s:%s>\n", popsSequence->popStart->GetName(), popsSequence->popStart->GetUserName());
			//printf("\tLast Ope <%s:%s>\n", popsSequence->popEnd->GetName(), popsSequence->popEnd->GetUserName());

			printf("\tToLoadSeq Count<%d>\n", popsSequence->dwSeqsToLoadCount);
			for (udword i=0; i<popsSequence->dwSeqsToLoadCount; i++)
			{
				printf("\t\tSeqtoLoad<%s>\n", popsSequence->apSeqsToLoad[i]->szName);
			}

			popsSequence = popsSequence->pnextOpsSeq;
		};

		//Sequences are compiled, so now process
		NEngineOp* peng = fgCreateOpEngine();
		popsSequence = stComp.pfirstOpsSeq;
		popsSequence = popsSequence->pnextOpsSeq;	//Grid1
		popsSequence = popsSequence->pnextOpsSeq;	//Grid2
		popsSequence = popsSequence->pnextOpsSeq;	//Grid3
		popsSequence = popsSequence->pnextOpsSeq; //Metal
		SRessource* pres = peng->ProcessSequence(popsSequence, 0.0f, 1.0f);

		//Save Generated sequence
		BMP bmp(pres->dwWidth, pres->dwHeight);
		bmp.write(string(popsSequence->szName) + ".bmp", pres->pbyPixels);
	}

	//Free
	if (paopDesc)
		free(paopDesc);
}


