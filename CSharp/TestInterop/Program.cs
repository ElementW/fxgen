using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SharpFxGenLib;

namespace TestInterop
{
  class Program
  {
    static void Main(string[] args)
    {
      EngineOp eng = new EngineOp();
      uint count = eng.GetOpsDescCount();
      OperatorDescFx desc = eng.GetOpsDesc(0);

    }
  }
}
