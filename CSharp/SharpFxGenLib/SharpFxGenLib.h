// SharpFxGenLib.h

#pragma once

using namespace System;


#include "Source/OpEngine/EngineOp.h"
#include "Source/OpEngine/Bitmap.h"
#include "Source/OpEngine/RectangularArray.h"
#include "Source/OpEngine/Controllers.h"


namespace SharpFxGenLib {


	public ref struct OperatorDescFx
	{
		String^ strRTClassName;
		String^ szName;
		String^ szCategorie;
		udword	dwColor;
	};


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
		od->strRTClassName = gcnew String(pd->pszRTClassName);
		return od;
	}

		//Datas
	protected: NEngineOp* m_native;

	};
}
