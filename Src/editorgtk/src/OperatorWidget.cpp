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
#define RESIZE_HANDLE .15

/** Create a web color string from a windows dword color value
\param number An ARGB color value
\return A string of the form #rrggbb
*/
Glib::ustring hex_color(int number)
{
    const char digits[] = "0123456789abcdef";
    char retval[8];
    retval[0]='#';
    retval[7]='\0';

    for (int i = 1; i<7; i+=2)
    {
        int msd = number & 0xf;
        number >>= 4;
        int lsd = number & 0xf;
        retval[i] = digits[lsd];
        retval[i+1] = digits[msd];
        number >>= 4;
    }

    return Glib::ustring(retval);
}

// static members
OperatorWidget* OperatorWidget::active_op(NULL);
OperatorWidget* OperatorWidget::current_op(NULL);
//vector<OperatorWidget*> OperatorWidget::selected_ops;


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


OperatorWidget::OperatorWidget(NOperator* _op)
        :op(_op), parent(NULL), x0(0), y0(0), xi(0), resizable(false), moved(true)
{
    add(frame);
    frame.set_shadow_type(Gtk::SHADOW_OUT);
    frame.add(vbox);
    update_label();

    modify_bg(Gtk::STATE_NORMAL, Gdk::Color(hex_color(op->GetColor())));
    vbox.pack_start(label, false, false, 0);
    //ctor
}

/// control operator movement
bool OperatorWidget::on_motion_notify_event(GdkEventMotion* event)
{
    parent = dynamic_cast<OperatorsLayout*>(get_parent()->get_parent());
    if (parent && event->state&GDK_BUTTON1_MASK)
    {
        if (event->x > (1-RESIZE_HANDLE) * get_width() && resizable) // resize: todo set the constant to an appropriate value
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

/// notify the layout to align the operator to its grid
bool OperatorWidget::on_button_release_event(GdkEventButton* event)
{
    parent = dynamic_cast<OperatorsLayout*>(get_parent()->get_parent());
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

/// activate the operator
bool OperatorWidget::on_button_press_event(GdkEventButton* event)
{
    current_op = this;
    if(op)
		property_table->DisplayOperatorProperties(this);
    if (event->type == GDK_2BUTTON_PRESS)
    {
        if (resizable && event->x >= get_width() * (1-RESIZE_HANDLE))
        {
            set_size_request(-1, get_height());
            parent->release(*this);
        }
        else
        {
            if (active_op)
            {
                active_op->set_state(Gtk::STATE_NORMAL);
                active_op->frame.set_shadow_type(Gtk::SHADOW_OUT);
            }
            set_state(Gtk::STATE_ACTIVE);
            frame.set_shadow_type(Gtk::SHADOW_IN);
            active_op = this;
            UpdateImage();
        }
    }

    return true;
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
        engine->Execute(0.f/*time*/, op, 1.0f/*detail*/, NULL/*FXGEN_OPSPROCESSCB* _cbOpsProcess*/);
        NBitmap* bitmap = dynamic_cast<NBitmap*>(op->m_pObj);
        if (bitmap)
            image->SetBitmap(bitmap);
    }
}
