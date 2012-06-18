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

#include <gtkmm.h>

#ifndef _CLOSINGDIALOG_H_
#define _CLOSINGDIALOG_H_

class closingdialog
{
public:
	closingdialog(bool fullscreent);
	closingdialog();
	~closingdialog();
	
	void init(Gtk::Window *windt, Gtk::Main *runt);
	void default_fullscreen(bool shall_fullscreent);
	void run();
protected:
	void close_prog();
	void go_back();
	void fullscreen();
	void maximize();
	void normalize();
	void iconify();

	
private:
	Gtk::Window *wind;
	Glib::RefPtr<Gdk::Window> wind_gdk;
	Gtk::Main *run_main;
	bool stopiconify(GdkEventWindowState* eventt);

	void endrunit();
	Glib::RefPtr<Gtk::Window> close_win;

	Glib::RefPtr<Gtk::Image> extreme;
	Glib::RefPtr<Gtk::Button> extremeb;
	bool shall_fullscreen;
	void toggleextreme(bool shall_extreme);
	
};

#endif // _CLOSINGDIALOG_H_
