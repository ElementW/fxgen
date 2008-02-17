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
    friend class MainWindow;
	// no explicit instantiation
    OperatorWidget(NOperator*);
    ~OperatorWidget();

	// mouse signal handlers
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    bool on_motion_notify_event(GdkEventMotion* event);
    void update_label();
    void suicide();
    void activate();

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
    static OperatorWidget* preview_op; // currently modified operator
    static vector<OperatorWidget*> ops_group; // used for selections
    static vector<NObject*> clipboard;
	static void copy_clipboard();
	static void cut_clipboard();
	static void delete_ops();
	static void cleanup();
    int x0, y0; // initial click point on the operator (operator movement is relative to this point)
    int xi; // operator initial width (for collisions resolving)
    bool resizable, moved;
    DynamicMenu context_menu;
};

class OperatorsLayout : public Gtk::Fixed
{
    std::map<OperatorWidget*,pair<int,int> >operators;
    void put(OperatorWidget&);
    void move(OperatorWidget&, int, int);
    bool on_button_press_event(GdkEventButton* event);
    DynamicMenu context_menu;
    NOperatorsPage* page;
    const int grid_size;
    // mouse click data
    int last_x, last_y;
    friend class OperatorWidget;
    // thread operation
    bool need_update;
	void update_op();
	Glib::Thread* tid;
    bool kill_thread;
public:
    OperatorsLayout(GtkFixed* cobject, const RefPtr<Xml>& refGlade);
    // page methods
    void display_selected_page(Gtk::TreeView* view);
    void clear(bool = false);
	// operators methods
    void add_new(uint32_t CLASSID);
    void add(NOperator* op);
    void remove(OperatorWidget&);
    bool release(OperatorWidget&);
    void update(NOperator*, int=-1, int=-1);
	void paste_clipboard();
    ~OperatorsLayout();
};

#endif // OPERATORS_H
