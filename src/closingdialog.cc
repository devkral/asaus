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


void closingdialog::init (Gtk::Window *windt, Gtk::Main *runt)
{
	wind=windt;
	grey=Gdk::RGBA();
	grey.set_rgba(0.5,0.5,0.5,1);
	wind_gdk=wind->get_window();
	run_main=runt;
	
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
	//close_win->set_deletable (false);
	close_win->override_background_color(grey);
//close_win->set_skip_pager_hint (true);
	close_win->hide();
	close_win->set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG ) ;
	close_win->signal_window_state_event().connect (sigc::mem_fun(*this,&closingdialog::stopiconify));

	//close_win
	
	Glib::RefPtr<Gtk::Button> minimize=transform_to_rptr<Gtk::Button>(builder->get_object("minimize"));
	//minimize->override_background_color(grey);
	minimize->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::iconify));
	
	Glib::RefPtr<Gtk::Button> back=transform_to_rptr<Gtk::Button>(builder->get_object("back"));
	//back->override_background_color(grey);
	back->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::go_back));

	Glib::RefPtr<Gtk::Button> close=transform_to_rptr<Gtk::Button>(builder->get_object("close"));
	//close->override_background_color(grey);
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
		
		if (shall_fullscreen==true)
		{
			extreme->set(Gtk::StockID("gtk-fullscreen"),Gtk::IconSize (96));
			extremeb->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::fullscreen));
		}
		else
		{

			extreme->set(Gtk::StockID("gtk-fullscreen"),Gtk::IconSize (96));
			extremeb->signal_clicked ().connect(sigc::mem_fun(*this,&closingdialog::maximize));
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
		//std::cerr << wind_gdk->get_state() << std::endl;
		//std::cerr << Gdk::WINDOW_STATE_MAXIMIZED << std::endl;
		//std::cerr << (Gdk::WINDOW_STATE_MAXIMIZED | Gdk::WINDOW_STATE_FULLSCREEN ) << std::endl;
		//doesn't look nice; Where the hell is a documentation?
		if (wind_gdk->get_state() == 132 || wind_gdk->get_state() ==144)
		//if (is_extreme)
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
}

void closingdialog::maximize()
{
	endrunit();
	wind->maximize();
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
