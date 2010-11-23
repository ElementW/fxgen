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
#include "PropertiesCtrl.h"
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
FIMPLEMENT_CLASS(NUbyteProp, NPropertyItem);

void NUbyteProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_strValue.Format("%d", m_pvarValue->byVal);
	pdc->DrawString(m_strValue.Buffer(), rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
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
FIMPLEMENT_CLASS(NUwordProp, NPropertyItem);

void NUwordProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_strValue.Format("%d", m_pvarValue->wVal);
	pdc->DrawString(m_strValue.Buffer(), rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
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
FIMPLEMENT_CLASS(NFloatProp, NPropertyItem);

void NFloatProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_strValue.Format("%0.3f", m_pvarValue->fVal);
	pdc->DrawString(m_strValue.Buffer(), rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
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
	m_pvarValue->fVal = m_pvarValue->fVal + ((float)dwDelta)/1000.0f;
	m_pvarValue->fVal *= 10000;
	m_pvarValue->fVal = floor(m_pvarValue->fVal + .5);
	m_pvarValue->fVal /= 10000;
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUFloatProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NUFloatProp, NPropertyItem);

void NUFloatProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_strValue.Format("%0.3f", m_pvarValue->fVal);
	pdc->DrawString(m_strValue.Buffer(), rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NUFloatProp::BeginEdit(NRect& rcItem)
{
	return false;
}

bool NUFloatProp::EndEdit(bool bSaveChanged)
{
	return false;
}

bool NUFloatProp::AddValue(sdword dwDelta)
{
	m_pvarValue->fVal = m_pvarValue->fVal + ((float)dwDelta)/1000.0f;
	m_pvarValue->fVal *= 10000;
	m_pvarValue->fVal = floor(m_pvarValue->fVal + .5);
	m_pvarValue->fVal /= 10000;
	if(m_pvarValue->fVal < 0)
		m_pvarValue->fVal = 0;
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NColorProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NColorProp, NPropertyItem);

void NColorProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	NRGBA val;
	val.dwCol = m_pvarValue->dwVal;

	///////////////////////////////////////
	//Display RGBA Values
	NString cstr;
	NRect rc(rcItem);
	udword w = rc.Width()/5;

	//Red
	rc.right = rc.left + w;
	cstr.Format("%d", val.r);
	pdc->DrawString(cstr.Buffer(), rc, NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );

	//Green
	rc.Move(w,0);
	cstr.Format("%d",  val.g);
	pdc->DrawString(cstr.Buffer(), rc, NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );

	//Blue
	rc.Move(w,0);
	cstr.Format("%d",  val.b);
	pdc->DrawString(cstr.Buffer(), rc, NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );

	//Alpha
	rc.Move(w,0);
	cstr.Format("%d",  val.a);
	pdc->DrawString(cstr.Buffer(), rc, NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );

	///////////////////////////////////////
	//Display Color Rect
	NColor col( val.r,  val.g,  val.b, 255);
	rc.left = rc.right;
	rc.right = rcItem.right;
	pdc->Draw3dRect(rc, RGBA(255,255,255,255), RGBA(0,0,0,255));
	rc.Deflate(1,1);
	pdc->FillSolidRect(rc, col);

}


bool NColorProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	//Color Picker
	if (m_bFirst)
	{
		m_bFirst=false;
		m_wndPicker.Create(m_pParent);
		m_wndPicker.OnColorClick=FDelegate(this, (TDelegate)&NColorProp::OnColorClick);
	}

	NPoint pt(rcItem.left, rcItem.bottom);
	m_pParent->ClientToScreen(pt);
	m_wndPicker.TrackPopup(pt, NColor(m_pvarValue->dwVal));

	return false;	//Not End of Edition
}

bool NColorProp::EndEdit(bool bSaveChanged)
{
	return true;
}

bool NColorProp::AddValue(sdword dwDelta)
{
	if (m_dwRGBEditingIdx<4)
	{
		NRGBA* pval = (NRGBA*) &m_pvarValue->dwVal;

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
	int w = rc.Width()/6; // should be 5, but then the pointer misses controls - why?
	rc.right = rc.left + w;

	for (udword i=0; i<5; i++)
	{
		if (rc.Contain(pt))
		{
			m_dwRGBEditingIdx = i;
			break;
		}
		rc.Move(w,0);
	}

}

void NColorProp::OnColorClick(NObject* _psender)
{
	//TRACE("NColorProp::OnColorClick\n");
	m_pvarValue->dwVal = m_wndPicker.GetClickedColor().GetRGBA();
	((NPropertiesCtrl*)m_pParent)->SaveRowEditing();
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUbyteComboProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NUbyteComboProp, NPropertyItem);

void NUbyteComboProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
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
		m_strValue.Format("%s", 	m_carrayStringsList[val].Buffer());
	else
		m_strValue="?";

	pdc->DrawString(m_strValue.Buffer(), rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NUbyteComboProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	//Menu
	if (m_wndMenu.GetItemsCount()==0)
	{
		m_wndMenu.Create("", m_pParent);
		m_wndMenu.OnItemClick=FDelegate(this, (TDelegate)&NUbyteComboProp::OnMenuClick);
		for (udword i=0; i<m_carrayStringsList.Count(); i++)
		{
			m_wndMenu.AddItem(m_carrayStringsList[i].Buffer(), i+1, 0);
		}
	}

	NPoint pt(rcItem.left, rcItem.bottom);
	m_pParent->ClientToScreen(pt);
	m_wndMenu.TrackPopupMenu(pt, null);

	return false;
}

bool NUbyteComboProp::EndEdit(bool bSaveChanged)
{
	return true;
}

void NUbyteComboProp::OnMenuClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	ubyte byVal = (ubyte)pmenu->GetClickedCmdID();
	if (byVal!=0)
		m_pvarValue->byVal = byVal-1;

	((NPropertiesCtrl*)m_pParent)->SaveRowEditing();

}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NFileBrowserProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NFileBrowserProp, NPropertyItem);

void NFileBrowserProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	pdc->DrawString(m_pvarValue->szVal, rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NFileBrowserProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

/*	NFileDialog dlg;
	dlg.Create("Choose a File to Load", m_pParent);
	if (1==dlg.DoModal())
	{
		NString str = dlg.GetPathName(0);
		strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());
	}*/

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
FIMPLEMENT_CLASS(NUseStoredOpsProp, NPropertyItem);

void NUseStoredOpsProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	NObject* pobj = m_pvarValue->pcRefObj;

	//Display referenced object name (from varbloc)
	if (pobj)
	{
		char* pszName;
		NVarsBloc* pbloc = pobj->GetFirstVarsBloc();
		pbloc->GetValue(0, 0.0f, pszName);
		pdc->DrawString(pszName, rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
	} else {
		pdc->DrawString("", rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
	}

}

bool NUseStoredOpsProp::BeginEdit(NRect& rcItem)
{
	NRect rc = rcItem;
	rc.left = rc.right;

	//Creation du menu
	NTreeNode* pnode = NEngineOp::GetEngine()->GetRootGroup();
	BuildMenu(pnode);

	NPoint pt(rcItem.left, rcItem.bottom);
	m_pParent->ClientToScreen(pt);
	m_wndMenu.TrackPopupMenu(pt, null);

	return false;
}

bool NUseStoredOpsProp::EndEdit(bool bSaveChanged)
{
	return true;
}

void NUseStoredOpsProp::OnMenuClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	NOperator* popSel = (NOperator*)pmenu->GetClickedCmdID();

	//Affect selected 'stored operator'
	if (popSel!=null)
		m_pvarBloc->SetValue(m_dwvarIdx, 0.0f, popSel);

	((NPropertiesCtrl*)m_pParent)->SaveRowEditing();
}

void NUseStoredOpsProp::BuildMenu(NTreeNode* _pnode)
{

	if (m_wndMenu.GetItemsCount()==0)
	{
		m_wndMenu.Create("", m_pParent);
		m_wndMenu.OnItemClick=FDelegate(this, (TDelegate)&NUseStoredOpsProp::OnMenuClick);
	}	else {
		m_wndMenu.DeleteAllItems();
	}

	//Parse Alls Pages to add 'NStoreOp'
	NObjectArray& arrayObjs = _pnode->GetObjsArray();
	udword dwCount = arrayObjs.Count();
	udword idx=0;
	while (dwCount--)
	{
		NOpGraphModel* ppage = (NOpGraphModel*)arrayObjs[idx++];

		NMenuCtrl* popMenu = m_wndMenu.CreatePopupMenu(ppage->GetName(), -1);

		NObjectArray storedOp;
		storedOp.SetManageDelete(false);
		ppage->GetOpsFromClassName("NStoreOp", storedOp);

		for (udword i=0; i<storedOp.Count(); i++)
		{
			NOperator* pop = (NOperator*)storedOp[i];
			if (strlen(pop->GetUserName())!=0)
				popMenu->AddItem(pop->GetUserName(), (udword)pop, 0);
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
FIMPLEMENT_CLASS(NStringProp, NPropertyItem);

void NStringProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	pdc->DrawString(m_pvarValue->szVal, rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NStringProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	//Delete Old Control
	if (m_pwNGraphicstrl)	delete m_pwNGraphicstrl;

	//Create edit
	NRect rcCB(rcItem);

	NEditCtrl* pwNGraphicsB = new NEditCtrl;
	pwNGraphicsB->Create("", rcCB, m_pParent);
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
