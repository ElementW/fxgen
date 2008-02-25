//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorWidget.cpp
//! \brief	FxGen GTK editor - a widget class for operators
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
#define RESIZE_HANDLE .1


// static members
OperatorWidget* OperatorWidget::active_op(NULL);
OperatorWidget* OperatorWidget::preview_op(NULL);
map<OperatorWidget*, void*> OperatorWidget::ops_group;
vector<NObject*> OperatorWidget::clipboard;
float OperatorWidget::detail(1.0);


OperatorWidget::OperatorWidget(NOperator* _op)
        :state(STATE_NORMAL), op(_op), parent(NULL), x0(0), y0(0), xi(0), resizable(false), moved(true)
{
    add(frame);
    frame.set_shadow_type(Gtk::SHADOW_NONE);
    frame.add(vbox);
    update_label();

	set_state(STATE_NORMAL);
    vbox.pack_start(label, false, false, 0);

    context_menu.add("", "Copy", &OperatorWidget::copy_clipboard);
    context_menu.add("", "Cut", &OperatorWidget::cut_clipboard);
    context_menu.add("", "Delete", &OperatorWidget::delete_ops);
    context_menu.show_all_children();
    //ctor
}

bool OperatorWidget::on_button_press_event(GdkEventButton* event)
{
    if (event->button == 3) // popup menu
    {
        activate();
        context_menu.popup(event->button, event->time);
    }
    else if (event->state & Gdk::CONTROL_MASK) // multiple selection
    {
        if (ops_group.find(this) != ops_group.end())
        {
            ops_group.erase(this);
			set_state(STATE_SELECTED, false);
            if (active_op == this)
            {
                active_op = NULL;
                property_table->clear();
            }
        }
        else
        {
            ops_group[this] = NULL; // no data, we only make use of the tree storage
            set_state(STATE_SELECTED);
        }
    }
    else if (event->state & Gdk::SHIFT_MASK) // block selection
    {
        if (ops_group.find(this) == ops_group.end())
        {
			parent = dynamic_cast<OperatorsLayout*>(get_parent());
			if (parent)
			{
				std::map<OperatorWidget*,pair<int,int> >::iterator i;
				for(i = parent->operators.begin(); i != parent->operators.end(); i++)
				{
					;
				}
				std::map<OperatorWidget*,void*>::iterator j;
			}
        }
    }
    else
    {
        activate();
        if (event->type == GDK_2BUTTON_PRESS) // process
        {
            if (resizable && event->x >= get_width() * (1-RESIZE_HANDLE))
            {
                set_size_request(-1, get_height());
                parent->release(*this);
            }
            else
            {
                if (preview_op)
                    preview_op->set_state(STATE_PREVIEW, false);
                set_state(STATE_PREVIEW);
                preview_op = this;
                UpdateImage();
            }
        }
    }

    return true;
}

/// notify the layout to align the operator to its grid
bool OperatorWidget::on_button_release_event(GdkEventButton* event)
{
    parent = dynamic_cast<OperatorsLayout*>(get_parent());
    if (parent)
        if (moved)
            while (!parent->release(*this))
                parent->move(*this, 0, parent->grid_size);
        else if (!parent->release(*this))
            set_size_request(xi ? xi : parent->grid_size, parent->grid_size);

    x0 = y0 = xi = 0;
    moved = true; // default

    return true;
}

/// control operator movement
bool OperatorWidget::on_motion_notify_event(GdkEventMotion* event)
{
    parent = dynamic_cast<OperatorsLayout*>(get_parent());
    if (parent && event->state&GDK_BUTTON1_MASK)
    {
        if (event->x > (1-RESIZE_HANDLE) * get_width() && resizable && ops_group.empty()) // resize: todo set the constant to an appropriate value
        {
            if (xi == 0) // initial size backup (for collisions)
                xi = get_width();
            set_size_request((int)event->x, get_height());
            moved = false;
        }
        else // move
        {
            if (!x0) x0 = (int)event->x;
            if (!y0) y0 = (int)event->y;
            parent->move(*this, (int)event->x-x0, (int)event->y-y0);
            moved = true;
        }
    }
    return false;
}

/// activate the operator
void OperatorWidget::activate()
{
    if (active_op != this)
    {
        if (ops_group.find(this) == ops_group.end())
            clear_selection();
        if (active_op)
            active_op->set_state(STATE_ACTIVE, false);
        active_op = this;
        ops_group[this] = NULL; // no data, we only make use of the tree storage
		set_state(STATE_ACTIVE);
    }

    if (op)
        property_table->DisplayOperatorProperties(this);
}

/// Keep the operator's label valid despite of where it's coming from
void OperatorWidget::update_label()
{
    if (!op)
        return;
    const char* stored_name = op->GetUserName();
    if (stored_name && *stored_name)
        label.set_text(stored_name);
    else
        label.set_text(op->GetName());
}

/** update the operator's visible data
\todo Efficiency of this process is subject to investigation
*/
void OperatorWidget::UpdateImage()
{
    update_label();
    NEngineOp* engine = NEngineOp::GetEngine();
    if (engine)
    {
        engine->Execute(0.f/*time*/, op, detail, NULL/*FXGEN_OPSPROCESSCB* _cbOpsProcess*/);
        NBitmap* bitmap = dynamic_cast<NBitmap*>(op->m_pObj);
        if (bitmap)
            image->SetBitmap(bitmap);
    }
}

/// copy selected operators to the clipboard
void OperatorWidget::copy_clipboard()
{
	if(ops_group.size())
        clipboard.clear();
	std::map<OperatorWidget*,void*>::iterator i;
	for(i=ops_group.begin(); i!= ops_group.end(); i++)
	{
        NObject* op = NRTClass::CreateByName(((string("N")+i->first->op->GetName())+"Op").c_str());
        if (op)
        {
            *op = (NObject&)*i->first;
            clipboard.push_back(op);
        }
    }
}

/// copy selected operators to the clipboard and remove them from the layout
void OperatorWidget::cut_clipboard()
{
    copy_clipboard();
    delete_ops();
}

/** operator widget must commit this function due to no parent during construction, which could remove() it
\fixme Add a callback for the realize signal, set parent there.
*/
void OperatorWidget::suicide()
{
    parent = dynamic_cast<OperatorsLayout*>(get_parent());
    if (parent)
        parent->remove(*this);
    if (preview_op == this)
    {
        preview_op = NULL;
        image->clear();
    }
    if (active_op == this)
    {
        active_op = NULL;
        property_table->clear();
    }
    if(ops_group.find(this) != ops_group.end())
		ops_group.erase(this);
}

/// remove selected operators from the layout
void OperatorWidget::delete_ops()
{
	std::map<OperatorWidget*,void*>::iterator i;

	for(i=ops_group.begin(); i!= ops_group.end(); i++)
        i->first->suicide();

}

/// clear static data
void OperatorWidget::cleanup()
{
    active_op = preview_op = NULL;
    clear_selection();
}

void OperatorWidget::clear_selection()
{
    std::map<OperatorWidget*,void*>::iterator i;

    for (i = ops_group.begin(); i != ops_group.end(); i++)
    {
        if (i->first != preview_op)
            i->first->set_state(STATE_SELECTED, false);
    }

    ops_group.clear();
}

void OperatorWidget::set_state(StateType _state, bool set_true)
{
//	Gtk::MessageDialog(lexical_cast<string>(_state)).run();
	if(set_true)
	{
		state |= _state;
		if(_state == STATE_ACTIVE)
			state |= STATE_SELECTED;
	}
	else
	{
		state &= ~_state;
		if(_state == STATE_SELECTED)
			state &= ~STATE_ACTIVE;
	}


    if (state&STATE_ACTIVE)
        modify_bg_pixmap(Gtk::STATE_NORMAL, "operator-active-center.png");
    else
        modify_bg_pixmap(Gtk::STATE_NORMAL, "operator-inactive-center.png");

	Gdk::Color color[4];
	color[STATE_NORMAL] = color[STATE_SELECTED] = color[STATE_PREVIEW] = color[STATE_PREVIEW_SELECTED] = Gdk::Color(hex_color(op->GetColor()));

	color[STATE_SELECTED].set_red(min(0xffff, color[STATE_SELECTED].get_red() * 1.3));
	color[STATE_SELECTED].set_green(min(0xffff, color[STATE_SELECTED].get_green() * 1.3));
	color[STATE_SELECTED].set_blue(min(0xffff, color[STATE_SELECTED].get_blue() * 1.3));
    color[STATE_PREVIEW].set_grey((color[STATE_PREVIEW].get_red() + color[STATE_PREVIEW].get_green() + color[STATE_PREVIEW].get_blue())/ 3);
    color[STATE_PREVIEW_SELECTED].set_grey(min(0xffff,(color[STATE_PREVIEW_SELECTED].get_red() + color[STATE_PREVIEW_SELECTED].get_green() + color[STATE_PREVIEW_SELECTED].get_blue())/ 2.4));

	modify_bg(Gtk::STATE_NORMAL, color[state&~STATE_ACTIVE]);
}
