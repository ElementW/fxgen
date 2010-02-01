//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Main.h
//! \brief	Main entry for application
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		31-08-2008
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include <stdio.h>

#include "EditorApp.h"

/*//Structure partage avec le moteur d'operateurs
struct SRotoParam
{
  ubyte wpow;
  ubyte hpow;
  vec2  v2Center;
  float fRotate;
  vec2  v2Zoom;
  ubyte byWrap;
};

//En prevision d'un language scriptable...
class NRotoZoom_OperatorUI : NObject
{
  public:
  //Called when user modify a value
  // now properties editing are fully dynamic !

  //A serialiser (nom, type)
  void OnUIProperties_Init()
  {
    VAR("fRotate",  eFloat, sparams.fRotate, "0.0", -180.0, 180.0, 0.01); //pszDefValue, min ,max, step
    VAR("v2Center", eVec2f, sparams.v2Center, "0.0,0.0", -1.0, 1.0, 0.01);
    //La valeur par defaut est-elle maintenant necessaire => peut etre a deplacer dans le constructeur ?
  }

  void OnUIProperties_Show(/*layout as Layout, NVarBlock* pblock  )
  {
    //float frot = pblock->Get("fRotate");
    //l = layout.Split("Parametres")
    //   l.Add(pblock, "fRotate", "mon angle")  //Varname, UI_text
    //   l.Add(pblock, "v2Center", "mon center")
    //if (bWrap)
    //  Add..

    //Etend la possibilitee
    //  - d'afficher des parametres differents en fonction des modes ...
    //  - de cache/afficher des parametres en fonction des coches
  }

  //Gestion de l'edition
  //...

  //Datas
  SRotoParam sparams;

};
*/

struct SRotoParam
{
  ubyte wpow;
  ubyte hpow;
  vec2  v2Center;
  float fRotate;
  vec2  v2Zoom;
  ubyte byWrap;
};

class NTestReflexion : public NObject
{
public:
	FDECLARE_FIELDS();

	NTestReflexion()	{dwSeed=1234; fAngle = 3.14f; }

	SRotoParam st;

	float fAngle;
	udword dwSeed;
};

FBEGIN_FIELDS_SUBCLASS(NTestReflexion, NObject)
	NEWFIELD(eFloat, "angle", NTestReflexion, fAngle, NULL),
	NEWFIELD(eInteger, "seed", NTestReflexion, dwSeed, NULL),
//Struct
	NEWFIELD(eInteger, "wpow", NTestReflexion, st.wpow, NULL),
FEND_FIELDS()

NDebugMemoryMgr gMemMgr;

//-----------------------------------------------------------------
//!\func	Export WinMain to force linkage to this module
//-----------------------------------------------------------------
int main(int argc, char *argv[])
{
  //###TEST### Debut
	NTestReflexion class1;
	FIELDS_LIST* fields = class1.GetFields();

	FIELD_DESC*	fdesc = fields->lpFields;
	float fAngle = * ((float*) (((ubyte*)&class1) + fdesc->DataOffset));

	fdesc = fields->lpFields+1;
	udword dwSeed = * ((udword*) (((ubyte*)&class1) + fdesc->DataOffset));

	//Serialization
	NMemoryStream *stream = new NMemoryStream();

	NArchive ar(stream);
	ar.PutClass(&class1);


	//###TEST### Fin

	// Application
	NFxGenApp* pApp = new NFxGenApp();

	// Perform specific initializations
	if (!pApp->Init())		pApp->Exit();

	// Run Application
	pApp->Run();

	// Exit Application
	pApp->Exit();

	delete pApp;

	return EXIT_SUCCESS;
}
