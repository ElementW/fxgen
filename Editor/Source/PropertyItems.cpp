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
FIMPLEMENT_CLASS(NUbyteProp, NPropertyItem)

void NUbyteProp::Init()
{
	ubyte byVal;
	m_pvarBloc->GetValue(m_dwvarIdx, 0.0f, byVal);

	m_slider.Create(m_pvarBlocDesc->pszName, NRect(0,0,0,0), m_pParent);
	m_slider.SetRange(m_pvarBlocDesc->fMin, m_pvarBlocDesc->fMax);
	m_slider.SetStep(m_pvarBlocDesc->fStep);
	m_slider.SetPos(byVal);
	m_slider.OnValueChanged = FDelegate(this, (TDelegate)&NUbyteProp::OnValueChanged);
}

void NUbyteProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_slider.SetWindowRect(rcItem);
}

void NUbyteProp::OnValueChanged(NObject* _psender)
{
	//Change field value
	m_pvarBloc->SetValue(m_dwvarIdx, 0.0f, (ubyte)m_slider.GetPos());

	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperatorFx*)m_pvarBloc->GetOwner());
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUwordProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NUwordProp, NPropertyItem)

void NUwordProp::Init()
{
	uword wVal;
	m_pvarBloc->GetValue(m_dwvarIdx, 0.0f, wVal);

	m_slider.Create(m_pvarBlocDesc->pszName, NRect(0,0,0,0), m_pParent);
	m_slider.SetRange(m_pvarBlocDesc->fMin, m_pvarBlocDesc->fMax);
	m_slider.SetStep(m_pvarBlocDesc->fStep);
	m_slider.SetPos(wVal);
	m_slider.OnValueChanged = FDelegate(this, (TDelegate)&NUwordProp::OnValueChanged);
}

void NUwordProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_slider.SetWindowRect(rcItem);
}

void NUwordProp::OnValueChanged(NObject* _psender)
{
	//Change field value
	m_pvarBloc->SetValue(m_dwvarIdx, 0.0f, (uword)m_slider.GetPos());

	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperatorFx*)m_pvarBloc->GetOwner());
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NFloatProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NFloatProp, NPropertyItem)

void NFloatProp::Init()
{
	float fVal;
	m_pvarBloc->GetValue(m_dwvarIdx, 0.0f, fVal);

	m_slider.Create(m_pvarBlocDesc->pszName, NRect(0,0,0,0), m_pParent);
	m_slider.SetRange(m_pvarBlocDesc->fMin, m_pvarBlocDesc->fMax);
	m_slider.SetStep(m_pvarBlocDesc->fStep);
	m_slider.SetPos(fVal);
	m_slider.OnValueChanged = FDelegate(this, (TDelegate)&NFloatProp::OnValueChanged);
}

void NFloatProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_slider.SetWindowRect(rcItem);
}

void NFloatProp::OnValueChanged(NObject* _psender)
{
	//Change field value
	m_pvarBloc->SetValue(m_dwvarIdx, 0.0f, m_slider.GetPos());

	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperatorFx*)m_pvarBloc->GetOwner());
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
	udword dwVal;
	m_pvarBloc->GetValue(m_dwvarIdx, 0.0f, dwVal);

	NColor col;
	col.SetFromRGBA(dwVal);

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
	m_pvarBloc->SetValue(m_dwvarIdx, 0.0f,  m_button.GetPicker()->GetClickedColor().GetRGBA());


	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperatorFx*)m_pvarBloc->GetOwner());
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
	ubyte byVal;
	m_pvarBloc->GetValue(m_dwvarIdx, 0.0f, byVal);

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
	ubyte val = byVal;
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
	m_pvarBloc->SetValue(m_dwvarIdx, 0.0f,  (ubyte)(m_button.GetMenu()->GetClickedCmdID()-1));

	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperatorFx*)m_pvarBloc->GetOwner());
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NFileBrowserProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*FIMPLEMENT_CLASS(NFileBrowserProp, NPropertyItem);

void NFileBrowserProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	pdc->DrawString(m_pvarValue->szVal, rcItem, NDT_VCENTER|NDT_SINGLELINE|NDT_END_ELLIPSIS, RGBA(0,0,0,255) );
}

bool NFileBrowserProp::BeginEdit(NRect& rcItem)
{
	assert(m_pParent!=null);

	NFileDialog dlg;
	dlg.Create("Choose a File to Load", m_pParent);
	if (1==dlg.DoModal())
	{
		NString str = dlg.GetPathName(0);
		strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());
	}

	return true;	//End of Edition
}

bool NFileBrowserProp::EndEdit(bool bSaveChanged)
{
	return true;
}
*/

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUseStoredOpsProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NUseStoredOpsProp, NPropertyItem);

void NUseStoredOpsProp::Init()
{
	NObject* pobj;
	m_pvarBloc->GetValue(m_dwvarIdx, 0.0f, pobj);

	char* pszName="";

	//Display referenced object name (from varbloc)
	if (pobj)
	{
		NVarsBloc* pbloc = pobj->GetFirstVarsBloc();
		pbloc->GetValue(0, 0.0f, pszName);
	}

	//Create menu button
	m_button.Create(pszName, NRect(0,0,0,0), m_pParent, 0);
	m_button.OnChanged = FDelegate(this, (TDelegate)&NUseStoredOpsProp::OnValueChanged);

	//Buid menu's items
	NTreeNode* pnode = NEditorGUI::GetInstance()->GetAsset()->GetRootGroup();
	BuildMenu(pnode);
}


void NUseStoredOpsProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_button.SetWindowRect(rcItem);
}

void NUseStoredOpsProp::OnValueChanged(NObject* _psender)
{
	//Change value
	NOperatorFx* popSel = (NOperatorFx*)m_button.GetMenu()->GetClickedCmdID();

	//Affect selected 'stored operator'
	if (popSel!=null)
		m_pvarBloc->SetValue(m_dwvarIdx, 0.0f, popSel);

	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperatorFx*)m_pvarBloc->GetOwner());
}


/*void NUseStoredOpsProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
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
*/
void NUseStoredOpsProp::BuildMenu(NTreeNode* _pnode)
{
	//Parse alls Graphs in order to add 'NStoreOp'
	NObjectArray& arrayObjs = _pnode->GetObjsArray();
	udword dwCount = arrayObjs.Count();
	udword idx=0;
	while (dwCount--)
	{
		NOpGraphModel* ppage = (NOpGraphModel*)arrayObjs[idx++];

		NMenuCtrl* popMenu = m_button.GetMenu();	//->CreatePopupMenu(ppage->GetName(), -1);

		NObjectArray storedOp;
		storedOp.SetManageDelete(false);
		ppage->GetOpsFromClassName("NStoreOp", storedOp);

		for (udword i=0; i<storedOp.Count(); i++)
		{
			NOperatorFx* pop = (NOperatorFx*)storedOp[i];
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

void NStringProp::Init()
{
	char* psz;
	m_pvarBloc->GetValue(m_dwvarIdx, 0.0f, psz);

	m_edit.Create(m_pvarBlocDesc->pszName, NRect(0,0,0,0), m_pParent);
	m_edit.SetText(psz);

	m_edit.OnEnter = FDelegate(this, (TDelegate)&NStringProp::OnValueChanged);
}

void NStringProp::DrawItem(N2DPainter* pdc, NRect& rcItem)
{
	m_edit.SetWindowRect(rcItem);
}

void NStringProp::OnValueChanged(NObject* _psender)
{
	//Change field value
	NString str = m_edit.GetText();
	m_pvarBloc->SetValue(m_dwvarIdx, 0.0f,  str.Buffer());

	// Send Event
	NEditorGUI::GetInstance()->EmitPropertiesChanged((NOperatorFx*)m_pvarBloc->GetOwner());
}
