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

#ifndef THREADS_SUCK
void *page_move_op(void* arg)
{
	OperatorsLayout* layout = (OperatorsLayout*)arg;
	while(!layout->kill_thread)
	{
		if(layout->updates.size())
		{
			pthread_mutex_lock(&layout->updates_mutex);
			std::map<NOperator*, pair<int,int> >::iterator i = layout->updates.begin();
			pair<int,int> p = i->second;
			NOperator* op = i->first;
			layout->updates.erase(i);
			pthread_mutex_unlock(&layout->updates_mutex);
			layout->page->MoveOp(op, p.first, p.second);
		}
		else
			usleep(100);
	}
	pthread_exit((void *) 0);
}

OperatorsLayout::~OperatorsLayout()
{
	kill_thread = true;
	pthread_join(tid, NULL);
}
#endif

OperatorsLayout::OperatorsLayout(GtkEventBox* cobject, const RefPtr<Xml>& refGlade)
        :Gtk::EventBox(cobject), page(NULL), grid_size(20)
{
	EventBox::add(fixed);
#ifndef THREADS_SUCK
	pthread_mutex_init(&updates_mutex, NULL);
	pthread_create(&tid, NULL, page_move_op, this);
#endif
}

/// place an operator, coords and width in pixels
void OperatorsLayout::put(OperatorWidget&widget, int x, int y, int w)
{
    operators[&widget].first = x;
    operators[&widget].second = y;
    widget.set_size_request(w, grid_size);
    widget.resizable = true;
    fixed.put(widget, x, y);
    show_all_children();
}

/// add an operator, coords and width in grid units
void OperatorsLayout::add(NOperator* op, int x, int y, int w)
{
    OperatorWidget* widget = Gtk::manage(new OperatorWidget(op));
    put(*widget, grid_size * x, grid_size * y, grid_size * w);
}

/// move an operator, coords in pixels
void OperatorsLayout::move(OperatorWidget&widget, int x, int y)
{
    // add relative offsets
    operators[&widget].first += x;
    operators[&widget].second += y;
    fixed.move(widget, operators[&widget].first, operators[&widget].second);
}

/// align an operator placement and width into grid units and detect collisions
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

    fixed.move(widget, x, y);
    widget.set_size_request(width_request, grid_size);
    // store
    operators[&widget].first = x;
    operators[&widget].second = y;

    NOperator* op = dynamic_cast<NOperator*>(widget.op);
    if (op)
    {
        op->m_wWidth = width_request / grid_size;
        if (page)
        {
#ifdef THREADS_SUCK
            page->MoveOp(op, x / grid_size, y / grid_size);
#else
			pthread_mutex_lock(&updates_mutex);
        	updates[op] = pair<int,int>(x / grid_size, y / grid_size);
			pthread_mutex_unlock(&updates_mutex);
#endif
        }
		else
			Gtk::MessageDialog("Error: No operatorspage.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CANCEL, true).run();
        if(OperatorWidget::active_op)
			OperatorWidget::active_op->UpdateImage();
    }

	project_modified = true;
    return true;
}

void OperatorsLayout::clear()
{
	std::map<OperatorWidget*,pair<int,int> >::iterator iter;
	for(iter = operators.begin(); iter != operators.end(); iter++)
	{
		fixed.remove(*iter->first);
	}
	operators.clear();
}

void OperatorsLayout::AddOperator(Glib::ustring OpGroup, Glib::ustring OpName)
{
	;
}

bool OperatorsLayout::on_button_press_event(GdkEventButton* event)
{
    if (event->button == 3)
    {
    	Gtk::Menu* menu = dynamic_cast<Gtk::Menu*>(dynamic_menu->uim->get_widget("/InsertPopup"));
    	if(menu)
    	{
			menu->popup(event->button, event->time);
			Gtk::MessageDialog("Kuku a!").run();
    	}
    	else
			Gtk::MessageDialog(lexical_cast<string>(event->button)).run();
	}

	return false;
}
