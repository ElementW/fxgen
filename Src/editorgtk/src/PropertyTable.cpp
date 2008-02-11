//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PropertyTable.cpp
//! \brief	FxGen GTK editor - operators' properties processing
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

#include <pch.h>
#include "globals.h"


/// Split an FxGen ubyte combo definition into a set of strings
vector<string> parse_fxgen_combo_string(string csv)
{
	vector<string> retval;
	string::size_type first_comma = csv.find(',');
	csv.erase(0, first_comma + 2); // get rid of the default value

	while(csv.size() > 1)
	{
		first_comma = csv.find(',');
		if(first_comma == string::npos)
			first_comma = csv.find(']');
		retval.push_back(csv.substr(0, first_comma));
		csv.erase(0, first_comma + 1);
	}

	return retval;
}

PropertyTable::PropertyTable(GtkTable*cobject, const RefPtr<Xml>& refGlade)
:Gtk::Table(cobject)
{
	//ctor
}

/** Signal callback to put properties back into their NVarValues
\param w calling widget
\param i current property id
\param op current operator widget
*/
void PropertyTable::AdjustProperty(Gtk::Widget* w, int i, OperatorWidget* op)
{
	NVarValue* val = op->op->m_pcvarsBloc->GetValues() + i;
	int type = op->op->m_pcvarsBloc->GetBlocDesc()[i].eType;

	Gtk::ColorButton* b = dynamic_cast<Gtk::ColorButton*>(w);
	if(b)
	{
		uint32_t col = 0;
		Gdk::Color c;
		c = b->get_color();
		col = (b->get_alpha() / 256) << 24;
		col += c.get_blue() / 256 << 16;
		col += (c.get_green() / 256) << 8;
		col += (c.get_red() / 256);
		val->dwVal = col;
		op->parent->release(*op);
		return;
	}

	Gtk::SpinButton* sb = dynamic_cast<Gtk::SpinButton*>(w);
	if(sb)
	{
		double num = sb->get_value();
		switch(type)
		{
			case eubyte: val->byVal = (ubyte)num; break;
			case euword: val->wVal = (uword)num; break;
			case eudword: val->dwVal = (udword)num; break;
			case efloat: val->fVal = num; break;
		}
		op->parent->release(*op);
		return;
	}

	Gtk::Range* r = dynamic_cast<Gtk::Range*>(w);
	if(r)
	{
		double num = r->get_value();
		switch(type)
		{
			case eubyte: val->byVal = (ubyte)num; break;
			case euword: val->wVal = (uword)num; break;
			case eudword: val->dwVal = (udword)num; break;
			case efloat: val->fVal = num; break;
			default:
			break;
		}
		op->parent->release(*op);
		return;
	}

	Gtk::Entry* e = dynamic_cast<Gtk::Entry*>(w);
	if(e)
	{
		e->get_text().copy(val->szVal, 255);
		val->szVal[min(255,e->get_text_length())] = '\0';
		op->update_label();
		op->parent->release(*op);
		return;
	}

	Gtk::ComboBox* cb = dynamic_cast<Gtk::ComboBox*>(w);
	if(cb)
	{
		val->byVal = cb->get_active_row_number();
		op->parent->release(*op);
		return;
	}
}

/// Find out how properties should be displayed and connect the signal callback to each of them
void PropertyTable::DisplayOperatorProperties(OperatorWidget* op)
{
	// init
	children().clear();
	NVarsBloc* bloc = op->op->m_pcvarsBloc;
	if(!bloc) return; // operators that don't have a bloc, don't need to be processed here
	NVarsBlocDesc* desc = bloc->GetBlocDesc();
	NVarValue* values = bloc->GetValues();

	// create and attach widgets
	resize(bloc->Count() + 1, 2);
	attach(*Gtk::manage(new Gtk::Label(op->op->GetName())), 0, 2, 0, 1); // set title

	// iterate over properties
	for(unsigned i = 0; i < bloc->Count(); i++)
	{
		// property name
		attach(*Gtk::manage(new Gtk::Label(desc[i].pszName)), 0, 1, i+1, i+2);

		Gtk::Widget* display_item = NULL;
		double f = 0;
		uint32_t n = 0;
		void *pointer = NULL;

		// retrieve data value - they generally fit into three types
		switch(desc[i].eType)
		{
			case eubyte: n = values[i].byVal; break;
			case euword: n = values[i].wVal; break;
			case eudword: n = values[i].dwVal; break;
			case efloat: f = values[i].fVal; break;
			case estring: pointer = values[i].szVal; break;
			case erefobj: pointer = values[i].pcRefObj; break;
		}

		// look for known control names; they can be followed by optional data such as numeric limits
		string ref = desc[i].pszCLASSGUI;

		if(ref.find("NColorProp") == 0)
		{
			Gtk::ColorButton* cb = Gtk::manage(new Gtk::ColorButton(Gdk::Color(hex_color(n))));
			cb->set_use_alpha();
			cb->set_alpha(((n>>24)&0xff)<<8);
			display_item = cb;
			cb->signal_color_set().connect(bind(bind(bind(mem_fun(*this,&PropertyTable::AdjustProperty),op), i),display_item));
		}
		else if(ref.find("NFloatProp") == 0)
		{
			Gtk::SpinButton* sb = Gtk::manage(new Gtk::SpinButton(.02, 3));
			sb->set_range(-1e6,1e6);
			sb->set_increments(.001, .01);
			sb->set_value(f);
			display_item = sb;
			sb->signal_changed().connect(bind(bind(bind(mem_fun(*this,&PropertyTable::AdjustProperty),op), i),display_item));
		}
		else if(ref.find("NUwordProp") == 0)
		{
			Gtk::SpinButton* sb = Gtk::manage(new Gtk::SpinButton(1, 0));
			sb->set_range(0,65535);
			sb->set_increments(1, 16);
			sb->set_value(n);
			display_item = sb;
			sb->signal_changed().connect(bind(bind(bind(mem_fun(*this,&PropertyTable::AdjustProperty),op), i),display_item));
		}
		else if(ref.find("NUbyteProp") == 0)
		{
			Gtk::HScale* hs = Gtk::manage(new Gtk::HScale(0, 256, 1));
			hs->set_value(n);
			display_item = hs;
			hs->signal_value_changed().connect(bind(bind(bind(mem_fun(*this,&PropertyTable::AdjustProperty),op), i),display_item));
		}
		else if(ref.find("NUbyteComboProp") == 0)
		{
			Gtk::ComboBoxText* cbt = Gtk::manage(new Gtk::ComboBoxText);
			vector<string> data = parse_fxgen_combo_string(desc[i].pszDefValue);
			for(unsigned j = 0; j < data.size(); j++)
				cbt->append_text(data[j]);
			cbt->set_active(n);
			display_item = cbt;
			cbt->signal_changed().connect(bind(bind(bind(mem_fun(*this,&PropertyTable::AdjustProperty),op), i),display_item));
		}
		else if(ref.find("NCFloatProp") == 0)
		{
#if 1
			Gtk::SpinButton* sb = Gtk::manage(new Gtk::SpinButton(.003, 4));
			sb->set_range(0,1);
			sb->set_increments(.0001, .001);
			sb->set_value(f);
			display_item = sb;
			sb->signal_changed().connect(bind(bind(bind(mem_fun(*this,&PropertyTable::AdjustProperty),op), i),display_item));
#else
			Gtk::HScale* hs = Gtk::manage(new Gtk::HScale(0, 1.0001, .0001));
			hs->set_value(f);
			display_item = hs;
			hs->signal_value_changed().connect(bind(bind(bind(mem_fun(*this,&PropertyTable::AdjustProperty),op), i),display_item));
#endif
		}
		else if(ref.find("NStringProp") == 0)
		{
			Gtk::Entry* e = Gtk::manage(new Gtk::Entry);
			e->set_max_length(sizeof(values[i].szVal));
			e->set_text(values[i].szVal);
			display_item = e;
			e->signal_changed().connect(bind(bind(bind(mem_fun(*this,&PropertyTable::AdjustProperty),op), i),display_item));
		}
		else if(ref.find("NUseStoredOpsProp") == 0)
		{
//			Glib::RefPtr<Gtk::ListStore>store=Gtk::ListStore::create();
			display_item = Gtk::manage(new Gtk::ComboBox);
			;
		}
		else if(ref.find("NVectorDataProp") == 0)
		{
			display_item = Gtk::manage(new Gtk::FileChooserButton);
			;
		}

		if(display_item) // i.e. found out how to display
		{
			attach(*display_item, 1, 2, i+1, i+2);
		}
	}
	show_all_children();
}
