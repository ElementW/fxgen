//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		SmartWindow.cpp
//! \brief	FxGen GTK editor - application main window
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
#include "MainWindow.h"
#include "fxgen_pch.h"
#include "globals.h"

extern MainMenu* main_menu;

pair<string, string> parse_filename(string filename)
{
    pair<Glib::ustring,Glib::ustring> name;
    Glib::ustring::size_type i=filename.rfind('.');
    if (i!=Glib::ustring::npos)
    {
        name.first=filename.substr(0,i);
        filename.erase(0,i+1);
        name.second=filename; //.lowercase();
        return name;
    }
    name.first=filename;
    name.second=Glib::ustring();
    return name;
}

MainWindow::MainWindow(GtkWindow* cobject, const RefPtr<Xml>& refGlade)
        :Gtk::Window(cobject), project_modified(this)
{
    main_menu = new MainMenu(*this);

//	modify_bg(Gtk::STATE_NORMAL, Gdk::Color("#737373"));
    //ctor
}

MainWindow::~MainWindow()
{
    //dtor
}

bool MainWindow::on_delete_event(GdkEventAny*)
{
    if (project_modified && ConfirmProjectLoss() != Gtk::RESPONSE_OK)
        return true; // stop signal propagation
    return false;
}

int MainWindow::ConfirmProjectLoss()
{
    Gtk::MessageDialog dlg("Your project contains unsaved data.", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK_CANCEL, true);
    dlg.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_APPLY);
    int response = dlg.run();
    switch (response)
    {
    case Gtk::RESPONSE_APPLY:
        SaveProject(filename);
        return Gtk::RESPONSE_OK;
        break;
    default:
        return response;
    }
    return Gtk::RESPONSE_CANCEL;
}

void MainWindow::QuickSaveProject()
{
    SaveProject(filename);
}


void MainWindow::SaveProject(string _filename)
{
    if (_filename.empty())
    {
        Gtk::FileChooserDialog dlg(*(Gtk::Window*)this, "Save", Gtk::FILE_CHOOSER_ACTION_SAVE);
        dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dlg.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
        dlg.add_filter(filter_fxgen);
        dlg.add_filter(filter_all);
        dlg.set_current_name(Glib::path_get_basename(filename));
        int response = dlg.run();
        if (response == Gtk::RESPONSE_OK)
            _filename = dlg.get_filename();
        else return;
    }

	// add extension, if needed
	pair<string,string>name = parse_filename(_filename);
	if(name.second != "prj")
		_filename += ".prj";

    NEngineOp* engine = NEngineOp::GetEngine();
    if (engine->SaveProject(_filename.c_str()))
    {
        filename = _filename;
        set_title(Glib::filename_display_basename(filename));
        project_modified = false;
    }
    else
        Gtk::MessageDialog dlg("Problem saving file.", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
}

void MainWindow::ExportImage()
{
    OperatorWidget* preview_op = OperatorWidget::preview_op;

	if(!preview_op)
		return; // no image to save

    // retrieve operator's saved name, if exists
    string _filename = Glib::path_get_basename(filename);

    if (dynamic_cast<NStoreOp*>(preview_op->op) || dynamic_cast<NStoreResultOp*>(preview_op->op))
    {
        _filename += string(".") + preview_op->op->GetUserName();
        // name suffix
        if (dynamic_cast<NStoreResultOp*>(preview_op->op))
        {
            NVarsBloc* bloc = preview_op->op->m_pcvarsBloc;
            if (bloc->Count() > 1) // just in case
            {
                NVarsBlocDesc* desc = bloc->GetBlocDesc();
                vector<string> data = parse_fxgen_combo_string(desc[1].pszDefValue);
                NVarValue* values = bloc->GetValues();
                _filename += string(".") + data[values[1].byVal];
            }
        }
    }

	// prepare dialog
    Gtk::FileChooserDialog dlg(*(Gtk::Window*)this, "Export", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dlg.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
    dlg.set_current_name(_filename);

    vector<Gdk::PixbufFormat> formats, allformats = image->get_pixbuf()->get_formats();
    vector<Gdk::PixbufFormat>::const_iterator iter;

    // set up writable image formats, if any
    for (iter = allformats.begin(); iter != allformats.end(); iter++)
        if (iter->is_writable())
        {
        	if(iter->get_name() == "ico")
				if(image->get_pixbuf()->get_width() > 255 || image->get_pixbuf()->get_height() > 248)
					continue; // icos cannot be that big

            formats.push_back(*iter);
        }

    if (!formats.size())
        return; // no suitable image format

    vector<string> extensions;
    Gtk::FileFilter *file_filter;

	// Add file filters
    for (iter = formats.begin(); iter != formats.end(); iter++)
    {
        extensions = iter->get_extensions();
        file_filter = new Gtk::FileFilter;
        file_filter->set_name(iter->get_description());
        for (unsigned i = 0; i < extensions.size(); i++)
            file_filter->add_pattern("*." + extensions[i]);
        dlg.add_filter(*file_filter);
    }

	// Show dialog
    int response = dlg.run();

    if (response == Gtk::RESPONSE_OK)
        _filename = dlg.get_filename();
    else
		return; // Cancel was pressed

	pair<string, string> name = parse_filename(_filename);
    string format;

	// Check if user provided an extension (will override format)
	try{
		for (iter = formats.begin(); iter != formats.end(); iter++)
		{
			extensions = iter->get_extensions();
			for(unsigned i = 0; i < extensions.size(); i++)
				if (extensions[i] == name.second)
					throw iter->get_name();
		}
	}
	catch(Glib::ustring& name)
	{
		format = name;
	}

	// Save image
    if(!format.empty())
		image->get_pixbuf()->save(_filename, format);
    else
    {
		for (iter = formats.begin(); iter != formats.end(); iter++)
		{
			if(iter->get_description() == dlg.get_filter()->get_name())
			{
				format = iter->get_name();
				extensions = iter->get_extensions();
				break;
			}
		}
		image->get_pixbuf()->save(_filename + "." + extensions[0], format);
    }
}

void MainWindow::ClearProject()
{
    if (project_modified)
    {
        if (ConfirmProjectLoss() == Gtk::RESPONSE_CANCEL)
            return;
    }

    image->clear();
    OperatorWidget::cleanup();
    filename.clear();
    set_title("");

    project_tree->clear();

    NEngineOp* engine = NEngineOp::GetEngine();
    engine->Clear();

// Copied from the original editor - is there any other way to do this?
    //Create empty project
    NTreeNode* prootNode = engine->GetRootGroup();
    NTreeNode* pNewGrpNode = new NTreeNode;
    pNewGrpNode->SetName("Group");
    prootNode->AddSon(pNewGrpNode);

    NOperatorsPage* ppage = new NOperatorsPage;
    ppage->SetName("Page");
    pNewGrpNode->GetObjsArray().AddItem(ppage);
// -----------------------------------
    project_tree->DisplayProject(NEngineOp::GetEngine());
    project_modified = false;
}

void MainWindow::LoadProject(string _filename, bool append)
{
    if (project_modified)
    {
        if (window->ConfirmProjectLoss() == Gtk::RESPONSE_CANCEL)
            return;
    }

    if (_filename.empty() || !Glib::file_test(_filename, Glib::FILE_TEST_EXISTS))
    {
        Gtk::FileChooserDialog dlg(*window, "Open", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dlg.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
        dlg.add_filter(filter_fxgen);
        dlg.add_filter(filter_all);
        int response = dlg.run();
        if (response == Gtk::RESPONSE_OK)
            _filename = dlg.get_filename();
        else return;
    }

    project_modified = false;
    ClearProject();
    NEngineOp* engine = NEngineOp::GetEngine();
    if (engine->LoadProject(_filename.c_str()))
    {
        filename = _filename;
        window->set_title(Glib::filename_display_basename(filename));
        project_tree->DisplayProject(NEngineOp::GetEngine());
    }
}
