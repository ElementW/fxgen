//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		VectorPropertyItems.cpp
//! \brief	Base class for the VectorData Property Item
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	        Anders Stenberg (anders.stenberg@gmail.com)
//!	\date		31-05-2007
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//			Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "VectorPropertyItems.h"
#include "Editor.h"

#include "tinyxml.h"
#include "vgvm/vgvm.h"
#include "vgvm/compilers/svg.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NVectorDataProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVectorDataProp, NPropertyItem);

void NVectorDataProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	//pdc->DrawText(m_pvarValue->szVal, rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	pdc->Draw3dRect(rcItem, RGB(255,255,255), RGB(0,0,0));
	pdc->DrawText("Import", rcItem, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

static void* vgvmAlloc( size_t size )
{
  return NMemAlloc( size );
}

static void vgvmFree( void* memory )
{
  return NMemFree( memory );
}

bool NVectorDataProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	NFileDialog dlg;
	dlg.Create("Choose a Vector Data File to Load", m_pParent);
	if (1==dlg.DoModal())
	{
		//char* szval = ((char*)m_pObj + m_dwCPPPropOffset);
		/*NString str = dlg
		strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());*/

		TiXmlDocument svgDocument(dlg.GetPathName().Buffer());
		bool loadOkay = svgDocument.LoadFile();
		if (loadOkay && strcmp(svgDocument.FirstChildElement()->Value(), "svg") != 0)
			return false;

		vgvm::Program* pProgram = new vgvm::Program( vgvmAlloc, vgvmFree );
		vgvm::svg::compileSVG( &svgDocument, pProgram );
		
		((NVectorOp*)m_pvarBloc->GetOwner())->SetVectorData( pProgram->getBuffer(), pProgram->getSize() );

		delete pProgram;
	}


	return true;	//End of Edition
}

bool NVectorDataProp::EndEdit(bool bSaveChanged)
{
	return true;
}
