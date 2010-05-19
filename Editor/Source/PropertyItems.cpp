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
#include "EditorApp.h"
#include "Operator.h"

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
	m_pParent				= null;
	m_pObject				= null;
	m_dwFieldIdx		= 0;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NPropertyItem::~NPropertyItem()
{
	if (m_pwNGraphicstrl)		NDELETE(m_pwNGraphicstrl, NGUIWnd);
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NIntProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NIntProp, NPropertyItem)
FIMPLEMENT_CLASS_END()


void NIntProp::Init()
{
	NFieldDesc* pfd = &m_pObject->GetRTClass()->m_paFieldsDesc[m_dwFieldIdx];
	sdword* paddr = (sdword*)m_pObject->GetFieldVarAddr(m_dwFieldIdx);

	m_slider.Create(pfd->pszName, NRect(0,0,0,0), m_pParent);
	m_slider.SetRange(pfd->fMin, pfd->fMax);
	m_slider.SetPos(*paddr);
	m_slider.SetStep(pfd->fStep);
	m_slider.OnValueChanged = FDelegate(this, (TDelegate)&NIntProp::OnValueChanged);
}

void NIntProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_slider.SetWindowRect(rcItem);
}

void NIntProp::OnValueChanged(NObject* _psender)
{
	//Change field value
	sdword* paddr = (sdword*)m_pObject->GetFieldVarAddr(m_dwFieldIdx);
	float fpos = m_slider.GetPos();
	*paddr = (sdword)fpos;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NColorProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NColorProp, NPropertyItem)
FIMPLEMENT_CLASS_END()

void NColorProp::Init()
{
	m_button.Create(NColor(255,0,0,0), NRect(0,0,0,0), m_pParent, 0);
}

void NColorProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_button.SetWindowRect(rcItem);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NComboProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NComboProp, NPropertyItem)
FIMPLEMENT_CLASS_END();

void NComboProp::Init()
{
	NFieldDesc* pfd = &m_pObject->GetRTClass()->m_paFieldsDesc[m_dwFieldIdx];
	ubyte* paddr = (ubyte*)m_pObject->GetFieldVarAddr(m_dwFieldIdx);

	//Make menu items
	NString str;
	str = pfd->pszDef;

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

	//Create menu
	m_button.Create(m_carrayStringsList[*paddr].Buffer(), NRect(0,0,0,0), m_pParent, 0);
//	m_button.GetMenu()->OnItemClick=FDelegate(this, (TDelegate)&NComboProp::OnMenuClick);
	for (udword i=0; i<m_carrayStringsList.Count(); i++)
	{
		m_button.GetMenu()->AddItem(m_carrayStringsList[i].Buffer(), i+1, 0);
	}

}

void NComboProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_button.SetWindowRect(rcItem);
}

/*void NComboProp::OnMenuClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	udword dwVal = pmenu->GetClickedCmdID();
	NMEItemDesc* pitem = pmenu->GetItemDescFromID( pmenu->GetClickedCmdID() );
	if (pitem)
	{
		m_button.SetText(pitem->strName.Buffer());	
	}
}*/


/*
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUwordProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NUwordProp, NPropertyItem);
FIMPLEMENT_CLASS_END();

void NUwordProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_strValue.Format("%d", m_pvarValue->wVal);
  pdc->DrawString(m_strValue.Buffer(), rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NUwordProp::BeginEdit(NRect& rcItem)
{
	if (m_pwNGraphicstrl)	delete m_pwNGraphicstrl;

	NRect rcCB(rcItem);

	NEditCtrl* pwNGraphicsB = new NEditCtrl;
	pwNGraphicsB->Create("", rcCB, m_pParent);
	m_pwNGraphicstrl = pwNGraphicsB;

	NString text;
	text.Format("%u", (udword)m_pvarValue->wVal);
	pwNGraphicsB->SetText(text.Buffer());
	pwNGraphicsB->SelectAll();
	pwNGraphicsB->SetFocus();

	pwNGraphicsB->OnEnter = FDelegate(this, (TDelegate)&NUwordProp::OnEnter);
	pwNGraphicsB->OnEscape = FDelegate(this, (TDelegate)&NUwordProp::OnEscape);

	return false;
}

bool NUwordProp::EndEdit(bool bSaveChanged)
{
	if (m_pwNGraphicstrl==null)		return false;

	//Save changed
	if (bSaveChanged)
	{
		NString str = ((NEditCtrl*)m_pwNGraphicstrl)->GetText();
		sdword dwVal = atoi(str.Buffer());
		if (dwVal>65535)	dwVal = 65535;
		if (dwVal<0)		dwVal = 0;
		m_pvarValue->wVal = (uword)dwVal;
		//strcpy(m_pvarValue->szVal, str.Buffer());
	}

	//Destroy control
	delete m_pwNGraphicstrl;
	m_pwNGraphicstrl=null;

	return bSaveChanged;
}

bool NUwordProp::AddValue(sdword dwDelta)
{
	sdword dwVal = (sdword)m_pvarValue->wVal + (sdword)dwDelta;
	if (dwVal>65535)	dwVal = 65535;
	if (dwVal<0)			dwVal = 0;
	m_pvarValue->wVal = (uword)dwVal;
	return true;
}

void NUwordProp::OnEnter(NEditCtrl* pEdit)
{
	((NPropertiesCtrl*)m_pParent)->EndRowEditing();
}

void NUwordProp::OnEscape(NEditCtrl* pEdit)
{
	((NPropertiesCtrl*)m_pParent)->EndRowEditing(false);
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NFloatProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NFloatProp, NPropertyItem);
FIMPLEMENT_CLASS_END();

void NFloatProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_strValue.Format("%0.3f", m_pvarValue->fVal);
  pdc->DrawString(m_strValue.Buffer(), rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NFloatProp::BeginEdit(NRect& rcItem)
{
	if (m_pwNGraphicstrl)	delete m_pwNGraphicstrl;

	NRect rcCB(rcItem);

	NEditCtrl* pwNGraphicsB = new NEditCtrl;
	pwNGraphicsB->Create("", rcCB, m_pParent);
	m_pwNGraphicstrl = pwNGraphicsB;

	NString text;
	text.Format("%f", m_pvarValue->fVal);
	pwNGraphicsB->SetText(text.Buffer());
	pwNGraphicsB->SelectAll();
	pwNGraphicsB->SetFocus();

	pwNGraphicsB->OnEnter = FDelegate(this, (TDelegate)&NFloatProp::OnEnter);
	pwNGraphicsB->OnEscape = FDelegate(this, (TDelegate)&NFloatProp::OnEscape);

	return false;
}

bool NFloatProp::EndEdit(bool bSaveChanged)
{
	if (m_pwNGraphicstrl==null)		return false;

	//Save changed
	if (bSaveChanged)
	{
		NString str = ((NEditCtrl*)m_pwNGraphicstrl)->GetText();
		m_pvarValue->fVal = atof(str.Buffer());
		//strcpy(m_pvarValue->szVal, str.Buffer());
	}

	//Destroy control
	delete m_pwNGraphicstrl;
	m_pwNGraphicstrl=null;

	return bSaveChanged;
}

bool NFloatProp::AddValue(sdword dwDelta)
{
	m_pvarValue->fVal = m_pvarValue->fVal + ((float)dwDelta)/1000.0f;
	m_pvarValue->fVal *= 10000;
	m_pvarValue->fVal = floor(m_pvarValue->fVal + .5);
	m_pvarValue->fVal /= 10000;
	return true;
}

void NFloatProp::OnEnter(NEditCtrl* pEdit)
{
	((NPropertiesCtrl*)m_pParent)->EndRowEditing();
}

void NFloatProp::OnEscape(NEditCtrl* pEdit)
{
	((NPropertiesCtrl*)m_pParent)->EndRowEditing(false);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUFloatProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NUFloatProp, NPropertyItem);
FIMPLEMENT_CLASS_END();

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

void NUFloatProp::OnEnter(NEditCtrl* pEdit)
{
	((NPropertiesCtrl*)m_pParent)->EndRowEditing();
}

void NUFloatProp::OnEscape(NEditCtrl* pEdit)
{
	((NPropertiesCtrl*)m_pParent)->EndRowEditing(false);
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NColorProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NColorProp, NPropertyItem);
FIMPLEMENT_CLASS_END();

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
*/




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NFileBrowserProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*FIMPLEMENT_CLASS(NFileBrowserProp, NPropertyItem)
FIMPLEMENT_CLASS_END();

void NFileBrowserProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
  pdc->DrawString(m_pvarValue->szVal, rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NFileBrowserProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	//NFileDialog dlg;
	//dlg.Create("Choose a File to Load", m_pParent);
	//if (1==dlg.DoModal())
	//{
	//	NString str = dlg.GetPathName(0);
	//	strcpy(m_pvarValue->szVal, str.Buffer());
	//}

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
FIMPLEMENT_CLASS_END();

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

	//Menu Creation
	if (m_wndMenu.GetItemsCount()==0)
	{
		m_wndMenu.Create("", m_pParent);
		m_wndMenu.OnItemClick=FDelegate(this, (TDelegate)&NUseStoredOpsProp::OnMenuClick);
	}	else {
		m_wndMenu.DeleteAllItems();
	}

	//Insert Menu Items
//	NTreeNode* pnode = NEngineOp::GetProject()->GetRootGroup();
//	BuildMenu(pnode);

	//Display  menu
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
	NOperatorNode* popSel = (NOperatorNode*)pmenu->GetClickedCmdID();

	//Affect selected 'stored operator'
	if (popSel!=null)
		m_pvarBloc->SetValue(m_dwvarIdx, 0.0f, popSel);

	((NPropertiesCtrl*)m_pParent)->SaveRowEditing();
}

void NUseStoredOpsProp::BuildMenu(NTreeNode* _pnode)
{
//
//	//Parse Alls Pages to add 'NStoreOp'
//	NObjectArray& arrayObjs = _pnode->GetObjsArray();
//	udword dwCount = arrayObjs.Count();
//	udword idx=0;
//	while (dwCount--)
//	{
//		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[idx++];
//
//		NMenuCtrl* popMenu = m_wndMenu.CreatePopupMenu(ppage->GetName(), -1);
//
//		NObjectArray storedOp;
//		storedOp.SetManageDelete(false);
//		ppage->GetOpsFromClassName("NStoreOp", storedOp);
//
//		for (udword i=0; i<storedOp.Count(); i++)
//		{
//			NOperatorNode* pop = (NOperatorNode*)storedOp[i];
//			if (strlen(pop->GetUserName())!=0)
//				popMenu->AddItem(pop->GetUserName(), (udword)pop, 0);
//		}
//	}
//
//	//Childs
//	_pnode = _pnode->GetSon();
//	if (_pnode)
//		BuildMenu(_pnode);
//
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NStringProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NStringProp, NPropertyItem);
FIMPLEMENT_CLASS_END();

void NStringProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
  pdc->DrawString(m_pvarValue->szVal, rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NStringProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	//Delete Old Control
	if (m_pwNGraphicstrl)	NDELETE(m_pwNGraphicstrl, NGUIWnd);

	//Create edit
	NRect rcCB(rcItem);

	NEditCtrl* pwNGraphicsB = NNEW(NEditCtrl);
	pwNGraphicsB->Create("", rcCB, m_pParent);
	m_pwNGraphicstrl = pwNGraphicsB;

	pwNGraphicsB->SetText(m_pvarValue->szVal);
	pwNGraphicsB->SetFocus();

	pwNGraphicsB->OnEnter = FDelegate(this, (TDelegate)&NStringProp::OnEnter);
	pwNGraphicsB->OnEscape = FDelegate(this, (TDelegate)&NStringProp::OnEscape);

	return false;
}

bool NStringProp::EndEdit(bool bSaveChanged)
{
	if (m_pwNGraphicstrl==null)		return false;

	//Save changed
	if (bSaveChanged)
	{
		NString str = ((NEditCtrl*)m_pwNGraphicstrl)->GetText();
		strcpy(m_pvarValue->szVal, str.Buffer());
	}

	//Destroy control
	NDELETE(m_pwNGraphicstrl, NGUIWnd);
	m_pwNGraphicstrl=null;

	return bSaveChanged;
}

void NStringProp::OnEnter(NEditCtrl* pEdit)
{
	((NPropertiesCtrl*)m_pParent)->EndRowEditing();
}

void NStringProp::OnEscape(NEditCtrl* pEdit)
{
	((NPropertiesCtrl*)m_pParent)->EndRowEditing(false);
}
*/