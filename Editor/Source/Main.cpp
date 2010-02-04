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
	FDECLARE_CLASS();

	NTestReflexion()	{dwSeed=1234; fAngle = 3.14f; }

	SRotoParam st;

	float fAngle;
	udword dwSeed;
};

FIMPLEMENT_CLASS(NTestReflexion, NObject)
	NEWFIELD(eFloat, "angle", NTestReflexion, fAngle, -3.14f, 1.14f, 0.01f, ""),
	NEWFIELD(eInteger, "seed", NTestReflexion, dwSeed, 0, 65536, 1, ""),
//Struct
	NEWFIELD(eInteger, "wpow", NTestReflexion, st.wpow, 0, 65536, 1, ""),
FIMPLEMENT_CLASS_END()


NDebugMemoryMgr gMemMgr;

//-----------------------------------------------------------------
//!\func	Export WinMain to force linkage to this module
//-----------------------------------------------------------------
int main(int argc, char *argv[])
{
  //###TEST### Debut
	NTestReflexion class1;

	NFieldDesc* pdesc = class1.m_RTClass.m_paFieldsDesc;

	/*NFieldDesc*	fdesc = fields->m_paFieldsDesc;
	float fAngle = * ((float*) (((ubyte*)&class1) + fdesc->DataOffset));

	fdesc = fields->m_paFieldsDesc+1;
	udword dwSeed = * ((udword*) (((ubyte*)&class1) + fdesc->DataOffset));

	//Serialization
	NMemoryStream *stream = new NMemoryStream();

	NArchive ar(stream);
	ar.PutClass(&class1);
*/

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
