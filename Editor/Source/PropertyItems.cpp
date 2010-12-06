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
#include "EditorGUI.h"

#include "AssetModel.h"
#include "OpGraphModel.h"

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
/*FIMPLEMENT_CLASS(NUbyteProp, NPropertyItem);

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
}*/


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

void NColorProp::Init()
{
	NColor col;
	col.SetFromRGBA(m_pvarValue->dwVal);

	m_button.Create(col, NRect(0,0,0,0), m_pParent, 0);
	m_button.OnChanged = FDelegate(this, (TDelegate)&NColorProp::OnValueChanged);
}

void NColorProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_button.SetWindowRect(rcItem);
}

void NColorProp::OnValueChanged(NObject* _psender)
{
	//Change field value
	m_pvarValue->dwVal = m_button.GetPicker()->GetClickedColor().GetRGBA();
	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperator*)m_pvarBloc->GetOwner());
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUbyteComboProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NUbyteComboProp, NPropertyItem);


void NUbyteComboProp::Init()
{
	//Make menu items
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

	//Create menu button
	ubyte val = m_pvarValue->byVal;
	word = "?";
	if (val<(ubyte)m_carrayStringsList.Count())
		word = m_carrayStringsList[val].Buffer();

	m_button.Create(word.Buffer(), NRect(0,0,0,0), m_pParent, 0);
	m_button.OnChanged = FDelegate(this, (TDelegate)&NUbyteComboProp::OnValueChanged);

	//Add items to menu
	for (udword i=0; i<m_carrayStringsList.Count(); i++)
	{
		m_button.GetMenu()->AddItem(m_carrayStringsList[i].Buffer(), i+1, 0);
	}

}

void NUbyteComboProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_button.SetWindowRect(rcItem);
}

void NUbyteComboProp::OnValueChanged(NObject* _psender)
{
	//Change value
	m_pvarValue->byVal = m_button.GetMenu()->GetClickedCmdID()-1;
	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperator*)m_pvarBloc->GetOwner());
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
	NTreeNode* pnode = NEditorGUI::GetInstance()->GetAsset()->GetRootGroup();
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

	//((NPropertiesCtrl*)m_pParent)->SaveRowEditing();
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

	//Parse alls Graphs in order to add 'NStoreOp'
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



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUbyteProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NUbyteProp, NPropertyItem)

void NUbyteProp::Init()
{
	m_slider.Create(m_pvarBlocDesc->pszName, NRect(0,0,0,0), m_pParent);
	m_slider.SetRange(0.0f, 255.0f);	//###TOFIX###
	m_slider.SetPos(m_pvarValue->byVal);
	m_slider.SetStep(1.0f);	//###TOFIX###
	m_slider.OnValueChanged = FDelegate(this, (TDelegate)&NUbyteProp::OnValueChanged);
}

void NUbyteProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_slider.SetWindowRect(rcItem);
}

void NUbyteProp::OnValueChanged(NObject* _psender)
{
	//Change field value
	m_pvarValue->byVal = (sdword)m_slider.GetPos();

	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperator*)m_pvarBloc->GetOwner());
}
