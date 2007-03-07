//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PropertyItems.cpp
//! \brief	Base classes for Properties Items
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
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
#include "PropertyItems.h"
#include "Editor.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NPropertyItem base class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NPropertyItem::NPropertyItem()
{
	m_pwNGraphicstrl= null;
	m_pvarBloc			= null;
	m_pvarBlocDesc	= null;
	m_pvarValue			= null;
	m_pParent				= null;
	m_dwvarIdx			= 0;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NPropertyItem::~NPropertyItem()
{
	if (m_pwNGraphicstrl)		delete m_pwNGraphicstrl;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUbyteProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NUbyteProp, NPropertyItem);

void NUbyteProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	m_strValue.Format("%d", m_pvarValue->byVal);
	pdc->DrawText(m_strValue.Buffer(), rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

bool NUbyteProp::BeginEdit(NRect& rcItem)
{
 return false;
}

bool NUbyteProp::EndEdit(bool bSaveChanged)
{
	return false;
}

bool NUbyteProp::AddValue(sdword dwDelta)
{
	sdword dwVal = (sdword)m_pvarValue->byVal + (sdword)dwDelta;
	if (dwVal>255)		dwVal = 255;
	if (dwVal<0)			dwVal = 0;
	m_pvarValue->byVal = (ubyte)dwVal;
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUwordProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NUwordProp, NPropertyItem);

void NUwordProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	m_strValue.Format("%d", m_pvarValue->wVal);
	pdc->DrawText(m_strValue.Buffer(), rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

bool NUwordProp::BeginEdit(NRect& rcItem)
{
	return false;
}

bool NUwordProp::EndEdit(bool bSaveChanged)
{
	return false;
}

bool NUwordProp::AddValue(sdword dwDelta)
{
	sdword dwVal = (sdword)m_pvarValue->wVal + (sdword)dwDelta;
	if (dwVal>65535)	dwVal = 65535;
	if (dwVal<0)			dwVal = 0;
	m_pvarValue->wVal = (uword)dwVal;
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NFloatProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NFloatProp, NPropertyItem);

void NFloatProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	m_strValue.Format("%f", m_pvarValue->fVal);
	pdc->DrawText(m_strValue.Buffer(), rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

bool NFloatProp::BeginEdit(NRect& rcItem)
{
	return false;
}

bool NFloatProp::EndEdit(bool bSaveChanged)
{
	return false;
}

bool NFloatProp::AddValue(sdword dwDelta)
{
	m_pvarValue->fVal = m_pvarValue->fVal + ((float)dwDelta)/10.0f;
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NColorProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NColorProp, NPropertyItem);

void NColorProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	RGBA val;
	val.dwCol = m_pvarValue->dwVal;

	//Affichage des valeurs RGB
	NString cstr;
	NRect rc(rcItem);
	udword w = rc.Width()/4;

	//Red
	rc.right = rc.left + w;
	cstr.Format("%d", val.r);
//	pdc->Rectangle(rc);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//Green
	rc.Move(w,0);
	cstr.Format("%d",  val.g);
//	pdc->Rectangle(rc);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//Blue
	rc.Move(w,0);
	cstr.Format("%d",  val.b);
//	pdc->Rectangle(rc);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//Affichage du rectangle de couleur
	COLORREF col = RGB( val.r,  val.g,  val.b);
	rc.left = rc.right;
	rc.right = rcItem.right;
	pdc->FillSolidRect(rc, col);
}


bool NColorProp::BeginEdit(NRect& rcItem)
{
 return false;
}

bool NColorProp::EndEdit(bool bSaveChanged)
{
	return false;
}

bool NColorProp::AddValue(sdword dwDelta)
{
	if (m_dwRGBEditingIdx<3)
	{
		RGBA* pval = (RGBA*) &m_pvarValue->dwVal;

		//Propiation
		sdword dwVal = (sdword)pval->col_array[m_dwRGBEditingIdx]+(sdword)dwDelta;
		if (dwVal>255)		dwVal = 255;
		if (dwVal<0)			dwVal = 0;
		pval->col_array[m_dwRGBEditingIdx]=(ubyte)dwVal;
	}

	return true;
}

void NColorProp::Click(NPoint& pt, NRect& rcItem)
{
	NRect rc(rcItem);
	int w = rc.Width()/4;
	rc.right = rc.left + w;

	for (udword i=0; i<4; i++)
	{
		if (rc.Contain(pt))
		{
			m_dwRGBEditingIdx = i;
			break;
		}
		rc.Move(w,0);
	}

}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUbyteComboProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NUbyteComboProp, NPropertyItem);

void NUbyteComboProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	//Init
	if (m_carrayStringsList.Count()==0)
	{
		NString str;
		str = m_pvarBlocDesc->pszDefValue;

		NString word;
		udword i=3;
		do
		{
			i = str.ExtractToken(i, word, ",]");
			if (i!=-1)
			{
				m_carrayStringsList.AddItem(word);
				i+=word.Length()+1;
			}

		} while (i!=-1);

	}


	//Draw
	ubyte val = m_pvarValue->byVal;

	if (val<(ubyte)m_carrayStringsList.Count())
		m_strValue.Format("%s", 	m_carrayStringsList[val]);
	else
		m_strValue="?";

	pdc->DrawText(m_strValue.Buffer(), rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

bool NUbyteComboProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);


	NFxGenApp* papp = (NFxGenApp*)GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();

	m_hMenu = CreatePopupMenu();

	for (udword i=0; i<m_carrayStringsList.Count(); i++)
	{
		::AppendMenu(m_hMenu, MF_STRING, (DWORD)i+1, m_carrayStringsList[i].Buffer());
	}

	POINT pt;
	::GetCursorPos(&pt);	//Cursor position even with keyboard 'Context key'
	ubyte byVal = (ubyte)::TrackPopupMenu(m_hMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RETURNCMD, pt.x, pt.y, null, pfrm->m_W32HWnd, null);

	::DestroyMenu(m_hMenu);

	if (byVal!=0)
		m_pvarValue->byVal = byVal-1;

	return true;
}

bool NUbyteComboProp::EndEdit(bool bSaveChanged)
{
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NFileBrowserProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NFileBrowserProp, NPropertyItem);

void NFileBrowserProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	pdc->DrawText(m_pvarValue->szVal, rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

bool NFileBrowserProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	NFileDialog dlg;
	dlg.Create("Choose a JPEG File to Load", m_pParent);
	if (1==dlg.DoModal())
	{
		//char* szval = ((char*)m_pObj + m_dwCPPPropOffset);
		NString str = dlg.GetPathName(0);
		strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());
	}

	return true;	//End of Edition
}

bool NFileBrowserProp::EndEdit(bool bSaveChanged)
{
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUseStoredOpsProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NUseStoredOpsProp, NPropertyItem);

void NUseStoredOpsProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	NObject* pobj = m_pvarValue->pcRefObj;

	//Display referenced object name (from varbloc)
	if (pobj)
	{
		char* pszName;
		NVarsBloc* pbloc = pobj->GetFirstVarsBloc();
		pbloc->GetValue(0, 0.0f, pszName);
		pdc->DrawText(pszName, rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	} else {
		pdc->DrawText("", rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

}

bool NUseStoredOpsProp::BeginEdit(NRect& rcItem)
{
	NRect rc = rcItem;
	rc.left = rc.right;

	NFxGenApp* papp = (NFxGenApp*)GetApp();
	NMainFrm* pfrm = (NMainFrm*)papp->GetMainWnd();

	//Creation du menu
	m_hMenu = CreatePopupMenu();

	NTreeNode* pnode = gNFxGen_GetEngine()->GetRootGroup();
	BuildMenu(pnode);

	POINT pt;
	::GetCursorPos(&pt);	//Cursor position even with keyboard 'Context key'
	NOperator* popSel = (NOperator*)::TrackPopupMenu(m_hMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RETURNCMD, pt.x, pt.y, null, pfrm->m_W32HWnd, null);

	::DestroyMenu(m_hMenu);

	//Affect selected 'stored operator'
	if (popSel!=null)
		m_pvarBloc->SetValue(m_dwvarIdx, 0.0f, popSel);

	return true;
}

bool NUseStoredOpsProp::EndEdit(bool bSaveChanged)
{
	return true;
}

void NUseStoredOpsProp::BuildMenu(NTreeNode* _pnode)
{
	//Parse Alls Pages to add 'NStoreOp'
	NObjectArray& arrayObjs = _pnode->GetObjsArray();
	udword dwCount = arrayObjs.Count();
	while (dwCount--)
	{
		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[dwCount];

		HMENU popMenu = CreateMenu();
		::AppendMenu(m_hMenu, MF_POPUP, (UINT)popMenu, ppage->GetName());

		NObjectArray storedOp;
		storedOp.SetManageDelete(false);
		ppage->GetOpsFromClassName("NStoreOp", storedOp);

		for (udword i=0; i<storedOp.Count(); i++)
		{
			NOperator* pop = (NOperator*)storedOp[i];
			if (strlen(pop->GetUserName())!=0)
				::AppendMenu(popMenu, MF_STRING, (DWORD)pop, pop->GetUserName());
		}
	}

	//Childs
	_pnode = _pnode->GetSon();
	if (_pnode)
		BuildMenu(_pnode);

}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NStringProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NStringProp, NPropertyItem);

void NStringProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	pdc->DrawText(m_pvarValue->szVal, rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

bool NStringProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	//Delete Old Control
	if (m_pwNGraphicstrl)	delete m_pwNGraphicstrl;

	//Create edit
	NRect rcCB(rcItem);
	NEditCtrl* pwNGraphicsB = new NEditCtrl;
	pwNGraphicsB->Create("", rcCB, m_pParent,false);
	m_pwNGraphicstrl = pwNGraphicsB;

	pwNGraphicsB->SetText(m_pvarValue->szVal);
	pwNGraphicsB->SetFocus();

	return false;
}

bool NStringProp::EndEdit(bool bSaveChanged)
{
	if (m_pwNGraphicstrl==null)		return false;

	//Save changed
	if (bSaveChanged)
	{
		NString str = ((NEditCtrl*)m_pwNGraphicstrl)->GetText();
		strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());
	}

	//Destroy control
	delete m_pwNGraphicstrl;
	m_pwNGraphicstrl=null;

	return bSaveChanged;
}
