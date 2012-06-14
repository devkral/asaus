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


closingdialog::closingdialog ()
{
	run_main=0;
	wind=0;
	shall_fullscreen=false;
};

closingdialog::closingdialog (bool fullscreent)
{
	run_main=0;
	wind=0;
	shall_fullscreen=fullscreent;
};
/**
namespace Gtk::Window
{
	Gdk::WindowState get_wi_state(){
	return Gdk::Window::get_state();
	}

}*/


void closingdialog::init (Gtk::Window *windt, Gtk::Main *runt)
{
	wind=windt;
	run_main=runt;
	is_extreme=false;
	wind->signal_window_state_event().connect (sigc::mem_fun(*this,&closingdialog::get_window_state_event));

	
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
	
	close_win=transform_to_rptr<Gtk::Window>(builder->get_object("closedialog"));
	close_win->set_deletable (false);
	close_win->set_skip_pager_hint (true);
	close_win->hide();
	close_win->signal_window_state_event().connect (sigc::mem_fun(*this,&closingdialog::stopiconify));

	//close_win
	
	Glib::RefPtr<Gtk::Button> minimize=transform_to_rptr<Gtk::Button>(builder->get_object("minimize"));
	minimize->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::iconify));
	Glib::RefPtr<Gtk::Button> back=transform_to_rptr<Gtk::Button>(builder->get_object("back"));
	back->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::go_back));
	Glib::RefPtr<Gtk::Button> close=transform_to_rptr<Gtk::Button>(builder->get_object("close"));
	close->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::close_prog));

	extreme=transform_to_rptr<Gtk::Image>(builder->get_object("extreme"));
	extremeb=transform_to_rptr<Gtk::Button>(builder->get_object("extremeb"));
	toggleextreme(true);
	
}

void closingdialog::default_fullscreen(bool shall_fullscreent)
{
	shall_fullscreen=shall_fullscreent;
}

void closingdialog::toggleextreme(bool shall_extreme=true)
{
	if(shall_extreme)
	{
		
		if (shall_fullscreen==false)
		{
			extreme->set(Gtk::StockID("gtk-fullscreen"),Gtk::IconSize (96));
			extremeb->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::maximize));
		}
		else
		{
			extreme->set(Gtk::StockID("gtk-fullscreen"),Gtk::IconSize (96));
			extremeb->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::fullscreen));
		}
	}
	else
	{
		if (shall_fullscreen==false)
			extreme->set(Gtk::StockID("gtk-zoom-100"),Gtk::IconSize (96));
		else
			extreme->set(Gtk::StockID("gtk-leave-fullscreen"),Gtk::IconSize (96));
		extremeb->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::normalize));
	}
}


bool closingdialog::get_window_state_event(GdkEventWindowState* eventt)
{
		if (eventt->new_window_state==GDK_WINDOW_STATE_MAXIMIZED || eventt->new_window_state==GDK_WINDOW_STATE_FULLSCREEN)
		{
			is_extreme=true;
		}
	return false;
}

bool closingdialog::stopiconify(GdkEventWindowState* eventt)
{
	eventt;
	if (eventt->new_window_state==GDK_WINDOW_STATE_ICONIFIED)
	{	
		close_win->deiconify();
		return false;
	}
	else
		return false;
}

void closingdialog::endrunit()
{
	close_win->unset_transient_for();
	wind->set_opacity (1);
	close_win->hide();
}


void closingdialog::run()
{
	if (run_main!=0 && wind!=0)
	{

		//if (Gdk::Window(*wind).get_state() ==(Gdk::WINDOW_STATE_MAXIMIZED ))
		if (is_extreme)
		{
			toggleextreme(false);
		}
		else
		{
			toggleextreme(true);
		}
		
		
		close_win->set_transient_for (*wind);
		wind->set_opacity (0.8);
		close_win->show();
	}
}

void closingdialog::close_prog()
{
	run_main->quit();
}

void closingdialog::fullscreen()
{
	endrunit();
	wind->fullscreen();
	is_extreme=true;
}

void closingdialog::maximize()
{
	endrunit();
	wind->maximize();
	is_extreme=true;
}

void closingdialog::iconify()
{
	endrunit();
	wind->iconify();
}

void closingdialog::normalize()
{
	endrunit();
	wind->unmaximize();
	wind->unfullscreen();
	is_extreme=false;
	
}

void closingdialog::go_back()
{
	endrunit();
}
closingdialog::~closingdialog()
{
	wind=0;
	run_main=0;
}
