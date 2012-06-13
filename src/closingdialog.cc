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


#ifdef test_an
#undef PACKAGE_DATA_DIR
#define PACKAGE_DATA_DIR "src"
#endif

#include "closingdialog.h"

#include <gtkmm.h> // closingdialog.h
#include <iostream> 

template< class T_CppObject > Glib::RefPtr<T_CppObject>
transform_to_rptr(const Glib::RefPtr< Glib::Object >& p)
{
	return Glib::RefPtr<T_CppObject>::cast_dynamic(p);
}

closingdialog::closingdialog (Gtk::Window *windt, Gtk::Main *runt) // : myrun()
{
	wind=windt;
	run=runt;

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	try
	{
		builder->add_from_file(PACKAGE_DATA_DIR"/ui/closedialog.ui");
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
		throw(ex);
	}
	catch(const Glib::MarkupError& ex)
	{
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		throw(ex);
	}
	catch(const Gtk::BuilderError& ex)
	{
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		throw(ex);
	}
	
	Glib::RefPtr<Gtk::Window> close_win=transform_to_rptr<Gtk::Window>(builder->get_object("closedialog"));

	//close_win
	Glib::RefPtr<Gtk::Button> maximize=transform_to_rptr<Gtk::Button>(builder->get_object("maximize"));
	maximize->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::maximize));
	Glib::RefPtr<Gtk::Button> minimize=transform_to_rptr<Gtk::Button>(builder->get_object("minimize"));
	minimize->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::iconify));
	Glib::RefPtr<Gtk::Button> back=transform_to_rptr<Gtk::Button>(builder->get_object("back"));
	back->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::go_back));
	Glib::RefPtr<Gtk::Button> close=transform_to_rptr<Gtk::Button>(builder->get_object("close"));
	close->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::close_prog));
	
}
closingdialog::closingdialog ()
{
	run=0;
	wind=0;
};

void closingdialog::runit()
{
	if (run!=0)
	{
		//close_win->set_transient_for (*wind);
		wind->set_opacity (0.8);
		//close_win->show();
		//myrun.run();
	}
}

void closingdialog::close_prog()
{
	//myrun.quit();
	run->quit();
}

void closingdialog::fullscreen()
{
	//myrun.quit();
	//if wind.
	//if wind.getproperty
	wind->set_opacity (1);
	wind->fullscreen();
}

void closingdialog::maximize()
{
	//myrun.quit();
	wind->set_opacity (1);
	wind->maximize();
}

void closingdialog::iconify()
{
	//myrun.quit();
	wind->set_opacity (1);
	wind->iconify();
}

void closingdialog::become_normal()
{
	//myrun.quit();
	wind->set_opacity (1);
	wind->unmaximize();
	wind->unfullscreen();
}

void closingdialog::go_back()
{
	//myrun.quit();
	wind->set_opacity (1);
	//close_win->hide();
}
closingdialog::~closingdialog()
{
	wind=0;
}
