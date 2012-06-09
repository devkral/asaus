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



#include <string>
#include <gtkmm.h>


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
	GtkWidget* vteterm=0;
	Gtk::Widget* vtetermcc=0;
	GPid pidterm;
	//neccessary for vte_terminal_fork_command_full
	GError *err=0;
	
};

#endif // _TERMINAL_H_
