#include <iostream>

using namespace std;

#include "../../Src/FxGenLib/EngineOp.h"

int main()
{
  //Get FxGenEngine
  //NEngineOp* peng = NEngineOp::GetEngine();

  //Load a new module (or plugins)
  HMODULE hmod = LoadLibrary("libVector_D.dll");

  if (NEngineOp::GetEngine()->LoadProject("D:\\Temp\\Cle\\FxGen\\Datas\\Bricks.prj"))
    cout << "Project loaded" << endl;

  //Display RTClass by Modules
  NRTClassModule* pmod = NRTClassModule::m_pFirstRTClassModule;
  while (pmod)
  {
    NRTClass* prtClass = pmod->m_pFirstRTClass;
    while (prtClass)
    {
      cout << pmod->m_pszModuleName << ":" << prtClass->m_pszClassName << endl;
      prtClass = prtClass->m_pNextRTC;
    }

    pmod = pmod->m_pNextRTClassModule;
  }

  //Free Module
  FreeModule(hmod);

  return 0;
}
