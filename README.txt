//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//									 FxGen synthesis texture generation
//													version alpha 0.1
//
//	Author Johann Nadalutti (fxgen@free.fr)
//----------------------------------------------------------------- 
//-----------------------------------------------------------------


FxGen is a procedural texture generation library and tool to let you implement high resolution textures that fit in kilobytes in your applications.
The principle of FxGen is same as .werkkzeug by  farbrausch (see http://www.theprodukkt.com/theprodukkt).
The texture generator is separated into small operators with each their set of parameters. This operators can be connected togethers by stacking to produce a final result.
The avantage of "operator stacking" in contrast with "traditional" solution by "cables" is simplicity !

When I see Roboblitz game using ProFx2 solution, the interest of procedural is not to prove ! (see www.profxengine.com)

FxGen is completely free, released under the LGPL for libraries and GPL for editor license.
I only ask for your recognition, nothing else.
Try the library and tool, and let me know your impression.


//-----------------------------------------------------------------
//	FxGenLib Description
//----------------------------------------------------------------- 

 	This is the operators engine which managed operators database and compute operators results.
 	For user application, this is the only library useful in order to re-generate texture result from editor file.
	
 	
//-----------------------------------------------------------------
//	GUILib Description
//----------------------------------------------------------------- 
 	
 	This is a Win32 wrapper for moment, for cross-platform an OpenGL and SDL version is scheduled.
 	

//-----------------------------------------------------------------
//	Editor
//----------------------------------------------------------------- 
 
 	Win32 version only but a cross-plaform is scheduled (see GUILib) 
 
	Keyboard shortcuts:
		S 		-> allow to see final result from a selected operator.
		C 		-> copy selected operators to clipboard.
		V 		-> paste copied operators under cursor.
		Supr 	-> Delete selected operators.


//-----------------------------------------------------------------
// 	How to compute operators result from your application
//----------------------------------------------------------------- 

#include "Core.h"
#include "EngineOp.h"

#include "MainOps.h"
#include "BitmapOps.h"

NStoreResultOp op;
NNopOp op2;

void main()
{

		//Create engine instance
		NEngineOp* pgen = gNFxGen_GetEngine();

		//Load a project
		pgen->LoadProject("Datas\\Example01.prj");

		//Get finals operators type from opened project
		NObjectArray finalsOp;
		finalsOp.SetManageDelete(false);
		pgen->GetFinalOps(pgen->GetRootGroup(), finalsOp, true);

		//Execute processing at time 0
		for (udword i=0; i<finalsOp.Count(); i++)
		{
			pgen->Execute(0.0f, (NOperator*)finalsOp[i]);
		}

		//Get Bitmap Result
		NOperator* pop = (NOperator*)finalsOp[0];
		NBitmap* pbmp = (NBitmap*)pop->m_pObj;
		
} 