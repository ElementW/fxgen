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
#include "resource.h"
#include "gui.h"

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

bool NUbyteProp::MenuEdit	(NRect& rcItem)
{
   	NRect rc = rcItem;
   	rc.left = rc.right;

   	//Creation du menu
   	NTreeNode* pnode = NEngineOp::GetEngine()->GetRootGroup();
   	BuildMenu(pnode);

   	NPoint pt(rcItem.left, rcItem.bottom);
   	m_pParent->ClientToScreen(pt);
   	NOperator* popSel = (NOperator*)m_wndMenu.TrackPopupMenu(pt, null, true);

	NUWordVarOp *pOp = (NUWordVarOp*)popSel;

   	//Affect selected 'stored operator'
   	if (popSel!=null)
	{
		m_pvarValue->byVal = (ubyte)pOp->GetUserVal();
		//pOp->val = &m_pvarValue->fVal;
		pOp->AddUWordVar( (udword*)&m_pvarValue->wVal );
		//m_pvarBloc->SetValue(0, 0.0f, pOp->GetUserVal());
	}


		/*
			float val = 0.0;
   			pop->m_pcvarsBloc->GetValue(0,0,(float&)val);

		*/
		/*float val = 0.0;
		popSel->m_pcvarsBloc->GetValue(0,0,(float&)val);
		m_pvarBloc->SetValue(0, 0.0f, val);*/
	return true;

}

bool NUbyteProp::EndEdit(bool bSaveChanged)
{
	if (m_pwNGraphicstrl==null)		return false;

	//Save changed
	if (bSaveChanged)
	{
		NString str = ((NEditCtrl*)m_pwNGraphicstrl)->GetText();
		m_pvarValue->byVal =(char) atoi(str.Buffer());
		//strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());
	}

	//Destroy control
	delete m_pwNGraphicstrl;
	m_pwNGraphicstrl=null;

	return bSaveChanged;
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

void NUbyteProp::BuildMenu(NTreeNode* _pnode)
{

	if (m_wndMenu.m_W32HWnd==0)
   		m_wndMenu.Create("", m_pParent);
   	else
   		m_wndMenu.DeleteAllItems();

   	//Parse Alls Pages to add 'NStoreOp'
   	NObjectArray& arrayObjs = _pnode->GetObjsArray();
   	udword dwCount = arrayObjs.Count();
   	udword idx=0;
   	while (dwCount--)
   	{
   		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[idx++];

   		NMenuCtrl* popMenu = m_wndMenu.CreatePopupMenu(ppage->GetName(), -1);

   		NObjectArray storedOp;
   		storedOp.SetManageDelete(false);
   		ppage->GetOpsFromClassName("NUWordVarOp", storedOp);

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

bool NUwordProp::MenuEdit	(NRect& rcItem)
{
   	NRect rc = rcItem;
   	rc.left = rc.right;

   	//Creation du menu
   	NTreeNode* pnode = NEngineOp::GetEngine()->GetRootGroup();
   	BuildMenu(pnode);

   	NPoint pt(rcItem.left, rcItem.bottom);
   	m_pParent->ClientToScreen(pt);
   	NOperator* popSel = (NOperator*)m_wndMenu.TrackPopupMenu(pt, null, true);



   	//Affect selected 'stored operator'

   	//Affect selected 'stored operator'
   	if (popSel!=null)
	{
		if( !strcmp(popSel->GetRTClass()->m_pszClassName, "NFloatVarOp") )
		{
			NFloatVarOp *pOp = (NFloatVarOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddFloatVar( (float*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pOp = (NUWordVarOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddUWordVar( (udword*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsSumOp") )
		{
			NVarsSumOp *pOp = (NVarsSumOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddFloatVar( (float*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NLoadVarOp") )
		{
			NLoadVarOp *pOp = (NLoadVarOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddFloatVar( (float*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsLogOp") )
		{
			NLoadVarOp *pOp = (NLoadVarOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddFloatVar( (float*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsPowerOp") )
		{
			NVarsPowerOp *pOp = (NVarsPowerOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddFloatVar( (float*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsMultiplyOp") )
		{
			NVarsMultiplyOp *pOp = (NVarsMultiplyOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddFloatVar( (float*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsDivideOp") )
		{
			NVarsDivideOp *pOp = (NVarsDivideOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddFloatVar( (float*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsSubtractOp") )
		{
			NVarsSubtractOp *pOp = (NVarsSubtractOp*)popSel;
			m_pvarValue->wVal = (udword)pOp->GetUserVal();
			pOp->AddFloatVar( (float*)&m_pvarValue->fVal );
		}
	}

	return true;

}

bool NUwordProp::EndEdit(bool bSaveChanged)
{
	if (m_pwNGraphicstrl==null)		return false;

	//Save changed
	if (bSaveChanged)
	{
		NString str = ((NEditCtrl*)m_pwNGraphicstrl)->GetText();
		m_pvarValue->wVal =(int) atoi(str.Buffer());
		//strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());
	}

	//Destroy control
	delete m_pwNGraphicstrl;
	m_pwNGraphicstrl=null;

	return bSaveChanged;
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

void NUwordProp::BuildMenu(NTreeNode* _pnode)
{

	if (m_wndMenu.m_W32HWnd==0)
   		m_wndMenu.Create("", m_pParent);
   	else
   		m_wndMenu.DeleteAllItems();

   	//Parse Alls Pages to add 'NStoreOp'
   	NObjectArray& arrayObjs = _pnode->GetObjsArray();
   	udword dwCount = arrayObjs.Count();
   	udword idx=0;
   	while (dwCount--)
   	{
   		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[idx++];

   		NMenuCtrl* popMenu = m_wndMenu.CreatePopupMenu(ppage->GetName(), -1);

   		NObjectArray storedOp;
   		storedOp.SetManageDelete(false);
   		ppage->GetOpsFromClassName("NUWordVarOp", storedOp);
		ppage->GetOpsFromClassName("NFloatVarOp", storedOp);
		ppage->GetOpsFromClassName("NLoadVarOp", storedOp);
		ppage->GetOpsFromClassName("NVarsLogOp", storedOp);
		ppage->GetOpsFromClassName("NVarsPowerOp", storedOp);
		ppage->GetOpsFromClassName("NVarsMultiplyOp", storedOp);
		ppage->GetOpsFromClassName("NVarsDivideOp", storedOp);
		ppage->GetOpsFromClassName("NVarsSubtractOp", storedOp);

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
//										NFloatProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NFloatProp, NPropertyItem);

void NFloatProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	m_strValue.Format("%0.3f", m_pvarValue->fVal);
	pdc->DrawText(m_strValue.Buffer(), rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}


bool NFloatProp::BeginEdit(NRect& rcItem)
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

bool NFloatProp::MenuEdit	(NRect& rcItem)
{
   	NRect rc = rcItem;
   	rc.left = rc.right;

   	//Creation du menu
   	NTreeNode* pnode = NEngineOp::GetEngine()->GetRootGroup();
   	BuildMenu(pnode);

   	NPoint pt(rcItem.left, rcItem.bottom);
   	m_pParent->ClientToScreen(pt);
   	NOperator* popSel = (NOperator*)m_wndMenu.TrackPopupMenu(pt, null, true);



   	//Affect selected 'stored operator'
   	if (popSel!=null)
	{
		if( !strcmp(popSel->GetRTClass()->m_pszClassName, "NFloatVarOp") )
		{
			NFloatVarOp *pOp = (NFloatVarOp*)popSel;
			m_pvarValue->fVal = pOp->GetUserVal();
			pOp->AddFloatVar( &m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NUWordVarOp") )
		{
			NUWordVarOp *pOp = (NUWordVarOp*)popSel;
			m_pvarValue->wVal = pOp->GetUserVal();
			pOp->AddUWordVar( (udword*)&m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsSumOp") )
		{
			NVarsSumOp *pOp = (NVarsSumOp*)popSel;
			m_pvarValue->fVal = pOp->GetUserVal();
			pOp->AddFloatVar( &m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NLoadVarOp") )
		{
			NLoadVarOp *pOp = (NLoadVarOp*)popSel;
			m_pvarValue->fVal = pOp->GetUserVal();
			pOp->AddFloatVar( &m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsLogOp") )
		{
			NLoadVarOp *pOp = (NLoadVarOp*)popSel;
			m_pvarValue->fVal = pOp->GetUserVal();
			pOp->AddFloatVar( &m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsPowerOp") )
		{
			NVarsPowerOp *pOp = (NVarsPowerOp*)popSel;
			m_pvarValue->fVal = pOp->GetUserVal();
			pOp->AddFloatVar( &m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsMultiplyOp") )
		{
			NVarsMultiplyOp *pOp = (NVarsMultiplyOp*)popSel;
			m_pvarValue->fVal = pOp->GetUserVal();
			pOp->AddFloatVar( &m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsDivideOp") )
		{
			NVarsDivideOp *pOp = (NVarsDivideOp*)popSel;
			m_pvarValue->fVal = pOp->GetUserVal();
			pOp->AddFloatVar( &m_pvarValue->fVal );
		}
		else if(  !strcmp(popSel->GetRTClass()->m_pszClassName, "NVarsSubtractOp") )
		{
			NVarsSubtractOp *pOp = (NVarsSubtractOp*)popSel;
			m_pvarValue->fVal = pOp->GetUserVal();
			pOp->AddFloatVar( &m_pvarValue->fVal );
		}
	}

	return true;
}

bool NFloatProp::EndEdit(bool bSaveChanged)
{
	if (m_pwNGraphicstrl==null)		return false;

	//Save changed
	if (bSaveChanged)
	{
		NString str = ((NEditCtrl*)m_pwNGraphicstrl)->GetText();
		m_pvarValue->fVal =(float) atof(str.Buffer());



		//strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());
	}

	//Destroy control
	delete m_pwNGraphicstrl;
	m_pwNGraphicstrl=null;

	return bSaveChanged;
}

void NFloatProp::BuildMenu(NTreeNode* _pnode)
{

	if (m_wndMenu.m_W32HWnd==0)
   		m_wndMenu.Create("", m_pParent);
   	else
   		m_wndMenu.DeleteAllItems();

   	//Parse Alls Pages to add 'NStoreOp'
   	NObjectArray& arrayObjs = _pnode->GetObjsArray();
   	udword dwCount = arrayObjs.Count();
   	udword idx=0;
   	while (dwCount--)
   	{
   		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[idx++];

   		NMenuCtrl* popMenu = m_wndMenu.CreatePopupMenu(ppage->GetName(), -1);

   		NObjectArray storedOp;
   		storedOp.SetManageDelete(false);
   		ppage->GetOpsFromClassName("NFloatVarOp", storedOp);
		ppage->GetOpsFromClassName("NUWordVarOp", storedOp);
		ppage->GetOpsFromClassName("NLoadVarOp", storedOp);
		ppage->GetOpsFromClassName("NVarsLogOp", storedOp);
		ppage->GetOpsFromClassName("NVarsPowerOp", storedOp);
		ppage->GetOpsFromClassName("NVarsMultiplyOp", storedOp);
		ppage->GetOpsFromClassName("NVarsDivideOp", storedOp);
		ppage->GetOpsFromClassName("NVarsSubtractOp", storedOp);


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
IMPLEMENT_CLASS(NUFloatProp, NPropertyItem);

void NUFloatProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	m_strValue.Format("%0.3f", m_pvarValue->fVal);
	pdc->DrawText(m_strValue.Buffer(), rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
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
//NGradientDialogProp
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NColorProp, NPropertyItem);

void NColorProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	RGBA val;
	val.dwCol = m_pvarValue->dwVal;

	///////////////////////////////////////
	//Display RGB Values
	NString cstr;
	NRect rc(rcItem);
	udword w = rc.Width()/5;

	//Red
	rc.right = rc.left + w;
	cstr.Format("%d", val.r);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//Green
	rc.Move(w,0);
	cstr.Format("%d",  val.g);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//Blue
	rc.Move(w,0);
	cstr.Format("%d",  val.b);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//Alpha
	rc.Move(w,0);
	cstr.Format("%d",  val.a);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	///////////////////////////////////////
	//Display Color Rect
	COLORREF col = RGB( val.r,  val.g,  val.b);
	rc.left = rc.right;
	rc.right = rcItem.right;
	pdc->FillSolidRect(rc, col);

}


bool NColorProp::BeginEdit(NRect& rcItem)
{
	//Color Picker
	NColorDialog dlg;
	dlg.Create("Choose a Color", m_pParent, NColor(m_pvarValue->dwVal));
	if (1==dlg.DoModal())
	{
		m_pvarValue->dwVal = dlg.GetColor().GetARGB();
	}

	return true;	//End of Edition


}

bool NColorProp::EndEdit(bool bSaveChanged)
{
	return true;
}

bool NColorProp::AddValue(sdword dwDelta)
{
	if (m_dwRGBEditingIdx<4)
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

#warning Unsure
		m_wndMenu.Create("", m_pParent);
   		for (udword i=0; i<m_carrayStringsList.Count(); i++)
   		{
   			m_wndMenu.AddItem(m_carrayStringsList[i].Buffer(), i+1, 0);
   		}
	}

	//Draw
	ubyte val = m_pvarValue->byVal;


	if (val<(ubyte)m_carrayStringsList.Count())
		m_strValue.Format("%s", 	m_carrayStringsList[val].Buffer());
	else
		m_strValue="?";

	pdc->DrawText(m_strValue.Buffer(), rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}

bool NUbyteComboProp::MenuEdit	(NRect& rcItem)
{
	assert(m_pParent!=null);

	//Menu
	if (m_wndMenu.m_W32HWnd==0)
	{
		m_wndMenu.Create("", m_pParent);
		for (udword i=0; i<m_carrayStringsList.Count(); i++)
		{
			m_wndMenu.AddItem(m_carrayStringsList[i].Buffer(), i+1, 0);
		}
	}

 	assert(m_pParent!=null);

	NPoint pt(rcItem.left, rcItem.bottom);
	m_pParent->ClientToScreen(pt);
	ubyte byVal = (ubyte)m_wndMenu.TrackPopupMenu(pt, null, true);

   	if (byVal!=0)
   		m_pvarValue->byVal = byVal-1;

	return true;

}

bool NUbyteComboProp::BeginEdit(NRect& rcItem)
{
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

   	//Creation du menu
   	NTreeNode* pnode = NEngineOp::GetEngine()->GetRootGroup();
   	BuildMenu(pnode);

   	NPoint pt(rcItem.left, rcItem.bottom);
   	m_pParent->ClientToScreen(pt);
   	NOperator* popSel = (NOperator*)m_wndMenu.TrackPopupMenu(pt, null, true);

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

if (m_wndMenu.m_W32HWnd==0)
   		m_wndMenu.Create("", m_pParent);
   	else
   		m_wndMenu.DeleteAllItems();

   	//Parse Alls Pages to add 'NStoreOp'
   	NObjectArray& arrayObjs = _pnode->GetObjsArray();
   	udword dwCount = arrayObjs.Count();
   	udword idx=0;
   	while (dwCount--)
   	{
   		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[idx++];

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
	//###TODO###
	//Parse Alls Pages to add 'NStoreOp'
/*	NObjectArray& arrayObjs = _pnode->GetObjsArray();
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
*/
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

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NGradientDialogProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NGradientDialogProp, NPropertyItem);


NGradientDlg grad_dlg(IDD_GRADIENT, 0);

NGradientDialogProp::NGradientDialogProp()
{
}

NGradientDialogProp::~NGradientDialogProp()
{
	//if( pArray )
	//	delete pArray;

	//delete grad_dlg;
}

void NGradientDialogProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	RGBA val;
	val.dwCol = m_pvarValue->dwVal;

	///////////////////////////////////////
	//Display RGB Values
	NString cstr;
	NRect rc(rcItem);
	udword w = rc.Width()/4;

	//Red
	rc.right = rc.left + w;
	cstr.Format("%d", val.r);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	///////////////////////////////////////
	//Display Color Rect
	COLORREF col = RGB( val.r,  val.g,  val.b);
	rc.left = rc.right;
	rc.right = rcItem.right;
	pdc->FillSolidRect(rc, col);

}


bool NGradientDialogProp::BeginEdit(NRect& rcItem)
{

	grad_dlg.m_elems.Clear();
	grad_dlg.SetName( "Gradient Dialog..." );
	if (grad_dlg.DoModal())
	{
		m_pvarValue->pcRefObj = (NObject*)&grad_dlg.m_elems;
	}
	/*else
	{
		m_pvarValue->pcRefObj = (NObject*)&grad_dlg.m_elems;
	}*/

	return true;	//End of Edition
}

bool NGradientDialogProp::EndEdit(bool bSaveChanged)
{
	return true;
}

bool NGradientDialogProp::AddValue(sdword dwDelta)
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

void NGradientDialogProp::Click(NPoint& pt, NRect& rcItem)
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
//										NCurveDialogProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NCurveDialogProp, NPropertyItem);


NCurveDlg curve_dlg(IDD_CURVE, 0);

NCurveDialogProp::NCurveDialogProp()
{
}

NCurveDialogProp::~NCurveDialogProp()
{
}

void NCurveDialogProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	RGBA val;
	val.dwCol = m_pvarValue->dwVal;

	///////////////////////////////////////
	//Display RGB Values
	NString cstr;
	NRect rc(rcItem);
	udword w = rc.Width()/4;

	//Red
	rc.right = rc.left + w;
	cstr.Format("%d", val.r);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	///////////////////////////////////////
	//Display Color Rect
	COLORREF col = RGB( val.r,  val.g,  val.b);
	rc.left = rc.right;
	rc.right = rcItem.right;
	pdc->FillSolidRect(rc, col);

}


bool NCurveDialogProp::BeginEdit(NRect& rcItem)
{
	curve_dlg.m_curveElemArray.Clear();
	curve_dlg.SetName( "Curve Dialog..." );

	if (curve_dlg.DoModal())
	{
		m_pvarValue->pcRefObj = (NObject*)&curve_dlg.m_curveElemArray;
	}

	return true;	//End of Edition
}

bool NCurveDialogProp::EndEdit(bool bSaveChanged)
{
	return true;
}

bool NCurveDialogProp::AddValue(sdword dwDelta)
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

void NCurveDialogProp::Click(NPoint& pt, NRect& rcItem)
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
//										NTerraceDialogProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NTerraceDialogProp, NPropertyItem);


NTerraceDlg terrace_dlg(IDD_TERRACE, 0);

NTerraceDialogProp::NTerraceDialogProp()
{
}

NTerraceDialogProp::~NTerraceDialogProp()
{
}

void NTerraceDialogProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	RGBA val;
	val.dwCol = m_pvarValue->dwVal;

	///////////////////////////////////////
	//Display RGB Values
	NString cstr;
	NRect rc(rcItem);
	udword w = rc.Width()/4;

	//Red
	rc.right = rc.left + w;
	cstr.Format("%d", val.r);
	pdc->DrawText(cstr.Buffer(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	///////////////////////////////////////
	//Display Color Rect
	COLORREF col = RGB( val.r,  val.g,  val.b);
	rc.left = rc.right;
	rc.right = rcItem.right;
	pdc->FillSolidRect(rc, col);

}


bool NTerraceDialogProp::BeginEdit(NRect& rcItem)
{
	terrace_dlg.m_curveElemArray.Clear();
	terrace_dlg.SetName( "Terrace Dialog..." );

	if (terrace_dlg.DoModal())
	{
		m_pvarValue->pcRefObj = (NObject*)&terrace_dlg.m_curveElemArray;
	}

	return true;	//End of Edition
}

bool NTerraceDialogProp::EndEdit(bool bSaveChanged)
{
	return true;
}

bool NTerraceDialogProp::AddValue(sdword dwDelta)
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

void NTerraceDialogProp::Click(NPoint& pt, NRect& rcItem)
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
//										NFloatProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVarProp, NPropertyItem);

void NVarProp::DrawItem(NGraphics* pdc, NRect& rcItem)
{
	m_strValue.Format("%f", m_pvarValue->fVal);
	pdc->DrawText(m_strValue.Buffer(), rcItem, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}


bool NVarProp::BeginEdit(NRect& rcItem)
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

bool NVarProp::MenuEdit	(NRect& rcItem)
{
   	NRect rc = rcItem;
   	rc.left = rc.right;

   	//Creation du menu
   	NTreeNode* pnode = NEngineOp::GetEngine()->GetRootGroup();
   	BuildMenu(pnode);

   	NPoint pt(rcItem.left, rcItem.bottom);
   	m_pParent->ClientToScreen(pt);
   	NOperator* popSel = (NOperator*)m_wndMenu.TrackPopupMenu(pt, null, true);

	NFloatVarOp *pOp = (NFloatVarOp*)popSel;

   	//Affect selected 'stored operator'
   	if (popSel!=null)
	{
		m_pvarValue->fVal = pOp->GetUserVal();
		//pOp->val = &m_pvarValue->fVal;
		pOp->AddFloatVar( &m_pvarValue->fVal );
		//m_pvarBloc->SetValue(0, 0.0f, pOp->GetUserVal());
	}


		/*
			float val = 0.0;
   			pop->m_pcvarsBloc->GetValue(0,0,(float&)val);

		*/
		/*float val = 0.0;
		popSel->m_pcvarsBloc->GetValue(0,0,(float&)val);
		m_pvarBloc->SetValue(0, 0.0f, val);*/
	return true;

}

bool NVarProp::EndEdit(bool bSaveChanged)
{
	if (m_pwNGraphicstrl==null)		return false;

	//Save changed
	if (bSaveChanged)
	{
		NString str = ((NEditCtrl*)m_pwNGraphicstrl)->GetText();
		m_pvarValue->fVal =(float) atof(str.Buffer());
		//strcpy_s(m_pvarValue->szVal, sizeof(m_pvarValue->szVal), str.Buffer());
	}

	//Destroy control
	delete m_pwNGraphicstrl;
	m_pwNGraphicstrl=null;

	return bSaveChanged;
}

void NVarProp::BuildMenu(NTreeNode* _pnode)
{

	if (m_wndMenu.m_W32HWnd==0)
   		m_wndMenu.Create("", m_pParent);
   	else
   		m_wndMenu.DeleteAllItems();

   	//Parse Alls Pages to add 'NStoreOp'
   	NObjectArray& arrayObjs = _pnode->GetObjsArray();
   	udword dwCount = arrayObjs.Count();
   	udword idx=0;
   	while (dwCount--)
   	{
   		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[idx++];

   		NMenuCtrl* popMenu = m_wndMenu.CreatePopupMenu(ppage->GetName(), -1);

   		NObjectArray storedOp;
   		storedOp.SetManageDelete(false);
   		ppage->GetOpsFromClassName("NFloatVarOp", storedOp);

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

bool NVarProp::AddValue(sdword dwDelta)
{
	m_pvarValue->fVal = m_pvarValue->fVal + ((float)dwDelta)/1000.0f;
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NUseStoredOpsProp class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NLoadLibnoiseCache, NPropertyItem);

void NLoadLibnoiseCache::DrawItem(NGraphics* pdc, NRect& rcItem)
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

bool NLoadLibnoiseCache::BeginEdit(NRect& rcItem)
{
   	NRect rc = rcItem;
   	rc.left = rc.right;

   	//Creation du menu
   	NTreeNode* pnode = NEngineOp::GetEngine()->GetRootGroup();
   	BuildMenu(pnode);

   	NPoint pt(rcItem.left, rcItem.bottom);
   	m_pParent->ClientToScreen(pt);
   	NOperator* popSel = (NOperator*)m_wndMenu.TrackPopupMenu(pt, null, true);

   	//Affect selected 'stored operator'
   	if (popSel!=null)
   		m_pvarBloc->SetValue(m_dwvarIdx, 0.0f, popSel);

	return true;
}

bool NLoadLibnoiseCache::EndEdit(bool bSaveChanged)
{
	return true;
}

void NLoadLibnoiseCache::BuildMenu(NTreeNode* _pnode)
{

if (m_wndMenu.m_W32HWnd==0)
   		m_wndMenu.Create("", m_pParent);
   	else
   		m_wndMenu.DeleteAllItems();

   	//Parse Alls Pages to add 'NStoreOp'
   	NObjectArray& arrayObjs = _pnode->GetObjsArray();
   	udword dwCount = arrayObjs.Count();
   	udword idx=0;
   	while (dwCount--)
   	{
   		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[idx++];

   		NMenuCtrl* popMenu = m_wndMenu.CreatePopupMenu(ppage->GetName(), -1);

   		NObjectArray storedOp;
   		storedOp.SetManageDelete(false);
   		ppage->GetOpsFromClassName("NCache", storedOp);

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
	//###TODO###
	//Parse Alls Pages to add 'NStoreOp'
/*	NObjectArray& arrayObjs = _pnode->GetObjsArray();
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
*/
}
