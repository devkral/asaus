/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * asaus
 * Copyright (C) alex 2012 <devkral@web.de>
 * 
 * asaus is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * asaus is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/asaus.ui" */
#ifdef test_an
#define UI_FILE "src/asaus.ui"
#else
#define UI_FILE PACKAGE_DATA_DIR"/ui/asaus.ui"
#endif

#include "gui.h"

#include <iostream>

//#include <chrono>
//#include <ctime>
#include <cassert>
#include <unistd.h>


template< class T_CppObject > Glib::RefPtr<T_CppObject>
transform_to_rptr(const Glib::RefPtr< Glib::Object >& p)
{
	return Glib::RefPtr<T_CppObject>::cast_dynamic(p);
}

gui::gui(int argc, char *argv[]) : kit(argc,argv),compilethread(this),executethread(this),iconthread(this)
{		
	red=Gdk::RGBA();
	red.set_rgba(1,0,0,1);
	white=Gdk::RGBA();
	white.set_rgba(1,1,1,1);
	black=Gdk::RGBA();
	black.set_rgba(0,0,0,1);
	
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	try
	{
		builder->add_from_file(UI_FILE);
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
	}
	catch(const Glib::MarkupError& ex)
	{
		std::cerr << "MarkupError: " << ex.what() << std::endl;
	}
	catch(const Gtk::BuilderError& ex)
	{
		std::cerr << "BuilderError: " << ex.what() << std::endl;
	}

	//now get root window
	main_win=transform_to_rptr<Gtk::Window>(builder->get_object("main_window"));
	
	//init basics
	//init drag
	Targets.push_back( Gtk::TargetEntry("STRING") );
	Targets.push_back( Gtk::TargetEntry("text/plain") );
	main_win->drag_dest_set(Targets);
	main_win->signal_drag_data_received().connect(sigc::mem_fun(*this, &gui::drag_data_get));

	//init closebutton
	main_win->signal_delete_event().connect(sigc::mem_fun(*this,&gui::closebutton));

	//init hide completely
	main_win->signal_window_state_event().connect (sigc::mem_fun(*this,&gui::on_my_window_state_event));
	
	//Spinner
	spinner1=transform_to_rptr<Gtk::Spinner>(builder->get_object("spinner1"));
	//how many processes use spinner
	spinuser=0;
	
	//for terminal
	terminal=transform_to_rptr<Gtk::Alignment>(builder->get_object("termspace"));
	

	
	//for different options (compile/execute)
	actionchoose=transform_to_rptr<Gtk::Alignment>(builder->get_object("actionchoose"));
	//elements of execute
	exeops=transform_to_rptr<Gtk::Grid>(builder->get_object("exeops"));
	exeargs=transform_to_rptr<Gtk::Entry>(builder->get_object("exeargs"));
	
	//elements of compile
	compops=transform_to_rptr<Gtk::Grid>(builder->get_object("compops"));
	
	debugbutton=transform_to_rptr<Gtk::CheckButton>(builder->get_object("debugbutton"));
	assert(debugbutton);
	obbutton=transform_to_rptr<Gtk::CheckButton>(builder->get_object("obbutton"));
	assert(obbutton);
	optimizelevel=transform_to_rptr<Gtk::Entry>(builder->get_object("optimizelevel"));
	assert(optimizelevel);
	compargs=transform_to_rptr<Gtk::Entry>(builder->get_object("compargs"));
	assert(compargs);
	
	//choose compops first
	actionchoose->add(*compops.operator->());
	actionchoose->show_all_children ();
	
	//radio button
	compilechosebutton=transform_to_rptr<Gtk::RadioButton>(builder->get_object("radiobutton1"));
	compilechosebutton2=transform_to_rptr<Gtk::RadioButton>(builder->get_object("radiobutton2"));
	compilechosebutton->signal_toggled ().connect(sigc::mem_fun(*this,&gui::changeaction));

	//change to execute and execute then
	execute=transform_to_rptr<Gtk::Button>(builder->get_object("execute"));
	execute->signal_clicked ().connect(sigc::mem_fun(*this,&gui::exeact));
	//change to compile and compile then
	compilebutton=transform_to_rptr<Gtk::Button>(builder->get_object("compile"));
	compilebutton->signal_clicked ().connect(sigc::mem_fun(*this,&gui::compact));

	//filechoose
	filechoosebutton=transform_to_rptr<Gtk::Button>(builder->get_object("filechoosebutton"));
	filechoosebutton->signal_clicked ().connect(sigc::mem_fun(*this,&gui::fileact));
	fileentry=transform_to_rptr<Gtk::Entry>(builder->get_object("fileentry"));
	fileentry->signal_editing_done().connect(sigc::mem_fun(*this,&gui::paintitwhite));
	lastfile="";

	spinner1->hide();

	settermspace(*compilethread.givevteterm());
	kit.run();
}

void gui::settermspace(Gtk::Widget &term)
{
	terminal->remove();
	terminal->add(term);
	terminal->show_all_children ();
}

void gui::changeaction()
{
	if (compilechosebutton->get_active())
	{
		actionchoose->remove();
		actionchoose->add(*compops.operator->());
		actionchoose->show_all_children ();
		settermspace(*compilethread.givevteterm());
	}
	else
	{
		actionchoose->remove();
		actionchoose->add(*exeops.operator->());
		actionchoose->show_all_children ();
		settermspace(*executethread.givevteterm());
	}
	
}
void gui::startspin()
{
	if (spinuser==0)
	{
		spinner1->show();
		spinner1->start();
		spinuser++;
	}
	else
		spinuser++;
}

void gui::stopspin()
{
	if (spinuser<=1)
	{
		spinner1->stop();
		spinner1->hide();
		spinuser=0;
	}
	else
		spinuser--;
}
//get method block

std::string gui::getexecuteargs()
{
	return exeargs->get_text();
}

std::string gui::getcompileargs()
{
	return compargs->get_text();
}

std::string gui::getfilepath()
{
	return fileentry->get_text();
}

bool gui::isobcode()
{
	return obbutton->get_active();
}
bool gui::isdebug()
{
	return debugbutton->get_active();
}

std::string gui::optimizegrade()
{
	return optimizelevel->get_text();
}

//block end

bool gui::isvisible()
{
	return main_win->get_visible();
}


void gui::exeact()
{
	
	startspin();
	compilechosebutton2->set_active(true);
	settermspace(*executethread.givevteterm());
	executethread.execute();
	stopspin();
	//blockexecute.unlock();
}

/**
void gui::exeacthelper()
{
if(blockexecute.try_lock())
	{
		
#ifdef STD_THREAD_CALL
		c11_threade=std::thread(&gui::exeacthelper2, this);
#else
		pthread_create(&p_threade,NULL,&gui::exeacthelper2,this);
#endif //STD_THREAD_CALL
	}	
	
}
*/ //not needed anymore

void gui::compact()
{

	startspin();
	compilechosebutton->set_active(true);
	settermspace(*compilethread.givevteterm());
	compilethread.compile();
	stopspin();
	//blockcompile.unlock();

}



/**
void gui::compacthelper()
{
if(blockcompile.try_lock())
	{
#ifdef STD_THREAD_CALL
		c11_threadc=std::thread(compacthelper2, this);
#else
		pthread_create(&p_threadc,NULL,&gui::compacthelper2,this);
#endif //STD_THREAD_CALL
	}

}

*/ //not needed anymore

void gui::fileact()
{
	Gtk::FileChooserDialog select("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	select.set_filename (lastfile);
	select.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	select.add_button("Select", Gtk::RESPONSE_OK);
	switch
	(
		select.run()
	)
	{
		case(Gtk::RESPONSE_OK):
			fileentry->set_text(select.get_filename());
			lastfile=select.get_filename();
		break;
		case(Gtk::RESPONSE_CANCEL):
		break;
	}
}

void gui::paintitred()
{
	//fileentry->override_background_color(red);
	fileentry->override_color(red);
}

void gui::paintitwhite()
{
	//fileentry->override_background_color (black);
	
	
}

void gui::unsetcolor()
{
	fileentry->unset_color();
}

int gui::fileentrylength()
{
	fileentry->get_text_length();
}

void gui::drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, int, int,
          const Gtk::SelectionData& selection_data, guint, guint time)
{
	std::string temp=selection_data.get_data_as_string();
	temp=temp.erase(temp.length()-2,temp.length());
    temp=Glib::filename_from_uri (temp );

	fileentry->set_text(temp);
	context->drag_finish(false, true, time);	
}

void gui::show()
{
	main_win->deiconify();
	main_win->show();
}

void gui::hide()
{
	main_win->hide();
}

bool gui::closebutton(GdkEventAny*)
{
	Gtk::Dialog closedialog("Close program completely?",false);
	closedialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	closedialog.add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE );
	closedialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	//closedialog.set_title("Close program completely?");
	//closedialog.set_transient_for (*main_win);
	closedialog.set_attached_to (*main_win.operator->());
	closedialog.set_skip_taskbar_hint(true);
	main_win->set_opacity (0.8);
	
	switch(closedialog.run())
	{
		case(Gtk::RESPONSE_OK):
			kit.quit();
			return true;
		break;
		case(Gtk::RESPONSE_CLOSE ):
			main_win->set_opacity (1);
			return false;
		break;
		case(Gtk::RESPONSE_CANCEL ):
			main_win->set_opacity (1);
			return true;
		default:
			main_win->set_opacity (1);
			return true;
	}
}

bool gui::on_my_window_state_event(GdkEventWindowState* event)
{
	if (event->new_window_state==GDK_WINDOW_STATE_ICONIFIED)
	{	
		hide();
		return false;
	}
	else
		return false;
}


void gui::hideshow()
{
	if (isvisible()==true)
		hide();
	else
		show();
}

gui::~gui()
{
}
