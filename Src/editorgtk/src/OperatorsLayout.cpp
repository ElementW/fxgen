//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorLayout.cpp
//! \brief	FxGen GTK editor - layout class for operators
//!
//!	\author	Sebastian Olter (qduaty@gmail.com)
//!	\date		07-02-2008
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

#include "pch.h"
#include "globals.h"

//-----------------------------------------------------------------
//!	\brief	Return First RTClass from a super class name
//!	\author	Johann Nadalutti (fxgen@free.fr)
//-----------------------------------------------------------------
NRTClass* GetFirstClassBySuperClass(char* _pszSuperClassName)
{
	NRTClass* pcurRTC = NRTClass::m_pFirstRTClass;
	while (pcurRTC!=null)
	{
		if (strcmp(pcurRTC->m_pszSuperClassName, _pszSuperClassName) == 0)
			return pcurRTC;
		pcurRTC = pcurRTC->m_pNextRTC;
	}

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Return Next RTClass from a super class name
//!	\author	Johann Nadalutti (fxgen@free.fr)
//-----------------------------------------------------------------
NRTClass* GetNextClassBySuperClass(char* _pszSuperClassName, NRTClass* _prtclass)
{
	_prtclass = _prtclass->m_pNextRTC;

	while (_prtclass!=null)
	{
		if (strcmp(_prtclass->m_pszSuperClassName, _pszSuperClassName) == 0)
			return _prtclass;

		_prtclass = _prtclass->m_pNextRTC;
	}

	return null;
}

/// thread routine for operators' updating
void OperatorsLayout::update_op()
{
	while(!kill_thread)
	{
		if(need_update)
		{
			need_update = false;
			NEngineOp* engine = NEngineOp::GetEngine();
			if (engine)
			{
				if(OperatorWidget::preview_op)
					engine->Execute(0.f/*time*/, OperatorWidget::preview_op->op, 1.0f/*detail*/, NULL/*FXGEN_OPSPROCESSCB* _cbOpsProcess*/);
				image->queue_draw();
			}
		}
#ifdef _WIN32
			Sleep(1);
#else
			usleep(1);
#endif
	}
}

OperatorsLayout::~OperatorsLayout()
{
	kill_thread = true;
	tid->join();
}

OperatorsLayout::OperatorsLayout(GtkFixed* cobject, const RefPtr<Xml>& refGlade)
        :Gtk::Fixed(cobject), page(NULL), grid_size(20), last_x(0), last_y(0), need_update(false)
{
	set_has_window();
	modify_bg_pixmap(Gtk::STATE_NORMAL, "operator-field-texture.png");

	tid = Glib::Thread::create(mem_fun(this, &OperatorsLayout::update_op), true);
	tid->set_priority(Glib::THREAD_PRIORITY_LOW);

	// context menu initialization - the idea is taken from NOperatorsWnd::InitCtxMenu()
	NRTClass* prtc;
	for (prtc = GetFirstClassBySuperClass("NOperator"); prtc; prtc = GetNextClassBySuperClass("NOperator", prtc))
	{
		NOperator* pop = (NOperator*)prtc->m_pCreateCB();
		Glib::ustring category = pop->GetCategory();
		Glib::ustring name = pop->GetName();
		context_menu.add(category, name, bind(mem_fun(this, &OperatorsLayout::add_new),prtc->CLASSID));
	}
    context_menu.add("", "Paste", mem_fun(this, &OperatorsLayout::paste_clipboard));
   	context_menu.show_all_children();
}

/// place the operator, coords and width in pixels
void OperatorsLayout::put(OperatorWidget&widget)
{
    operators[&widget].first = widget.op->m_wPosX * grid_size;
    operators[&widget].second = widget.op->m_wPosY * grid_size;
    widget.set_size_request(widget.op->m_wWidth * grid_size, grid_size);
    widget.resizable = true;
    Gtk::Fixed::put(widget, operators[&widget].first, operators[&widget].second);
	show_all_children();

}

/// add an operator
void OperatorsLayout::add(NOperator* op)
{
    OperatorWidget* widget = Gtk::manage(new OperatorWidget(op));
    put(*widget);
}

/// move the operator, coords in pixels
void OperatorsLayout::move(OperatorWidget&widget, int x, int y)
{
    // add relative offsets
    operators[&widget].first += x;
    operators[&widget].second += y;
    Gtk::Fixed::move(widget, operators[&widget].first, operators[&widget].second);
	window->project_modified = true;
}

/// align the operator placement and width into grid units and detect collisions
bool OperatorsLayout::release(OperatorWidget& widget)
{
    int x = operators[&widget].first + grid_size / 2;
    int y = operators[&widget].second + grid_size / 2;
    // round down
    x -= x%grid_size;
    y -= y%grid_size;
    // align
    int width_request = widget.get_width() + grid_size / 2;
    width_request -= width_request%grid_size;
    // collision detection
    std::map<OperatorWidget*,pair<int,int> >::const_iterator i;
    for (i = operators.begin(); i != operators.end(); i++)
    {
        if ((i->first != &widget) && (i->second.second == y))
        {
            int x0 = x;
            int x1 = x + width_request;
            int x2 = i->second.first;
            int x3 = i->second.first + i->first->get_width();
            if ((x0>x2&&x0<x3)||(x1>x2&&x1<x3)||(x2>x0&&x2<x1)||(x3>x0&&x3<x1)||(x0==x0&&x1==x3))
                return false;
        }
    }

    Gtk::Fixed::move(widget, x, y);
    if(width_request) // new operators, which weren't ever seen, have it set to 0
		widget.set_size_request(width_request, grid_size);
    // store
    operators[&widget].first = x;
    operators[&widget].second = y;

    NOperator* op = dynamic_cast<NOperator*>(widget.op);
    if (op)
    {
        op->m_wWidth = width_request / grid_size;
       	update(op, x / grid_size, y / grid_size);
    }

    return true;
}

/** remove all operator widgets from the layout
\param purge if true, remove operators from the page as well
*/
void OperatorsLayout::clear(bool purge)
{
	std::map<OperatorWidget*,pair<int,int> >::iterator iter;
	for(iter = operators.begin(); iter != operators.end(); iter++)
	{
		if(purge)
			remove(*iter->first);
		else
			Gtk::Fixed::remove(*iter->first);
	}

	operators.clear();
}

/// Create a new operator of given CLASSID
void OperatorsLayout::add_new(uint32_t CLASSID)
{
	NOperator* op = dynamic_cast<NOperator*>(NRTClass::CreateByID(CLASSID));
	if(op)
	{
		op->m_wPosX = last_x / grid_size;
		op->m_wPosY = last_y / grid_size;
		op->m_wWidth = 4; // default width
		page->AddOp(op);
		add(op);
		window->project_modified = true;
	}
}

/// Popup menu request handler
bool OperatorsLayout::on_button_press_event(GdkEventButton* event)
{
	last_x = (int)event->x;
	last_y = (int)event->y;

    if (event->button == 3)
    {
   	    context_menu.get_child_by_label("Paste")->set_sensitive(!OperatorWidget::clipboard.empty());
		context_menu.popup(event->button, event->time);
    }

	return false;
}

/// Delete a widget from the project
void OperatorsLayout::remove(OperatorWidget& widget)
{
	Gtk::Fixed::remove(widget);
	std::map<OperatorWidget*,pair<int,int> >::iterator iter = operators.find(&widget);
	operators.erase(iter);
	page->DeleteOp(widget.op);
	window->project_modified = true;
}


/// Display the page selected on the list.
void OperatorsLayout::display_selected_page(Gtk::TreeView* view)
{
	if(!view)
		return;
	Gtk::TreeRow row = *view->get_selection()->get_selected();
	NOperatorsPage* tp = dynamic_cast<NOperatorsPage*>((NObject*)(void*)(row[columns_pattern.m_col_ptr]));
	if(!tp)
		return;
	clear();
	page = tp;
	NObjectArray& array = page->m_arrayOps;
	for(unsigned i = 0; i < array.Count(); i++)
	{
		NOperator* op = dynamic_cast<NOperator*>(array[i]);
		if(op)
			add(op);
	}
}

/** Request operator update in the engine.
Current implementation requests the engine to move the operator, despite of what has been actually done.
\fixme Is this correct?
*/
void OperatorsLayout::update(NOperator* op, int x, int y)
{
	if(x == -1) x = op->m_wPosX;
	if(y == -1) y = op->m_wPosY;
	if(page && (x != op->m_wPosX || y != op->m_wPosY))
		page->MoveOp(op, x, y);
	else
	{
		op->m_bInvalided = true;
		need_update = true;
	}
}

/// paste operators clipboard at given pixel coordinates
void OperatorsLayout::paste_clipboard()
{
	int size = OperatorWidget::clipboard.size();
	if(size)
	{
		OperatorWidget::ops_group.clear();
//		int x = ((NOperator*)(OperatorWidget::clipboard[0]))->m_wPosX;
//		int y = ((NOperator*)(OperatorWidget::clipboard[0]))->m_wPosY;
		for(int i = 0; i < size; i++)
		{
			add((NOperator*)OperatorWidget::clipboard[i]);
		}
	}
}
