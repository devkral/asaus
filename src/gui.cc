/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * asaus
 * Copyright (C) alex 2012 <alex@archal>
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

#include "gui.h"
//#include "executecode.h"

#include <iostream>

#include <chrono>
#include <ctime>


gui::gui(int argc, char *argv[]) : kit(argc,argv),compilethread(this),executethread(this),iconthread(this)
{
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
	builder->get_widget("main_window", main_win);

	//init basics
	//init drag
	Targets.push_back( Gtk::TargetEntry("STRING") );
	Targets.push_back( Gtk::TargetEntry("text/plain") );
	main_win->drag_dest_set(Targets);
	main_win->signal_drag_data_received().connect(sigc::mem_fun(*this, &gui::drag_data_get));

	//init closebutton
	main_win->signal_delete_event().connect(sigc::mem_fun(*this,&gui::closebutton));

	//init hide completely
	//main_win->
	//main_win->signal_size_allocate().connect(sigc::mem_fun(*this,&gui::on_window_iconify));
	
	//Spinner
	builder->get_widget("movecont", movecont);
	builder->get_widget("spinner1", spinner1);
	
	//for terminal
	builder->get_widget("termspace", terminal);

	

	//for different options (compile/execute)
	builder->get_widget("actionchoose", actionchoose);
	//elements of execute
	builder->get_widget("exeops", exeops);
	
	builder->get_widget("exeargs", exeargs);

	//elements of compile
	builder->get_widget("compops", compops);

	builder->get_widget("debugbutton", debugbutton);
	builder->get_widget("obbutton", obbutton);
	builder->get_widget("optimizelevel", opimizelevel);
	builder->get_widget("compargs", compargs);
	
	//choose compops first
	actionchoose->add(*compops);
	actionchoose->show_all_children ();
	
	//radio button
	builder->get_widget("radiobutton1", compilechosebutton);
	builder->get_widget("radiobutton2", compilechosebutton2);
	compilechosebutton->signal_toggled ().connect(sigc::mem_fun(*this,&gui::changeaction));

	//change to execute and execute then
	builder->get_widget("execute", execute);
	execute->signal_clicked ().connect(sigc::mem_fun(*this,&gui::exeact));
	//change to compile and compile then
	builder->get_widget("compile", compilebutton);
	compilebutton->signal_clicked ().connect(sigc::mem_fun(*this,&gui::compact));

	//filechoose
	builder->get_widget("filechoosebutton", filechoosebutton);
	filechoosebutton->signal_clicked ().connect(sigc::mem_fun(*this,&gui::fileact));
	builder->get_widget("fileentry", fileentry);
	fileentry->signal_editing_done().connect(sigc::mem_fun(*this,&gui::paintitwhite));
	lastfile="";

	movecont->show_all_children();
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
		actionchoose->add(*compops);
		actionchoose->show_all_children ();
		settermspace(*compilethread.givevteterm());
	}
	else
	{
		actionchoose->remove();
		actionchoose->add(*exeops);
		actionchoose->show_all_children ();
		settermspace(*executethread.givevteterm());
	}
	
}
int gui::startspin()
{
	auto now=std::chrono::steady_clock::now();
	bool temp=blockspin.try_lock_until(now+std::chrono::milliseconds(500));
	if(temp)
	{
		//movecont->show_all_children();
		spinner1->show();
		spinner1->start();
		blockspin.unlock();
		return 0;
	}
	else
		return 1;
}

void gui::stopspin()
{
	auto now=std::chrono::steady_clock::now();
	bool temp=blockspin.try_lock_until(now+std::chrono::milliseconds(500));
	if(temp)
	{
		spinner1->stop();
		spinner1->hide();
		blockspin.unlock();
	}
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
	return opimizelevel->get_text();
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
	fileentry->override_background_color(Gdk::RGBA("ff0000"),Gtk::STATE_FLAG_NORMAL);
}

void gui::paintitwhite()
{
	fileentry->override_background_color (Gdk::RGBA("ffffff"),Gtk::STATE_FLAG_NORMAL);
}

int gui::fileentrylength()
{
	fileentry->get_text_length ();
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
	closedialog.set_attached_to (*main_win);
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
