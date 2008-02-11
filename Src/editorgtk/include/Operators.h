//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Operators.h
//! \brief	FxGen GTK editor - operator handling framework header
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

#ifndef OPERATORS_H
#define OPERATORS_H

#include "fxgen_pch.h"

class OperatorsLayout;

class OperatorWidget : public Gtk::EventBox
{
    friend class OperatorsLayout;
    friend class PropertyTable;
	// no explicit instantiation
    OperatorWidget(NOperator*);
	// mouse signal handlers
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    bool on_motion_notify_event(GdkEventMotion* event);
    void update_label();
    // widget layout
    Gtk::VBox vbox;
    Gtk::Label label;
    Gtk::Frame frame;
    // internals
    NOperator* op;
	// related objects
    OperatorsLayout* parent;
    void UpdateImage();
    static OperatorWidget* active_op; // this will be executed on any changes on a page
    static OperatorWidget* current_op; // currently modified operator
//    static vector<OperatorWidget*> selected_ops;
    int x0, y0; // initial click point on the operator (operator movement is relative to this point)
    int xi; // operator initial width (for collisions resolving)
    bool resizable, moved;
};

class OperatorsLayout : public Gtk::EventBox
{
    friend class OperatorWidget;
public:
    OperatorsLayout(GtkEventBox* cobject, const RefPtr<Xml>& refGlade);
    void AddOperator(Glib::ustring, Glib::ustring);
    void add(NOperator* op, int x, int y, int w);
    NOperatorsPage*page;
    bool release(OperatorWidget&);
    void clear();
#ifndef THREADS_SUCK
    bool kill_thread;
    std::map<NOperator*, pair<int,int> > updates;
    pthread_mutex_t updates_mutex;
    ~OperatorsLayout();
#endif
protected:
    std::map<OperatorWidget*,pair<int,int> >operators;
    void put(OperatorWidget&, int, int, int);
    void move(OperatorWidget&, int, int);
    bool on_button_press_event(GdkEventButton* event);
    const int grid_size;
    Gtk::Fixed fixed;
#ifndef THREADS_SUCK
    pthread_t tid;
#endif
};

#endif // OPERATORS_H
