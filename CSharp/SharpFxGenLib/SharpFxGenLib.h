// SharpFxGenLib.h

#pragma once

using namespace System;


#include "Source/OpEngine/EngineOp.h"
#include "Source/OpEngine/Bitmap.h"
#include "Source/OpEngine/RectangularArray.h"
#include "Source/OpEngine/Controllers.h"


namespace SharpFxGenLib {


//-----------------------------------------------------------------
//!	\struct		OperatorDescFx
//!	\brief		Operator description
//-----------------------------------------------------------------
	public ref struct OperatorDescFx
	{
		String^ strRTClassName;
		String^ strName;
		String^ strCategorie;
		udword	dwColor;
	};

//-----------------------------------------------------------------
//!	\class		CompiledAsset
//!	\brief		Asset processed by engine
//-----------------------------------------------------------------
	public ref class CompiledAsset
	{
		/*udword GetOutputsCount()		{ return m_arrayOutputOps.Count(); }
		NOperatorFx* GetOutput(udword _idx);

		void Clear();	//!< Clear this assets
		void LinkFx(NOperatorFx* _op, NOperatorFx* _opRoot, NOperatorFx* _opPrev);*/

	};

//-----------------------------------------------------------------
//!	\class		EngineOp
//!	\brief		Engine operators process
//-----------------------------------------------------------------
	public ref class EngineOp
	{
		public: EngineOp::EngineOp()
		{
			m_native = new NEngineOp();
		}

		EngineOp::~EngineOp()
		{
			delete m_native;
		}

		public: udword GetOpsDescCount()									{ return m_native->GetOpsDescCount(); }

		public: OperatorDescFx^ GetOpsDesc(udword _idx)
		{
			NOperatorDescFx* pd = m_native->GetOpsDesc(_idx);

			OperatorDescFx^ od = gcnew OperatorDescFx();
			od->strRTClassName	= gcnew String(pd->pszRTClassName);
			od->strName					= gcnew String(pd->szName);
			od->strCategorie		= gcnew String(pd->szCategorie);
			od->dwColor					= pd->dwColor;
			return od;
		}

		//Datas
		protected: NEngineOp* m_native;

	};

}
