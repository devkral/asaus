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



#include <string>
#include <gtkmm.h>
#include <memory>


//https://jazzy.wordpress.com/2006/06/20/how-to-get-gcc-version-on-compile-time/
#define GCC_VERSION (__GNUC__ * 10000 \
                               + __GNUC_MINOR__ * 100 \
                               + __GNUC_PATCHLEVEL__)

#ifndef _TERMINAL_H_
#define _TERMINAL_H_

class terminal
{
public:
	terminal();
	~terminal();
	Gtk::Widget *givevteterm();
	std::string makecompatible(std::string input);
	void feedexe (std::string input);
	void feedtext(std::string input);
	void reset();

protected:

private:

//because gcc < 4.7 don't understand this declaration type
#if GCC_VERSION < 40700
	GtkWidget* vteterm;
	Gtk::Widget* vtetermcc;
	GError *err;
#else
	GtkWidget* vteterm=0;
	Gtk::Widget* vtetermcc=0;
	//neccessary for vte_terminal_fork_command_full
	GError* err=0;
#endif
	
	GPid pidterm;
	
};

#endif // _TERMINAL_H_
