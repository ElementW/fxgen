#include <iostream>

using namespace std;

#include "../../Src/FxGenLib/EngineOp.h"
#include "../../Src/FxGenLib/MainOps.h"


int main()
{
  	NEngineOp* peng = NEngineOp::GetEngine();

  cout << NRTClassModule::m_pFirstRTClassModule->m_pszModuleName << endl;
  cout << NRTClassModule::m_pFirstRTClassModule->m_pFirstRTClass->m_pszClassName << endl;

    cout << "Hello world!" << endl;
    return 0;
}
