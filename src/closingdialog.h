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
	closingdialog(Gtk::Window *windt, Gtk::Main *runt);
	closingdialog();
	~closingdialog();
	
	void runit();
protected:
	void close_prog();
	void go_back();
	void fullscreen();
	void maximize();
	void become_normal();
	void iconify();

	
private:
	Gtk::Window *wind;
	Gtk::Main *run;

	Gtk::Main myrun;
	Gtk::Window *close_win;
		
};

#endif // _CLOSINGDIALOG_H_
