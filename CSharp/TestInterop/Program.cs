using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SharpFxGenLib;
using System.Diagnostics;

namespace TestInterop
{
  class Program
  {
    static void Main(string[] args)
    {
      EngineOp eng = new EngineOp();

      //Get Operators List Desc
      uint count = eng.GetOpsDescCount();
      for (uint i = 0; i < count; i++)
      {
        OperatorDescFx desc = eng.GetOpsDesc(i);
        Debug.Print("{0} {1}", desc.strName, desc.strCategorie);
      }

      //


    }
  }
}
